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
 *  File swmodel_UART.c: UART functions.
 *
 *===========================================================================*/

#include "../../inc/UART.h"
#include <string.h>

#define UART_MINCLK_DIVIDER     1        //Minimum uart clock divider
#define UART_MAXCLK_DIVIDER     65535    //Maximum uart clock divider
#define UART_MAX_SPEED          1000000

/* */
typedef struct tag_UARTClkParams
{
    //Input params
    u32 speed;

    //Output params
    u32 brg;
    float divider;
} UARTClkParams;



//UART configurations
static UARTConfig cfgUART1;
static UARTConfig cfgUART2;
static UARTConfig cfgUART3;

/* */
static MCRESULT UARTSetSpeedInternal(UARTBLOCK uart, UARTClkParams *p);


/* */
_uart *get_uart(UARTBLOCK uart)
{
    switch(uart)
    {
        case UART_N1: return UART1;
        case UART_N2: return UART2;
        case UART_N3: return UART3;
        default: return 0;
    }
}



/* Tune UART clock parameters */
static MCRESULT TuneUARTClkParams(UARTClkParams *p)
{
        u32 hclk, brg;
        float uartCLK, divider;
        u8 isFound = 0;

        if (!p)
                return MCR_SETUPCLK_FAIL;

        hclk = GetHCLK();

        brg = 8;
        do
        {
                brg--;

                //UARTCLK = HCLK / (1 << BRG), BRG=[0..7]
                uartCLK = (float)hclk / (float)(1 << brg);

                //Divider = UARTCLK / (16 * UARTSPEED);
                divider =  uartCLK / (float)(16 * p->speed);

                if (uartCLK >= UART_MINCLK &&
                    divider >= UART_MINCLK_DIVIDER &&
                    divider <= UART_MAXCLK_DIVIDER)
                {
                        p->brg     = brg;
                        p->divider = divider;
                        isFound    = 1;
                        break;
                }
        } while(brg > 0);

        return isFound ? MCR_OK : MCR_SETUPCLK_FAIL;
}



