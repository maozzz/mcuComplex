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
 *  File sensor-mpu6000.c. Motion Processing Units MPU60X0 functions.
 *                         3-axis gyroscope, 3-axis accelerometer and
 *                         temperature sensor.
 *                         16-bits ADCs.
 *                         I2C interface.
 *                         Vdd = 2.4-3.46V.
 *
 *===========================================================================*/

#include "sensor.h"
#include <math.h>

//Debug messages (comment to disable)
#define PrintMsg(...)       //std_printf(__VA_ARGS__)


//PI
#define PIVAL           3.141592653589793F

//One radian, degrees (rad=180/PI)
#define ONERADIAN       57.29577951308232F

//All registers is 8-bit width
//The reset value is 0x00 for all registers other than the registers below:
//Register 0x6B (power management 1): 0x40 (SLEEP[6]=1)
//Register 0x75 ("Who am i?"):        0x68 (MPU6050 I2C address)

//Register 0x19 (samplerate divider)
//This register specifies the divider from the gyroscope output
//rate used to generate the Sample Rate for the MPU-60X0.
#define MPU6050_SMPLRT_DIV          0x19 //address
//SMPLRT_DIV[7..0]: Sample_Rate = Gyro_Out_Rate / (1 + SMPLRT_DIV),
//where Gyro_Out_Rate = 8kHz when the DLPF is disabled (DLPF_CFG = 0 or 7),
//and 1kHz when the DLPF is enabled


//Register 0x1A (config)
#define MPU6050_CONFIG              0x1A //address
//EXT_SYNC_SET[5..3]: an external signal connected to the FSYNC pin
//can be sampled by configuring EXT_SYNC_SET
#define MPU6050_EXT_SYNC_ACC_ZOUT   (7 << 3)
#define MPU6050_EXT_SYNC_ACC_YOUT   (6 << 3)
#define MPU6050_EXT_SYNC_ACC_XOUT   (5 << 3)
#define MPU6050_EXT_SYNC_GYRO_ZOUT  (4 << 3)
#define MPU6050_EXT_SYNC_GYRO_YOUT  (3 << 3)
#define MPU6050_EXT_SYNC_GYRO_XOUT  (2 << 3)
#define MPU6050_EXT_SYNC_TEMP_OUT   (1 << 3)
#define MPU6050_EXT_SYNC_DISABLED   (0 << 3) //(default)
#define MPU6050_EXT_SYNC_SET_MASK   (7 << 3) //mask
//DLPF_CFG[2..0]: the accelerometer and gyroscope are filtered
//#define MPU6050_DLPF_CFG_x          (7 << 0) //reserved, Fs=8kHz
#define MPU6050_DLPF_CFG_5Hz        (6 << 0) //Fs=1kHz
#define MPU6050_DLPF_CFG_10Hz       (5 << 0) //Fs=1kHz
#define MPU6050_DLPF_CFG_20Hz       (4 << 0) //Fs=1kHz
#define MPU6050_DLPF_CFG_42Hz       (3 << 0) //Fs=1kHz
#define MPU6050_DLPF_CFG_98Hz       (2 << 0) //Fs=1kHz
#define MPU6050_DLPF_CFG_188Hz      (1 << 0) //Fs=1kHz
#define MPU6050_DLPF_CFG_256Hz      (0 << 0) //Fs=8kHz (default)
#define MPU6050_DLPF_CFG_MASK       (7 << 0) //mask


//Register 0x1B (gyroscope configuration)
#define MPU6050_GYRO_CONFIG         0x1B //address
//bits[7..5]: activate self-test
#define MPU6050_XG_ST               (1 << 7) //Self-test by X-axis
#define MPU6050_YG_ST               (1 << 6) //Self-test by Y-axis
#define MPU6050_ZG_ST               (1 << 5) //Self-test by Z-axis
//FS_SEL[4..3]: selects the full scale range of the gyroscope outputs
#define MPU6050_FS_SEL_2000dps      (3 << 3) //+/-2000 deg/s,  16.4 LSB/deg/s
#define MPU6050_FS_SEL_1000dps      (2 << 3) //+/-1000 deg/s,  32.8 LSB/deg/s
#define MPU6050_FS_SEL_500dps       (1 << 3) //+/-500  deg/s,  65.5 LSB/deg/s
#define MPU6050_FS_SEL_250dps       (0 << 3) //+/-250  deg/s, 131.0 LSB/deg/s (default)
#define MPU6050_FS_SEL_MASK         (3 << 3) //mask


