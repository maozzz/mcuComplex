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
 *  File sensor-poll.c. Polling functions.
 *
 *  Notes:
 *   1. This file must be in Windows-1251 encoding.
 *
 *===========================================================================*/

#include "sensor.h"
#include <LCD.h>
#include <RTC.h>
#include <CRC.h>
#include <screen.h>
#include <stdcf.h>
#include <string.h>
#include <controls.h>
#include <evbrd.h>
#include <DateTime.h>

volatile u32 senWaitTimeCounter = 0;

#ifdef __LCD_USE_EXT_BUS
#define STR_LCD_MODE    "EBUS"
#else
#define STR_LCD_MODE    "PORTIO"
#endif

//List of sensors (runtime state)
static SensorCtrl sensorList[SENSOR_MAXCOUNT];

//Variables
static u32 sensorListIndex  = 0;
static u32 sensorListCount  = 0;
static u32 sensorTimer      = 0;
static u32 sensorState      = SENSOR_STATE_START;

u32 blinkCounter = 0;

//Polling interval, msec
static u32 sensorPollInterval = SENSOR_POLL_INTERVAL;

//Pointer to BMP280 sensor control structure
SensorCtrl *pBMP280Sensor = 0;


//Get pointer to end of string
#define pend(s)    &s[strlen(s)]

//Indicates that timer expired
#define IsSenTimerExpired(msec)      ((i2cTimerMs - sensorTimer) > (msec))

//Restart timer
#define SenTimerRestart()            sensorTimer = i2cTimerMs



/* Initialize sensors */
void InitializeSensors(const SensorConfig *scfgPtr, u32 pollInterval)
{
    const SensorConfig *scfg = scfgPtr;
    SensorCtrl *sen = &sensorList[0];
    s32 i;

    //-------------------------------------------------
    //Clear structures
    for(i = 0; i < SENSOR_MAXCOUNT; i++)
        memset(&sensorList[i], 0, sizeof(SensorCtrl));

    //-------------------------------------------------
    //Configure sensors    
    sensorListCount = 0;
    pBMP280Sensor   = 0;

    if (scfg != 0)
    {
        for(i = 0; i < SENSOR_MAXCOUNT; i++)
        {
            switch(scfg->type)
            {
                case SENSOR_TYPE_BH1750 : InitializeBH1750 (sen, scfg); break;

                case SENSOR_TYPE_BMP280 :
                {
                    InitializeBMP280(sen, scfg);
                    pBMP280Sensor = sen;
                    break;
                }

                case SENSOR_TYPE_DHT12  : InitializeDHT12  (sen, scfg); break;

                case SENSOR_TYPE_DS18B20: InitializeDS18B20(sen, scfg); break;

                case SENSOR_TYPE_HMC5883: InitializeHMC5883(sen, scfg); break;

                case SENSOR_TYPE_INA219 : InitializeINA219 (sen, scfg); break;

                case SENSOR_TYPE_LM75A  : InitializeLM75A  (sen, scfg); break;

                case SENSOR_TYPE_MPU6000: InitializeMPU6000(sen, scfg); break;

                case SENSOR_TYPE_PCF8591: InitializePCF8591(sen, scfg); break;

                //It's redundant to count Si470x and KT0803 as sensors
                //Si470x:  FM radio receiver    76-108MHz
                //KT0803x: FM radio transmitter 76-108MHz
                //case SENSOR_TYPE_SI470X:  InitializeSi470x(sen, scfg); break;
                //case SENSOR_TYPE_KT0803:  InitializeKT0803(sen, scfg); break;

                //Unknown sensor type: skip
                default: scfg++; continue;
            }

            scfg++;
            sen++;
            sensorListCount++;
        } //for
    }

    //-------------------------------------------------
    //Set N/A values
    for(i = 0; i < SENSOR_MAXCOUNT; i++)
        SetNAValues(&sensorList[i]);


    //-------------------------------------------------
    //Set poll interval
    sensorPollInterval = pollInterval;

    //Set "START" state
    sensorState = SENSOR_STATE_START;
}



/* Get poll interval */
u32 GetPollInterval()
{
    return sensorPollInterval;
}



/* Set poll interval */
void SetPollInterval(u32 pollInterval)
{
    sensorPollInterval = pollInterval;
}



