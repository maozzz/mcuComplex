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
 *  File 1986BE1x_I2C.c: I2C functions.
 *  Note:
 *   1. MCU 1986BE1x does not has I2C controller, 
 *      software implementation used
 *
 *===========================================================================*/

#include "../../inc/I2C.h"
#include "../../inc/SysTimer.h"

//Pointer to I2C port
static _port *i2c_port_ptr;

//Clock line mask (SCL)
static u32 i2c_pnclk_mask;

//Data line mask (SDA)
static u32 i2c_pndata_mask;

//1/4 of SCL period, usec
static u32 i2c_oneq_period = 640;

//Timer, msec
volatile u32 i2cTimerMs = 0;


//Toggle SCL to high state ("In", Z-state)
#define I2CSCLToHigh()                  i2c_port_ptr->OE &= ~i2c_pnclk_mask

//Toggle SDA to high state ("In", Z-state)
#define I2CSDAToHigh()                  i2c_port_ptr->OE &= ~i2c_pndata_mask

//Toggle SCL and SDA to high state ("In", Z-state)
#define I2CToHigh()                     i2c_port_ptr->OE &= ~(i2c_pnclk_mask|i2c_pndata_mask)


//Toggle SCL to low state ("Out" and "0")
#define I2CSCLToLow()                   i2c_port_ptr->OE |= i2c_pnclk_mask; \
                                        i2c_port_ptr->RXTX &= ~i2c_pnclk_mask

//Toggle SDA to low state ("Out" and "0")
#define I2CSDAToLow()                   i2c_port_ptr->OE |= i2c_pndata_mask; \
                                        i2c_port_ptr->RXTX &= ~i2c_pndata_mask


//Indicates that SCL in high state
#define I2CIsSCLHighState()             (i2c_port_ptr->RXTX & i2c_pnclk_mask)

//Indicates that SDA in high state (bit "1")
#define I2CIsSDAHighState()             (i2c_port_ptr->RXTX & i2c_pndata_mask)

//Indicates that SCL and SDA in high state (line clear)
#define I2CIsHighState()                ((i2c_port_ptr->RXTX & (i2c_pnclk_mask|i2c_pndata_mask)) ==\
                                         (i2c_pnclk_mask|i2c_pndata_mask))


//Wait specified number of clocks
#define I2CWait(nclk)                   WaitSysTimer(SYSTIM_N1, (nclk))



//SCL/SDA pin options: note that opendrain, pull up (~50kOm), but 4.7kOm recommended
#define I2Cx_PORT_OPTS  (PN_PORTIO |PN_PULLUP | PN_NOPULLDOWN | PN_NOSHMIT |\
                         PN_OPENDRAIN | PN_NOINFILTER | PN_PWR_FASTEST)

//Transaction timeout, msec
#ifndef I2C_TRAN_TIMEOUT
#define I2C_TRAN_TIMEOUT        200
#endif


/* */
static MCRESULT I2CMapPortPins(const I2CConfig *cfg)
{
    #if (TARGET_MCU == MCU_1986BE1x)
    //MCU: 1986BE1x
    switch(cfg->mappins)
    {
        case I2C1_MAP_C11C12:  //I2C SCL/SDA -> C11/C12   (portio)
        {
            InitializePORTEx(PORT_C, PN11|PN12, 0, I2Cx_PORT_OPTS);

            //Pointer to port and data pin mask
            i2c_port_ptr    = PORTC;
            i2c_pnclk_mask  = PN11;
            i2c_pndata_mask = PN12;

            I2CToHigh();
            return MCR_OK;
        }

        case I2C1_MAP_D0D1:    //I2C SCL/SDA -> D0/D1 (portio)
        {
            InitializePORTEx(PORT_D, PN0|PN1, 0, I2Cx_PORT_OPTS);

            //Pointer to port and data pin mask
            i2c_port_ptr    = PORTD;
            i2c_pnclk_mask  = PN0;
            i2c_pndata_mask = PN1;

            I2CToHigh();
            return MCR_OK;
        }

        default: return MCR_INVALID_MAPPING;
    }

    #else
    return MCR_INVALID_MAPPING;

    #endif
}



/* Initialize I2C controller */
MCRESULT InitializeI2C(const I2CConfig *cfg)
{
    MCRESULT r;

    if (!cfg)
        return MCR_INVALID_CFG;

    //Initialize timers
    //Count of clocks per 1/4 period
    i2c_oneq_period = (u32)(GetHCLK() / (cfg->speed << 2));

    if (i2c_oneq_period == 0)
        i2c_oneq_period = 1;

    if ((r = I2CMapPortPins(cfg)) != MCR_OK)
        return r;

    return MCR_OK;
}



/* Turn I2C */
void TurnI2C(I2CBLOCK i2c, u8 turn)
{
    //Software implementation: no acions
}



/* Get I2C speed, bit/s */
u32 GetI2CSpeed(I2CBLOCK i2c)
{
    return (u32)(GetHCLK() / (i2c_oneq_period << 2));
}



