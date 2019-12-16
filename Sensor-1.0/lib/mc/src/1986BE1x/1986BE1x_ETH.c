/*===========================================================================
 *
 *  MCU library
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2014-2018
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File 1986BE1x_ETH.c: Ethernet controller functions.
 *
 *===========================================================================*/

#include "../../inc/ETH.h"

#include <string.h>

__io_reg16 *ETH_PHY_CTRL   = ((__io_reg16 *)BASE_ETH_PHY_CTRL);
__io_reg16 *ETH_PHY_STATUS = ((__io_reg16 *)BASE_ETH_PHY_STATUS);

/*typedef struct tag_ETHClkParams
{
        u32 clk;
} ETHClkParams;

MCRESULT TuneETHClkParams(ETHClkParams *p)
{
        return MCR_OK;
}*/

//Pointer to user-defined frame handler
static void (*EthFrameReceived)(void) = 0;

/* Initialize Ethernet controller */
MCRESULT InitializeETH(const ETHConfig *cfg)
{        
        u16 val16, phyMode;

        //1. Initialize generator 25MHz
        //Initialize PF0, PF1 as analog inputs
        //PF0 - OSC_IN_25 (Input 25MHz)
        InitializePORTEx(PORT_F, PN0, 0, PN_ANALOG  | PN_INPUT |
                        PN_NOPULLUP | PN_NOPULLDOWN |
                        PN_NOSHMIT  | PN_CTRLDRIVER |
                        PN_NOINFILTER);

        //PF1 - OSC_OUT_25 (Output 25MHz)
        InitializePORTEx(PORT_F, PN1, 0, PN_ANALOG | PN_PWR_FASTEST |
                        PN_NOPULLUP | PN_NOPULLDOWN |
                        PN_NOSHMIT  | PN_CTRLDRIVER |
                        PN_NOINFILTER);

        //Turn HSE2
        //HSEBYP2[3] = 0, HSEON2[2] = 1
        RST_CLK->HS_CONTROL &= ~PN3;
        RST_CLK->HS_CONTROL |= PN2;

        //Wait HSE2 ready (HSERDY2[3])
        while(!(RST_CLK->CLOCK_STATUS & PN3));

        //--------------------------------------------
        //2. Setup ethernet clock parameters
        //Clear bits except: Reserved[31,30], MAN_CLK_EN[25], MAN_BRG[15..8]
        RST_CLK->ETH_CLOCK &= (PN31 | PN30 | PN25| (0xFF << 8));

        //PHY_CLK_SEL[29..28]: [0,1,2,3] => [HSI,HSE,PLLCPU,HSE2]
        //PHY_CLK_EN[27]     : 1/0 - enable/disable PHY clock
        //SLEEP[26]          : 1/0 - sleep/work
        //ETH_CLK_EN[24]     : 1/0 - enable/disable MAC clock
        //PHY_BRG[23..16]    : [0..7]; PHY_CLK = PHY1_CLK / (1 << PHY_BRG)
        //ETH_BRG[7..0]      : should set 0
        //Select HSE2, enable PHY/MAC clock, no sleep, PHY_BRG=0, ETH_BRG=0
        RST_CLK->ETH_CLOCK |= ((3 << 28) | PN27 | PN24);


        //--------------------------------------------
        //3. Setup PHY parameters
        //Load PHY control register (16 bit)
        val16 = *ETH_PHY_CTRL;

        //Reset PHYADD[15..11], FX_EN[7], MODE[3..1], nRST[0]
        //Without change MDC[10], MDIO_SEL[9], MDI[8], RESERVED[6..4]
        val16 &= 0x0770;

        //MODE[3..1]: (modes [0..4] without autoneg)
        //0 - 10BaseT  HD; 1 - 10BaseT  FD;
        //2 - 100BaseT HD; 3 - 100BaseT FD;
        //4 - 100BaseT HD + autoneg;
        //5 - hub mode; 6 - low consump; 7 - full autoneg

        if (cfg->opts & ETH_OPTS_FULL_AUTONEG)
                phyMode = 7;
        else if (cfg->opts & ETH_OPTS_LOW_CONSUMP)
                phyMode = 6;
        else if (cfg->opts & ETH_OPTS_HUB)
                phyMode = 5;
        else if ((cfg->opts & 0x01) == ETH_OPTS_100M)
        {
                //100BaseT HD + autoneg / 100BaseT HD / 100BaseT FD
                if (cfg->opts & ETH_OPTS_HD)
                {
                        phyMode = (cfg->opts & ETH_OPTS_AUTONEG) ? 4 : 2;
                }
                else
                        phyMode = 3;
        }
        else
        {
                //10BaseT HD / 10BaseT FD
                phyMode = (cfg->opts & ETH_OPTS_HD) ? 0 : 1;
        }


        //FX_EN[7]: 1/0 enable/disable FX mode
        if (cfg->opts & ETH_OPTS_FX)
                val16 |= PN7;

        //PHYADD[15..11]: 0x1C
        //MODE[3..1], see above
        //nRST[0]: 1/0 - work/reset
        val16 |= (0x1C << 11) | ((phyMode & 0x07) << 1) | 1;

        //Set register value
        *ETH_PHY_CTRL = val16;

        //Wait Ready[4] = 1 - PHY is ready
        while((*ETH_PHY_STATUS & PN4) == 0);


        //---------------------------------------------
        //4. MAC level parameters
        ETH->Delimiter = ETH_DELIMITER;

        //Hash table
        //ETH->HASH0 = 0x0000;
        //ETH->HASH1 = 0x0000;
        //ETH->HASH2 = 0x0000;
        //ETH->HASH3 = 0x8000;

        //Packet interval, bit intervals
        //ETH->IPG = 0x0060;

        //BAG, JitterWnd step, 1/(5*10^7)s units
        //ETH->PSC = 0x0050;

        //Packet period
        //ETH->BAG = 0x0200;

        //Transmit packets jitter window
        //ETH->JitterWnd = 0x0005;

        //---------------------------------------------
        //5. Configure receiver
        val16 = ETH->R_CFG;

        //MCA_EN[0], BCA_EN[1], UCA_EN[2]: 1/0 - enable/disable
        //Receive frames with (multi,broad,uni)-cast MAC addresses
        val16 |= PN0 | PN1 | PN2;

        //AC_EN[3]: 1/0 - enable/disable all MAC addresses
        val16 |= PN3;

        //EF_EN[4], CF_EN[5], LF_EN[6], SF_EN[7]: 1/0 - enable/disable
        //Receive frames: with error, control, large, short
        val16 &= ~(PN4 | PN5 | PN6 | PN7);

        //EVNT_MODE[10..8] (EVNT[1]): in buffer
        //0 – is not empty; 1 – is almost empty;
        //2 – is half full; 3 – is almost full;
        //4 – is not full;  5 – receive completed;
        //6 – buffer writed;
        //7 – frame was dropped.
        val16 |= (5 << 8);

        //bit[11] - reserved

        //MSB1st[12]: 1/0 - bit order: major/low bit first
        val16 &= ~PN12; //low bit first

        //BE[13]: 1/0 - word bytes order: (big/little)-endian
        val16 &= ~PN13; //little-endian

        //bit[14]: reserved

        //EN[15]: 1/0 - receiver ON/OFF
        //Use "TurnETHRx" function to turn ON/OFF receiver

        ETH->R_CFG = val16;

        //---------------------------------------------
        //6. Configure transmitter
        val16 = ETH->X_CFG;

        //RtryCnt[3..0]: transmission retry count
        val16 &= ~0x0F;
        val16 |= 0x0A;

        //IPG_EN[4]: 1/0 - frame interval ON/OFF
        val16 |= PN4;

        //CRC_EN[5]: 1/0 - frame CRC calculation ON/OFF
        val16 |= PN5;

        //PRE_EN[6]: 1/0 - frame preambula ON/OFF
        val16 |= PN6;

        //PAD_EN[7]: 1/0 - frame padding ON/OFF
        val16 |= PN7;

        //EVNT_MODE[10..8] (EVNT[2]): out buffer
        //0 – is empty;  1 – is almost empty;
        //2 – is half full; 3 – is almost full;
        //4 – is full;   5 – transmission completed;
        //6 – buffer read;
        //7 – transmission begin.
        val16 |= 1 << 8;

        //bit[11] - reserved

        //MSB1st[12]: 1/0 - bit order: major/low bit first
        val16 &= ~PN12; //low bit first

        //BE[13]: 1/0 - word bytes order: (big/little)-endian
        val16 &= ~PN13; //little-endian

        //bit[14]: reserved

        //EN[15]: 1/0 - transmitter ON/OFF
        //Use "TurnETHTx" function to turn ON/OFF transmitter

        ETH->X_CFG = val16;

        //---------------------------------------------
        //7. Other parameters
        //7.1 LOW halfword
        val16 = ETH->G_CFGl;

        //ColWnd[7..0]: collision window (bit intervals x4)
        //val16 &= ~0xFF;
        //val16 |= 0x80;

        //PAUSE_EN[8]: 1/0 - pause packet hands auto/no auto
        //DTRM_EN[9]:  1/0 - determ time enable/disable
        //HD_EN[10]:   1/0 - half duplex enable/disable
        val16 &= ~(PN8 | PN9 | PN10);

        //HD_EN[10]: 1/0 - enable/disable half duplex
        if (cfg->opts & ETH_OPTS_HD)
                val16 |= PN10;

        //EXT_EN[11]: 1/0 - pad short packets with "Extension" field
        val16 &= ~PN11;

        //Set BUF_MODE[13..12] (rx/tx buffer mode):
        //0 - linear, 1 - auto increment, 2 - FIFO, 3 - reserved
        val16 &= ~(3 << 12);
        //val16 |= 2 << 12;

        //RCLR_EN[14]:
        //1 - reset IFR bits with reading
        //0 - reset IFR bits with writing "1"
        val16 |= PN14;

        //bit[15] - reserved

        //Setup LOW halfword
        ETH->G_CFGl = val16;

        //7.2 HIGH halfword
        val16 = ETH->G_CFGh;

        //XRST[16]([0]): 1/0 - transmitter reset/work
        //val16 |= PN0;

        //RRST[17]([1]): 1/0 - receiver reset/work
        //val16 |= PN1;

        //DLB[18]([2]): 1/0 - loop/no loop
        val16 &= ~PN2;

        //bits[27..19]([11..3]) - reserved

        //Set DBG_XF_EN[29]([13]), DBG_RF_EN[28]([12]).
        //Used whether BUF_MODE = 2 (FIFO)
        val16 |= PN13 | PN12;

        //DBG_mode[31..30]([15..14]): 0 - free run; 2 - halt; 3 - stop
        val16 &= ~(3 << 14);

        //Setup HIGH halfword
        ETH->G_CFGh = val16;

        //--------------------------------------------
        //8. Enable IRQs

        //Interrupt masks, IMR[15..0]
        //MII_RDY[15]:  MDIO command execution completed
        //bit[13]:      reserved
        //MDIO_INT[14]: MDIO interrupt
        //CRS_LOST[12]: half duplex mode carry loss (on transmit)
        //LC[11]:       late collision detect
        //UNDF[10]:     transmitter's buffer is empty
        //XF_ERR[9]:    frame transmission error
        //XF_OK[8]:     frame transmission success
        //SF[7]:        receive frame with length < min length
        //LF[6]:        receive frame with length > max length
        //CF[5]:        receive control frame
        //CRC_ERR[4]:   frame CRC error
        //SMB_ERR[3]:   data frame error
        //OVF[2]:       receiver's buffer overflow
        //MISSED_F[1]   frame loss (input buffer space not enough)
        //RF_OK[0]:     receive completed successfully
        ETH->IMR |= PN0;

        //If frame handler defined -
        //enable interrupt
        if (cfg->FrameReceived)
        {
                EthFrameReceived = cfg->FrameReceived;

                IRQEnable(IRQ_ETH);
        }
        else
        {
                IRQDisable(IRQ_ETH);

                EthFrameReceived = 0;
        }

        return MCR_OK;
}