/* Convert sensor address at hexadecimal format to address */
s32 ParseSensorAddress(const char *src, u8 *addr)
{
    s32 i, n, len;
    char ch;

    if (!addr)
        return -1;

    memset(addr, 0, SENSOR_ADDRESS_MAXLENGTH);

    if (!src || (len = strlen(src)) == 0)
        return -2;

    i = 0;
    n = 0;
    while(i < len && n < (2 * SENSOR_ADDRESS_MAXLENGTH))
    {
        ch = src[i++];

        if (ch >= 0x30 && ch <= 0x39)      //c at ['0'-'9']
            ch -= 0x30;
        else if (ch >= 0x41 && ch <= 0x46) //c at ['A'-'F']
            ch -= 0x37;
        else if (ch >= 0x61 && ch <= 0x66) //c at ['a'-'f']
            ch -= 0x57;
        else //Skip other chars
            continue;

        //Odd/even i: low/high 4 bits
        if (n & 1)
            addr[(n >> 1)] |= (u8)ch;
        else
            addr[(n >> 1)]  = (u8)(ch << 4);

        n++;
    }

    if (n == (2 * SENSOR_ADDRESS_MAXLENGTH))
        return 0;
    else
        return -3;
}



/* Convert sensor type to readable string */
const char *SensorTypeToString(u8 type)
{
    switch(type)
    {
        case SENSOR_TYPE_BH1750 : return "BH1750";
        case SENSOR_TYPE_BMP280 : return "BMP280";
        case SENSOR_TYPE_DHT12  : return "DHT12";
        case SENSOR_TYPE_DS18B20: return "DS18B20";
        case SENSOR_TYPE_HMC5883: return "HMC5883";
        case SENSOR_TYPE_INA219 : return "INA219";
        case SENSOR_TYPE_LM75A  : return "LM75A";
        case SENSOR_TYPE_MPU6000: return "MPU6000";
        case SENSOR_TYPE_PCF8591: return "PCF8591";
        case SENSOR_TYPE_KT0803 : return "KT0803";
        case SENSOR_TYPE_SI470X : return "Si470x";
        default: return "\0";
    }
}



/* Get free item at list */
SensorConfig *GetFreeSensorCfg(SensorConfig *dcfg)
{
    s32 i;
    SensorConfig *p;

    for(i = 0; i < SENSOR_MAXCOUNT; i++)
    {
        p = &dcfg[i];

        if (p->type == SENSOR_TYPE_NONE || p->type == SENSOR_TYPE_UNDEF)
            return p;
    }

    return 0;
}



/* Get item with specified address */
SensorConfig *GetSensorCfgByAddr(SensorConfig *dcfg, const u8 *addr)
{
    s32 i;
    SensorConfig *p;

    for(i = 0; i < SENSOR_MAXCOUNT; i++)
    {
        p = &dcfg[i];

        if (p->type != SENSOR_TYPE_NONE && p->type != SENSOR_TYPE_UNDEF &&
            memcmp(p->addr, addr, SENSOR_ADDRESS_MAXLENGTH) == 0)
            return p;
    }

    return 0;
}



/* Get pointer to sensor control structure with specified index */
SensorCtrl *GetSensor(u32 index)
{
    if (index < sensorListCount)
        return &sensorList[index];
    else
        return 0;
}



/* Get count of sensors */
u32 GetSensorCount()
{
    return sensorListCount;
}



/* Set N/A values */
void SetNAValues(SensorCtrl *sen)
{
    s32 i;
    for(i = 0; i < SENSOR_ENCVALUE_MAXCOUNT; i++)
        sen->encValue[i] = SENSOR_VALUE_NA;
}



/* I2C bus: decode error code */
const char *I2CDecodeError(SensorCtrl *sen)
{
    switch(sen->lastError)
    {
        case I2C_RESULT_OK:
        {
            if (sen->encValue[0] == SENSOR_VALUE_NA)
                return "N/A";  //No device with specified address on bus
            else
                return "\0";   //OK, no error

            break;
        }

        case I2C_RESULT_BUSY:     return "CUR"; //SCL/SDA at low (possible short curcuit)
        case I2C_RESULT_NACK:     return "N/A"; //No device on bus
        case I2C_RESULT_CRCERROR: return "CRC"; //CRC error
        default:                  return "ERR"; //Unknown error
    }
}



/* 1-Wire bus: decode error code */
const char *IWireDecodeError(SensorCtrl *sen)
{
    switch(sen->lastError)
    {
        case IWIRE_RESULT_OK:
        {
            if (sen->encValue[0] == SENSOR_VALUE_NA)
                return "N/A";  //No device with specified address on bus
            else
                return "\0";   //OK, no error
        }

        case IWIRE_RESULT_NOPRESENCE:   return "DEV"; //No any device on bus
        case IWIRE_RESULT_SHORTCURCUIT: return "CUR"; //Short curcuit
        case IWIRE_RESULT_CRCERROR:     return "CRC"; //CRC error
        default:                        return "ERR"; //Unknown error
    }
}



