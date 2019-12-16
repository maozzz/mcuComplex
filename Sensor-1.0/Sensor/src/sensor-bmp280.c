/*===========================================================================
 *
 *  Sensor controller
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2019
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File sensor-bmp280.c. Pressure/temperature sensor BMP280 functions.
 *                        I2C interface.
 *                        Vdd = 1.7-3.6V.
 *                        Temp.  = -40..+85 DegC, res 0.0025 DegC.
 *                        Press. = 300..110 hPa,  res 0.16Pa.
 *
 *===========================================================================*/

#include "sensor.h"

//Debug messages (comment to disable)
#define PrintMsg(...)       //std_printf(__VA_ARGS__)

//Select version of source code
#define __BMP280_CALC_INT32     0 //32-bit integers
#define __BMP280_CALC_INT64     1 //64-bit integers
#define __BMP280_CALC_DOUBLE    2 //double type

#ifndef __BMP280_CALC_VARIANT
#define __BMP280_CALC_VARIANT   __BMP280_CALC_INT64
#endif


//Register 0xFA (temperature)
#define BMP280_REG_TEMPERATURE  0xFA //address

//Register 0xF7 (pressure)
#define BMP280_REG_PRESSURE     0xF7 //address

//Register 0xF5 (config):
#define BMP280_REG_CONFIG       0xF5 //address
//t_sb[7..5]: tstandby
#define BMP280_TSB_0p5ms        (0 << 5) //0.5ms
#define BMP280_TSB_62p5ms       (1 << 5) //62.5ms
#define BMP280_TSB_125ms        (2 << 5) //125ms
#define BMP280_TSB_250ms        (3 << 5) //250ms
#define BMP280_TSB_500ms        (4 << 5) //500ms
#define BMP280_TSB_1000ms       (5 << 5) //1000ms
#define BMP280_TSB_2000ms       (6 << 5) //2000ms
#define BMP280_TSB_4000ms       (7 << 5) //4000ms
#define BMP280_TSB_MASK         (7 << 5) //mask
//filter[4..2]: IIR filter coefs
#define BMP280_FILTER_OFF       (0 << 2) //Off
#define BMP280_FILTER_2         (1 << 2) //2
#define BMP280_FILTER_4         (2 << 2) //4
#define BMP280_FILTER_8         (3 << 2) //8
#define BMP280_FILTER_16        (4 << 2) //16
#define BMP280_FILTER_MASK      (7 << 2) //mask
//spi3w_en[0]: enable 3-wire SPI
#define BMP280_SPI3W_ENABLE     (1 << 0) //enable

//Register 0xF4 (ctrl_meas):
#define BMP280_REG_CTRLMEAS     0xF4 //address
//osrs_t[7..5]: temperature oversampling
#define BMP280_OSRST_SKIP       (0 << 5) //Skipped, output 0x80000
#define BMP280_OSRST_x1         (1 << 5) //16bit/0.0050C
#define BMP280_OSRST_x2         (2 << 5) //17bit/0.0025C
#define BMP280_OSRST_x4         (3 << 5) //18bit/0.0012C
#define BMP280_OSRST_x8         (4 << 5) //19bit/0.0006C
#define BMP280_OSRST_x16        (5 << 5) //20bit/0.0003C
#define BMP280_OSRST_MASK       (7 << 5) //mask
//osrs_p[4..2]: pressure oversampling
#define BMP280_OSRSP_SKIP       (0 << 2) //Skipped, output 0x80000
#define BMP280_OSRSP_x1         (1 << 2) //16bit/2.62Pa
#define BMP280_OSRSP_x2         (2 << 2) //17bit/1.31Pa
#define BMP280_OSRSP_x4         (3 << 2) //18bit/0.66Pa
#define BMP280_OSRSP_x8         (4 << 2) //19bit/0.33Pa
#define BMP280_OSRSP_x16        (5 << 2) //20bit/0.16Pa
#define BMP280_OSRSP_MASK       (7 << 2) //mask
//mode[1..0]: work/sleep mode
#define BMP280_MODE_SLEEP       (0 << 0) //Sleep
#define BMP280_MODE_FORCED      (1 << 0) //Forced
#define BMP280_MODE_NORMAL      (3 << 0) //Normal