/* Indicates that frame received */
u32 IsFrameReceived(ETHBLOCK eth)
{
        u16 ifr = ETH->IFR;
        //ETH->IFR = ifr;
        //if RF_OK[0] = 1 - receive completed
        return (ifr & 0x01);
}



/* Indicates that cable is linked */
u32 IsETHLinked(ETHBLOCK eth)
{
        return ((*ETH_PHY_STATUS) & PN1) ? 0 : 1;
}



/* Get speed */
u32 GetETHSpeed(ETHBLOCK eth)
{
        //LED[0]: 1/0 - 10/100 MBit/s
        return ((*ETH_PHY_STATUS) & PN0) ? ETH_OPTS_10M : ETH_OPTS_100M;
}



/* Get controller mode */
u32 GetETHMode(ETHBLOCK eth)
{
        //LED[3]: 1/0 - half/full duplex
        return ((*ETH_PHY_STATUS) & PN3) ? ETH_OPTS_HD : ETH_OPTS_FD;
}



/* Turn ON/OFF ethernet controller receiver */
void TurnETHRx(ETHBLOCK eth, u8 turn)
{
        //EN[15]: 1/0 - enable/disable receiver
        if (turn)
                ETH->R_CFG |= PN15;
        else
                ETH->R_CFG &= ~PN15;
}



