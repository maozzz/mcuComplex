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
 *  File sensor-bh1750x.c. Ambient light sensor BH1750x functions.
 *                         I2C interface.
 *                         Vdd = 2.4-3.6V.
 *
 *===========================================================================*/

#include "sensor.h"

//Debug messages (comment to disable)
#define PrintMsg(...)       //std_printf(__VA_ARGS__)


//Operational codes (8 bits)
//Power Down. No active state
#define BH1750_OPCODE_POWERDOWN      0x00
//Power On. Wait measurement command
#define BH1750_OPCODE_POWERON        0x01
//Reset. Is for only reset illuminance data register. (reset value is '0').
#define BH1750_OPCODE_RESET          0x07

//Continuously H-Resolution Mode (recommended mode).
//Resolution: 1 lx. Meas.time: max 180ms.
#define BH1750_OPCODE_CONT_HRES      0x10
//Continuously H-Resolution Mode2.
//Resolution: 0.5 lx. Meas.time: max 180ms.
#define BH1750_OPCODE_CONT_HRES2     0x11
//Continuously L-Resolution Mode.
//Resolution: 4 lx. Meas.time: max 24ms.
#define BH1750_OPCODE_CONT_LRES      0x13

//One time H-Resolution Mode
#define BH1750_OPCODE_ONE_HRES       0x20
//One time H-Resolution Mode2
#define BH1750_OPCODE_ONE_HRES2      0x21
//One time L-Resolution Mode
#define BH1750_OPCODE_ONE_LRES       0x23

//Change Measurement time( High bit ) 01000_MT[7,6,5]
//Change Masurement time( Low bit ) 011_MT[4,3,2,1,0]


/* BH1750: initialize control structure */
void InitializeBH1750(SensorCtrl *sen, const SensorConfig *cfg)
{    
    memset(sen, 0, sizeof(SensorCtrl));

    //Save pointer to configuration
    sen->cfg = cfg;

    //Set functions
    sen->pfConfigure = &BH1750Configure;
    sen->pfWriteData = &BH1750WriteData;
    sen->pfReadData  = &BH1750ReadData;
    sen->pfToString  = &BH1750ToString;
}



/* BH1750 sensor: decode light code to floating point number */
float BH1750DecodeData(SensorCtrl *sen)
{
    float light;

    light = ((float)sen->encValue[0]) / 1.2F;

    return light;
}



/* BH1750 sensor: convert encoded value to readable string */
char *BH1750ToString(SensorCtrl *sen, char *dest)
{   
    if (sen->encValue[0] == SENSOR_VALUE_NA)
    {
        std_sprintf(dest, "%s", I2CDecodeError(sen));
    }
    else
    {
        std_sprintf(dest, "%4.1flx", BH1750DecodeData(sen));
    }

    return dest;
}



/* BH1750 sensor: configure */
s32 BH1750Configure(SensorCtrl *sen)
{
    s32 r;
    u8 txdata[2];

    //txdata[0]: I2C address + RW=0 (write)
    txdata[0] = ((sen->cfg->addr[0]) << 1);

    //Select mode: continuous high resolution
    txdata[1] = BH1750_OPCODE_CONT_HRES;

    //Tx mode to device
    if ((r = I2CWrite(I2C_N1, txdata, 2)) == 2)    
        return 0;
    else                                    
        return r;
}



/* BH1750 sensor: restart convertation */
s32 BH1750WriteData(SensorCtrl *sen)
{
    //No actions
    return 0;
}



/* BH1750 sensor: read data using I2C bus */
s32 BH1750ReadData(SensorCtrl *sen)
{
    s32 r;
    u8 rxdata[2];

    //rxdata[0]: I2C address + RW=1 (read)
    rxdata[0] = ((sen->cfg->addr[0]) << 1) | 1;

    if ((r = I2CRead(I2C_N1, rxdata, 2)) == 2)
    {
        //Success
        sen->encValue[0] = (rxdata[0] << 8) | rxdata[1];

        if (sen->encValue[0] == 0xFFFFU)
            return I2C_RESULT_CRCERROR;
        else
            return 0;
    }
    else //Read data fail
        return r;
}

/*===========================================================================
 * End of file sensor-bh1750x.c
 *===========================================================================*/
