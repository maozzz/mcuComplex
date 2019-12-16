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
 *  File sensor-lm75a.c. Temperature sensor LM75A, STLM75 functions.
 *                       I2C interface.
 *                       Vdd = 2.8-5.5V.
 *                       Temp. = -55..+125 DegC, res 0.125 DegC (11-bit ADC).
 *
 *===========================================================================*/

#include "sensor.h"

//Debug messages (comment to disable)
#define PrintMsg(...)       //std_printf(__VA_ARGS__)

//LM75A/STLM75 registers:
//Temperature, 16-bits, read only
//LM75A:
//bits[15..5]: temp. value at 2's complement format, LSB=0.125 DegC.
//bits[4..0]:  unused and 0
//STLM75:
//bits[15..7]: temp. value at 2's complement format, LSB=0.5 DegC.
//bits[6..0]:  unused and 0
#define LM75A_REG_TEMP       0x00 //address

//Thyst, hysteresis (default 75 GegC), read/write
//bits[15..7]: Thyst value at 2's complement format, LSB=0.5 DegC.
//bits[6..0]:  unused and 0
#define LM75A_REG_THYST      0x02 //address

//Tos, overtemperature shutdown treshold (default 80 GegC), read/write
//bits[15..7]: Tos value at 2's complement format, LSB=0.5 DegC.
//bits[6..0]:  unused and 0
#define LM75A_REG_TOS        0x03 //address


//Configuration, read/write
#define LM75A_REG_CONF       0x01 //address
//OS fault queue programming
#define LM75A_OSFQUE_1       (0 << 3) //queue value = 1 (default)
#define LM75A_OSFQUE_2       (1 << 3) //queue value = 2
#define LM75A_OSFQUE_4       (2 << 3) //queue value = 4
#define LM75A_OSFQUE_6       (3 << 3) //queue value = 6
#define LM75A_OSFQUE_MASK    (3 << 3) //mask
//OS polarity selection
#define LM75A_OSPOL_LOW      (0 << 2) //OS active LOW (default)
#define LM75A_OSPOL_HIGH     (1 << 2) //OS active HIGH
//OS operation mode selection
#define LM75A_OSMODE_COMP    (0 << 1) //OS comparator (default)
#define LM75A_OSMODE_INT     (1 << 1) //OS interrupt
//Device operation mode
#define LM75A_NORMAL         (0 << 0) //Normal (default)
#define LM75A_SHUTDOWN       (1 << 0) //Shutdown


//User-defined configuration
//Configuration register value
#define LM75A_REG_CONF_VALUE    (LM75A_OSFQUE_2|\
                                 LM75A_OSPOL_LOW|\
                                 LM75A_OSMODE_COMP|\
                                 LM75A_NORMAL)
//Tos = 29 DegC
#define LM75A_REG_TOS_VALUE     (29 << (1 + 7)) //LSB=0.5 DegC and shift left 7 bits

//Thyst = 27 DegC
#define LM75A_REG_THYST_VALUE   (27 << (1 + 7)) //LSB=0.5 DegC and shift left 7 bits


/* LM75A: initialize control structure */
void InitializeLM75A(SensorCtrl *sen, const SensorConfig *cfg)
{
    memset(sen, 0, sizeof(SensorCtrl));

    //Save pointer to configuration
    sen->cfg = cfg;

    //Set functions
    sen->pfConfigure = &LM75AConfigure;
    sen->pfWriteData = &LM75AWriteData;
    sen->pfReadData  = &LM75AReadData;
    sen->pfToString  = &LM75AToString;
}



/* LM75A sensor: decode temperature to floating point number */
float LM75ADecodeData(SensorCtrl *sen)
{
    float temp;
    u32 isNegative = 0;
    u32 encv = sen->encValue[0];

    //Temperature register: 16 bits, 2's complement format
    //bits[15..8]: integer part and sign
    //bits[7..5]:  fractional part
    //bits[4..0]:  unused

    //Negative value
    if (encv & PN15)
    {        
        isNegative = 1;
        encv = (~encv + 1) & 0xFFFFU;
    }   

    //Integer part: 8 bits
    temp = (float)((encv >> 8) & 0xFF);

    //Fractional part: 3 bits
    if (encv & PN7) temp += 0.500F;
    if (encv & PN6) temp += 0.250F;
    if (encv & PN5) temp += 0.125F;

    //Negative temperature
    if (isNegative)
        temp = -1.0F * temp;

    return temp;
}