//Register 0xF3 (status)
#define BMP280_REG_STATUS       0xF3 //address
//measuring[3]: 1/0 - measuring/completed
#define BMP280_ISMEASURING      (1 << 3)
//im_update[0]: 1 - copy from NVM to image registers
#define BMP280_ISIMUPDATE       (1 << 0)


//Register 0xE0 (reset)
#define BMP280_REG_RESET        0xE0 //address
//Write this value to 0xE0 register to reset BMP280
#define BMP280_RESET_KEY        0xB6


//Register 0xD0 (chip id)
#define BMP280_REG_CHIPID       0xD0 //address
//Chip ID must be 0x58
#define BMP280_REG_CHIPID_VALUE 0x58


//Register 0x88 (calib00), first calibration register
#define BMP280_REG_CALIB00      0x88
//Count of calibrating registers
#define BMP280_REG_CALIB_COUNT  24

/*
//---------------------------------------------------------------------------
BMP280 calibrating registers [0x88..0x9F]
Add  Name     Description
0xA1 calib25  reserved
0xA0 calib24
0x9F calib23  dig_P9  s16
0x9E calib22
0x9D calib21  dig_P8  s16
0x9C calib20
0x9B calib19  dig_P7  s16
0x9A calib18
0x99 calib17  dig_P6  s16
0x98 calib16
0x97 calib15  dig_P5  s16
0x96 calib14
0x95 calib13  dig_P4  s16
0x94 calib12
0x93 calib11  dig_P3  s16
0x92 calib10
0x91 calib09  dig_P2  s16
0x90 calib08
0x8F calib07  dig_P1  u16
0x8E calib06
0x8D calib05  dig_T3  s16
0x8C calib04
0x8B calib03  dig_T2  s16
0x8A calib02
0x89 calib01  dig_T1  u16  MSB
0x88 calib00               LSB

Sample #1 of calib. registers
T1: 27504; T2: 26435;  T3: -1000
P1: 36477; P2: -10685; P3: 3024
P4: 2855;  P5: 140;    P6: -7
P7: 15500; P8: -14600; P9: 6000

Sample #2 of calib. registers
T1: 27328; T2: 26891;  T3: -1000
P1: 36481; P2: -10610; P3: 3024
P4: 5931;  P5: 18;     P6: -7
P7: 15500; P8: -14600; P9: 6000
*/

/* BMP280 calib. registers */
typedef struct tag_BMP280CalibRegs
{
    u16 dig_T1;
    s16 dig_T2;
    s16 dig_T3;

    u16 dig_P1;
    s16 dig_P2;
    s16 dig_P3;
    s16 dig_P4;
    s16 dig_P5;
    s16 dig_P6;
    s16 dig_P7;
    s16 dig_P8;
    s16 dig_P9;
} BMP280CalibRegs;


/*
---------------------------------------------------------------------------
BMP280 profiles (Table 15)
Profile            Mode   osrs_p osrs_t IIRcoef Timing ODR,Hz BW,Hz RMS noise,cm
                                                             tsdby
Handheld low-power Normal x16    x2     4       62.5ms 10.0   0.92  4.0
Handheld dynamic   Normal x4     x1     16      0.5ms  83.3   1.75  2.4
Weather monitor.   Forced x1     x1     Off     1/min  1/60   full  26.4
Elevator           Normal x4     x1     4       125ms  7.3    0.67  6.4
Drop detection     Normal x2     x1     Off     0.5ms  125    full  20.8
Indoor navigation  Normal x16    x2     16      0.5ms  26.3   0.55  1.6
*/

//Profile: Handheld low-power
static const u8 bmp280_profile_handheld_lowpower[2] =
{    
    BMP280_MODE_NORMAL | BMP280_OSRSP_x16 | BMP280_OSRST_x2, //Reg 0xF4
    BMP280_FILTER_4 | BMP280_TSB_62p5ms                      //Reg 0xF5
};

//Profile: Handheld dynamic
static const u8 bmp280_profile_handheld_dynamic[2] =
{    
    BMP280_MODE_NORMAL | BMP280_OSRSP_x4 | BMP280_OSRST_x1,  //Reg 0xF4
    BMP280_FILTER_16 | BMP280_TSB_0p5ms                      //Reg 0xF5
};

