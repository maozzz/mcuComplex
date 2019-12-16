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
 *  File 1986BE9x_IWire.c: 1-Wire functions.
 *  Note:
 *   1. MCUs 1986x, 1901x does not has 1-Wire controller, 
 *      software implementation used
 *
 *===========================================================================*/

#include "../../inc/IWire.h"
#include "../../inc/SysTimer.h"
#include <string.h>

//Reset/presence signals time intervals
//Reset Time Low, usec (>480usec)
#define IWIRE_TIME_TRSTL                (480+20)

//Reset Time High, usec (>480usec > TPDHIGH+TPDLOW)
#define IWIRE_TIME_TRSTH                (480+20)

//Presence-Detect High, usec ([15..60]usec)
#define IWIRE_TIME_TPDHIGH              (8)

//Presence-Detect Low, usec ([60..240]usec)
#define IWIRE_TIME_TPDLOW               (50)

//High state after presence signal, usec
#define IWIRE_TIME_TPDHIGH2             (100)

//Polling data line interval, usec (2..3usec)
#define IWIRE_TIME_TPOLL                2


//Time slot, usec (>60 usec)
#ifndef IWIRE_TIME_TSLOT
#define IWIRE_TIME_TSLOT                62
#endif

//Read/write bit: initiate tx/rx bit, usec ([2..15] usec)
#ifndef IWIRE_TIME_TINT
#define IWIRE_TIME_TINT                 3
#endif

//Read bit: read bit moment (TRC < 15usec - TINT), usec
#ifndef IWIRE_TIME_TRC
#define IWIRE_TIME_TRC                 (15 - IWIRE_TIME_TINT - 3)
#endif

//Pause between bits, usec (>1 usec)
#ifndef IWIRE_TIME_TREC
#define IWIRE_TIME_TREC                 7
#endif


//List of pointers to port's registers
static _port *mcu_ports_list[6] = { PORTA, PORTB, PORTC, PORTD, PORTE, PORTF };

//Pointer to 1-Wire port
static _port *iwire_port_ptr;

//Data line mask
static u32 iwire_pndata_mask;

//1-Wire timers, not constants, depended from HCLK
static u32 iwire_time_trec;
static u32 iwire_time_trstl;
static u32 iwire_time_tpoll;
static u32 iwire_time_tint;
static u32 iwire_time_tendslot;
static u32 iwire_time_tslot;
static u32 iwire_time_trc;
static u32 iwire_time_twait_trec;


//Toggle data line to high state ("In", Z-state)
#define IWireToHigh()                   iwire_port_ptr->OE &= ~iwire_pndata_mask

//Toggle data line to low state ("Out" and "0")
#define IWireToLow()                    iwire_port_ptr->OE |= iwire_pndata_mask; \
                                        iwire_port_ptr->RXTX &= ~iwire_pndata_mask

//Indicates that data line in high state (bit "1")
#define IWireIsHighState()              (iwire_port_ptr->RXTX & iwire_pndata_mask)

//Wait specified number of clocks
#define IWireWait(nclk)                 WaitSysTimer(SYSTIM_N1, nclk)


/* */
MCRESULT InitializeIWire(const IWireConfig *cfg)
{
    u32 usec, pinnum;
    PORTBLOCK portnum;

    if (!cfg)
        return MCR_INVALID_CFG;

    //"mapping" field:
    //bits[15..8]: port number [0..5] => [PA..PF]
    //bits[7..0]:  data pin number [0..15]
    portnum = (PORTBLOCK)((cfg->mapping >> 8) & 0xFF);
    pinnum  = (cfg->mapping & 0xFF);

    //Check configuration
    //Milandr MCUs 1986x, 1901x: 6 ports x 16bit
    if (portnum > 5 || pinnum > 15)
        return MCR_INVALID_CFG;

    //Pointer and data pin mask
    iwire_port_ptr    = mcu_ports_list[portnum];
    iwire_pndata_mask = (1U << pinnum);


    //Initialize timers
    //Count of clocks per usec
    usec = (u32)(GetHCLK() / 1000000U);

    iwire_time_trec       = IWIRE_TIME_TREC  * usec;
    iwire_time_trstl      = IWIRE_TIME_TRSTL * usec;
    iwire_time_tpoll      = IWIRE_TIME_TPOLL * usec;
    iwire_time_tint       = IWIRE_TIME_TINT  * usec;
    iwire_time_tendslot   = (IWIRE_TIME_TSLOT - IWIRE_TIME_TINT + IWIRE_TIME_TREC) * usec;
    iwire_time_tslot      = IWIRE_TIME_TSLOT * usec;
    iwire_time_trc        = IWIRE_TIME_TRC   * usec;
    iwire_time_twait_trec = (IWIRE_TIME_TSLOT - IWIRE_TIME_TINT - IWIRE_TIME_TRC + IWIRE_TIME_TREC) * usec;


    //Initialize port
    //Note that: output, open drain and pullup
    InitializePORTEx(portnum, iwire_pndata_mask, 0,
                     PN_PORTIO|PN_PWR_FAST|PN_PULLUP|PN_NOPULLDOWN|
                     PN_NOINFILTER|PN_NOSHMIT|PN_OPENDRAIN);


    //Release line (toggle to input)
    IWireToHigh();

    return MCR_OK;
}