//Register 0x1C (accelerometer configuration)
#define MPU6050_ACCEL_CONFIG        0x1C //address
//bits[7..5]: activate self-test
#define MPU6050_XA_ST               (1 << 7) //Self-test by X-axis
#define MPU6050_YA_ST               (1 << 6) //Self-test by Y-axis
#define MPU6050_ZA_ST               (1 << 5) //Self-test by Z-axis
//AFS_SEL[4..3]:  selects the full scale range of the accelerometer outputs
#define MPU6050_AFS_SEL_16g         (3 << 3) //+/-16g,  2048 LSB/g
#define MPU6050_AFS_SEL_8g          (2 << 3) //+/-8g,   4096 LSB/g
#define MPU6050_AFS_SEL_4g          (1 << 3) //+/-4g,   8192 LSB/g
#define MPU6050_AFS_SEL_2g          (0 << 3) //+/-2g,  16384 LSB/g (default)
#define MPU6050_AFS_SEL_MASK        (3 << 3) //mask


//Register 0x23 (enable/disable FIFOs)
#define MPU6050_FIFO_EN             0x23 //address
//bits[7..0]: enables put sensor data to FIFO
#define MPU6050_TEMP_FIFO_EN        (1 << 7) //Enables TEMP_OUT_H and TEMP_OUT_L
#define MPU6050_XG_FIFO_EN          (1 << 6) //Enables GYRO_XOUT_H and GYRO_XOUT_L
#define MPU6050_YG_FIFO_EN          (1 << 5) //Enables GYRO_YOUT_H and GYRO_YOUT_L
#define MPU6050_ZG_FIFO_EN          (1 << 4) //Enables GYRO_ZOUT_H and GYRO_ZOUT_L
#define MPU6050_ACCEL_FIFO_EN       (1 << 3) //Enables ACCEL_X(Y,Z)OUT_H, ACCEL_X(Y,Z)OUT_L,
#define MPU6050_SLV2_FIFO_EN        (1 << 2) //Enables EXT_SENS_DATA of slave 2
#define MPU6050_SLV1_FIFO_EN        (1 << 1) //Enables EXT_SENS_DATA of slave 1
#define MPU6050_SLV0_FIFO_EN        (1 << 0) //Enables EXT_SENS_DATA of slave 0


//Register 0x68 (signal path reset)
#define MPU6050_SIGNAL_PATH_RESET   0x68 //address
//bits[2..0]: when 1, resets the analog/digital signal paths,
//but not clear registers, wait 100ms
#define MPU6050_GYRO_RESET          (1 << 2) //Gyroscope sensor reset
#define MPU6050_ACCEL_RESET         (1 << 1) //Accelerometer sensor reset
#define MPU6050_TEMP_RESET          (1 << 0) //Temperature sensor reset


//Register 0x6A (user control)
#define MPU6050_USER_CTRL           0x6A //address
//bits[6..4,2..0]: flags
#define MPU6050_FIFO_OPER_EN        (1 << 6) //Enables FIFO operations.
#define MPU6050_I2C_MST_EN          (1 << 5) //Enables I2C Master Mode.
#define MPU6050_I2C_IF_DIS          (1 << 4) //MPU-6050: must be zero;
                                             //MPU-6000: 1 - disables I2C, enables SPI.
#define MPU6050_FIFO_RESET          (1 << 2) //Resets the FIFO buffer when set to 1
                                             //while FIFO_EN=0, auto clears.
#define MPU6050_I2C_MST_RESET       (1 << 1) //Resets the I2C Master
                                             //while I2C_MST_EN=0, auto clears.
#define MPU6050_SIG_COND_RESET      (1 << 0) //Resets sensors and registers, auto clears.
                                             //When resetting only the signal path
                                             //(and not the sensor registers) use Reg 0x68


//Register 0x6B (power management 1)
#define MPU6050_PWR_MGMT_1          0x6B //address
//bits[7..3]: flags
#define MPU6050_DEVICE_RESET        (1 << 7) //Resets all internal registers to default values,
                                             //auto clears, wait 100ms