//Profile: Weather monitoring
static const u8 bmp280_profile_weather_monitoring[2] =
{    
    BMP280_MODE_FORCED | BMP280_OSRSP_x1 | BMP280_OSRST_x1,  //Reg 0xF4
    BMP280_FILTER_OFF | BMP280_TSB_4000ms                    //Reg 0xF5
};

//Profile: Elevator/floor change detection
static const u8 bmp280_profile_elevator[2] =
{    
    BMP280_MODE_NORMAL | BMP280_OSRSP_x4 | BMP280_OSRST_x1,  //Reg 0xF4
    BMP280_FILTER_4 | BMP280_TSB_125ms                       //Reg 0xF5
};

//Profile: Drop detection
static const u8 bmp280_profile_drop_detection[2] =
{
    BMP280_MODE_NORMAL | BMP280_OSRSP_x2 | BMP280_OSRST_x1,  //Reg 0xF4
    BMP280_FILTER_OFF | BMP280_TSB_0p5ms                     //Reg 0xF5
};

//Profile: Indoor navigation
static const u8 bmp280_profile_indoor_navigation[2] =
{    
    BMP280_MODE_NORMAL | BMP280_OSRSP_x16 | BMP280_OSRST_x2, //Reg 0xF4
    BMP280_FILTER_16 | BMP280_TSB_0p5ms                      //Reg 0xF5
};

//List of profiles
static const u8 *bmp280_profiles_list[6] =
{
    bmp280_profile_handheld_lowpower,
    bmp280_profile_handheld_dynamic,
    bmp280_profile_weather_monitoring,
    bmp280_profile_elevator,
    bmp280_profile_drop_detection,
    bmp280_profile_indoor_navigation
};


//Calibraiting registers (must read from BMP280 when configured)
//Device with SDO=0 (I2C address 0x76)
static BMP280CalibRegs bmp280CalibA0;
//Device with SDO=1 (I2C address 0x77)
static BMP280CalibRegs bmp280CalibA1;

//Get pointer to calibratin registers
#define bm280_getcalib_ptr(sen) ((sen)->cfg->addr[0] & 1) ? &bmp280CalibA1 : &bmp280CalibA0

/* BMP280: initialize control structure */
void InitializeBMP280(SensorCtrl *sen, const SensorConfig *cfg)
{    
    memset(sen, 0, sizeof(SensorCtrl));

    //Save pointer to configuration
    sen->cfg = cfg;

    //Set functions
    sen->pfConfigure = &BMP280Configure;
    sen->pfWriteData = &BMP280WriteData;
    sen->pfReadData  = &BMP280ReadData;
    sen->pfToString  = &BMP280ToString;
}


//---------------------------------------------------------------------------
#if (__BMP280_CALC_VARIANT == __BMP280_CALC_INT32)

/* Solve "t_fine" value */
s32 bmp280_get_t_fine(BMP280CalibRegs *pcalib, s32 tcode)
{
    s32 var1, var2;

    var1 = ((((tcode >> 3) - ((s32)pcalib->dig_T1 << 1))) *
            ((s32)pcalib->dig_T2)) >> 11;

    var2 = (((((tcode >> 4) - ((s32)pcalib->dig_T1)) *
              ((tcode >> 4) - ((s32)pcalib->dig_T1))) >> 12) *
              ((s32)pcalib->dig_T3)) >> 14;

    return (s32)(var1 + var2);
}



/* Returns temperature in DegC, resolution is 0.01 DegC.
   Output value of "5123" equals 51.23 DegC.
   t_fine carries fine temperature as global value */
s32 bmp280_compensate_T_int32(BMP280CalibRegs *pcalib, s32 tcode)
{    
    return (s32)((bmp280_get_t_fine(pcalib, tcode) * 5 + 128) >> 8);
}



/* Returns pressure in Pa as unsigned 32 bit integer.
   Output value of "96386" equals 96386 Pa */
