/*===========================================================================
 *
 *  MCU library
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2014-2016
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File 1901BC1x_SDIO.c: SDIO functions.
 *
 *===========================================================================*/

#include "../../inc/SDIO.h"
#include "../../../std/inc/CRC.h"
#include <string.h>

#ifdef __SDIO_DEBUG_MESSAGES
#include <stdcf.h>
#define PrintMsg(...)       std_printf(__VA_ARGS__)
#else
#define PrintMsg(...)
#endif


//SDIO time counter, msecs
volatile u32 sdioTimerMs = 0;

//CLK pin options: note that pull down
#define SDIOx_PORT_CLK_OPTS  (PN_NOPULLUP   | PN_PULLDOWN | PN_NOSHMIT |\
                              PN_CTRLDRIVER | PN_NOINFILTER | PN_PWR_FASTEST)

//CMD and DAT0..DAT3 pins options: note that pull up
#define SDIOx_PORT_CMDDAT_OPTS  (PN_PULLUP   | PN_NOPULLDOWN | PN_NOSHMIT |\
                                 PN_CTRLDRIVER | PN_NOINFILTER | PN_PWR_FASTEST)

/* */
static MCRESULT SDIOMapPortPins(const SDIOConfig *cfg)
{
    #if (TARGET_MCU == MCU_1901BC1x)
    //MCU: 1901BC1x
    switch(cfg->mappins)
    {
        case SDIO1_MAP_C8C13:
        {
            //[C8..C13]->[CLK,CMD,DATA0..DATA3] (basic)
            InitializePORTEx(PORT_C, PN8, 0, PN_BASIC|SDIOx_PORT_CLK_OPTS);
            InitializePORTEx(PORT_C, PN9|PN10|PN11|PN12|PN13, 0, PN_BASIC|SDIOx_PORT_CMDDAT_OPTS);
            break;
        }

        case SDIO1_MAP_C1C8C13:
        {            
            //[C1]->[CMD] (remap);
            //[C8,C10..C13]->[CLK,DATA0..DATA3] (basic)
            InitializePORTEx(PORT_C, PN1, 0, PN_REMAP|SDIOx_PORT_CMDDAT_OPTS);
            InitializePORTEx(PORT_C, PN8, 0, PN_BASIC|SDIOx_PORT_CLK_OPTS);
            InitializePORTEx(PORT_C, PN10|PN11|PN12|PN13, 0, PN_BASIC|SDIOx_PORT_CMDDAT_OPTS);
            break;
        }

        case SDIO1_MAP_C0C5:
        {
            //[C0..C5]->[CLK,CMD,DATA0..DATA3] (remap)
            InitializePORTEx(PORT_C, PN0, 0, PN_REMAP|SDIOx_PORT_CLK_OPTS);
            InitializePORTEx(PORT_C, PN1|PN2|PN3|PN4|PN5, 0, PN_REMAP|SDIOx_PORT_CMDDAT_OPTS);
            break;
        }

        case SDIO1_MAP_A0A5:
        {
            //[A0..A5]->[CLK,CMD,DATA0..DATA3] (alter)
            InitializePORTEx(PORT_A, PN0, 0, PN_ALTER|SDIOx_PORT_CLK_OPTS);
            InitializePORTEx(PORT_A, PN1|PN2|PN3|PN4|PN5, 0, PN_ALTER|SDIOx_PORT_CMDDAT_OPTS);
            break;
        }

        default:
            return MCR_INVALID_MAPPING;
    }

    #else
    //Other MCUs: no SDIO
    return MCR_INVALID_MAPPING;
    #endif

    return MCR_OK;
}