/* */
void TurnIWire(IWIREBLOCK iwire, u8 on)
{
    //Software implementation: no actions
}



/* */
s32 IWireResetPresence(IWIREBLOCK iwire)
{
    u32 tmTRSTH, tmPollState, isDLHigh;
    u32 tmTPDH, tmTPDL, tmTPDH2;

    //Release line
    IWireToHigh();
    IWireWait(iwire_time_trec);

    //Reset signal: TRSTL > 480us
    IWireToLow();
    IWireWait(iwire_time_trstl);
    IWireToHigh();

    //<-----       TRSTH > 480us ---------->
    //[15..60]us     [60..240]us    [>100]us
    //  TPDHIGH       TPDLOW        TPDHIGH2
    tmTRSTH = tmTPDH = tmTPDL = tmTPDH2 = 0;
    tmPollState = 0;
    while(tmTRSTH < IWIRE_TIME_TRSTH)
    {
        IWireWait(iwire_time_tpoll);
        tmTRSTH += IWIRE_TIME_TPOLL;

        isDLHigh = IWireIsHighState() ? 1 : 0;

        switch(tmPollState)
        {
            case 0: //High state before presence signal
            {
                if (isDLHigh)
                {
                    tmTPDH += IWIRE_TIME_TPOLL;
                    if (tmTPDH > IWIRE_TIME_TPDHIGH)
                        tmPollState = 1; //0->1
                }
                else
                    tmTPDH = 0;

                break;
            }

            case 1: //Presence signal (low state)
            {
                if (!isDLHigh)
                {
                    tmTPDL += IWIRE_TIME_TPOLL;
                    if (tmTPDL > IWIRE_TIME_TPDLOW)
                        tmPollState = 2; //1->2
                }
                else
                    tmTPDL = 0;

                break;
            }

            case 2: //High state after presence signal
            {
                if (isDLHigh)
                {
                    tmTPDH2 += IWIRE_TIME_TPOLL;
                    if (tmTPDH2 > IWIRE_TIME_TPDHIGH2)
                        tmPollState = 3; //2->3
                }
                else
                    tmTPDH2 = 0;

                break;
            }

            //No actions
            default: break;
        } //switch

    } //while

    if (tmPollState == 3)
        return IWIRE_RESULT_OK;
    else
    {
        if (tmTPDH == 0)
            return IWIRE_RESULT_SHORTCURCUIT;
        else
            return IWIRE_RESULT_NOPRESENCE;
    }
}



/* */
s32 IWireWriteByte(IWIREBLOCK iwire, u8 val)
{
    s32 i = 8;

    while(i--)
    {
        //Low state: bit start
        IWireToLow();

        if (val & 1)
        {
            //Wait interval
            IWireWait(iwire_time_tint);

            //Bit "1": high state
            IWireToHigh();

            //Wait time to end of slot + bit pause
            IWireWait(iwire_time_tendslot);
        }
        else
        {
            //Low state (bit start) + bit "0"
            IWireWait(iwire_time_tslot);

            //Toggle to high state
            IWireToHigh();

            //Pause between bits
            IWireWait(iwire_time_trec);
        }

        val >>= 1;
    }

    return 1;
}



/* */
u8 IWireReadByte(IWIREBLOCK iwire)
{
    s32 i = 8;
    u8 val = 0;

    while(i--)
    {
        //Low bit first
        val >>= 1;

        //Bit start: low state [1..15] usec
        IWireToLow();
        IWireWait(iwire_time_tint);

        //Release line
        IWireToHigh();
        IWireWait(iwire_time_trc);

        //Line at high state: bit "1"
        if (IWireIsHighState())
            val |= 0x80;

        //Wait to end of slot + pause between bits
        IWireWait(iwire_time_twait_trec);
    }

    return val;
}



/* */
s32 IWireWrite(IWIREBLOCK iwire, const void *src, s32 n)
{
    s32 i;
    const u8 *psrc = (u8*)src;

    for(i = 0; i < n; i++)
    {
        IWireWriteByte(iwire, psrc[i]);
    }

    return i;
}



/* */
s32 IWireRead(IWIREBLOCK iwire, void *dest, s32 n)
{
    s32 i;
    u8 *psrc = (u8*)dest;

    for(i = 0; i < n; i++)
    {
        psrc[i] = IWireReadByte(iwire);
    }

    return i;
}