#define MPU6050_SLEEP               (1 << 6) //Sleep mode (set by default)
#define MPU6050_CYCLE               (1 << 5) //Cycle between sleep mode and waking up
                                             //to take a single sample (see LP_WAKE_CTRL)
#define MPU6050_TEMP_DIS            (1 << 3) //Disable temperature sensor
//CLK_SEL[2..0]
#define MPU6050_CLK_SEL_NONE        (7 << 0) //Stops the clock and keeps the generator in reset
//#define MPU6050_CLK_SEL_6           (6 << 0) //reserved
#define MPU6050_CLK_SEL_19200kHz    (5 << 0) //PLL with external 19.2MHz reference
#define MPU6050_CLK_SEL_32768Hz     (4 << 0) //PLL with external 32.768kHz reference
#define MPU6050_CLK_SEL_ZGYRO       (3 << 0) //PLL with Z axis gyroscope reference
#define MPU6050_CLK_SEL_YGYRO       (2 << 0) //PLL with Y axis gyroscope reference
#define MPU6050_CLK_SEL_XGYRO       (1 << 0) //PLL with X axis gyroscope reference (recommended)
#define MPU6050_CLK_SEL_8MHz        (0 << 0) //Internal 8MHz oscillator
#define MPU6050_CLK_SEL_MASK        (7 << 0) //mask


//Register 0x6C (power management 2)
#define MPU6050_PWR_MGMT_2          0x6C //address
//LP_WAKE_CTRL[7..6]: Wake-Up frequency
#define MPU6050_LP_WAKE_CTRL_40Hz   (3 << 6) //40Hz
#define MPU6050_LP_WAKE_CTRL_20Hz   (2 << 6) //20Hz
#define MPU6050_LP_WAKE_CTRL_5Hz    (1 << 6) //5Hz
#define MPU6050_LP_WAKE_CTRL_1p25Hz (0 << 6) //1.25Hz
#define MPU6050_LP_WAKE_CTRL_MASK   (3 << 6) //mask
//bits[5..0]: standby modes
#define MPU6050_STBY_XA             (1 << 5) //X axis accelerometer
#define MPU6050_STBY_YA             (1 << 4) //Y axis accelerometer
#define MPU6050_STBY_ZA             (1 << 3) //Z axis accelerometer
#define MPU6050_STBY_XG             (1 << 2) //X axis gyroscope
#define MPU6050_STBY_YG             (1 << 1) //Y axis gyroscope
#define MPU6050_STBY_ZG             (1 << 0) //Z axis gyroscope


//Registers 0x72, 0x73 (count of bytes at FIFO, high/low parts)
#define MPU6050_FIFO_COUNTH         0x72 //address
#define MPU6050_FIFO_COUNTL         0x73 //address

//Register 0x74 (FIFO data)
#define MPU6050_FIFO_R_W            0x74 //address
//bits[7..0]: FIFO data


//Register 0x75 ("Who am i?")
#define MPU6050_WHO_AM_I            0x75 //address
//This register contains constant value: 0x68 (MPU6050 I2C address)
#define MPU6050_WHO_AM_I_VALUE      0x68 //value


//Sensor outputs
//Registers 0x3B..0x3D (accelerometer output), 16-bit 2's complement values
#define MPU6050_ACCEL_XOUT_H        0x3B
#define MPU6050_ACCEL_XOUT_L        0x3C
#define MPU6050_ACCEL_YOUT_H        0x3D
#define MPU6050_ACCEL_YOUT_L        0x3E
#define MPU6050_ACCEL_ZOUT_H        0x3F
#define MPU6050_ACCEL_ZOUT_L        0x40

//Registers 0x41..0x42 (temperature output)
//Temperature in degrees C = (TEMP_OUT_VAL as a signed quantity)/340 + 36.53
#define MPU6050_TEMP_OUT_H          0x41
#define MPU6050_TEMP_OUT_L          0x42

//Registers 0x43..0x48 (gyroscope output), 16-bit 2's complement values
#define MPU6050_GYRO_XOUT_H         0x43
#define MPU6050_GYRO_XOUT_L         0x44
#define MPU6050_GYRO_YOUT_H         0x45
#define MPU6050_GYRO_YOUT_L         0x46
#define MPU6050_GYRO_ZOUT_H         0x47
#define MPU6050_GYRO_ZOUT_L         0x48