/* 1-Wire bus: read 64-bit ROM.
   Note: must be only one device on 1-Wire bus */
s32 IWireReadROM(u8 *dest)
{
    s32 r;

    if ((r = IWireResetPresence(IWIRE_N1)) == IWIRE_RESULT_OK)
    {
        //1-Wire device presence
        //Tx "Read ROM" command
        IWireWriteByte(IWIRE_N1, IWIRE_CMD_READROM);

        //Receive ROM data (8 bytes with CRC)
        IWireRead(IWIRE_N1, dest, 8);

        if (GetCRC8IWire(dest, 8) != 0)
            return IWIRE_RESULT_CRCERROR;
        else
            return 0;
    }
    else //No 1-Wire devices or short curcuit
        return r;
}



/* Write register with specified address:
   writes register address + register data */
s32 I2CWriteRegister(I2CBLOCK i2c, u8 devaddr, u8 regaddr, const u8 *src, s32 n)
{
    s32 r;
    u8 txbuf[128];

    if (n > (128 - 2))
        n = (128 - 2);

    //Device I2C address + RW=0 (write)
    txbuf[0] = (devaddr << 1);

    //Register address
    txbuf[1] = regaddr;

    //Copy register value to internal buffer
    memcpy(&txbuf[2], src, n);

    //Write: device address, register pointer and register value
    if ((r = I2CWrite(i2c, txbuf, n + 2)) == (n + 2))
        return n;
    else
        return r;
}



/* Read register with specified address:
   writes register address than read register(s) */
s32 I2CReadRegister(I2CBLOCK i2c, u8 devaddr, u8 regaddr, u8 *dest, s32 n)
{
    s32 r;
    u8 txregptr[2];

    //Device I2C address + RW=1 (read)
    dest[0] = (devaddr << 1) | 1;

    //Device I2C address + RW=0 (write)
    txregptr[0] = (devaddr << 1);

    //Register address
    txregptr[1] = regaddr;

    //Write register address to register pointer
    //and read value of selected register
    if ((r = I2CWrite(i2c, txregptr, 2)) == 2)
    {
        r = I2CRead(i2c, dest, n);
    }

    return r;
}