/* Scan 1-Wire bus, one iteration (one ROM search) */
static s32 IWireScanBus0(u8 *rxrom,            //out
                         u32 *confcount,       //out
                         u8 *confbitpos,       //in/out
                         const u8 *confbitval) //in
{
    s32 r, bitcnt, rxbit, rxnbit, txbit;
    u32 ccnt = 0;
    u8 mask;

    //Clear out variables
    memset(rxrom, 0, 8);
    *confcount = 0;

    //No 1-Wire devices or short curcuit: return error
    if ((r = IWireResetPresence(IWIRE_N1)) != IWIRE_RESULT_OK)
        return r;

    //1-Wire device presence
    //Tx "Search ROM" command
    IWireWriteByte(IWIRE_N1, IWIRE_CMD_SEARCHROM);

    //ROM: 64 bits
    bitcnt = 0;
    while(bitcnt < 64)
    {
        //----------------------
        //RX ROM BIT
        //Bit start: low state [1..15] usec
        IWireToLow();
        IWireWait(iwire_time_tint);

        //Release line
        IWireToHigh();
        IWireWait(iwire_time_trc);

        //Line at high state: bit "1"
        if (IWireIsHighState())
        {
            rxbit = 1;
        }
        else
        {
            rxbit = 0;
        }

        //Wait to end of slot + pause between bits
        IWireWait(iwire_time_twait_trec);

        //----------------------
        //RX ROM ~BIT
        //Bit start: low state [1..15] usec
        IWireToLow();
        IWireWait(iwire_time_tint);

        //Release line
        IWireToHigh();
        IWireWait(iwire_time_trc);

        //Line at high state: bit "1"
        if (IWireIsHighState())
        {
            rxnbit = 1;
        }
        else
        {
            rxnbit = 0;
        }

        //Wait to end of slot + pause between bits
        IWireWait(iwire_time_twait_trec);


        //----------------------
        //Check rx bit and ~bit
        mask = 1 << (bitcnt & 7);

        if (rxbit == 0 && rxnbit == 0)
        {
            //00 - conflict, select branch 0 or 1
            txbit = confbitval[(bitcnt >> 3)] & mask;

            //Save conflict position
            if (txbit == 0)
            {
                ccnt++;
                confbitpos[(bitcnt >> 3)] |= mask;
            }
        }
        else
        {
            //01 or 10
            txbit = rxbit;

            //Don't change confbitval, confbitpos here
        }


        //Fill dest. buffer, low bit first
        rxrom[(bitcnt >> 3)] >>= 1;

        if (txbit)
            rxrom[(bitcnt >> 3)] |= 0x80;

        //----------------------
        //TX BIT (SELECT BRANCH)
        //Low state: bit start
        IWireToLow();

        if (txbit)
        {
            //Wait interval
            IWireWait(iwire_time_tint);

            //Bit "1": high state
            IWireToHigh();

            //Wait time to end of slot + bit pause
            IWireWait(iwire_time_tendslot);
        }
        else
        {
            //Low state (bit start) + bit "0"
            IWireWait(iwire_time_tslot);

            //Toggle to high state
            IWireToHigh();

            //Pause between bits
            IWireWait(iwire_time_trec);
        }

        bitcnt++;
    } //while

    *confcount = ccnt;

    return 0;
}



/* Scan 1-Wire bus (search ROMs) */
s32 IWireScanBus(IWIREBLOCK iwire, u8 *roms, s32 maxcount)
{
    s32 r, i, foundcount;
    u32 confcount;
    //Current found rom
    u8 rom[8];
    //Conflict bit positions/values
    u8 confbitpos[8];
    u8 confbitval[8];
    u8 mask;

    //Clear out array
    memset(roms, 0, maxcount * 8);

    //Clear conflict positions/values
    memset(confbitpos, 0, 8);
    memset(confbitval, 0, 8);

    confcount  = 1;
    foundcount = 0;
    r = 0;

    while (confcount && foundcount < maxcount)
    {
        //ROM: 64 bits
        for(i = 63; i >= 0; i--)
        {
            mask = 1 << (i & 7);

            //Clear bit value
            confbitval[(i >> 3)] &= ~mask;

            if (confbitpos[(i >> 3)] & mask)
            {
                //Select 1 instead 0 at next scan
                confbitval[(i >> 3)] |= mask;

                //Clear conflict position
                confbitpos[(i >> 3)] &= ~mask;
                break;
            }
        }

        //Scan 1-Wire bus
        if ((r = IWireScanBus0(rom, &confcount, confbitpos, confbitval)) == 0)
        {
            memcpy(&roms[(foundcount << 3)], rom, 8);
            foundcount++;
        }
        else
            break;
    }

    return r;
}

/*===========================================================================
 * End of file 1986BE9x_IWire.c
 *===========================================================================*/