/* Wait while transaction in progress */
static s32 WaitI2CTranCompleted()
{
    u32 tm = i2cTimerMs + I2C_TRAN_TIMEOUT;
    u32 cnt = 0;

    //Wait while TRPROG[1]=1 (tx/rx not completed)
    while((tm > i2cTimerMs) && cnt < 5)
    {
        if (I2CIsHighState())        
            cnt++;
        else
            cnt = 0;

        I2CWait(i2c_oneq_period);
    }

    return (tm > i2cTimerMs) ? 0 : -1;
}



/* Set SCL=1 and check SCL state */
__STATIC_INLINE void WaitI2CSCLHigh()
{
    //Timeout: 10 bits
    u32 tm = (10 << 2);

    I2CSCLToHigh();

    //Wait while TRPROG[1]=1 (tx/rx not completed)
    while(tm)
    {
        if (I2CIsSCLHighState())
            break;

        I2CWait(i2c_oneq_period);
        tm--;
    }
}



/* Generate start bit */
__STATIC_INLINE void I2CTxStartBit()
{
    I2CSDAToLow();
    I2CWait(i2c_oneq_period);

    I2CSCLToLow();
    I2CWait(i2c_oneq_period);
}



/* Generate stop bit */
__STATIC_INLINE void I2CTxStopBit()
{
    I2CSDAToLow();
    I2CWait(i2c_oneq_period);

    WaitI2CSCLHigh();
    I2CWait(i2c_oneq_period);

    I2CSDAToHigh();
}



/* Tx 8 data bits and receive ACK/NACK */
__STATIC_INLINE s32 I2CTxByte(u8 val)
{
    s32 nbit = 8;
    u32 isRxNACK;

    while(nbit--)
    {
        //Now SCL low: change SDA
        //Major bit first
        if (val & 0x80)
        {
            I2CSDAToHigh(); //"1"
        }
        else
        {
            I2CSDAToLow();  //"0"
        }

        //Wait 1/4 period
        I2CWait(i2c_oneq_period);


        //SCL high (1/2 period)
        WaitI2CSCLHigh();
        I2CWait(i2c_oneq_period << 1);
        I2CSCLToLow();

        I2CWait(i2c_oneq_period);

        //Major bit first: shift left
        val <<= 1;
    }


    //Read ACK/NACK from slave
    //SDA=1: release data line before rx ACK/NACK
    I2CSDAToHigh();
    I2CWait(i2c_oneq_period);

    WaitI2CSCLHigh();
    I2CWait(i2c_oneq_period);

    //Read SDA state: ACK/NACK
    isRxNACK = I2CIsSDAHighState();

    I2CWait(i2c_oneq_period);
    I2CSCLToLow();

    I2CWait(i2c_oneq_period);

    return isRxNACK;
}



/* Write data to selected I2C controller */
s32 I2CWrite(I2CBLOCK i2c, const void *src, s32 count)
{
    s32 i;
    const u8 *psrc = (u8*)src;
    u8 val;

    I2CToHigh();

    if (WaitI2CTranCompleted() != 0)
        return I2C_RESULT_BUSY;

    for(i = 0; i < count; i++)
    {
        val = psrc[i];

        if (i == 0)
        {
            //psrc[0]: address + RW=0
            val &= 0xFE;

            //Generate start bit
            I2CTxStartBit();
        }

        //Tx address/data byte
        if (I2CTxByte(val) != 0)
        {
            i = I2C_RESULT_NACK;
            break;
        }
    }

    //Generate stop bit
    I2CTxStopBit();

    return i;
}



/* Read data from selected I2C controller */
s32 I2CRead(I2CBLOCK i2c, void *dest, s32 count)
{
    s32 i, nbit;
    u8 *psrc = (u8*)dest;
    u8 val = 0;

    I2CToHigh();

    if (WaitI2CTranCompleted() != 0)
        return I2C_RESULT_BUSY;

    //psrc[0]: address + RW=1 (read)
    val = psrc[0] | 0x01;

    //Generate start bit
    I2CTxStartBit();

    //Tx address byte
    if (I2CTxByte(val) != 0)
    {
        I2CTxStopBit();
        return I2C_RESULT_NACK;
    }


    //Read data bytes
    for(i = 0; i < count; i++)
    {      
        val = 0;
        nbit = 8;        

        //SDA=1: release data line before receive databits
        I2CSDAToHigh();

        while(nbit--)
        {
            //Major bit first
            val <<= 1;

            I2CWait(i2c_oneq_period);

            WaitI2CSCLHigh();
            I2CWait(i2c_oneq_period);

            //Read SDA state "1" or "0" bits
            if (I2CIsSDAHighState())
                val |= 1;

            I2CWait(i2c_oneq_period);
            I2CSCLToLow();            
            I2CWait(i2c_oneq_period);
        }


        //Tx NACK/ACK
        if (i + 1 == count)
        {
            I2CSDAToHigh(); //NACK
        }
        else
        {
            I2CSDAToLow();  //ACK
        }

        I2CWait(i2c_oneq_period);

        WaitI2CSCLHigh();
        I2CWait(i2c_oneq_period << 1);
        I2CSCLToLow();

        I2CWait(i2c_oneq_period);

        psrc[i] = val;
    }

    //Generate stop bit
    I2CTxStopBit();

    return i;
}

/*===========================================================================
 * End of file 1986BE1x_I2C.c
 *===========================================================================*/