u32 bmp280_compensate_P_int32(BMP280CalibRegs *pcalib, s32 tcode, s32 pcode, s32 *Tout)
{
    s32 var1, var2, t_fine;
    u32 p;

    //Solve temperature    
    t_fine = bmp280_get_t_fine(pcalib, tcode);

    if (Tout)
        *Tout = (t_fine * 5 + 128) >> 8;


    //Solve pressure
    var1 = (((s32)t_fine) >> 1) - (s32)64000;

    var2 = (((var1 >> 2) * (var1 >> 2)) >> 11) * ((s32)pcalib->dig_P6);

    var2 = var2 + ((var1 * ((s32)pcalib->dig_P5)) << 1);

    var2 = (var2 >> 2) + (((s32)pcalib->dig_P4) << 16);

    var1 = (((pcalib->dig_P3 * (((var1 >> 2) * (var1 >> 2)) >> 13)) >> 3) +
            ((((s32)pcalib->dig_P2) * var1) >> 1)) >> 18;

    var1 = ((((32768 + var1)) * ((s32)pcalib->dig_P1)) >> 15);

    //Avoid exception caused by division by zero
    if (var1 == 0)
        return 0;

    p = (((u32)(((s32)1048576) - pcode) - (var2 >> 12))) * 3125;

    if (p < 0x80000000)
    {
        p = (p << 1) / ((u32)var1);
    }
    else
    {
        p = (p / (u32)var1) * 2;
    }

    var1 = (((s32)pcalib->dig_P9) * ((s32)(((p >> 3) * (p >> 3)) >> 13))) >> 12;

    var2 = (((s32)(p >> 2)) * ((s32)pcalib->dig_P8)) >> 13;

    p = (u32)((s32)p + ((var1 + var2 + pcalib->dig_P7) >> 4));

    return p;
}



/* BMP280 sensor: decode temperature to DegC */
float BMP280DecodeTemperature(SensorCtrl *sen)
{
    float temp;

    //"5123" = 51.23 DegC
    temp = (float)bmp280_compensate_T_int32(bm280_getcalib_ptr(sen), (s32)sen->encValue[1]);

    temp /= 100.0F;

    return temp;
}



/* BMP280 sensor: decode pressure/temperature to Pa/DegC */
float BMP280DecodePressure(SensorCtrl *sen, float *Tout)
{
    float press;
    s32 temp;

    //"96386" = 96386 Pa
    press = (float)bmp280_compensate_P_int32(bm280_getcalib_ptr(sen),
                                             (s32)sen->encValue[1], (s32)sen->encValue[0], &temp);

    if (Tout)
        *Tout = ((float)temp) / 100.0F;

    return press;
}


//---------------------------------------------------------------------------
#elif (__BMP280_CALC_VARIANT == __BMP280_CALC_INT64)

/* Solve "t_fine" value */
s32 bmp280_get_t_fine(BMP280CalibRegs *pcalib, s32 tcode)
{
    s32 var1, var2;

    var1 = ((((tcode >> 3) - ((s32)pcalib->dig_T1 << 1))) * ((s32)pcalib->dig_T2)) >> 11;

    var2 = (((((tcode >> 4) - ((s32)pcalib->dig_T1)) *
              ((tcode >> 4) - ((s32)pcalib->dig_T1))) >> 12) *
            ((s32)pcalib->dig_T3)) >> 14;

    return (s32)(var1 + var2);
}



/* Returns temperature in DegC, resolution is 0.01 DegC.
   Output value of "5123" equals 51.23 DegC.
   t_fine carries fine temperature as global value */
s32 bmp280_compensate_T_int32(BMP280CalibRegs *pcalib, s32 tcode)
{
    return (s32)((bmp280_get_t_fine(pcalib, tcode) * 5 + 128) >> 8);
}



/* Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format
   (24 integer bits and 8 fractional bits).
   Output value of "24674867" represents 24674867/256 = 96386.2 Pa */