/*
CR register:
CLKOE[21]     : 0/1 - slave/master (CLK output enable)
WORK2[20]     : 1/0 - start/stop command transaction
ENDBUSY[19]   : 1/0 - noset/set BUSY to DAT0
WRITECMD[18]  : 1/0 - send/nosend answer 101b to write command
WIDTHDAT[17]  : 1/0 - 1bit/4bit data
RXFIE_CMD[16] : 1/0 - enable/disable interrupt "RX CMD FIFO FULL"
RXNEIE_CMD[15]: 1/0 - enable/disable interrupt "RX CMD FIFO NOT EMPTY"
TXEIE_CMD[14] : 1/0 - enable/disable interrupt "TX CMD FIFO EMPTY"
RXFIE_DAT[13] : 1/0 - enable/disable interrupt "RX DATA FIFO FULL"
RXNEIE_DAT[12]: 1/0 - enable/disable interrupt "RX DATA FIFO NOT EMPTY"
TXEIE_DAT[11] : 1/0 - enable/disable interrupt "TX DATA FIFO EMPTY"
CRCEN_CMD[10] : 1/0 - enable/disable command CRC hardware calculation
CRCEN_DAT[9]  : 1/0 - enable/disable DAT3..DAT0 CRC hardware calculation
BR[8..6]      : RATE = PCLK/(2^(n + 1)), n=[1..7]
SBITDAT[5]    : 1/0 - on/off wait (or tx) data start bit
SBITCMD[4]    : 1/0 - on/off wait command start bit
WORK1[3]      : 1/0 - start/stop data transaction
DIRDAT[2]     : 1 - DATx as OUTPUTS; 0 - DATx as INPUTS
DIRCMD[1]     : 1 - CMD as OUTPUT; 0 - CMD as INPUT
SDE[0]        : 1/0 - on/off SDIO peripherals

SR register:
FIFODAT_FUL[3]:   1 - DATA FIFO full
FIFOCMD_FUL[2]:   1 - CMD FIFO full
FIFODAT_EMPTY[1]: 1 - DATA FIFO empty
FIFOCMD_EMPTY[0]: 1 - CMD FIFO empty
*/



/* Initialize SDIO controller */
MCRESULT InitializeSDIO(const SDIOConfig *cfg)
{    
    MCRESULT r;

    //Initialize port
    if ((r = SDIOMapPortPins(cfg)) != MCR_OK)
        return r;

    //Enable clock
    ClkEnable(CLK_SDI0);

    //CLKOE[21]=1:     master - CLK output enable
    //WIDTHDAT[17]=0:  4bit data
    //CRCEN_CMD[10]=1: use hardware calculation of command CRC
    //CRCEN_DAT[9]=1:  use hardware calculation of data CRC
    //BR[8..6]:        RATE = PCLK/(2^(n + 1)), n=[1..7]
    SDIO->CR = PN21 | PN10 | PN9 | (7 << 6);

    //Disable IRQs from SDIO block
    IRQDisable(IRQ_SDIO);

    //PrintMsg("SDIO init OK\n");

    return MCR_OK;
}




/* Set SDIO speed, bit/s */
void SDIOSetSpeed(SDIOBLOCK sdio, u32 speed)
{    
    s32 i, d, dspeed;
    u32 hclk, br;

    hclk = GetHCLK();

    //Minimum speed 400KHz
    if (speed == 0)
        speed = 400000;

    //BR[8..6]: RATE = HCLK/(2^(n + 1)), n=[1..7]
    i = br = 1;
    dspeed = (s32)hclk;
    while(i < 8)
    {
        d = (s32)speed - (s32)(hclk >> (i + 1));

        if (d < 0)
            d = -d;

        if (dspeed > d)
        {
            dspeed = d;
            br = i;

            if (d == 0)
                break;
        }

        i++;
    }

    //Set BR[8..6]
    SDIO->CR = (SDIO->CR & ~(7 << 6)) | (br << 6);
}



/* Get SDIO speed (frequency on clock line), Hz */
u32 GetSDIOSpeed(SDIOBLOCK sdio)
{
    u32 cr = SDIO->CR;

    //SDE[0]=0: - SDIO is turned off, return 0 speed
    if ((cr & PN0) == 0)
        return 0;

    //BR[8..6]: RATE = PCLK/(2^(n + 1)), n=[1..7]
    return (u32)(GetHCLK() >> (((cr >> 6) & 0x07) + 1));
}



/* */
void TurnSDIO(SDIOBLOCK sdio, u8 on)
{
    //SDE[0]: 1/0 - turn SDIO on/off
    if (on)
        SDIO->CR |= PN0;
    else
        SDIO->CR &= ~PN0;
}



