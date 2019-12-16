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
 *  File sensor-dht12.c. Temperature/humidity sensor DHT12 functions.
 *                       I2C interface.
 *                       Vdd   = 2.7-5.5V.
 *                       Temp. = -20..+60 DegC, res 0.1  DegC.
 *                       Hum.  = 20..95% RH,    res 0.1% RH.
 *                       Conversion time ~2000 msec.
 *
 *===========================================================================*/

#include "sensor.h"

//Debug messages (comment to disable)
#define PrintMsg(...)       //std_printf(__VA_ARGS__)


//Registers
//Addr
//00   Humidity integer part
//01   Humidity fractional part (tenth of %RH)
//02   Temperature integer part
//03   Temperature fractional part (tenth of DegC)
//04   Checksum: byte0+byte1+byte2+byte3

/* DHT12: initialize control structure */
void InitializeDHT12(SensorCtrl *sen, const SensorConfig *cfg)
{    
    memset(sen, 0, sizeof(SensorCtrl));

    //Save pointer to configuration
    sen->cfg = cfg;

    //Set functions
    sen->pfConfigure = &DHT12Configure;
    sen->pfWriteData = &DHT12WriteData;
    sen->pfReadData  = &DHT12ReadData;
    sen->pfToString  = &DHT12ToString;
}


/* DHT12 sensor: decode humidity code to floating point number */
float DHT12DecodeHumidity(SensorCtrl *sen)
{
    float hum;

    hum = ((float)(sen->encValue[0] >> 8)) +
          ((float)(sen->encValue[0] & 0xFF)) * 0.1F;

    return hum;
}



/* DHT12 sensor: decode temperature code to floating point number */
float DHT12DecodeTemperature(SensorCtrl *sen)
{
    float temp;

    temp = ((float)(sen->encValue[1] >> 8)) +
           ((float)(sen->encValue[1] & 0x7F)) * 0.1F;

    //Negative temperature
    if (sen->encValue[1] & 0x80)
        temp = -1.0F * temp;

    return temp;
}



/* DHT12 sensor: convert encoded value to readable string */
char *DHT12ToString(SensorCtrl *sen, char *dest)
{   
    if (sen->encValue[0] == SENSOR_VALUE_NA)
    {
        std_sprintf(dest, "%s", I2CDecodeError(sen));
    }
    else
    {
        std_sprintf(dest, "%3.1fC %2.1f%%",
                           DHT12DecodeTemperature(sen),
                           DHT12DecodeHumidity(sen));
    }

    return dest;
}



/* DHT12 sensor: configure */
s32 DHT12Configure(SensorCtrl *sen)
{
    //No actions
    return 0;
}



/* DHT12 sensor: restart convertation */
s32 DHT12WriteData(SensorCtrl *sen)
{
    //No actions
    return 0;
}



/* DHT12 sensor: read data using I2C bus */
s32 DHT12ReadData(SensorCtrl *sen)
{
    s32 r;    
    u8 rxdata[5];

    //DHT12 has one constant address 0x5C

    if ((r = I2CReadRegister(I2C_N1, DHT12_CADDR0, 0x00, rxdata, 5)) == 5)
    {        
        //Read success, check data
        if ((u8)(rxdata[0] + rxdata[1] + rxdata[2] + rxdata[3]) == (u8)rxdata[4])
        {
            //Humidity
            sen->encValue[0] = (rxdata[0] << 8) | rxdata[1];

            //Temperature
            sen->encValue[1] = (rxdata[2] << 8) | rxdata[3];            
            return 0;
        }
        else
        {
            return I2C_RESULT_CRCERROR;
        }
    }
    else //Read data fail
        return r;
}

/*===========================================================================
 * End of file sensor-dht12.c
 *===========================================================================*/