u32 bmp280_compensate_P_int64(BMP280CalibRegs *pcalib, s32 tcode, s32 pcode, s32 *Tout)
{
    s64 var1, var2, p;
    s32 t_fine;

    t_fine = bmp280_get_t_fine(pcalib, tcode);

    if (Tout)
        *Tout = (t_fine * 5 + 128) >> 8;

    //Solve pressure
    var1 = ((s64)t_fine) - 128000;

    var2 = var1 * var1 * (s64)pcalib->dig_P6;

    var2 = var2 + ((var1 * (s64)pcalib->dig_P5) << 17);

    var2 = var2 + (((s64)pcalib->dig_P4) << 35);

    var1 = ((var1 * var1 * (s64)pcalib->dig_P3) >> 8) + ((var1 * (s64)pcalib->dig_P2) << 12);

    var1 = (((((s64)1) << 47) + var1)) * ((s64)pcalib->dig_P1) >> 33;

    //Avoid exception caused by division by zero
    if (var1 == 0)
        return 0;

    p = 1048576 - pcode;

    p = (((p << 31) - var2) * 3125) / var1;

    var1 = (((s64)pcalib->dig_P9) * (p >> 13) * (p >> 13)) >> 25;

    var2 = (((s64)pcalib->dig_P8) * p) >> 19;

    p = ((p + var1 + var2) >> 8) + (((s64)pcalib->dig_P7) << 4);

    return (u32)p;
}



/* BMP280 sensor: decode temperature code to DegC */
float BMP280DecodeTemperature(SensorCtrl *sen)
{
    float temp;

    //"5123" = 51.23 DegC
    temp = (float)bmp280_compensate_T_int32(bm280_getcalib_ptr(sen), (s32)sen->encValue[1]);

    temp /= 100.0F;

    return temp;
}



/* BMP280 sensor: decode pressure/temperature to Pa/DegC */
float BMP280DecodePressure(SensorCtrl *sen, float *Tout)
{
    float press;
    u32 pressU32;
    s32 temp;

    //"24674867" = 24674867/256 Pa = 96386.2 Pa
    pressU32 = bmp280_compensate_P_int64(bm280_getcalib_ptr(sen),
                                        (s32)sen->encValue[1], (s32)sen->encValue[0], &temp);

    //Integer part
    press = (float)(pressU32 >> 8);

    //Fractional part
    press += (float)((pressU32 & 0xFF) / 256.0F);

    if (Tout)
        *Tout = ((float)temp) / 100.F;

    return press;
}


//---------------------------------------------------------------------------
#elif (__BMP280_CALC_VARIANT == __BMP280_CALC_DOUBLE)

/* Get t_fine value */
double bmp280_get_t_fine(BMP280CalibRegs *pcalib, s32 tcode)
{
    double var1, var2;

    var1 = (((double)tcode)/16384.0 - ((double)pcalib->dig_T1)/1024.0) * ((double)pcalib->dig_T2);

    var2 = ((((double)tcode)/131072.0 - ((double)pcalib->dig_T1)/8192.0) *
            (((double)tcode)/131072.0 - ((double) pcalib->dig_T1)/8192.0)) * ((double)pcalib->dig_T3);

    return (var1 + var2);
}



/* Returns temperature in DegC, double precision.
   Output value of "51.23" equals 51.23 DegC.
   t_fine carries fine temperature as global value */
double bmp280_compensate_T_double(BMP280CalibRegs *pcalib, s32 tcode)
{    
    return (bmp280_get_t_fine(pcalib, tcode) / 5120.0);
}



/* Returns pressure in Pa as double.
   Output value of "96386.2" equals 96386.2 Pa */
double bmp280_compensate_P_double(BMP280CalibRegs *pcalib, s32 tcode, s32 pcode, double *Tout)
{
    double var1, var2, p;
    s32 t_fine;

    //Solve temperature
    p = bmp280_get_t_fine(pcalib, tcode);

    t_fine = (s32)p;

    if (Tout)
        *Tout = p / 5120.0;

    //Solve pressure
    var1 = ((double)t_fine/2.0) - 64000.0;

    var2 = var1 * var1 * ((double)pcalib->dig_P6) / 32768.0;

    var2 = var2 + var1 * ((double)pcalib->dig_P5) * 2.0;

    var2 = (var2 / 4.0)+(((double)pcalib->dig_P4) * 65536.0);

    var1 = (((double)pcalib->dig_P3) * var1 * var1 / 524288.0 +
            ((double)pcalib->dig_P2) * var1) / 524288.0;

    var1 = (1.0 + var1 / 32768.0)*((double)pcalib->dig_P1);

     //Avoid exception caused by division by zero
    if (var1 == 0.0)
        return 0;

    p = 1048576.0 - (double)pcode;

    p = (p - (var2 / 4096.0)) * 6250.0 / var1;

    var1 = ((double)pcalib->dig_P9) * p * p / 2147483648.0;

    var2 = p * ((double)pcalib->dig_P8) / 32768.0;

    p = p + (var1 + var2 + ((double)pcalib->dig_P7)) / 16.0;

    return p;
}