/* Tx 8 clocks, CMD line at high state */
void SDIOTxClock(SDIOBLOCK sdio)
{
    //WORK2[20]=0:  stop previous command transaction
    //SBITCMD[4]=0: no wait start bit
    do
    {
        SDIO->CR &= ~(PN20|PN4);
    } while (SDIO->CR & PN20);

    //DIRCMD[1]=1: CMD as output (tx command)
    SDIO->CR |= PN1;

    //CRCEN_CMD[10]=0: no use hardware calculation of command CRC
    //SBITCMD[4]=0:    no generate start bit
    SDIO->CR &= ~(PN10|PN4);

    //Clear CRC
    SDIO->CMDCRC = 0x00000000;

    //Write command and argument to command fifo
    SDIO->CMDDR = 0xFF;

    //Tx 8 clocks
    SDIO->CMD_TRANSFER = 8;

    //WORK2[20]=1: start tx clock
    SDIO->CR |= PN20;

    //Wait tx command completed
    while (1)
    {
        if ((SDIO->CR & PN20) == 0)
            break;
    }
}



/* Tx command */
s32 SDIOWriteCommand(SDIOBLOCK sdio, u32 idcmd, u32 arg)
{
    SDIOTxClock(sdio);

    //WORK2[20]=0: stop previous command transaction
    do
    {
        SDIO->CR &= ~(PN20|PN4);
    } while (SDIO->CR & PN20);


    //CRCEN_CMD[10]=1: use hardware calculation of command CRC
    //CRCEN_DAT[9]=1:  use hardware calculation of data CRC
    //SBITCMD[4]=1:    wait start bit
    //DIRCMD[1]=1:     CMD as output (tx command)
    SDIO->CR |= (PN10 | PN9 | PN4 | PN1);    

    //Command index: 6 bits
    idcmd = 0x40 | (idcmd  & 0x3F);

    //Clear CRC
    SDIO->CMDCRC = 0x00000000;    

    //Write command and argument to command fifo
    //SDIO tx: low byte first; major bit first
    SDIO->CMDDR = (idcmd) |
                  ((arg >> 16) & 0x0000FF00U) | //arg bits[31..24] (high byte)
                  ((arg)       & 0x00FF0000U) | //arg bits[23..16]
                  ((arg << 16) & 0xFF000000U) ; //arg bits[15..8]

    //arg bits[7..0] (low byte)    
    SDIO->CMDDR = (arg & 0x000000FFU);    

    //Command length: 48 bits = 01b + idcmd:6 + arg:32 + crc:7 + stop:1
    SDIO->CMD_TRANSFER = 48;

    //WORK2[20]=1: start tx command
    SDIO->CR |= PN20;    

    //Wait tx command completed
    while (1)
    {
        if ((SDIO->CR & PN20) == 0)
            break;
    }    

    return 0;
}