/* Scan 1-Wire bus */
s32 ScanIWireBus(char *dest, u8 isHtmlFormat)
{    
    const char *devStr;
    s32 r, rdcounter;
    u8 *rom;    
    u8 roms[8 * SENSOR_MAXCOUNT];

    dest[0] = 0;

    //Try scan 1-Wire bus
    rdcounter = 0;
    while(rdcounter < SENSOR_SCANRETRY_COUNT)
    {
        if ((r = IWireScanBus(IWIRE_N1, roms, SENSOR_MAXCOUNT)) != 0)
        {
            WaitIntervalMs(SENSOR_SCANRETRY_INTERVAL);
        }
        else
            break;

        rdcounter++;
    }

    if (r != 0)
    {
        //Scan fail: print error
        switch(r)
        {
            case IWIRE_RESULT_NOPRESENCE:   devStr = "1-Wire: no device";     break;
            case IWIRE_RESULT_SHORTCURCUIT: devStr = "1-Wire: short curcuit"; break;
            case IWIRE_RESULT_CRCERROR:     devStr = "1-Wire: CRC error";     break;
            default:                        devStr = "1-Wire: error";         break;
        }

        std_sprintf(dest, "%s%s", devStr,
                    (isHtmlFormat == 0) ? "\n" : "<br>");
        return r;
    }

    rom = roms;
    r = 0;
    while(rom[0] != 0 && r < SENSOR_MAXCOUNT)
    {            
        //byte[0] - family code
        switch(rom[0])
        {
            case 0x01: devStr = "DS1990x";        break; //Unique serial number
            case 0x02: devStr = "DS1991";         break; //Multikey, 1152-bit EEPROM
            case 0x04: devStr = "DS1994,DS2404";  break; //4KB NVRAM + RTC + Timer
            case 0x05: devStr = "DS2405";         break; //Single address key
            case 0x06: devStr = "DS1993";         break; // 4 KB NVRAM
            case 0x08: devStr = "DS1992";         break; // 1 KB NVRAM
            case 0x09: devStr = "DS1982,DS2502";  break; // 1 KB PROM
            case 0x0A: devStr = "DS1995";         break; //16 KB NVRAM
            case 0x0B: devStr = "DS1985,DS2505";  break; //16 KB EEPROM
            case 0x0C: devStr = "DS1996";         break; //64 KB NVRAM
            case 0x0F: devStr = "DS1986,DS2506";  break; //64 KB EEPROM

            case 0x10: devStr = "DS18x20";        break; //Temperature sensor
            case 0x12: devStr = "DS2406,DS2407";  break; //1 KB EEPROM + 2 channel address key
            case 0x14: devStr = "DS1971,DS2430A"; break; //256 bit EEPROM + 64 bit PROM
            case 0x1A: devStr = "DS1963L";        break; //4 KB NVRAM + write counter
            case 0x1C: devStr = "DS28E04-100";    break; //4 KB EEPROM + 2 channel address key
            case 0x1D: devStr = "DS2423";         break; //4 KB NVRAM + external counter
            case 0x1F: devStr = "DS2409";         break; //2 channel address key
            case 0x20: devStr = "DS2450";         break; //4 channel ADC
            case 0x21: devStr = "DS1921x";        break; //Termochronous sensor
            case 0x23: devStr = "DS1973,DS2433";  break; //	4 KB EEPROM
            case 0x24: devStr = "DS1904,DS2415";  break; //Real time clock
            case 0x26: devStr = "DS2438";         break; //Temperature sensor + ADC

            case 0x27: devStr = "DS2417";         break; //Real time clock
            case 0x28: devStr = "DS18B20";        break; //Temperature sensor
            case 0x29: devStr = "DS2408";         break; //8-bit port
            case 0x2C: devStr = "DS2890";         break; //1 channel potentiometer
            case 0x2D: devStr = "DS1972,DS2431";  break; //1 KB EEPROM
            case 0x30: devStr = "DS2760";         break; //Temperature/current sensor + ADC
            case 0x37: devStr = "DS1977";         break; //32 KB EEPROM with password
            case 0x3A: devStr = "DS2413";         break; //2 channel commutator
            case 0x41: devStr = "DS1922x,DS1923,DS2422"; break; //Termochronous sensor, high resolution
            case 0x42: devStr = "DS28EA00";       break; //Temperature sensor, prog. resolution, portio
            case 0x43: devStr = "DS28EC20";       break; //20 KB EEPROM

            default: devStr = "Unknown"; break;
        }

        std_sprintf(pend(dest), "%02X:%02X%02X%02X%02X%02X%02X:%02X %s%s%s",
                    rom[0], rom[1], rom[2], rom[3],
                    rom[4], rom[5], rom[6], rom[7],
                    devStr,
                    (GetCRC8IWire(rom, 8) == 0) ? "" : " CRC!",
                    (isHtmlFormat == 0) ? "\n" : "<br>");


        //Pointer to next ROM
        rom = &rom[8];
        r++;
    }

    return 0;    
}