/* */
MCRESULT InitializeUART(const UARTConfig *cfg)
{
    _uart *reg;
    UARTConfig *destCfg;
    UARTClkParams p;
    u32 valLCR_H;
    u32 valCR;

    if (!cfg)
            return MCR_INVALID_CFG;

    switch(cfg->uart)
    {
            case UART_N1: destCfg = &cfgUART1; break;
            case UART_N2: destCfg = &cfgUART2; break;
            case UART_N3: destCfg = &cfgUART3; break;

        default:
               return MCR_INVALID_PORT;
    } //switch

    //Copy configuration
    memcpy(destCfg, cfg, sizeof(UARTConfig));

    //Check speed
    if (destCfg->speed > UART_MAX_SPEED)
        destCfg->speed = UART_MAX_SPEED;

    //Tune clock parameters
    p.speed = destCfg->speed;
    if (TuneUARTClkParams(&p) != MCR_OK)
            return MCR_SETUPSPEED_FAIL;

    //Map control and Rx/Tx pins
    //if (UARTMapPortPins(destCfg) != MCR_OK)
    //        return MCR_INVALID_MAPPING;

    //Enable UARTx clock
    switch(destCfg->uart)
    {
            case UART_N1: ClkEnable(CLK_UART1); break;
            case UART_N2: ClkEnable(CLK_UART2); break;
            case UART_N3: ClkEnable(CLK_UART3); break;
    } //switch


    //Get registers
    reg = get_uart(destCfg->uart);

    //Turn OFF UART, Rx, Tx
    TurnUART(destCfg->uart, 0);

    //Setup uart speed, bit/s
    if (UARTSetSpeedInternal(destCfg->uart, &p) != MCR_OK)
            return MCR_SETUPSPEED_FAIL;

    //Load register value
    valLCR_H = (u8)reg->LCR_H;
    valCR    = (u16)reg->CR;

    //Reset SIREN[1] (Disalble infrared port)
    valCR &= ~PN1;

    //Set/reset FEN[4] - enable/disable fifo
    if (cfg->opts & UART_FIFO_DISABLE)
            valLCR_H &= ~PN4;
    else
            valLCR_H |= PN4;

    //Set/reset BRK[0] - enable/disable breakline
    if (cfg->opts & UART_BREAKLINE)
            valLCR_H |= PN0;
    else
            valLCR_H &= ~PN0;

    //Set data bits count -----------------------------------------------
    //Reset WLEN[6..5]
    valLCR_H &= ~(PN5|PN6);

    //WLEN[6..5] at [0..3] => [5..8] bits
    switch(destCfg->dataBits)
    {
            case UART_DATABITS_5: /* bits reset */ break;
            case UART_DATABITS_6: valLCR_H |= PN5; break;
            case UART_DATABITS_7: valLCR_H |= PN6; break;
            case UART_DATABITS_8:
            default:
                    valLCR_H |= (PN5|PN6); break;
    }
    //------------------------------------------------------------------


    //Set parity -------------------------------------------------------
    //Reset PEN[1], EPS[2], SPS[7]
    valLCR_H &= ~(PN1|PN2|PN7);

    switch(destCfg->opts & UART_PARITY_MASK)
    {
        //Set bits  PEN[1] EPS[2] SPS[7]
        case UART_PARITY_NONE:                              break;
        case UART_PARITY_EVEN:   valLCR_H |= (PN1|PN2);     break;
        case UART_PARITY_ODD:    valLCR_H |= PN1;           break;
        case UART_PARITY_MARK:   valLCR_H |= (PN1|PN7);     break;
        case UART_PARITY_SPACE:  valLCR_H |= (PN1|PN2|PN7); break;
    }
    //---------------------------------------------------------------


    //Set stop bits count -------------------------------------------
    //STP2[3]: 0 - 1 stop; 1 - 2 stop bits;
    switch(destCfg->opts & UART_STOPBITS_MASK)
    {
            case UART_STOPBITS_2: valLCR_H |= PN3; break;
            default:              valLCR_H &= ~PN3; break;
    }
    //---------------------------------------------------------------


    //Set flow control (RTS, CTS) ---------------------------------
    //Reset RTSEn[14], CTSEn[15]
    valCR &= ~(PN14|PN15);

    switch(destCfg->opts & UART_CTRL_MASK)
    {
        case UART_NOCTRL:         /* RTS, CTS already reset */  break;
        case UART_RTS:              valCR |= PN14;              break;
        case UART_CTS:              valCR |= PN15;              break;
        case (UART_RTS | UART_CTS): valCR |= (PN14 | PN15);     break;
    }


    //Set/reset LBE[7] - enable/disable loopback
    if (destCfg->opts & UART_LOOPBACK)
             valCR |= PN7;
    else
             valCR &= ~PN7;

    //Setup register value
    reg->LCR_H = (u8)valLCR_H;
    reg->CR    = (u16)valCR;
    //---------------------------------------------------------------

    //Set input fifo threshold
    reg->IFLS &= ~0x3F;        //Reset RXIFLSEL[5..3], TXIFLSEL[2..0]
    reg->IFLS |= 2 << 3;       //Set RXIFLSEL[5..3] at [0..4]=>[1/8, 1/4, 1/2, 3/4, 7/8]
    reg->IFLS |= 2;            //Set TXIFLSEL[2..0] at [0..4]=>[1/8, 1/4, 1/2, 3/4, 7/8]

    //RXIM[4] - enable/disable receiver interrupt
    //RTIM[6] - enable/disable receiver timeout interrupt
    if (destCfg->received)
    {
            reg->IMSC |= PN4 | PN6;

            //Enable UARTx interrupt
             switch(destCfg->uart)
            {
                    case UART_N1: IRQEnable(IRQ_UART1); break;
                    case UART_N2: IRQEnable(IRQ_UART2); break;
                    case UART_N3: IRQEnable(IRQ_UART3); break;
            }
    }
    else
    {
            reg->IMSC &= ~(PN4 | PN6);

            //Disable UARTx interrupt
            switch(destCfg->uart)
            {
                    case UART_N1: IRQDisable(IRQ_UART1); break;
                    case UART_N2: IRQDisable(IRQ_UART2); break;
                    case UART_N3: IRQDisable(IRQ_UART3); break;
            }
    }

    return MCR_OK;
}