/* Start receive response */
s32 SDIOReadResponse(SDIOBLOCK sdio, u32 *dest, u32 bitlen, u32 tmout)
{
    u32 rxfifo[5];
    u32 rxtimer;
    u8 rxcrc;

    //DIRCMD[1]=0:  CMD as input (rx response)
    SDIO->CR &= ~PN1;

    //Set requested response length
    SDIO->CMD_TRANSFER = bitlen;

    //CRCEN_CMD[10]=1: use hardware calculation of command CRC
    //CRCEN_DAT[9]=1:  use hardware calculation of data CRC
    //SBITCMD[4]=1:    wait start bit
    SDIO->CR |= (PN10 | PN9 | PN4);

    //Clear CRC
    SDIO->CMDCRC = 0x00000000;    

    //Timer
    rxtimer = sdioTimerMs + tmout;

    //WORK2[20]=1: start rx response
    SDIO->CR |= PN20;

    //Wait transaction completed
    while(1)
    {
        if (rxtimer < sdioTimerMs)
        {            
            //Receive timeout
            //WORK2[20]=0: stop command transaction
            //SBITCMD[4]=0: off wait command start bit
            SDIO->CR &= ~(PN20|PN4);            
            return -1;
        }
        else if ((SDIO->CR & PN20) == 0)        
            break;
    }


    if (bitlen > 48)
    {
        //Long response (CID/CSD registers)
        //136bits = 17 bytes = [0x3F] + [15 bytes data] + [CRC7 and stopbit]
        //byte:
        // "0" is 0x3F;
        // "g" is CRC7 + stop bit;
        // "x" is unused and zero
        //rxfifo[]: 0123 4567 89ab cdef xxxg;
        //dest[]:   1234 5678 9abc defg;

        //Read FIFO
        rxfifo[0] = SDIO->CMDDR;
        rxfifo[1] = SDIO->CMDDR;
        rxfifo[2] = SDIO->CMDDR;
        rxfifo[3] = SDIO->CMDDR;
        rxfifo[4] = SDIO->CMDDR;

        dest[0] = ((rxfifo[0] << 8) & 0xFFFFFF00U) | ((rxfifo[1] >> 24) & 0x000000FFU);
        dest[1] = ((rxfifo[1] << 8) & 0xFFFFFF00U) | ((rxfifo[2] >> 24) & 0x000000FFU);
        dest[2] = ((rxfifo[2] << 8) & 0xFFFFFF00U) | ((rxfifo[3] >> 24) & 0x000000FFU);
        dest[3] = ((rxfifo[3] << 8) & 0xFFFFFF00U) | ((rxfifo[4] >>  0) & 0x000000FFU);

        //Don't check CRC7 for long response (rxfifo[4] and SDIO->CMDCRC).
        //SDIO host erroneously solve CRC with first byte of response (0x3F)
        //but SD card solve CRC only for 15 bytes of CID/CSD register
        return 0;
    }
    else
    {
        //Short response
        //48bits = 6 bytes = [command index] + [4 bytes data] + [CRC7 and stopbit]
        //byte:
        // "0" is command index;
        // "5" is CRC7 + stop bit;
        // "x" is unused and zero
        //rxfifo[]: 0123 xx45;
        //dest[]:   1234

        //Read FIFO
        rxfifo[0] = SDIO->CMDDR;
        rxfifo[1] = SDIO->CMDDR;

        dest[0] = ((rxfifo[0] << 8) & 0xFFFFFF00U) | ((rxfifo[1] >> 8) & 0x000000FFU);

        //Check CRC7 of response
        rxcrc = (rxfifo[1] >> 1) & 0x7F;

        if (rxcrc == ((u8)SDIO->CMDCRC) ||
            rxcrc == 0x7F ||
            rxcrc == 0x00)
            return 0;   //OK
        else
            return -1;  //FAIL
    }
}



/* Retrieve CRC from received data (from specified data line) */
static u32 SDIOGetRxCRC(u32 rxword, u32 rxword2, u32 mask)
{
    u32 i = 0, rxcrc = 0;

    while(i < 8)
    {
        rxcrc <<= 1;

        if (rxword & mask)
            rxcrc |= 0x0100;

        if (rxword2 & mask)
            rxcrc |= 0x0001;

        rxword  <<= 4;
        rxword2 <<= 4;
        i++;
    }

    return rxcrc;
}