/* Turn ON/OFF ethernet controller transmitter */
void TurnETHTx(ETHBLOCK eth, u8 turn)
{
        //EN[15]: 1/0 - enable/disable transmitter
        if (turn)
                ETH->X_CFG |= PN15;
        else
                ETH->X_CFG &= ~PN15;
}



/* Turn ON/OFF ethernet controller */
void TurnETH(ETHBLOCK eth, u8 turn)
{
        //EN[15]: 1/0 - enable/disable
        //X_CFG - transmitter; R_CFG - receiver
        if (turn)
        {
                ETHClearBuffers(eth);

                ETH->X_CFG |= PN15;
                ETH->R_CFG |= PN15;
        }
        else
        {
                ETH->R_CFG &= ~PN15;
                ETH->X_CFG &= ~PN15;
        }
}



/* Clear input/output buffers */
void ETHClearBuffers(ETHBLOCK eth)
{
        u32 i = 0;
        u32 *ptr = (u32*)BASE_ETH_BUFFER;

        while(i++ < (ETH_BUFFER_LENGTH >> 2))
                *ptr++ = (u32)0;

        ETH->R_Head = ETH->R_Tail;
        ETH->X_Tail = ETH->X_Head;
}



/* Clear input buffer */
void ETHClearInBuffer(ETHBLOCK eth)
{
        u32 i = 0;
        u32 *ptr = (u32*)BASE_ETH_BUFFER;

        while(i++ < (ETH_DELIMITER >> 2))
                *ptr++ = (u32)0;

        ETH->R_Head = ETH->R_Tail;
}