/* */
u32 GetUARTSpeed(UARTBLOCK uart)
{
    _uart *reg;

    u8 brg;

    switch(uart)
    {
            case UART_N1:
            {
                    if (RST_CLK->UART_CLOCK & PN24)
                            brg = (u8)(RST_CLK->UART_CLOCK & 0x07);
                    else
                            return 0;
                    break;
            }

            case UART_N2:
            {
                    if (RST_CLK->UART_CLOCK & PN25)
                            brg = (u8)((RST_CLK->UART_CLOCK >> 8) & 0x07);
                    else
                            return 0;
                    break;
            }

            case UART_N3:
            {
                    if (RST_CLK->UART_CLOCK & PN26)
                            brg = (u8)((RST_CLK->UART_CLOCK >> 16) & 0x07);
                    else
                            return 0;
                    break;
            }

            default: return 0;
    }

    reg = get_uart(uart);

    return (u32)( (float)GetHCLK() / ( 16.0F * (float)(1 << brg) *
                                    ((float)(reg->IBRD & 0xFFFF) +
                                     ((float)(reg->FBRD & 0x3F) - 0.5) / 64.0F) ) );
}



/* */
static MCRESULT UARTSetSpeedInternal(UARTBLOCK uart, UARTClkParams *p)
{
        _uart *reg;

        if (!p)
                return MCR_SETUPSPEED_FAIL;

        //Disable UART clock
        switch(uart)
        {
                //Reset UART1_CLK_EN[24] - disable UART1 clock
                case UART_N1: RST_CLK->UART_CLOCK &= ~PN24; break;

                //Reset UART2_CLK_EN[25] - disable UART2 clock
                case UART_N2: RST_CLK->UART_CLOCK &= ~PN25; break;

                //Reset UART3_CLK_EN[26] - disable UART3 clock
                case UART_N3: RST_CLK->UART_CLOCK &= ~PN26; break;

                default: return MCR_INVALID_PORT;
        }


        //Get registers
        reg = get_uart(uart);

        //Set integer part of divider
        reg->IBRD = (u32)p->divider;

        //Set fraction part of divider
        reg->FBRD = (u32)((p->divider - (float)((u32)p->divider)) * 64.0F + 0.5F);

        //UARTx->IBRD and UARTx->FBRD will updated
        //after write any value to UARTx->LCR_H
        //UARTx->LCR_H used first 8bit only
        reg->LCR_H &= 0xFF;

        //Update BRG
        switch(uart)
        {
                case UART_N1:
                {
                        //Reset BRG[7..0] bits
                        RST_CLK->UART_CLOCK &= ~0x000000FF;

                        //Set BRG[7..0]: UARTCLK = CPUCLK / 2^BRG
                        RST_CLK->UART_CLOCK |= p->brg;

                        //Enable UART clock: UART1_CLK_EN[24] = 1
                        RST_CLK->UART_CLOCK |= PN24;

                        break;
                }

                case UART_N2:
                {
                        //Reset BRG[15..8] bits
                        RST_CLK->UART_CLOCK &= ~0x0000FF00;

                        //Set BRG[15..8]: UARTCLK = CPUCLK / 2^BRG
                        RST_CLK->UART_CLOCK |= (p->brg << 8);

                        //Enable UART clock: UART2_CLK_EN[25] = 1
                        RST_CLK->UART_CLOCK |= PN25;

                        break;
                }

                case UART_N3:
                {
                    //Reset BRG[18..16] bits
                    RST_CLK->UART_CLOCK &= ~0x00070000;

                    //Set BRG[18..16]: UARTCLK = CPUCLK / 2^BRG
                    RST_CLK->UART_CLOCK |= (p->brg << 16);

                    //Enable UART clock: UART3_CLK_EN[26] = 1
                    RST_CLK->UART_CLOCK |= PN26;

                    break;
                }
        }//switch

        return MCR_OK;
}