/* BMP280 sensor: decode temperature code to DegC */
float BMP280DecodeTemperature(SensorCtrl *sen)
{
    float temp;

    //"51.23" = 51.23 DegC
    temp = (float)bmp280_compensate_T_double(bm280_getcalib_ptr(sen), (s32)sen->encValue[1]);

    return temp;
}



/* BMP280 sensor: decode pressure/temperature to Pa/DegC */
float BMP280DecodePressure(SensorCtrl *sen, float *Tout)
{
    float press;
    double temp;

    //"96386.2" = 96386.2 Pa
    press = (float)bmp280_compensate_P_double(bm280_getcalib_ptr(sen),
                                              (s32)sen->encValue[1], (s32)sen->encValue[0],
                                             &temp);

    if (Tout)
        *Tout = (float)temp;

    return press;
}

//---------------------------------------------------------------------------
#else
#error "Error: invalid __BMP280_CALC_VARIANT definition"
#endif


/* BMP280 sensor: convert encoded value to readable string */
char *BMP280ToString(SensorCtrl *sen, char *dest)
{   
    if (sen->encValue[0] == SENSOR_VALUE_NA)
    {
        std_sprintf(dest, "%s\n    -Hg, -C",
                    I2CDecodeError(sen));
    }
    else
    {
        float currPa, currmmHg, currT;

        currPa = BMP280DecodePressure(sen, &currT);

        //1 Pa   = 0.007501 mmHg
        //1 mmHg = 133.321995 Pa
        currmmHg = currPa * 0.007501F;

        std_sprintf(dest, "%.1fPa,\n    %.1fHg, %.1fC",
                          currPa, currmmHg, currT);
    }

    return dest;
}



static s32 bm280_P0 = 0;
static s32 bm280_T0 = 0;

/* Reset leveling */
void BMP280ResetLeveling(SensorCtrl *sen)
{    
    bm280_P0 = 0;
    bm280_T0 = 0;
}



/* Barometric leveling */
float BMP280DoBarometricLeveling(SensorCtrl *sen)
{
    #define PRESS_K     (0.007501F * 10.0F * 2.0F)

    if (!sen ||
        sen->encValue[0] == SENSOR_VALUE_NA)
    {
        bm280_P0 = 0;
        bm280_T0 = 0;
        return 0.0F;
    }
    else
    {
        s32 Ph, Th, dHI32;
        float temp;

        Ph = (s32)(BMP280DecodePressure(sen, &temp) * PRESS_K);

        if (bm280_P0 == 0.0F)
        {
            //Save base pressure and temperature
            bm280_P0 = Ph;
            bm280_T0 = (s32)temp;
            return 0.0F;
        }        

        //Get average temperature
        //Th = ((s32)temp + bm280_T0) >> 1;
        Th = bm280_T0;

        //Original formula (meters):
        //dH = 8000 * 2 * (P0 - Ph) / (P0 + Ph) * (1 + Th / 273)

        //Solve delta height at tenth of cm
        dHI32 = ((bm280_P0 - Ph) * (160000 + (s32)((160000 * Th) / 273))) / (bm280_P0 + Ph);

        //dH at meters, floating point
        return (((float)dHI32)/ 10.0F);
    }
}



