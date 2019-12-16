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
 *  File adc-pcf8591.c. 8-bit A/D and D/A converter PCF8591 functions.
 *                      I2C interface.
 *                      Vdd = 2.5-6.0V.
 *                      4 channel ADC.
 *                      1 channel DAC.
 *
 *===========================================================================*/

#include "sensor.h"

//Debug messages (comment to disable)
#define PrintMsg(...)       //std_printf(__VA_ARGS__)


//Control byte
//Selected A/D channel
#define PCF8591_AIN0            (0 << 0) //A/D channel 0
#define PCF8591_AIN1            (1 << 0) //A/D channel 1
#define PCF8591_AIN2            (2 << 0) //A/D channel 2
#define PCF8591_AIN3            (3 << 0) //A/D channel 3

//Auto-increment flag
#define PCF8591_AUTOINC         (1 << 2)

//Analog input programming
//Four single ended inputs: AIN0, AIN1, AIN2, AIN3
#define PCF8591_APROG_4SINGLE   (0 << 4)
//Three differential inputs: [AIN0+,AIN3-], [AIN1+,AIN3-] and [AIN2+,AIN3-]
#define PCF8591_APROG_3DIFF     (1 << 4)
//Two single and differetial inputs: AIN0, AIN1 and [AIN2+,AIN3-]
#define PCF8591_APROG_SINDIFF   (2 << 4)
//Two differential inputs: [AIN0+,AIN1-] and [AIN2+,AIN3-]
#define PCF8591_APROG_2DIFF     (3 << 4)

//Analog output enable (AOUT active)
#define PCF8591_AOUT_ENABLE     (1 << 6)



//User-defined configuration
#define PCF8591_CTRL_VALUE      (PCF8591_AIN0|\
                                 PCF8591_APROG_4SINGLE|\
                                 PCF8591_AOUT_ENABLE)


/* PCF8591: initialize control structure */
void InitializePCF8591(SensorCtrl *sen, const SensorConfig *cfg)
{    
    memset(sen, 0, sizeof(SensorCtrl));

    //Save pointer to configuration
    sen->cfg = cfg;

    //Set functions
    sen->pfConfigure = &PCF8591Configure;
    sen->pfWriteData = &PCF8591WriteData;
    sen->pfReadData  = &PCF8591ReadData;
    sen->pfToString  = &PCF8591ToString;
}



/* PCF8591: decode analog input 0 */
float PCF8591DecodeAIN0(SensorCtrl *sen)
{
    float ain;
    ain = (float)((sen->encValue[0] >> 8) & 0xFF);
    return ain;
}



/* PCF8591: decode analog input 1 */
float PCF8591DecodeAIN1(SensorCtrl *sen)
{
    float ain;
    ain = (float)((sen->encValue[0] >> 0) & 0xFF);
    return ain;
}



/* PCF8591: decode analog input 2 */
float PCF8591DecodeAIN2(SensorCtrl *sen)
{
    float ain;
    ain = (float)((sen->encValue[1] >> 8) & 0xFF);
    return ain;
}



/* PCF8591: decode analog input 3
   YL-40 eval.board: 0-10 kOhm var.resistor */
float PCF8591DecodeAIN3(SensorCtrl *sen)
{
    s32 ain;
    float res;

    ain = (sen->encValue[1] & 0xFF);

    //AIN3: [0..255] => [10..0] kOhm
    res = ((float)(((255 - ain) * 100) >> 8)) / 10.0F;

    return res;
}



/* PCF8591: convert encoded value to readable string */
char *PCF8591ToString(SensorCtrl *sen, char *dest)
{   
    if (sen->encValue[0] == SENSOR_VALUE_NA)
    {
        std_sprintf(dest, "%s\n"
                    " A1 -\n"
                    " A2 -\n"
                    " A3 -",
                    I2CDecodeError(sen));
    }
    else
    {
        //NOTE. For eval.board YL-40:
        //AIN0: photo resistor
        //AIN1: not connected
        //AIN2: termistor
        //AIN3: variable resistor 10 kOhm
        std_sprintf(dest,
                        "A0=%2.1f, \n"
                    "    A1=%2.1f, \n"
                    "    A2=%2.1f, \n"
                    "    A3=%2.1fkOhm",
                    PCF8591DecodeAIN0(sen),
                    PCF8591DecodeAIN1(sen),
                    PCF8591DecodeAIN2(sen),
                    PCF8591DecodeAIN3(sen));
    }

    return dest;
}