/* */
static MCRESULT UARTSetCharFramingInternal(UARTBLOCK uart, u8 dataBits, u32 opts)
{
    _uart *reg;
    u32 valLCR_H;

    //Get pointer to UART registers
    reg = get_uart(uart);

    //Load register value
    valLCR_H = (u8)reg->LCR_H;

    //Reset WLEN[6..5],  PEN[1], EPS[2], SPS[7],  STP2[3]
    valLCR_H &= ~(PN5|PN6 | PN1|PN2|PN7 | PN3);

    //Set data bits count
    //WLEN[6..5]: [0..3] => [5..8] bits
    switch(dataBits)
    {
        case UART_DATABITS_5: /* bits 5,6 already reset */ break;
        case UART_DATABITS_6: valLCR_H |= PN5; break;
        case UART_DATABITS_7: valLCR_H |= PN6; break;

        //case UART_DATABITS_8:
        default: //8 data bits
            valLCR_H |= (PN6|PN5); break;
    }

    //Set parity
    switch(opts & UART_PARITY_MASK)
    {
        //Set bits  PEN[1] EPS[2] SPS[7]
        case UART_PARITY_NONE:                              break;
        case UART_PARITY_EVEN:   valLCR_H |= (PN1|PN2);     break;
        case UART_PARITY_ODD:    valLCR_H |= PN1;           break;
        case UART_PARITY_MARK:   valLCR_H |= (PN1|PN7);     break;
        case UART_PARITY_SPACE:  valLCR_H |= (PN1|PN2|PN7); break;
    }

    //Set stop bits count
    //STP2[3]: 0/1 - 1/2 stop bits;
    switch(opts & UART_STOPBITS_MASK)
    {
        //2 stop bits
        case UART_STOPBITS_2: valLCR_H |= PN3; break;

        //Default: 1 stop bit, bit 3 already reset
    }

    //Save register value
    reg->LCR_H = (u8)valLCR_H;

    return MCR_OK;
}



/* */
MCRESULT UARTSetSpeed(UARTBLOCK uart, u32 speed)
{
        UARTClkParams p;

        p.speed = speed > UART_MAX_SPEED ? UART_MAX_SPEED : speed;

        TurnUART(uart, 0);

        if (TuneUARTClkParams(&p) != MCR_OK)
                return MCR_SETUPSPEED_FAIL;

        if (UARTSetSpeedInternal(uart, &p) != MCR_OK)
                return MCR_SETUPSPEED_FAIL;

        TurnUART(uart, 1);

        return MCR_OK;
}



/* */
MCRESULT UARTSetCharFraming(UARTBLOCK uart, u8 dataBits, u32 opts)
{
    //Turn OFF UART, Rx, Tx
    TurnUART(uart, 0);

    UARTSetCharFramingInternal(uart, dataBits, opts);

    //Turn ON UART
    TurnUART(uart, 1);

    return MCR_OK;
}



/* */
MCRESULT UARTSetSpeedCharFraming(UARTBLOCK uart, u32 speed, u8 dataBits, u32 opts)
{
        UARTClkParams p;

        p.speed = speed > UART_MAX_SPEED ? UART_MAX_SPEED : speed;

        TurnUART(uart, 0);

        if (TuneUARTClkParams(&p) != MCR_OK)
                return MCR_SETUPSPEED_FAIL;

        if (UARTSetSpeedInternal(uart, &p) != MCR_OK)
                return MCR_SETUPSPEED_FAIL;

        UARTSetCharFramingInternal(uart, dataBits, opts);

        TurnUART(uart, 1);

        return MCR_OK;
}



/* */
void TurnUART(UARTBLOCK uart, u8 turn)
{
    _uart *reg = get_uart(uart);

    if (turn)
    {
            //Set FEN[4] - enable Rx, Tx buffer
            //reg->LCR_H |= PN4;

            //Enable receive, transmit, uart:
            //set RXE[9], TXE[8], UARTEN[0]
            reg->CR |= PN9 | PN8 | PN0;
    }
    else
    {
            //Wait while uart transmit current data
            UARTWaitNotBusy(uart);

            //Disable receive, transmit, uart:
            //reset RXE[9], TXE[8], UARTEN[0]
            reg->CR &= ~(PN9 | PN8 | PN0);

            //Reset FEN[4] - disable Rx, Tx buffer
            //reg->LCR_H &= ~PN4;
    }
}



/* Turn UART loopback */
void TurnUARTLB(UARTBLOCK uart, u8 on)
{
    _uart *reg = get_uart(uart);
    u32 valCR;

    valCR    = (u16)reg->CR;

    //Set/reset LBE[7] - enable/disable loopback
    if (on)
        valCR |= PN7;
    else
        valCR &= ~PN7;

    reg->CR    = (u16)valCR;
}



/* */
void UARTWaitNotBusy(UARTBLOCK uart)
{
}



/* */
void UARTFlush(UARTBLOCK ssp)
{

}



/* */
u8 IsUARTOutFifoFull(UARTBLOCK uart)
{
	return 0;
}



/* */
s32 UARTRead(UARTBLOCK uart, void *dest, s32 count)
{
	return 0;
}



/* */
s32 UARTWrite(UARTBLOCK uart, const void *src, s32 count)
{
	return 0;
}

/*===========================================================================
 * End of file swmodel_UART.c
 *===========================================================================*/