//AFS_SELs
static const u8 mpu6000_AFS_SEL[4] =
{
    MPU6050_AFS_SEL_2g,
    MPU6050_AFS_SEL_4g,
    MPU6050_AFS_SEL_8g,
    MPU6050_AFS_SEL_16g
};


//FS_SELs
static const u8 mpu6000_FS_SEL[4] =
{
    MPU6050_FS_SEL_250dps,
    MPU6050_FS_SEL_500dps,
    MPU6050_FS_SEL_1000dps,
    MPU6050_FS_SEL_2000dps
};


//Accelerator scales
static const float mpu6000AccelScales[4] =
{
    32768.0F /  2.0F,
    32768.0F /  4.0F,
    32768.0F /  8.0F,
    32768.0F / 16.0F
};


//Gyroscope scales
static const float mpu6000GyroScales[4] =
{
    32768.0F /  250.0F,
    32768.0F /  500.0F,
    32768.0F / 1000.0F,
    32768.0F / 2000.0F
};


//Current scales (with 250 deg/s, 2g)
float mpu6000AccelScale = 32768.0F / 2.0F;
float mpu6000GyroScale  = 32768.0F / 250.0F;

//Accelerator offsets
s16 mpu6000AccelOffset[3] = { 230, 50, -10 };

//Gyroscope offsets
s16 mpu6000GyroOffset[3]  = { -471, -233, -138 };


/* MPU60X0 sensor: initialize control structure */
void InitializeMPU6000(SensorCtrl *sen, const SensorConfig *cfg)
{
    memset(sen, 0, sizeof(SensorCtrl));

    //Save pointer to configuration
    sen->cfg = cfg;

    //Set functions
    sen->pfConfigure = &MPU6000Configure;
    sen->pfWriteData = &MPU6000WriteData;
    sen->pfReadData  = &MPU6000ReadData;
    sen->pfToString  = &MPU6000ToString;
}



/* MPU60X0 sensor: decode light code to floating point number */
float MPU6000DecodeTemperature(SensorCtrl *sen)
{    
    float temp;
    s16 tout = (s16)sen->encValue[3];

    //Temperature in degrees C = (TEMP_OUT_VAL as a signed quantity)/340 + 36.53
    temp = ((float)tout) / 340.0F + 36.53F;

    return temp;
}



/* MPU60X0 sensor: convert encoded value to readable string */
char *MPU6000ToString(SensorCtrl *sen, char *dest)
{
    if (sen->encValue[0] == SENSOR_VALUE_NA)
    {
        std_sprintf(dest, "%s", I2CDecodeError(sen));
    }
    else
    {
        //Decode temperature (if need)
        //float temp = MPU6000DecodeTemperature(sen);

        float rotxy[2];

        MPU6000GetRollPitch(sen, rotxy);

        std_sprintf(dest, "Xr=%6.1f, Yr=%6.1f",
                    rotxy[0], rotxy[1]);
    }

    return dest;
}



/* MPU60X0 sensor: reset device */
s32 MPU6000Reset(SensorCtrl *sen)
{
    s32 r;
    u8 daddr;
    u8 pwr_mgmt;

    daddr = sen->cfg->addr[0];

    pwr_mgmt = MPU6050_DEVICE_RESET;

    if ((r = I2CWriteRegister(I2C_N1, daddr, MPU6050_PWR_MGMT_1, &pwr_mgmt, 1)) == 1)
    {
        //Wait ~100ms after reset
        u32 tm = i2cTimerMs + 100;
        while(tm > i2cTimerMs) __NOP();

        return 0;
    }
    else
        return r;
}



