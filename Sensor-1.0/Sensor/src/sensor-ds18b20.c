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
 *  File sensor-ds18b20.c. Temperature sensor DS18B20 functions.
 *                         1-Wire interface.
 *                         Vdd = 3.0-5.5V.
 *                         Temp. = -55..+125 DegC, res 0.0625 DegC.
 *                         Max. conversion time ~750 msec (12 bit res).
 *
 *===========================================================================*/

#include "sensor.h"

//Debug messages (comment to disable)
#define PrintMsg(...)                  std_printf(__VA_ARGS__)

//DS18B20 commands
//Initiates a single temperature conversion.
#define DS18B20_CMD_CONVERTT           0x44

//Write 3 bytes [TH,TL,CFG] to scratchpad bytes [2,3,4]
//Arguments:  3 bytes (TH,TL,CFG)
#define DS18B20_CMD_WRITESCRATCHPAD    0x4E

//Read the contents of the scratchpad.
//Will received 9 bytes (with CRC)
#define DS18B20_CMD_READSCRATCHPAD     0xBE

//Copies the scratchpad [TH,TL,CFG] (bytes 2, 3, 4) to EEPROM.
//Arguments: none.
//Excecution time: >10ms.
//Issue read time slots to determine execution status
//(1/0 - completed/not completed)
#define DS18B20_CMD_COPYSCRATCHPAD     0x48

//Recalls the TH, TL and configuration data
//from EEPROM and places the data in bytes 2, 3, and 4
//in the scratchpad memory.
//Arguments: none.
#define DS18B20_CMD_RECALLE            0xB8

//Indicates if any DS18B20s on the bus using parasite power.
//Arguments: none.
//Status at 1 read time slot: 0/1 - parasite/external power.
#define DS18B20_CMD_READPOWERSUPPLY    0xB4

//Alarm search is identical to the operation
//of the Search ROM command except that only slaves with
//a set alarm flag will respond.
#define DS18B20_CMD_ALARMSEARCH        0xEC

//Configuration register
//Temperature resolution
#define DS18B20_RES_9bits              (0 << 5) //9  bits,  93.75ms (tconv/8)
#define DS18B20_RES_10bits             (1 << 5) //10 bits, 187.50ms (tconv/4)
#define DS18B20_RES_11bits             (2 << 5) //11 bits, 375.00ms (tconv/2)
#define DS18B20_RES_12bits             (3 << 5) //12 bits, 750.00ms (tconv), default
#define DS18B20_RES_MASK               (3 << 5) //mask


//User defined configuration
//Registers Th,Tl (two's complement, 8 bit with sign).
//Alarm signal is set when: (t >= Th || t <= Tl)
#define DS18B20_REG_TH_VALUE           (30) //Th, DegC
#define DS18B20_REG_TL_VALUE           (15) //Tl, DegC

//Configuration register
//bit[7]=0; res[6..5]=xx; bits[4..0]=11111b
#define DS18B20_REG_CONF_VALUE         (DS18B20_RES_12bits|0x1F)



/* DS18B20: initialize control structure */
void InitializeDS18B20(SensorCtrl *sen, const SensorConfig *cfg)
{    
    memset(sen, 0, sizeof(SensorCtrl));

    //Save pointer to configuration
    sen->cfg = cfg;

    //Set functions
    sen->pfConfigure = &DS18B20Configure;
    sen->pfWriteData = &DS18B20WriteData;
    sen->pfReadData  = &DS18B20ReadData;
    sen->pfToString  = &DS18B20ToString;
}



/* DS18B20 sensor: decode temperature to floating point number */
float DS18B20DecodeData(SensorCtrl *sen)
{
    float temp;
    u32 isNegative = 0;
    u32 encv = sen->encValue[0];

    //The temperature code is a 16-bit sign-extended
    //two's complement number.

    //bit[15..11]: sign 1/0 - negative/positive
    //bits[10..4]: integer part of temperature
    //bits[3..0]:  fractional part of temperature
    // bit[3] - 0.5000 deg units
    // bit[2] - 0.2500 deg units
    // bit[1] - 0.1250 deg units
    // bit[0] - 0.0625 deg units

    if (encv & PN15)
    {
        isNegative = 1;
        encv = (~encv + 1) & 0xFFFFU;
    }

    //Integer part: 7 bits
    temp = (float)((encv >> 4) & 0x7F);

    //Decode fractional part
    if (encv & PN3) temp += 0.5F;
    if (encv & PN2) temp += 0.25F;
    if (encv & PN1) temp += 0.125F;
    if (encv & PN0) temp += 0.0625F;

    //Negative temperature: add sign
    if (isNegative)
        temp *= -1.0F;

    return temp;
}