/* PCF8591 sensor: configure */
s32 PCF8591Configure(SensorCtrl *sen)
{    
    //No configuration data, write zero sample to DAC
    s32 r;
    u8 txdata[4];

    sen->encValue[2] = 0x80;

    //txdata[0]: I2C address + RW=0 (write)
    txdata[0] = ((sen->cfg->addr[0]) << 1);

    //Control byte
    txdata[1] = PCF8591_CTRL_VALUE;

    //DAC sample
    txdata[2] = sen->encValue[2];

    if ((r = I2CWrite(I2C_N1, txdata, 3)) == 3)
        return 0;
    else
        return r;
}



/* PCF8591: write sample to DAC (AOUT) */
s32 PCF8591WriteData(SensorCtrl *sen)
{        
    s32 r;
    u16 smp, dir;
    u8 txdata[4];

    smp = sen->encValue[2] & 0xFF;
    dir = (sen->encValue[2] >> 8) & 0xFF;

    //txdata[0]: I2C address + RW=0 (write)
    txdata[0] = ((sen->cfg->addr[0]) << 1);

    //Control byte
    txdata[1] = PCF8591_CTRL_VALUE;

    //DAC sample
    txdata[2] = (u8)smp;

    if ((r = I2CWrite(I2C_N1, txdata, 3)) == 3)
    {
        //Increment DAC value
        if (dir)
        {
            smp -= 5;
            if (smp < 0x80)
            {
                smp = 0x80;
                dir = 0;
            }
        }
        else
        {
            smp += 5;
            if (smp > 0xFF)
            {
                smp = 0xFF;
                dir = 1;
            }
        }

        sen->encValue[2] = ((dir << 8) | smp);
        return 0;
    }
    else
        return r;
}



/* Read specified ADC channel */
static s32 PCF8591ReadAINx(SensorCtrl *sen, u8 chnum, u8 *ainx)
{
    s32 r;
    u8 txdata[4];
    u8 rxdata[4];

    //txdata[0]: I2C address + RW=0 (write)
    txdata[0] = ((sen->cfg->addr[0]) << 1);

    //Control byte: select channel AIN0..AIN3
    txdata[1] = (chnum & 0x03) | PCF8591_APROG_4SINGLE | PCF8591_AOUT_ENABLE;

    //DAC sample
    txdata[2] = (u8)(sen->encValue[2] & 0xFF);

    //rxdata[0]: I2C address + RW=1 (read)
    rxdata[0] = txdata[0] | 1;

    if ((r = I2CWrite(I2C_N1, txdata, 3)) == 3 &&
        (r = I2CRead (I2C_N1, rxdata, 2)) == 2)
    {
        *ainx = rxdata[1];
        return 0;
    }
    else
    {

        *ainx = 0;
        return r;
    }
}



/* */
s32 PCF8591ReadData(SensorCtrl *sen)
{
    s32 r;
    u8 ain0, ain1, ain2, ain3;

    //Read all analog inputs (AIN0..AIN3)
    if ((r = PCF8591ReadAINx(sen, 0, &ain0)) == 0 &&
        (r = PCF8591ReadAINx(sen, 1, &ain1)) == 0 &&
        (r = PCF8591ReadAINx(sen, 2, &ain2)) == 0 &&
        (r = PCF8591ReadAINx(sen, 3, &ain3)) == 0)
    {
        //Success
        //Pack 4 analog inputs to two 16-bit words
        //AIN0, AIN1
        sen->encValue[0] = (ain0 << 8) | ain1;

        //AIN2, AIN3
        sen->encValue[1] = (ain2 << 8) | ain3;

        return 0;
    }
    else //Read data fail
        return r;
}

/*===========================================================================
 * End of file adc-pcf8591.c
 *===========================================================================*/
