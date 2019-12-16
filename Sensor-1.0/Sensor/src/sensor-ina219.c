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
 *  File sensor-ina219.c. Voltage/current monitor INA219 functions.
 *                        I2C interface.
 *                        Vdd = 3.0-5.5V.
 *
 *===========================================================================*/

#include "sensor.h"

//Debug messages (comment to disable)
#define PrintMsg(...)       //std_printf(__VA_ARGS__)


//---------------------------------------------------------------------------
//INA219 registers:
//Configuration Register (read/write), default value 0x399F
#define INA219_REG_CONFIG       0x00 //address

//Reset bit: 1 - reset to default, self-clears
#define INA219_RESETBIT         (1 << 15)

//BRNG: Bus Voltage Range
#define INA219_BRNG_16V         (0 << 13) //16V FSR
#define INA219_BRNG_32V         (1 << 13) //32V FSR (default)

//PG: Shunt Voltage Range
#define INA219_PG_40mV          (0 << 11) //gain /1, +/-40mV
#define INA219_PG_80mV          (1 << 11) //gain /2, +/-80mV
#define INA219_PG_160mV         (2 << 11) //gain /4, +/-160mV
#define INA219_PG_320mV         (3 << 11) //gain /8, +/-320mV (default)
#define INA219_PG_MASK          (3 << 11) //mask

//BADC: Bus ADC resolution or set the number of samples
//used when averaging results of Bus Voltage Register (address 0x02)
#define INA219_BADC_9BIT        (0 << 7)  // 9bit  84us
#define INA219_BADC_10BIT       (1 << 7)  //10bit 148us
#define INA219_BADC_11BIT       (2 << 7)  //11bit 276us
#define INA219_BADC_12BIT       (3 << 7)  //12bit 532us (default)
//#define INA219_BADC_12BIT_2     (8 << 7)//12bit 532us
//Note: averaging use 12-bit resolution
#define INA219_BADC_AVG2        (9  << 7) //2   samples  1.06ms = 532us x 2
#define INA219_BADC_AVG4        (10 << 7) //4   samples  2.13ms = 532us x 4
#define INA219_BADC_AVG8        (11 << 7) //8   samples  4.26ms
#define INA219_BADC_AVG16       (12 << 7) //16  samples  8.51ms
#define INA219_BADC_AVG32       (13 << 7) //32  samples 17.02ms
#define INA219_BADC_AVG64       (14 << 7) //64  samples 34.05ms
#define INA219_BADC_AVG128      (15 << 7) //128 samples 68.10ms = 532us x 128
#define INA219_BADC_MASK        (15 << 7) //mask

//SADC: Shunt ADC resolution or set the number of samples
//used when averaging results of Shunt Voltage Register (address 0x02)
#define INA219_SADC_9BIT        (0 << 3)  // 9bit  84us
#define INA219_SADC_10BIT       (1 << 3)  //10bit 148us
#define INA219_SADC_11BIT       (2 << 3)  //11bit 276us
#define INA219_SADC_12BIT       (3 << 3)  //12bit 532us (default)
//#define INA219_SADC_12BIT_2     (8 << 3)//12bit 532us
//Note: averaging use 12-bit resolution
#define INA219_SADC_AVG2        (9  << 3) //2   samples  1.06ms = 532us x 2
#define INA219_SADC_AVG4        (10 << 3) //4   samples  2.13ms = 532us x 4
#define INA219_SADC_AVG8        (11 << 3) //8   samples  4.26ms
#define INA219_SADC_AVG16       (12 << 3) //16  samples  8.51ms
#define INA219_SADC_AVG32       (13 << 3) //32  samples 17.02ms
#define INA219_SADC_AVG64       (14 << 3) //64  samples 34.05ms
#define INA219_SADC_AVG128      (15 << 3) //128 samples 68.10ms = 532us x 128
#define INA219_SADC_MASK        (15 << 3) //mask

//MODE: work/sleep mode
#define INA219_MODE_POWERDOWN   (0 << 0)  //Power-down
#define INA219_MODE_TRIG_VSHUNT (1 << 0)  //Shunt voltage, triggered
#define INA219_MODE_TRIG_VBUS   (2 << 0)  //Bus voltage, triggered
#define INA219_MODE_TRIG_VALL   (3 << 0)  //Shunt and bus, triggered
#define INA219_MODE_ADC_OFF     (4 << 0)  //ADC off (disabled)
#define INA219_MODE_CONT_VSHUNT (5 << 0)  //Shunt voltage, continuous
#define INA219_MODE_CONT_VBUS   (6 << 0)  //Bus voltage, continuous
#define INA219_MODE_CONT_VALL   (7 << 0)  //Shunt and bus, continuous (default)
#define INA219_MODE_MASK        (7 << 0)  //mask