/* Clear output buffer */
void ETHClearOutBuffer(ETHBLOCK eth)
{
        u32 i = 0;
        u32 *ptr = (u32*)(BASE_ETH_BUFFER + ETH_DELIMITER);

        while(i++ < ((ETH_BUFFER_LENGTH - ETH_DELIMITER) >> 2))
                *ptr++ = (u32)0;

        ETH->X_Tail = ETH->X_Head;
}



/* Set MAC address */
void SetETHMAC(ETHBLOCK eth, u8 *mac)
{
        if (!mac)
                return;

        ETH->MAC_H = (u16)((mac[1] << 8) | mac[0]);
        ETH->MAC_M = (u16)((mac[3] << 8) | mac[2]);
        ETH->MAC_T = (u16)((mac[5] << 8) | mac[4]);
}



/* Read data from ethernet controller */
s32 ETHRead(ETHBLOCK eth, u8 *buffer, s32 length)
{
        s32 i, count;
        u16 tail, head;
        u16 frLength;
        u16 frFlags;
        u16 padding;
        u16 *src, *dst;

        /* Buffer scheme:
           Buffer start address: BASE_ETH_BUFFER.
           Read data from head.

        1) Tail >= Head:
                      Head         Tail
                      |-> read     |
        00000000000000rrrrrrrrrrrrr0000000000000-------------------x
        |             input buffer              |   output buffer  |
        0                           ETH_DELIMITER  ETH_BUFFER_LENGTH

        2) Tail < Head:
                      Tail         Head
        |-> read end  |            |-> read beg
        rrrrrrrrrrrrrr0000000000000rrrrrrrrrrrrr-------------------x
        |             input buffer              |   output buffer  |
        0                           ETH_DELIMITER  ETH_BUFFER_LENGTH

        '-'  - output buffer cell
        'r'  - cell to read
        '0'  - free cell
        '->' - read buffer direction */

        head = ETH->R_Head; //[0..ETH_DELIMITER-1]
        tail = ETH->R_Tail; //[0..ETH_DELIMITER-1]

        if (tail >= head)
                count = tail - head;
        else
                count = ETH_DELIMITER - head + tail;

        //Status header 4 bytes (frame length, frame flags)
        if (count < 4)
                return 0;

        src = (u16*)(BASE_ETH_BUFFER + head);
        dst = (u16*)buffer;

        //First 2 bytes: frame length, bytes
        frLength = *src++;

        padding = (frLength & 3) ? (4 - (frLength & 3)) : 0;

        //Check that frame is fully at buffer
        if (count < (frLength + 4 + padding))
                return 0;

        //Check buffer limit
        if (((u32)src - BASE_ETH_BUFFER) >= ETH_DELIMITER)
                src = (u16*)BASE_ETH_BUFFER;

        //Second 2 bytes: flags
        frFlags = *src++;

        i = 0;
        while((i + 1) < length && i < frLength)
        {
                if (((u32)src - BASE_ETH_BUFFER) >= ETH_DELIMITER)
                        src = (u16*)BASE_ETH_BUFFER;

                *dst++ = *src++;

                i += 2;
        }

        //Odd frame length
        if (i > 0 && (frLength & 1))
                i--;

        //New head = [old head] + [4 bytes status] + [frame length] + [padding]
        ETH->R_Head = (head + frLength + padding + 4) % ETH_DELIMITER;

        //Decrement count of frames
        ETH->STAT -= 0x20;

        return i;
}



