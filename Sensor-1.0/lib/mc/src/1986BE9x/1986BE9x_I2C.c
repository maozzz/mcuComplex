/*===========================================================================
 *
 *  MCU library
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
 *  File 1986BE9x_I2C.c: I2C functions.
 *  Note:
 *   1. MCUs 1986x, 1901x has implemented only MASTER mode:
 *      can't connect two this MCUs using I2C.
 *   2. MCU 1986BE1x does not have I2C.
 *
 *===========================================================================*/

#include "../../inc/I2C.h"
#include <string.h>

#include <stdcf.h>
#define PrintMsg(...)   //std_printf(__VA_ARGS__)

//I2C configuration
static I2CConfig cfgI2C1;

volatile u32 i2cTimerMs = 0;

//SCK/SDA pin options: note that pull up (~50kOm), but 4.7kOm recommended
#define I2Cx_PORT_OPTS  (PN_PULLUP     | PN_NOPULLDOWN | PN_NOSHMIT |\
                         PN_OPENDRAIN | PN_NOINFILTER | PN_PWR_FASTEST)

//Transaction timeout, msec
#ifndef I2C_TRAN_TIMEOUT
#define I2C_TRAN_TIMEOUT        200
#endif


/* */
static MCRESULT I2CMapPortPins(const I2CConfig *cfg)
{
    #if (TARGET_MCU == MCU_1986BE9x)
    //MCU: 1986BE9x
    switch(cfg->mappins)
    {
        case I2C1_MAP_C0C1:   //I2C1 SCK/SDA -> C0/C1   (alter)
        {
            InitializePORTEx(PORT_C, PN0|PN1, PN0|PN1, PN_ALTER|I2Cx_PORT_OPTS);
            return MCR_OK;
        }

        case I2C1_MAP_E14E15: //I2C1 SCK/SDA -> E14/E15 (remap)
        {
            InitializePORTEx(PORT_E, PN14|PN15, PN14|PN15, PN_REMAP|I2Cx_PORT_OPTS);
            return MCR_OK;
        }

        default: return MCR_INVALID_MAPPING;
    }

    #elif (TARGET_MCU == MCU_1901BC1x)
    //MCU: 1901BC1x
    switch(cfg->mappins)
    {
        case I2C1_MAP_C14C15: //I2C1 SCK/SDA -> C14/C15 (basic)
        {
            InitializePORTEx(PORT_C, PN14|PN15, PN14|PN15, PN_BASIC|I2Cx_PORT_OPTS);
            return MCR_OK;
        }

        case I2C1_MAP_D0D1:   //I2C1 SCK/SDA -> D0/D1   (alter)
        {
            InitializePORTEx(PORT_D, PN0|PN1, PN0|PN1, PN_ALTER|I2Cx_PORT_OPTS);
            return MCR_OK;
        }

        default: return MCR_INVALID_MAPPING;
    }

    #else
    //MCU 1986BE1x does not have I2C

    return MCR_INVALID_MAPPING;

    #endif
}



/* */
MCRESULT InitializeI2C(const I2CConfig *cfg)
{
    u32 ctr, hclk, div;

    //CTR register:
    //EN_I2C[7]:  1/0 - ON/OFF I2C controller
    //EN_INT[6]:  1/0 - enable/disable interrupt
    //S_I2C[5]:   1 - up to 1MBit; 0 - up to 400Kbit
    //bits[4..0]: reserved

    //Save configuration
    memcpy(&cfgI2C1, cfg, sizeof(I2CConfig));

    if (I2CMapPortPins(cfg) != MCR_OK)
        return MCR_INVALID_MAPPING;

    //Enable I2C clock
    ClkEnable(CLK_I2C1);

    //Turn off I2C
    I2C->CMD = 0;        
    I2C->CTR = 0;

    //-----------------------------------------------
    //Select divider
    //DIV = HCLK/(SCL*5-1)
    hclk = GetHCLK();
    div = (u32)((float)(hclk) / ((float)(cfg->speed) * 5.0F - 1.0F));

    if (div == 0)
        div = 1;
    else if (div > 65535)
        div = 65535;

    //PRL: low 8 bits; PRH: high 8 bits of divider
    I2C->PRL = (u8)(div);
    I2C->PRH = (u8)(div >> 8);

    //CTR.S_I2C[5]: 0 - up to 400Kbit; 1 - up to 1MBit
    if (cfg->speed > 400000)
        ctr = PN5;
    else
        ctr = 0;

    //-----------------------------------------------
    if (cfg->completed)
    {
        //CTR.EN_INT[6]=1: enable interrupt
        ctr |= PN6;

        IRQEnable(IRQ_I2C1);        
    }
    else
    {
        //CTR.EN_INT[6]=0: disable interrupt (already reset)
        //ctr &= ~PN6;

        IRQDisable(IRQ_I2C1);
    }

    //Set register value
    I2C->CTR = ctr;    

    return MCR_OK;
}



/* */
void I2CReset(I2CBLOCK i2c)
{
    //Clear STA register
    I2C->STA = 0;

    //CMD.CLRINT[0]=1: clear interrupt with writing 1
    I2C->CMD = PN0;
}



/* Turn I2C */
void TurnI2C(I2CBLOCK i2c, u8 turn)
{
    //CTR.EN_I2C[7]:  1/0 - ON/OFF I2C controller
    if (turn)
        I2C->CTR |= PN7;
    else
        I2C->CTR &= ~PN7;
}