/* DS18B20 sensor: convert encoded value to readable string */
char *DS18B20ToString(SensorCtrl *sen, char *dest)
{
    if (sen->encValue[0] == SENSOR_VALUE_NA)
    {
        std_sprintf(dest, "%s", IWireDecodeError(sen));
    }
    else
    {
        std_sprintf(dest, "%3.1fC", DS18B20DecodeData(sen));
    }

    return dest;
}



/* DS18B20 sensor: configure */
s32 DS18B20Configure(SensorCtrl *sen)
{
    s32 r;
    u8 txdata[4];

    //Reset/presence
    r = IWireResetPresence(IWIRE_N1);

    if (r == IWIRE_RESULT_OK)
    {
        //1-Wire device presence
        //Prepare configuration
        txdata[0] = DS18B20_CMD_WRITESCRATCHPAD;
        txdata[1] = DS18B20_REG_TH_VALUE;
        txdata[2] = DS18B20_REG_TL_VALUE;
        txdata[3] = DS18B20_REG_CONF_VALUE;

        //Tx "Match ROM" command
        IWireWriteByte(IWIRE_N1, IWIRE_CMD_MATCHROM);

        //Tx 64-bit ROM code
        IWireWrite(IWIRE_N1, sen->cfg->addr, 8);

        //Tx "Write Scratchpad" command and registers Th, Tl, Conf
        IWireWrite(IWIRE_N1, txdata, 4);

        //Tx "ConvertT"
        return DS18B20WriteData(sen);
    }
    else
    {
        //No 1-Wire devices or short curcuit
        return r;
    }
}



/* DS18B20 sensor: restart convertation (mandatory) */
s32 DS18B20WriteData(SensorCtrl *sen)
{
    s32 r;    

    //Reset/presence
    r = IWireResetPresence(IWIRE_N1);    

    if (r == IWIRE_RESULT_OK)
    {
        //1-Wire device presence

        //Tx "Match ROM" command
        IWireWriteByte(IWIRE_N1, IWIRE_CMD_MATCHROM);

        //Tx 64-bit ROM code
        IWireWrite(IWIRE_N1, sen->cfg->addr, 8);

        //Tx "Convert T" command: start A/D conversion, no arguments
        IWireWriteByte(IWIRE_N1, DS18B20_CMD_CONVERTT);        

        //No wait while convertation completed
        return 0;
    }
    else
    {
        //No 1-Wire devices or short curcuit
        return r;
    }
}



/* DS18B20 sensor: read data (1-Wire bus) */
s32 DS18B20ReadData(SensorCtrl *sen)
{
    s32 r;

    //DS18B20: 9 bytes for scpad including CRC
    u8 scpad[9];

    //Reset/presence
    r = IWireResetPresence(IWIRE_N1);    

    if (r == IWIRE_RESULT_OK)
    {
        //1-Wire device presence

        //Tx "Match ROM" command
        IWireWriteByte(IWIRE_N1, IWIRE_CMD_MATCHROM);

        //Tx 64-bit ROM code
        IWireWrite(IWIRE_N1, sen->cfg->addr, 8);

        //Tx "Read Scratchpad" command
        IWireWriteByte(IWIRE_N1, DS18B20_CMD_READSCRATCHPAD);

        //Receive scratchpad data (9 bytes with CRC)
        IWireRead(IWIRE_N1, scpad, 9);

        if (GetCRC8IWire(scpad, 9) == 0 &&
            scpad[2] == DS18B20_REG_TH_VALUE &&
            scpad[3] == DS18B20_REG_TL_VALUE &&
            scpad[4] == DS18B20_REG_CONF_VALUE)
        {            
            //PrintMsg("DS %02X %02X %02X\n", scpad[2], scpad[3], scpad[4]);

            //Temperature at first 2 bytes
            sen->encValue[0] = (scpad[1] << 8) | scpad[0];
            return 0;
        }
        else
        {            
            //Error in received data or
            //device not configured properly
            return IWIRE_RESULT_CRCERROR;
        }
    }
    else
    {
        //No 1-Wire devices or short curcuit
        return r;
    }
}

/*===========================================================================
 * End of file sensor-ds18b20.c
 *===========================================================================*/