/* LM75A sensor: convert encoded value to readable string */
char *LM75AToString(SensorCtrl *sen, char *dest)
{   
    if (sen->encValue[0] == SENSOR_VALUE_NA)
    {
        std_sprintf(dest, "%s", I2CDecodeError(sen));
    }
    else
    {
        std_sprintf(dest, "%3.1fC", LM75ADecodeData(sen));
    }

    return dest;
}



/* LM75A sensor: configure */
s32 LM75AConfigure(SensorCtrl *sen)
{
    s32 r;
    u8 devaddr;
    u8 conf;
    u8 tos[2];
    u8 thyst[2];

    devaddr = sen->cfg->addr[0];

    //Conf: 8 bits
    conf = LM75A_REG_CONF_VALUE;

    //Tos MSB,LSB
    tos[0] = (u8)(LM75A_REG_TOS_VALUE >> 8);
    tos[1] = (u8)(LM75A_REG_TOS_VALUE);

    //Thyst MSB,LSB
    thyst[0] = (u8)(LM75A_REG_THYST_VALUE >> 8);
    thyst[1] = (u8)(LM75A_REG_THYST_VALUE);

    if ((r = I2CWriteRegister(I2C_N1, devaddr, LM75A_REG_CONF,  &conf, 1)) == 1 &&
        (r = I2CWriteRegister(I2C_N1, devaddr, LM75A_REG_TOS,   tos,   2)) == 2 &&
        (r = I2CWriteRegister(I2C_N1, devaddr, LM75A_REG_THYST, thyst, 2)) == 2)
    {
        PrintMsg("LM75A: CFG OK\n");
        return 0;
    }
    else
    {
        PrintMsg("LM75A: CFG FAIL\n");
        return r;
    }
}



/* LM75A sensor: restart convertation */
s32 LM75AWriteData(SensorCtrl *sen)
{
    //No actions, continuous convertation
    return 0;
}



/* LM75A sensor: read data (I2C bus) */
s32 LM75AReadData(SensorCtrl *sen)
{
    s32 r;
    u8 devaddr;
    u8 conf;
    u8 rxdata[2];                
    u8 tos[2];
    u8 thyst[2];

    devaddr = sen->cfg->addr[0];

    if ((r = I2CReadRegister(I2C_N1, devaddr, LM75A_REG_TEMP,  rxdata, 2)) == 2 &&
        (r = I2CReadRegister(I2C_N1, devaddr, LM75A_REG_TOS,   tos,    2)) == 2 &&
        (r = I2CReadRegister(I2C_N1, devaddr, LM75A_REG_THYST, thyst,  2)) == 2 &&
        (r = I2CReadRegister(I2C_N1, devaddr, LM75A_REG_CONF,  &conf,  1)) == 1)
    {                
        //Read success, check received values
        //Temp: bits[4..0] - unused, must be zeros;
        //then check Tos, Thyst and Conf values
        if ((rxdata[1] & 0x1F) != 0 ||
            ((tos[0] << 8)   | tos[1])   != LM75A_REG_TOS_VALUE ||
            ((thyst[0] << 8) | thyst[1]) != LM75A_REG_THYST_VALUE ||
             conf != LM75A_REG_CONF_VALUE)
        {
            //Incorrect data            
            return I2C_RESULT_CRCERROR;
        }
        else
        {
            //Success
            sen->encValue[0] = (rxdata[0] << 8) | rxdata[1];            
            return 0;
        }
    }
    else
    {
        //Read data fail        
        return r;
    }
}

/*===========================================================================
 * End of file sensor-lm75a.c
 *===========================================================================*/