/* MPU60X0 sensor: configure */
s32 MPU6000Configure(SensorCtrl *sen)
{    
    s32 r;
    u8 daddr, ind;
    u8 wrcfg[4];
    u8 rdcfg[4];
    u8 pwr_mgmt_1, rdpwr_mgmt;

    daddr = sen->cfg->addr[0];

    //Configuration
    //SLEEP[6]=0: reset sleep bit, work mode;
    //Xgyro as clock source
    pwr_mgmt_1 = MPU6050_CLK_SEL_XGYRO;


    //Samplerate divider, DLPF and gyro/accel config.
    //1000Hz / (1 + 3) = 250Hz  (wrcfg[0])
    wrcfg[0] = 3;

    //DLPF=ON, Gyro Fs = 1000Hz (wrcfg[1])
    wrcfg[1] = MPU6050_DLPF_CFG_42Hz;

    //Gyroscope (wrcfg[2])
    ind              = (sen->cfg->profile) & 0x03;
    wrcfg[2]         = mpu6000_FS_SEL[ind];
    mpu6000GyroScale = mpu6000GyroScales[ind];

    //Accelerator (wrcfg[3])
    ind               = (sen->cfg->profile >> 2) & 0x03;
    wrcfg[3]          = mpu6000_AFS_SEL[ind];
    mpu6000AccelScale = mpu6000AccelScales[ind];


    //Start address "MPU6050_SMPLRT_DIV", write 4 bytes
    if ((r = I2CWriteRegister(I2C_N1, daddr, MPU6050_PWR_MGMT_1, &pwr_mgmt_1, 1)) == 1 &&
        (r = I2CWriteRegister(I2C_N1, daddr, MPU6050_SMPLRT_DIV, wrcfg,       4)) == 4)
    {
        //Check configuration
        if ((r = I2CReadRegister(I2C_N1, daddr, MPU6050_PWR_MGMT_1, &rdpwr_mgmt, 1)) == 1 &&
            (r = I2CReadRegister(I2C_N1, daddr, MPU6050_SMPLRT_DIV, rdcfg,       4)) == 4)
        {
            //std_printf("RD %02X, %02X %02X %02X %02X\n",
            //           rdpwr_mgmt, rdcfg[0], rdcfg[1], rdcfg[2], rdcfg[3]);

            if (rdpwr_mgmt == pwr_mgmt_1 &&
                memcmp(rdcfg, wrcfg, 4) == 0)
                return 0;
            else                            
                return I2C_RESULT_CRCERROR;            
        }
        else
            return r;
    }
    else
        return r;
}



/* MPU60X0 sensor: restart convertation */
s32 MPU6000WriteData(SensorCtrl *sen)
{
    //No actions
    return 0;
}



/* MPU60X0 sensor: read data using I2C bus */
s32 MPU6000ReadData(SensorCtrl *sen)
{
    s32 r;
    u8 rxdata[14];

    //Start read from "ACCEL_XOUT_H" register, autoincrement
    if ((r = I2CReadRegister(I2C_N1, sen->cfg->addr[0],
                             MPU6050_ACCEL_XOUT_H, rxdata, 14)) == 14)
    {
        //Accelerometer output
        sen->encValue[0] = (rxdata[0] << 8) | rxdata[1]; //Xaccel
        sen->encValue[1] = (rxdata[2] << 8) | rxdata[3]; //Yaccel
        sen->encValue[2] = (rxdata[4] << 8) | rxdata[5]; //Zaccel

        //Temperature output
        sen->encValue[3] = (rxdata[6] << 8) | rxdata[7];

        //Gyroscope output
        sen->encValue[4] = (rxdata[8] << 8)  | rxdata[9];  //Xgyro
        sen->encValue[5] = (rxdata[10] << 8) | rxdata[11]; //Ygyro
        sen->encValue[6] = (rxdata[12] << 8) | rxdata[13]; //Zgyro

        return 0;
    }
    else
        return r;    
}



/* Get roll/pitch (rotate about X/Y axis).
   Output: dest[0] - roll; dest[1] - pitch */