/* Scan I2C bus */
s32 ScanI2CBus(char *dest, u8 isHtmlFormat)
{
    const char *devStr;
    s32 r, rdcounter;
    u8 i2cAddr;
    u8 data[4];    

    dest[0] = 0;

    //I2C: 7-bit address = 128 devices
    for(i2cAddr = 0; i2cAddr < 128; i2cAddr++)
    {
        //Address + RW=1 (read)
        data[0] = (i2cAddr << 1) | 1;

        //Try read one byte from device with specified address
        rdcounter = 0;
        while(rdcounter < SENSOR_SCANRETRY_COUNT)
        {
            if ((r = I2CRead(I2C_N1, data, 1)) != 1)
            {
                //No "WaitIntervalMs" here
            }
            else
                break;

            rdcounter++;
        }

        if (r == I2C_RESULT_BUSY)
        {            
            std_sprintf(pend(dest), "I2C: short curcuit%s",
                        (isHtmlFormat == 0) ? "\n" : "<br>");
            return r;
        }
        else if (r != 1)
            continue;

        switch(i2cAddr)
        {
            case 0x10: //FM radio receiver
                devStr = "Si470x"; break;

            case 0x1E: //3-axis compass
                devStr = "HMC5883"; break;

            //Up to 2 BH1750 on bus
            case 0x23: //BH1750.ADDR=GND
                devStr = "BH1750"; break;

            case 0x5C: //BH1750.ADDR=Vdd; DHT12 has one address 0x5C
                devStr = "BH1750, DHT12"; break;

            case 0x68: //3-axis hyro/accel.
            case 0x69:
                devStr = "MPU60X0"; break;

            //Up to 2 BMP280 on bus
            case 0x76: //SDO=GND
            case 0x77: //SDO=Vdd
                devStr = "BMP280"; break;

            //Up to 16 INA219 on bus:
            //A1,A0 may connected to SDA,SCL
            case 0x40: //[A1,A0] => [GND,GND]
            case 0x41: //[A1,A0] => [GND,Vdd]
            case 0x44: //[A1,A0] => [Vdd,GND]
            case 0x45: //[A1,A0] => [Vdd,Vdd]
                devStr = "INA219"; break;

            default:
            {
                //Up to 8 LM75A on bus: [A2,A1,A0] => [0..7]
                if (i2cAddr >= 0x48 && i2cAddr <= 0x4F)
                    devStr = "LM75A, PCF8591"; //PCF8591 ADC/DAC
                else if (i2cAddr >= 0x20 && i2cAddr <= 0x27)
                    devStr = "PCF8574"; //PCF8574 - 8bit to I2C converter
                else if (i2cAddr >= 0x38 && i2cAddr <= 0x3F)
                {
                    //PCF8574 - 8bit to I2C converter OR
                    //KT0803x - FM radio transmitter
                    if (i2cAddr == 0x3E)
                        devStr = "PCF8574A, KT0803";
                    else
                        devStr = "PCF8574A";
                }
                else
                    devStr = "Unknown";
                break;
            }
        } //switch

        std_sprintf(pend(dest), "%02X %s%s", i2cAddr, devStr,
                    (isHtmlFormat == 0) ? "\n" : "<br>");
    } //for

    if (strlen(dest) == 0)
        std_sprintf(dest, "I2C: no device%s",
                    (isHtmlFormat == 0) ? "\n" : "<br>");

    return 0;
}