/* BMP280 sensor: configure */
s32 BMP280Configure(SensorCtrl *sen)
{    
    BMP280CalibRegs *pcalib;
    const u8 *profile;
    s32 r;
    u8 devaddr, profindex;
    u8 txdata[8];
    u8 rxdata[32];

    profindex = sen->cfg->profile;

    if (profindex > 5)
        profindex = 5;

    //Get pointer to profile
    profile = bmp280_profiles_list[profindex];

    //I2C address
    devaddr = sen->cfg->addr[0];


    //txdata[0]: address + RW=0 (write)
    txdata[0] = (devaddr << 1);

    //profile[1]: reg 0xF5 (config) value;
    //profile[0]: reg 0xF4 (ctrlmeas) value;
    txdata[1] = BMP280_REG_CONFIG;
    txdata[2] = profile[1];

    txdata[3] = BMP280_REG_CTRLMEAS;
    txdata[4] = profile[0];

    //STEP1. Tx configuration to BMP280 and
    //read and check configuration (registers 0xF4, 0xF5)
    if ((r = I2CWrite(I2C_N1, txdata, 5)) == 5 &&
        (r = I2CReadRegister(I2C_N1, devaddr, BMP280_REG_CTRLMEAS, rxdata, 2)) == 2)
    {                                        
        if (!(rxdata[0] == profile[0] && rxdata[1] == profile[1]))
            return I2C_RESULT_CRCERROR;
    }
    else
        return r;

    //STEP2. Read [calib00..calib23] registers
    if ((r = I2CReadRegister(I2C_N1, devaddr, BMP280_REG_CALIB00,
                             rxdata, BMP280_REG_CALIB_COUNT)) == BMP280_REG_CALIB_COUNT)
    {
        //Get pointer to calib. registers
        pcalib = bm280_getcalib_ptr(sen);

        //Save registers to structure
        memcpy(pcalib, rxdata, BMP280_REG_CALIB_COUNT);

        //Print calib. registers (test only)
        PrintMsg("T1: %u\nT2: %i\nT3: %i\n"
                 "P1: %u\nP2: %i\nP3: %i\n"
                 "P4: %i\nP5: %i\nP6: %i\n"
                 "P7: %i\nP8: %i\nP9: %i\n",
                    pcalib->dig_T1, pcalib->dig_T2, pcalib->dig_T3,
                    pcalib->dig_P1, pcalib->dig_P2, pcalib->dig_P3,
                    pcalib->dig_P4, pcalib->dig_P5, pcalib->dig_P6,
                    pcalib->dig_P7, pcalib->dig_P8, pcalib->dig_P9);

        return 0;
    }
    else
        return r;
}



/* BMP280 sensor: restart convertation */
s32 BMP280WriteData(SensorCtrl *sen)
{
    //No actions, continuous convertation
    return 0;
}



/* BMP280 sensor: read data (I2C bus) */
s32 BMP280ReadData(SensorCtrl *sen)
{
    s32 r;
    u8 txdata[2];
    u8 rxdata[8];

    //BMP280 registers (Table 18)
    //Addr Name         Bits
    //0xFC temp_xlsb    xlsb[7..4]  unused[3..0]
    //0xFB temp_lsb     lsb[7..0]
    //0xFA temp_msb     msb[7..0]

    //0xF9 press_xlsb   xlsb[7..4]  unused[3..0]
    //0xF8 press_lsb    lsb[7..0]
    //0xF7 press_msb    msb[7..0]

    //rxdata[0]: I2C address + RW=0 (write)     
    txdata[0] = ((sen->cfg->addr[0]) << 1);

    //I2C address + RW=1 (read)
    rxdata[0] = (txdata[0] | 1);

    //Set start address to pressure register
    txdata[1] = BMP280_REG_PRESSURE;

    if ((r = I2CWrite(I2C_N1, txdata, 2)) == 2 && //Set start address
        (r = I2CRead(I2C_N1,  rxdata, 6)) == 6)   //Read data (autoincrement, press+temp)
    {
        //Pressure code
        sen->encValue[0] = (rxdata[0] << 16) | (rxdata[1] << 8) | (rxdata[2]);

        //Temperature code
        sen->encValue[1] = (rxdata[3] << 16) | (rxdata[4] << 8) | (rxdata[5]);

        //Pressure/temperature code has 20-bit width
        sen->encValue[0] >>= 4;
        sen->encValue[1] >>= 4;

        //Check rx data
        if ((rxdata[2] & 0x0F) != 0 || //low 4 bits must 0
            (rxdata[5] & 0x0F) != 0 || //
            sen->encValue[0] == 0x80000 || //press and temp registers contains
            sen->encValue[1] == 0x80000)   //this value after power on
            return I2C_RESULT_CRCERROR;
        else        
            return 0;
    }
    else
        return r;
}

/*===========================================================================
 * End of file sensor-bmp280.c
 *===========================================================================*/