//---------------------------------------------------------------------------
//User-defined configuration
//INA219 configuration
#define INA219_REG_CONFIG_VALUE  (INA219_BRNG_32V|\
                                  INA219_PG_80mV|\
                                  INA219_BADC_AVG16|\
                                  INA219_SADC_AVG16|\
                                  INA219_MODE_CONT_VALL)

//Rshunt on board (CJMCU ev.board with Rshunt=0.1 Ohm)
#define INA219_RSHUNT               0.1F

//Requested current resolution, A
#define INA219_CURRENT_LSB          0.001F

//---------------------------------------------------------------------------
//Shunt Voltage Register (read only)
//Signed 16 bit integer (2's complement format for negative),
//LSB is 0.01mV
#define INA219_REG_VSHUNT       0x01 //address

//---------------------------------------------------------------------------
//Bus Voltage Register (read only)
//Value at bits[15..3], LSB=4mV
#define INA219_REG_VBUS         0x02 //address
//Conversion ready, cleared by reading Power register or writing MODE
#define INA219_CONV_READY
//Overflow flag: Power or Current are out of range
#define INA219_VBUS_OVF         (1 << 0)

//---------------------------------------------------------------------------
//Power Register (read only), default value 0
//Unsigned 16-bit integer, LSB = 20 * Current_LSB
#define INA219_REG_POWER        0x03 //address
//Power register resolution, W (depends from Current_LSB)
#define INA219_POWER_LSB       (20.0F * INA219_CURRENT_LSB)

//---------------------------------------------------------------------------
//Current Register (read only), default value 0
//LSB defined by Calibration Register; bit[15] is sign
#define INA219_REG_CURRENT      0x04 //address

//---------------------------------------------------------------------------
//Calibration Register (read/write), default value 0.
#define INA219_REG_CALIB        0x05 //address
//Calib = 0.04096 / (Rshunt * Current_LSB); bit[0] must always be 0
#define INA219_REG_CALIB_VALUE  (((u16)(0.04096F / (INA219_RSHUNT * INA219_CURRENT_LSB))) & 0xFFFEU)


/* INA219: initialize control structure */
void InitializeINA219(SensorCtrl *sen, const SensorConfig *cfg)
{
    memset(sen, 0, sizeof(SensorCtrl));

    //Save pointer to configuration
    sen->cfg = cfg;

    //Set functions
    sen->pfConfigure = &INA219Configure;
    sen->pfWriteData = &INA219WriteData;
    sen->pfReadData  = &INA219ReadData;
    sen->pfToString  = &INA219ToString;
}



/* INA219 sensor: decode Vshunt to mV */
float INA219DecodeVshunt(SensorCtrl *sen)
{
    float vshunt;
    u32 encv = sen->encValue[0];

    //All 16 bits used
    //LSB=0.01mV
    if (encv & PN15)
    {
        //Negative value
        encv = (~encv + 1) & 0xFFFFU;
        vshunt = -((float)encv) * 0.01F;
    }
    else
    {
        //Positive value
        vshunt = ((float)encv) * 0.01F;
    }

    return vshunt;
}



/* INA219 sensor: decode Vbus to volts */
float INA219DecodeVbus(SensorCtrl *sen)
{
    float vbus;
    u32 encv = sen->encValue[1];

    //Power/Current overflow, set N/A
    if (encv & INA219_VBUS_OVF)
    {
        sen->encValue[2] = SENSOR_VALUE_NA; //Power
        sen->encValue[3] = SENSOR_VALUE_NA; //Current
    }

    //Value at bits[15..3], bits[2..0] is 0    
    encv >>= 3;

    //LSB=4mV
    vbus = ((float)encv) * 0.004F;

    return vbus;
}



/* INA219 sensor: decode power code to mW */
float INA219DecodePower(SensorCtrl *sen)
{
    float pow;
    u32 encp = sen->encValue[2];

    pow = ((float)encp) * INA219_POWER_LSB;

    return pow;
}



/* INA219 sensor: decode current code to A */
float INA219DecodeCurrent(SensorCtrl *sen)
{
    float curr;
    u32 enci = sen->encValue[3];

    if (enci & PN15)
    {
        //Negative value
        enci = (~enci + 1) & 0xFFFFU;
        curr = -((float)enci) * INA219_CURRENT_LSB;
    }
    else
    {
        //Positive value
        curr = ((float)enci) * INA219_CURRENT_LSB;
    }

    return curr;
}