/* Scan 1-Wire and I2C bus and generate configuration */
s32 AutoconfigureSensors(SensorConfig *destCfg, s32 maxcount)
{
    SensorConfig *pcfg;
    s32 r, rdcounter, count, isValid;
    s32 nmTcnt, nmPcnt, nmVcnt, nmADcnt, nmLcnt;
    u8 *rom;
    u8 roms[8 * SENSOR_MAXCOUNT];
    u8 data[4];
    u8 i2cAddr;

    if (!destCfg || !maxcount)
        return 0;

    //------------------------------------------------------
    //Prepare    
    //Clear
    memset(destCfg, 0, sizeof(SensorConfig) * maxcount);

    //Count of configured sensors
    count = 0;

    //Sensor name counters
    nmTcnt = nmPcnt = nmVcnt = nmLcnt = nmADcnt = 1;

    //Check maxcount
    if (maxcount > SENSOR_MAXCOUNT)
        maxcount = SENSOR_MAXCOUNT;

    //------------------------------------------------------
    //1-Wire bus
    //Try scan
    rdcounter = 0;
    while(rdcounter < SENSOR_SCANRETRY_COUNT)
    {
        if ((r = IWireScanBus(IWIRE_N1, roms, SENSOR_MAXCOUNT)) != 0)
        {
            WaitIntervalMs(SENSOR_SCANRETRY_INTERVAL);
        }
        else
            break;

        rdcounter++;
    }

    //r == 0: scan 1-Wire ok
    if (r == 0)
    {
        rom = roms;
        while(rom[0] != 0 && count < maxcount)
        {
            //Get pointer to config structure
            pcfg = &destCfg[count];

            isValid = 1;

            //Check rx ROM CRC
            if (GetCRC8IWire(rom, 8) == 0)
            {
                //byte[0] - family code
                switch(rom[0])
                {
                    //Temperature sensors
                    case 0x10: //DS1920, DS1820, DS18S20, DS18B20
                    case 0x28: //DS18B20
                    {
                        pcfg->type = SENSOR_TYPE_DS18B20;
                        std_sprintf(pcfg->name, "T%i", nmTcnt++);
                        break;
                    }

                    default: isValid = 0; break;
                } //switch
            }
            else
                isValid = 0;

            if (isValid)
            {
                //Save ROM and increment count of devices
                memcpy(pcfg->addr, rom, 8);
                count++;
            }

            //Get pointer to next detected ROM
            rom  = &rom[8];
        }
    }


    //------------------------------------------------------
    //I2C bus: 7-bit address = 128 devices
    for(i2cAddr = 0; i2cAddr < 128 && count < maxcount; i2cAddr++)
    {
        //Get pointer to config structure
        pcfg = &destCfg[count];

        //Device address + RW=1 (read)
        data[0] = (i2cAddr << 1) | 1;

        //Try read one byte from device with specified address
        rdcounter = 0;
        while(rdcounter < SENSOR_SCANRETRY_COUNT)
        {
            if ((r = I2CRead(I2C_N1, data, 1)) != 1)
            {
                //No "WaitIntervalMs" here
            }
            else
                break;

            rdcounter++;
        }

        //"BUSY": possible short curcuit, break; otherwise
        //"NAK" received, continue with other address
        if (r == I2C_RESULT_BUSY)
            break;
        else if (r != 1)
            continue;

        isValid = 1;

        switch(i2cAddr)
        {
            //It's redundant to count Si470x and KT0803 as sensors
            /*case 0x10:
            {
                //Si470x: FM radio receiver 76-108MHz
                pcfg->type = SENSOR_TYPE_SI470X;
                std_sprintf(pcfg->name, "RR");
                break;
            }

            case 0x3E:
            {
                //KT0803: FM radio transmitter 76-108MHz
                pcfg->type = SENSOR_TYPE_KT0803;
                std_sprintf(pcfg->name, "RT");
                break;
            }*/

            case 0x1E:
            {
                //HMC5883: 3-axis digital compass
                pcfg->type = SENSOR_TYPE_HMC5883;
                std_sprintf(pcfg->name, "C1");
                break;
            }

            case 0x23:
            {
                //BH1750.ADDR=GND OR PCF8574 - 8bit to I2C converter
                pcfg->type = SENSOR_TYPE_BH1750;
                std_sprintf(pcfg->name, "L%i", nmLcnt++);
                break;
            }

            case 0x5C:
            {
                //BH1750.ADDR=Vdd OR DHT12
                //Autodetect by DHT12: has checksum for rx data
                SensorConfig testSenCfg;
                SensorCtrl testSen;

                memset(&testSen, 0, sizeof(SensorCtrl));

                testSenCfg.addr[0] = 0x5C;
                testSenCfg.type = SENSOR_TYPE_DHT12;

                InitializeDHT12(&testSen, &testSenCfg);

                if (testSen.pfConfigure(&testSen) == 0)
                {
                    WaitIntervalMs(200);

                    if (testSen.pfReadData(&testSen) == 0)
                    {
                        //It's DHT12 sensor
                        pcfg->type = SENSOR_TYPE_DHT12;
                        std_sprintf(pcfg->name, "T%i", nmTcnt++);
                        break;
                    }
                }

                //It's BH1750 sensor
                pcfg->type = SENSOR_TYPE_BH1750;
                std_sprintf(pcfg->name, "L%i", nmLcnt++);
                break;
            }

            case 0x68:
            case 0x69:
            {
                //MPU6000/MPU6050: 3-axis gyro/accel.
                pcfg->type = SENSOR_TYPE_MPU6000;
                std_sprintf(pcfg->name, "G1");
                break;
            }

            //Up to 2 BMP280 on bus
            case 0x76: //SDO=GND
            case 0x77: //SDO=Vdd
            {
                pcfg->type = SENSOR_TYPE_BMP280;
                std_sprintf(pcfg->name, "P%i", nmPcnt++);
                break;
            }

            //Up to 16 INA219 on bus:
            //A1,A0 may connected to SDA,SCL
            case 0x40: //[A1,A0] => [GND,GND]
            case 0x41: //[A1,A0] => [GND,Vdd]
            case 0x44: //[A1,A0] => [Vdd,GND]
            case 0x45: //[A1,A0] => [Vdd,Vdd]
            {
                pcfg->type = SENSOR_TYPE_INA219;
                std_sprintf(pcfg->name, "V%i", nmVcnt++);
                break;
            }

            default:
            {
                if (i2cAddr >= 0x48 && i2cAddr <= 0x4F)
                {
                    //LM75A OR PCF8591 ADC/DAC
                    //Up to 8 LM75A/PCF8591 on bus: [A2,A1,A0] => [0..7]

                    //Autodetect by LM75A
                    SensorConfig testSenCfg;
                    SensorCtrl testSen;

                    memset(&testSen, 0, sizeof(SensorCtrl));

                    testSenCfg.addr[0] = i2cAddr;
                    testSenCfg.type = SENSOR_TYPE_LM75A;

                    InitializeLM75A(&testSen, &testSenCfg);

                    if (testSen.pfConfigure(&testSen) == 0)
                    {
                        WaitIntervalMs(200);

                        if (testSen.pfReadData(&testSen) == 0)
                        {
                            //It's LM75A sensor
                            pcfg->type = SENSOR_TYPE_LM75A;
                            std_sprintf(pcfg->name, "T%i", nmTcnt++);
                            break;
                        }
                    }

                    //It's PCF8591 ADC/DAC
                    pcfg->type = SENSOR_TYPE_PCF8591;
                    std_sprintf(pcfg->name, "A%i", nmADcnt++);
                }                
                /*else if ((i2cAddr >= 0x20 && i2cAddr <= 0x27) ||
                         (i2cAddr >= 0x38 && i2cAddr <= 0x3F))
                {
                    //PCF8574/PCF8574A: 8bit/I2C converter
                    //currently no driver
                    isValid = 0;
                }*/
                else
                {
                    //Unknown device, reset flag
                    isValid = 0;
                }

                break;
            }
        } //switch

        if (isValid)
        {
            //Save I2C address and increment count of devices
            pcfg->addr[0] = i2cAddr;
            count++;
        }               
    } //while

    return count;
}