/* Get free space of output buffer */
s32 ETHGetOutBufferSpace(ETHBLOCK eth)
{
    s32 head = (s32)(ETH->X_Head);
    s32 tail = (s32)(ETH->X_Tail);

    //Calculate free space
    //[data length:4] + [status field:4] + [reserved:4] = 12 bytes
    if(head > tail)
        return (head - tail - (s32)12);
    else
        return (head - tail + (s32)(ETH_BUFFER_LENGTH - ETH_DELIMITER - 12));
}



/* Write data to ethernet controller */
s32 ETHWrite(ETHBLOCK eth, u8 *buffer, s32 length)
{
        u32 lengthU32, freeBegU32, head, tail;
        u32 *src, *dst;
        u16 i, freeBeg, freeEnd;

        /* Buffer scheme:
           Buffer start address: BASE_ETH_BUFFER.
           Write data from tail.

        1) Head > Tail:
                                         Tail          Head
                                         |-> write beg |
        -------------------wwwwwwwwwwwwww00000000000000wwwwwwwwwwwwwwx
        |   input buffer   |              output buffer              |
        0                  ETH_DELIMITER             ETH_BUFFER_LENGTH

        2) Head <= Tail:
                                         Head          Tail
                           |-> write end |             |-> write beg
        -------------------00000000000000wwwwwwwwwwwwww00000000000000x
        |   input buffer   |              output buffer              |
        0                  ETH_DELIMITER             ETH_BUFFER_LENGTH

        '-'  - input buffer cell
        'w'  - not-free cell
        '0'  - free cell
        '->' - write buffer direction */

        head = ETH->X_Head;
        tail = ETH->X_Tail;

        //Calc free space
        if(head > tail)
        {
                freeBeg = head - tail;
                freeEnd = 0;
        }
        else
        {
                freeBeg = ETH_BUFFER_LENGTH - tail;
                freeEnd = head - ETH_DELIMITER;
        }

        //[data length:4] + [status field:4] = 8
        if((length + 8) > (freeBeg + freeEnd))
                return 0;

        //Initial pointers
        //Pointer to frame buffer
        src = (u32*)buffer;
        //Pointer to start of free space
        dst = (u32*)(BASE_ETH_BUFFER + tail);

        //Write frame length
        *dst++ = (u32)length;
        freeBeg -= 4;

        //Get count of 32-bit words at frame
        lengthU32 = ((length + 3) >> 2);

        //Check border
        if (((u32)dst - BASE_ETH_BUFFER) > (ETH_BUFFER_LENGTH - 4))
                dst = (u32*)(BASE_ETH_BUFFER + ETH_DELIMITER);

        //If frame fits at begin free space
        if(length > freeBeg)
        {
                //Write part of frame to begin free space
                freeBegU32 = (freeBeg >> 2);
                lengthU32 -= freeBegU32;

                i = 0;
                while(i++ < freeBegU32)
                        *dst++ = *src++;

                //Set pointer to start output buffer
                dst = (u32*)(BASE_ETH_BUFFER + ETH_DELIMITER);
        }

        //Write frame
        i = 0;
        while(i++ < lengthU32)
                *dst++ = *src++;

        //Check border
        if(((u32)dst - BASE_ETH_BUFFER) > (ETH_BUFFER_LENGTH - 4))
                dst = (u32*)(BASE_ETH_BUFFER + ETH_DELIMITER);

        //Write status word (zero)
        *dst++ = (u32)0;

        //Check border
        if(((u32)dst - BASE_ETH_BUFFER) > (ETH_BUFFER_LENGTH - 4))
                dst = (u32*)(BASE_ETH_BUFFER + ETH_DELIMITER);

        //Set new tail value
		ETH->X_Tail = (u16)((u32)dst - BASE_ETH_BUFFER);

        return length;
}



/* Interrupt handler */
void ETHERNET_IRQHandler(void)
{
        u16 ifr;
        ifr = ETH->IFR;

        //RF_OK[0] = 1 - receive completed
        if(ifr & 0x01)
        {
                if (EthFrameReceived)
                        EthFrameReceived();
        }
}

/*===========================================================================
 * End of file 1986BE1x_ETH.c
 *===========================================================================*/