/* Get I2C speed, bit/s */
u32 GetI2CSpeed(I2CBLOCK i2c)
{
    u32 hclk, div;

    hclk = GetHCLK();
    div  = (I2C->PRH << 8) | I2C->PRL;

    //DIV = HCLK/(SCL*5-1) =>
    //SCL = (HCLK/DIV+1)/5
    return (u32)(((float)hclk / (float)div + 1.0F) / 5.0F);
}



/* Wait while transaction in progress */
static s32 WaitI2CTranCompleted()
{
    u32 tm = i2cTimerMs + I2C_TRAN_TIMEOUT;

    //Wait while TRPROG[1]=1 (tx/rx not completed)
    while((I2C->STA & PN1) && (tm > i2cTimerMs)) __NOP();

    return (tm > i2cTimerMs) ? 0 : -1;
}



/* Wait I2C bus is not free */
static s32 WaitI2CBusBusy()
{
    u32 tm = i2cTimerMs + I2C_TRAN_TIMEOUT;

    //BUSY[6]=1: start bit received (line is busy)
    while((I2C->STA & PN6) && (tm > i2cTimerMs)) __NOP();

    return (tm > i2cTimerMs) ? 0 : -1;
}



/* Write data to selected I2C controller */
s32 I2CWrite(I2CBLOCK i2c, const void *src, s32 count)
{
    s32 i;
    u8 *psrc = (u8*)src;

    //STA register:
    //RxACK[7]:   1/0 - rx NACK/ACK from slave
    //BUSY[6]:    1 - after rx start bit; 0 - after rx stop bit
    //LOSTARB[5]: 1/0 - lost/not lost arbitrage;
    //            rx unexpected stop bit, SDA to high but SDA in low state
    //bits[4..2]: reserved
    //TRPROG[1]:  1 - tx/rx in progress; 0 - tx/rx completed
    //INT[0]   :  1/0 - interrupt flag

    //CMD register:
    //START[7]:   1 - tx start bit. Cleared with writing 0
    //STOP[6]:    1 - tx stop bit.  Cleared with writing 0
    //RD[5]:      1 - read data from slave
    //WR[4]:      1 - write data to slave
    //ACK[3]:     1/0 - tx NACK/ACK (when read)
    //bit[2..1]:  reserved
    //CLRINT[0]:  clear interrupt flag with writing 1

    //Wait I2C bus is busy, return by timeout
    if (WaitI2CBusBusy() != 0)
        return I2C_RESULT_BUSY;

    //psrc[0]: slave address and RW=0 (write operation)    
    I2C->TXD = (psrc[0] & 0xFE);

    //START[7]=1: tx start bit; WR[4]=1: tx address
    I2C->CMD = PN7 | PN4;    

    //Wait while tx in progress
    if (WaitI2CTranCompleted() != 0)
        i = I2C_RESULT_BUSY;
    //RxACK[7]=1: NACK received, can't transmit data
    else if (I2C->STA & PN7)
        i = I2C_RESULT_NACK;
    else
    {        
        //psrc[0]:    I2C address
        //psrc[1..N]: tx data
        for (i = 1; i < count; i++)
        {
            //WR[4]=1: tx data
            I2C->TXD = psrc[i];
            I2C->CMD = PN4;

            //Wait while tx in progress, break by timeout
            if (WaitI2CTranCompleted() != 0)
            {
                i = I2C_RESULT_BUSY;
                break;
            }
            //RxACK[7]=1: NACK received, break
            else if (I2C->STA & PN7)
            {
                i = I2C_RESULT_NACK;
                break;
            }
        }
    }

    //STOP[6]=1: tx stop bit
    I2C->CMD = PN6;

    return i;
}



/* Read data from selected I2C controller */
s32 I2CRead(I2CBLOCK i2c, void *dest, s32 count)
{
    s32 i;
    u8 *pdest = (u8*)dest;

    //Wait I2C bus is busy
    if (WaitI2CBusBusy() != 0)
        return I2C_RESULT_BUSY;

    //pdest[0]: slave address and RW=1 (read operation)    
    I2C->TXD = (pdest[0] | 0x01);

    //START[7]=1: tx start bit; WR[4]=1: tx address
    I2C->CMD = PN7 | PN4;    

    //Wait while tx address in progress AND    
    if (WaitI2CTranCompleted() != 0)
        i = I2C_RESULT_BUSY;
    //RxACK[7]=1: NACK received, can't read data
    else if (I2C->STA & PN7)
        i = I2C_RESULT_NACK;
    else
    {
        for (i = 0; i < count; i++)
        {
            //RD[5]=1:  start rx data
            //ACK[3]=1: tx NACK (when last byte read)
            if (i + 1 == count)
                I2C->CMD = PN5 | PN3;
            else
                I2C->CMD = PN5;

            //Wait while rx in progress
            if (WaitI2CTranCompleted() != 0)
            {
                i = I2C_RESULT_BUSY;
                break;
            }

            //Get received data
            pdest[i] = I2C->RXD;
        }
    }

    //STOP[6]=1: tx stop bit
    I2C->CMD = PN6;

    return i;
}



/* */
void I2C_IRQHandler()
{
    //CMD.CLRINT[0]=1: clear interrupt with writing 1
    I2C->CMD |= PN0;    

    if (cfgI2C1.completed)
        cfgI2C1.completed();
}

/*===========================================================================
 * End of file 1986BE9x_I2C.c
 *===========================================================================*/