/* Convert UTF-8 codes to Windows-1251 */
static char *UTF8ToWin1251(char *dest, const char *src)
{
    s32 k = 0, i = 0, srclen = strlen(src);
    u16 chUTF8, chUTF16;
    u8 ch1251;

    while(i < srclen)
    {
        if (src[i] < 0x80)
        {
            ch1251 = src[i++];
        }
        else
        {
            //Convert from UTF-8 encoding to UTF-16 encoding
            //UTF8 (2 bytes case):
            //110x xxxx 10xx xxxx, x - significant bits
            chUTF8  = (src[i] << 8) + src[i + 1];
            chUTF16 = ((chUTF8 & 0x1F00) >> 2) | (chUTF8 & 0x3F);

            //UTF-16 russian letters:
            if (chUTF16 == 0x0401)
                ch1251 = 0xA8; //capital IO
            else if (chUTF16 == 0x0451)
                ch1251 = 0xB8; //small   io
            else if (chUTF16 >= 0x0410 && chUTF16 <= 0x042F)
                ch1251 = chUTF16 - 0x0410 + 0xC0; //capital A-YA
            else if (chUTF16 >= 0x0430 && chUTF16 <= 0x044F)
                ch1251 = chUTF16 - 0x0430 + 0xE0; //small   a-ya
            else
                ch1251 = 0x2E; //'.' by default

            i += 2;
        }

        dest[k++] = ch1251;
    }

    //Zero terminate
    dest[k] = 0;

    return dest;
}



/* Print data from all sensors to LCD */
void PrintSensorsData(u32 isRefresh)
{
    u32 i;
    DateTime dt;
    SensorCtrl *sen;
    char *senstr = textBox.text;
    char blch;

    //-----------------------------------------------------
    //Clear screen buffers (RAM)
    ClearScreen(pscreen);

    //-----------------------------------------------------
    //Indication that screen updated
    blinkCounter++;
    blch = (blinkCounter & 1) ? ':' : ' ';

    senstr[0] = 0;

    for(i = 0; i < sensorListCount; i++)
    {
        sen = &sensorList[i];

        if (sen->pfToString)
        {
            //Print sensor name
            char str1251[SENSOR_NAME_MAXLENGTH + 4];

            std_sprintf(pend(senstr), "%s%c ",
                        UTF8ToWin1251(str1251, sen->cfg->name),
                        blch);

            //Convert encoded value to string
            sen->pfToString(sen, pend(senstr));

            //Add LF char
            std_sprintf(pend(senstr), "%c\n",
                        sen->errCount ? '!' : ' ');
        }
        else
        {
            //"ToString" function not defined: print at hexadecimal format
            std_sprintf(pend(senstr), "S%i %08X%c\n",
                        i, sen->encValue[0], sen->errCount ? '!' : ' ');
        }
    }    

    //-----------------------------------------------------
    //Additional information (for sample)
    //delta height, meters
    std_sprintf(pend(senstr), "dH=%.2f m\n",
                BMP280DoBarometricLeveling(pBMP280Sensor));

    //Date/time
    BinaryToDateTime((u32)RTCRead(), &dt);
    std_sprintf(pend(senstr), "%02i.%02i.%02i %02i%c%02i\n",
                dt.day, dt.month, (dt.year > 2000) ? (dt.year - 2000) : (dt.year - 1900),
                dt.hour, blch, dt.min);

    //Count of main cycles
    std_sprintf(pend(senstr), "%s: %u\n", STR_LCD_MODE, mainSpeed);

    //Test strings
    //English text
    //std_sprintf(pend(senstr), "THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG!\n");
    //std_sprintf(pend(senstr), "the quick brown fox jumps over the lazy dog?\n");
    //std_sprintf(pend(senstr), "0123456789-+.\\{|}~%c%c%c%c%c%c.\n", 0x10, 0x11, 0x14, 0x15, 0x18, 0x19);

    //Russian text (must Windows-1251 encoding)
    //std_sprintf(pend(senstr), "—⁄≈ÿ‹ ∆≈ ≈Ÿ® ›“»’ Ãﬂ√ »’ ‘–¿Õ÷”«— »’ ¡”ÀŒ , ƒ¿ ¬€œ≈… ◊¿ﬁ:\n");
    //std_sprintf(pend(senstr), "Ò˙Â¯¸ ÊÂ Â˘∏ ˝ÚËı Ïˇ„ÍËı Ù‡ÌˆÛÁÒÍËı ·ÛÎÓÍ, ‰‡ ‚˚ÔÂÈ ˜‡˛!\n");


    //-----------------------------------------------------
    //Refresh textbox internal variables
    if (isRefresh)
        RefreshTextBox(&textBox);
    else
        UpdateTextBox(&textBox);

    //Redraw textbox at screen buffer
    DrawTextBox(pscreen, &textBox.control);


    //-----------------------------------------------------
    //Test only: LCD redraw start
    //SetLed(LED_3);

    //Flush screen buffer to LCD
    DrawScreen(pscreen);

    //Test only: LCD redraw end
    //ResetLed(LED_3);

    //-----------------------------------------------------
    //Print all data to stdout (UART)
    //std_printf(senstr);
}