/* INA219 sensor: convert encoded value to readable string */
char *INA219ToString(SensorCtrl *sen, char *dest)
{   
    if (sen->encValue[0] == SENSOR_VALUE_NA)
    {
        std_sprintf(dest, "%s - mV\n"
                         " I  - A - W",
                    I2CDecodeError(sen));
    }
    else
    {
        char strPower[32];
        char strCurrent[32];

        if (sen->encValue[1] & INA219_VBUS_OVF)
        {
            //Power/Current overflow, print N/A value
            strcpy(strCurrent, "---");
            strcpy(strPower,   "---");
        }
        else
        {
            //Valid value
            std_sprintf(strCurrent, "%.3f", INA219DecodeCurrent(sen));
            std_sprintf(strPower,   "%.2f", INA219DecodePower(sen));
        }

        std_sprintf(dest, "%.3fV, %.1fmV,\n"
                          " I=%sA, P=%sW",
                          INA219DecodeVbus(sen),
                          INA219DecodeVshunt(sen),
                          strCurrent,
                          strPower);
    }

    return dest;
}



/* INA219 sensor: write 16-bit register then read and check value */
static s32 INA219WriteRegister(SensorCtrl *sen, u8 regaddr, u16 regval)
{
    s32 r;
    u8 devaddr;
    u8 txval[2];
    u8 rxval[2];

    //Select device address
    devaddr = sen->cfg->addr[0];

    //Register value: major byte first
    txval[0] = (u8)(regval >> 8);
    txval[1] = (u8)(regval);

    if ((r = I2CWriteRegister(I2C_N1, devaddr, regaddr, txval, 2)) == 2 &&
        (r = I2CReadRegister (I2C_N1, devaddr, regaddr, rxval, 2)) == 2)
    {
        if (txval[0] != rxval[0] || txval[1] != rxval[1])
            r = I2C_RESULT_CRCERROR;
    }

    return r;
}



/* INA219 sensor: configure */
s32 INA219Configure(SensorCtrl *sen)
{
    s32 r;

    //STEP 1. Set and check configuration
    if ((r = INA219WriteRegister(sen, INA219_REG_CONFIG, INA219_REG_CONFIG_VALUE)) == 2 &&
        (r = INA219WriteRegister(sen, INA219_REG_CALIB,  INA219_REG_CALIB_VALUE))  == 2)
    {
        PrintMsg("INA219: CFG OK\n");
        return 0;
    }
    else
    {
        PrintMsg("INA219: CFG FAIL\n");
        return r;
    }
}



/* INA219 sensor: restart convertation */
s32 INA219WriteData(SensorCtrl *sen)
{
    //No actions
    return 0;
}



/* INA219 sensor: read data using I2C bus */
s32 INA219ReadData(SensorCtrl *sen)
{
    s32 r;
    u16 calib;
    u8 devaddr;
    u8 rxdata0[2];
    u8 rxdata1[2];
    u8 rxdata2[2];
    u8 rxdata3[2];
    u8 rxdata4[2];

    devaddr = sen->cfg->addr[0];

    if ((r = I2CReadRegister(I2C_N1, devaddr, INA219_REG_VSHUNT,  rxdata0, 2)) == 2 &&
        (r = I2CReadRegister(I2C_N1, devaddr, INA219_REG_VBUS,    rxdata1, 2)) == 2 &&
        (r = I2CReadRegister(I2C_N1, devaddr, INA219_REG_POWER,   rxdata2, 2)) == 2 &&
        (r = I2CReadRegister(I2C_N1, devaddr, INA219_REG_CURRENT, rxdata3, 2)) == 2 &&
        (r = I2CReadRegister(I2C_N1, devaddr, INA219_REG_CALIB,   rxdata4, 2)) == 2)
    {                
        calib = (rxdata4[0] << 8) | rxdata4[1];

        //Check calibration register value
        if (calib == INA219_REG_CALIB_VALUE)
        {
            sen->encValue[0] = (rxdata0[0] << 8) | rxdata0[1]; //Vshunt
            sen->encValue[1] = (rxdata1[0] << 8) | rxdata1[1]; //Vbus
            sen->encValue[2] = (rxdata2[0] << 8) | rxdata2[1]; //Power
            sen->encValue[3] = (rxdata3[0] << 8) | rxdata3[1]; //Current
            return 0;
        }
        else
            return I2C_RESULT_CRCERROR;
    }
    else
        return r;
}

/*===========================================================================
 * End of file sensor-ina219.c
 *===========================================================================*/