/*  */
s32 SDIORead(SDIOBLOCK sdio, void *dest, s32 count)
{
    s32 i;
    u8 *pdest = (u8*)dest;
    u32 rxword, rxword2, rxstopb;
    u32 rxcrc3, rxcrc2, rxcrc1, rxcrc0;
    u32 rxtimer;

    //DIRDAT[2]=0: DATx as INPUTS
    SDIO->CR &= ~PN2;

    //Set count of bits to receive (+17: CRC16 + stop bit)
    SDIO->DAT_TRANSFER = (count << 3) + (16 + 1);

    //SBITDAT[5]=1: wait start bit on DATx lines
    SDIO->CR |= PN5;

    //Clear CRC
    SDIO->DAT0CRC = 0x00000000;
    SDIO->DAT1CRC = 0x00000000;
    SDIO->DAT2CRC = 0x00000000;
    SDIO->DAT3CRC = 0x00000000;

    //Initiate data transaction
    //WORK1[3]=1: start data transaction
    SDIO->CR |= PN3;

    i = 0;
    rxtimer = sdioTimerMs + 100;

    //Wait for end of the transaction
    while(1)
    {
      if (rxtimer < sdioTimerMs)
      {
          //Receive timeout
          //SBITDAT[5]=0: no wait start bit
          //WORK1[3]=0: stop data transaction
          SDIO->CR &= ~(PN5|PN3);
          return 0;
      }

      //FIFODAT_EMPTY[1]=0: data FIFO is NOT empty: read FIFO
      if (((SDIO->SR & PN1) == 0) && (i < count))
      {
          rxword = SDIO->DATDR;
          pdest[i]     = (u8)((rxword >> 24) & 0x000000FF);
          pdest[i + 1] = (u8)((rxword >> 16) & 0x000000FF);
          pdest[i + 2] = (u8)((rxword >>  8) & 0x000000FF);
          pdest[i + 3] = (u8)(rxword         & 0x000000FF);

          i += 4;
      }

      //WORK1[3]=0: data transaction completed
      if ((SDIO->CR & PN3) == 0)
          break;
    }

    //Read last data from FIFO
    while (i < count)
    {
        rxword = SDIO->DATDR;
        pdest[i]     = (u8)((rxword >> 24) & 0x000000FF);
        pdest[i + 1] = (u8)((rxword >> 16) & 0x000000FF);
        pdest[i + 2] = (u8)((rxword >>  8) & 0x000000FF);
        pdest[i + 3] = (u8)(rxword         & 0x000000FF);

        i += 4;
    }    

    //Read received 4x16bit CRC16 and 4 stop bits from FIFO
    //"rstopb" must contains value 0x0F: 4 stop bits
    rxword  = SDIO->DATDR;
    rxword2 = SDIO->DATDR;
    rxstopb = SDIO->DATDR;

    rxcrc3 = SDIOGetRxCRC(rxword, rxword2, 0x80000000U);
    rxcrc2 = SDIOGetRxCRC(rxword, rxword2, 0x40000000U);
    rxcrc1 = SDIOGetRxCRC(rxword, rxword2, 0x20000000U);
    rxcrc0 = SDIOGetRxCRC(rxword, rxword2, 0x10000000U);

    //Caculated CRC stored at "DATxCRC" registers
    /*PrintMsg("RxCRC %04X %04X %04X %04X\n"
             "HwCRC %04X %04X %04X %04X\n",
             rxcrc3, rxcrc2, rxcrc1, rxcrc0,
             (u32)SDIO->DAT3CRC, (u32)SDIO->DAT2CRC, (u32)SDIO->DAT1CRC, (u32)SDIO->DAT0CRC);*/

    //Check CRC16
    if (rxcrc3 == (u32)(SDIO->DAT3CRC) &&
        rxcrc2 == (u32)(SDIO->DAT2CRC) &&
        rxcrc1 == (u32)(SDIO->DAT1CRC) &&
        rxcrc0 == (u32)(SDIO->DAT0CRC) &&
        rxstopb == 0x0F)
        return count;
    else
        return 0;
}



/* */
s32 SDIOWrite(SDIOBLOCK sdio, const void *src, s32 count)
{
    s32 i;
    const u8 *psrc = (u8*)src;

    //DIRDAT[2]=1: DATx as OUTPUTS
    SDIO->CR |= PN2;

    SDIO->DAT0CRC = 0x00000000;
    SDIO->DAT1CRC = 0x00000000;
    SDIO->DAT2CRC = 0x00000000;
    SDIO->DAT3CRC = 0x00000000;

    //Write tx data to FIFO
    i = 0;
    while (i < count)
    {
        SDIO->DATDR =  (u32)psrc[i] |
                      ((u32)psrc[i + 1] << 8)  |
                      ((u32)psrc[i + 2] << 16) |
                      ((u32)psrc[i + 3] << 24);
        i += 4;
    }


    //Set count of bits to transmit (+17: CRC16 + stop bit)
    SDIO->DAT_TRANSFER = (count << 3) + (16 + 1);

    //SBITDAT[5]=1: generate start bit on DATx lines
    SDIO->CR |= PN5;

    //WORK1[3]=1: start data transaction
    SDIO->CR |= PN3;


    while(1)
    {
        //Add CRC and stop bit
        //FIFODAT_FULL[3]=0: FIFO is not full
        if ((i == count) && (SDIO->SR & PN3) == 0)
        {
              SDIO->DATDR = 0x00000000;
              i += 2;
        }

        //WORK1[3]=0: data transaction completed
        if ((SDIO->CR & PN3) == 0)
            break;
    }

    //DIRDAT[2]=0: DATx as INPUTS
    SDIO->CR &= ~PN2;

    return count;
}


/* IRQs not used
void SDIO_IRQHandler()
{

}*/

/*===========================================================================
 * End of file 1901BC1x_SDIO.c
 *===========================================================================*/