/* Update sensors state */
void UpdateSensorsState()
{
    SensorCtrl *sen;
    s32 r = 0;

    switch(sensorState)
    {
        case SENSOR_STATE_READ_DATA:
        {
            //Wait specified interval before reading
            if (IsSenTimerExpired(SENSOR_READ_INTERVAL))
            {
                if (sensorListIndex < sensorListCount)
                {
                    //Read sensor data and restart convertation
                    sen = &sensorList[sensorListIndex++];

                    if (sen->flags & SENSOR_FLAG_CONFIGURED)
                    {
                        //Read data from sensor
                        if (sen->pfReadData)
                            r = sen->pfReadData(sen);

                        //Write data to sensor (restart convertation)
                        if (r == 0 && sen->pfWriteData)
                            r = sen->pfWriteData(sen);

                        //Save error code
                        sen->lastError = r;

                        if (r != 0)
                        {
                            //Read or restart error:
                            //set N/A, reset flag
                            SetNAValues(sen);
                            sen->flags &= ~SENSOR_FLAG_CONFIGURED;

                            //Set when sensor read/configure fail.
                            //For each success read it decrement by 1.
                            sen->errCount = (u16)(10000 / sensorPollInterval);
                        }
                        else
                        {
                            //Read and restart OK: decrement eror counter
                            if (sen->errCount)
                                sen->errCount--;
                        }
                    }
                    else
                    {                                                
                        if (sen->pfConfigure)
                            r = sen->pfConfigure(sen);

                        //Save error code
                        sen->lastError = r;

                        //Set N/A (device just configured, no data)
                        SetNAValues(sen);                        

                        if (r != 0)
                            sen->errCount = (u16)(10000 / sensorPollInterval);
                        else
                            sen->flags |= SENSOR_FLAG_CONFIGURED;
                    }
                }
                else
                {
                    //Print data from all sensors
                    PrintSensorsData(0);
                    sensorState = SENSOR_STATE_PAUSE;
                }

                //Restart timer
                SenTimerRestart();
            }

            break;
        } //SENSOR_STATE_READ_DATA


        case SENSOR_STATE_PAUSE:
        {
            //Long pause before read first sensor at list
            if (IsSenTimerExpired(sensorPollInterval))
            {
                sensorListIndex = 0;
                sensorState     = SENSOR_STATE_READ_DATA;
                SenTimerRestart();
            }

            break;
        } //SENSOR_STATE_PAUSE


        //case SENSOR_STATE_START:
        default:
        {
            sensorListIndex = 0;
            sensorState     = SENSOR_STATE_PAUSE;
            SenTimerRestart();
            break;
        }
    } //switch
}

/*===========================================================================
 * End of file sensor-poll.c
 *===========================================================================*/