void MPU6000GetRollPitch(SensorCtrl *sen, float *dest)
{
    s16 xa, ya, za;
    float xaf, yaf, zaf;

    //encValue[0..2]: accelerator x,y,z
    xa = (s16)sen->encValue[0] - mpu6000AccelOffset[0];
    ya = (s16)sen->encValue[1] - mpu6000AccelOffset[1];
    za = (s16)sen->encValue[2] - mpu6000AccelOffset[2];

    xaf = xa / mpu6000AccelScale;
    yaf = ya / mpu6000AccelScale;
    zaf = za / mpu6000AccelScale;

    //Test only
    //std_printf("A %5i %5i %5i ", xa, ya, za);

    //roll: (tilt left/right, about X axis)
    dest[0] = atan(yaf / sqrt(xaf * xaf + zaf * zaf)) * ONERADIAN;

    if (zaf < 0.0F)
    {
        if (yaf < 0.0F)
            dest[0] = -180.0F - dest[0];
        else
            dest[0] =  180.0F - dest[0];
    }

    //pitch: (nose up/down, about Y axis)
    dest[1] = atan(xaf / sqrt(yaf * yaf + zaf * zaf)) * (-ONERADIAN);


    //yaw: (about Z axis)
    //dest[2] = atan2(2 * qx * qy - 2 * qw * qz, 2 * qw * qw + 2 * qx * qx - 1);
}



/* MPU60X0 sensor: get gyroscope offsets */
void MPU6000GetGyroOffsets(SensorCtrl *sen, s16 *dest)
{
    #define RDCOUNT     100
    s32 i;
    s32 goff[3];
    s16 xg, yg, zg;

    goff[0] = 0;
    goff[1] = 0;
    goff[2] = 0;

    for(i = 0; i < RDCOUNT; i++)
    {
        if (MPU6000ReadData(sen) == 0)
        {
            //encValue[4..6]: gyroscope x,y,z
            xg = (s16)sen->encValue[4];
            yg = (s16)sen->encValue[5];
            zg = (s16)sen->encValue[6];

            goff[0] += (s32)xg;
            goff[1] += (s32)yg;
            goff[2] += (s32)zg;
        }
    }

    dest[0] = (s16)(goff[0] / RDCOUNT);
    dest[1] = (s16)(goff[1] / RDCOUNT);
    dest[2] = (s16)(goff[2] / RDCOUNT);

    #undef RDCOUNT
}



/* MPU60X0 sensor: get gyroscope offsets */
void MPU6000GetAccelOffsets(SensorCtrl *sen, s16 *dest)
{
    #define RDCOUNT     10000
    #define MINGYRO     2.0F
    s32 i, aoffmin[3], aoffmax[3];
    s16 xg, yg, zg, xa, ya, za;
    float xgf, ygf, zgf;

    aoffmin[0] = aoffmin[1] = aoffmin[2] =  32767;
    aoffmax[0] = aoffmax[1] = aoffmax[2] = -32768;

    for(i = 0; i < RDCOUNT; i++)
    {
        if (MPU6000ReadData(sen) == 0)
        {
            //encValue[4..6]: gyroscope x,y,z
            xg = (s16)sen->encValue[4] - mpu6000GyroOffset[0];
            yg = (s16)sen->encValue[5] - mpu6000GyroOffset[1];
            zg = (s16)sen->encValue[6] - mpu6000GyroOffset[2];

            xgf = (float)xg / mpu6000GyroScale;
            ygf = (float)yg / mpu6000GyroScale;
            zgf = (float)zg / mpu6000GyroScale;

            if (xgf < MINGYRO && xgf > -MINGYRO &&
                ygf < MINGYRO && ygf > -MINGYRO &&
                zgf < MINGYRO && zgf > -MINGYRO)
            {
                //encValue[0..2]: accelerator x,y,z
                xa = (s16)sen->encValue[0];
                ya = (s16)sen->encValue[1];
                za = (s16)sen->encValue[2];

                //Save max
                if (aoffmax[0] < xa) aoffmax[0] = xa;
                if (aoffmax[1] < ya) aoffmax[1] = ya;
                if (aoffmax[2] < za) aoffmax[2] = za;

                //Save min
                if (aoffmin[0] > xa) aoffmin[0] = xa;
                if (aoffmin[1] > ya) aoffmin[1] = ya;
                if (aoffmin[2] > za) aoffmin[2] = za;
            }
        }
    }

    //Average
    dest[0] = (s16)((aoffmin[0] + aoffmax[0]) >> 1);
    dest[1] = (s16)((aoffmin[1] + aoffmax[1]) >> 1);
    dest[2] = (s16)((aoffmin[2] + aoffmax[2]) >> 1);

    #undef RDCOUNT
}

/*===========================================================================
 * End of file sensor-mpu6000.c
 *===========================================================================*/
