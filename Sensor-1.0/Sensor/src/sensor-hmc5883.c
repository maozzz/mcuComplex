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
 *  File sensor-hmc5883.c. 3-Axis Digital Compass HMC5883L functions.
 *                         I2C interface.
 *                         Vdd = 2.16-3.6V.
 *
 *===========================================================================*/

#include "sensor.h"

//Debug messages (comment to disable)
#define PrintMsg(...)       //std_printf(__VA_ARGS__)



/* HMC5883: initialize control structure */
void InitializeHMC5883(SensorCtrl *sen, const SensorConfig *cfg)
{
    memset(sen, 0, sizeof(SensorCtrl));

    //Save pointer to configuration
    sen->cfg = cfg;

    //Set functions
    sen->pfConfigure = &HMC5883Configure;
    sen->pfWriteData = &HMC5883WriteData;
    sen->pfReadData  = &HMC5883ReadData;
    sen->pfToString  = &HMC5883ToString;
}



/* HMC5883 sensor: decode light code to floating point number */
float HMC5883DecodeData(SensorCtrl *sen)
{
    return 0;
}



/* HMC5883 sensor: convert encoded value to readable string */
char *HMC5883ToString(SensorCtrl *sen, char *dest)
{
    if (sen->encValue[0] == SENSOR_VALUE_NA)
    {
        std_sprintf(dest, "%s", I2CDecodeError(sen));
    }
    else
    {
        std_sprintf(dest, "X=%.1f, Y=0.0, Z=0.0", HMC5883DecodeData(sen));
    }

    return dest;
}



/* HMC5883 sensor: configure */
s32 HMC5883Configure(SensorCtrl *sen)
{
    return 0;

    /*
    //s32 r;
    //u8 txdata[2];

    //txdata[0]: I2C address + RW=0 (write)
    //txdata[0] = ((sen->cfg->addr[0]) << 1);

    //
    //txdata[1] = ...;

    //Tx mode to device
    if ((r = I2CWrite(I2C_N1, txdata, 2)) == 2)
        return 0;
    else
        return r;*/
}



/* HMC5883 sensor: restart convertation */
s32 HMC5883WriteData(SensorCtrl *sen)
{
    //No actions
    return 0;
}



/* HMC5883 sensor: read data using I2C bus */
s32 HMC5883ReadData(SensorCtrl *sen)
{
    return 0;

    //register pointer increment by read
    /*
    //s32 r;
    u8 rxdata[2];

    //rxdata[0]: I2C address + RW=1 (read)
    rxdata[0] = ((sen->cfg->addr[0]) << 1) | 1;

    if ((r = I2CRead(I2C_N1, rxdata, 2)) == 2)
    {
        //Success
        sen->encValue[0] = ...;

        if (sen->encValue[0] == ...)
            return I2C_RESULT_CRCERROR;
        else
            return 0;
    }
    else //Read data fail
        return r;*/
}

/*===========================================================================
 * End of file sensor-hmc5883.c
 *===========================================================================*/
