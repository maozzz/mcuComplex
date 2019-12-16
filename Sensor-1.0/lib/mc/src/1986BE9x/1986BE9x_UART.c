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
 *  File 1986BE9x_UART.c: UART functions.
 *
 *===========================================================================*/

#include "../../inc/UART.h"
#include <string.h>

#define UART_MINCLK_DIVIDER     1        //Minimum uart clock divider
#define UART_MAXCLK_DIVIDER     65535    //Maximum uart clock divider
#define UART_MAX_SPEED          1000000

//XON char
static const u8 chXON  = UART_CHAR_XON;

//XOFF char
static const u8 chXOFF = UART_CHAR_XOFF;


//UART1, UART2 configurations
static UARTConfig cfgUART1;
static UARTConfig cfgUART2;


#if (TARGET_MCU == MCU_1901BC1x)

//UART3 configuration
static UARTConfig cfgUART3;

static const u32 uartAddr[3]    = { BASE_UART1, BASE_UART2, BASE_UART3 };
static const u32 uartClkNums[3] = { CLK_UART1, CLK_UART2, CLK_UART3 };

#define get_uart(__x)  ((_uart*)uartAddr[(__x & 3)])
#define get_uart_clknum(__x)  uartClkNums[__x]

#else
static const u32 uartAddr[2]    = { BASE_UART1, BASE_UART2 };
static const u32 uartClkNums[2] = { CLK_UART1, CLK_UART2   };

#define get_uart(__x)  ((_uart*)uartAddr[(__x & 1)])
#define get_uart_clknum(__x)  uartClkNums[__x]
#endif


/* */
typedef struct tag_UARTClkParams
{
	//Input params
	u32 speed;

	//Output params	
	u32 brg;
    float divider;
} UARTClkParams;


static MCRESULT UARTSetSpeedInternal(UARTBLOCK uart, UARTClkParams *p);

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


#define UARTx_PORTCTRL_CONFIG    (PN_PULLUP     | PN_NOPULLDOWN | PN_NOSHMIT |\
				  PN_CTRLDRIVER | PN_NOINFILTER)

#define UARTx_PORTRXTX_CONFIG    UARTx_PORTCTRL_CONFIG


#define UARTRxTx(port,func,prx,ptx) portRxTx = port; pinFunc = func;\
                                    pinRx = prx; pinTx = ptx;

/* */
static MCRESULT UARTMapPortPins(const UARTConfig *cfg)
{
        PORTBLOCK portRxTx;
	u32 pinRx, pinTx;
	u32 pinFunc;
	u32 isRxTxDefined = 1;

        switch(cfg->uart)
        {
		//--- Select mapping 1986BE91T ---
		#if (TARGET_MCU == MCU_1986BE9x)
		case UART_N1: //1986BE91T UART1
                {
                        switch(cfg->mapCtrl)
                        {
                                case UART1_MAP_CTRL_A10A15:
                                {
                                        //Outputs: A10 - DTR; A11 - RTS
                                        InitializePORTEx(PORT_A, PN10|PN11, 0,
							PN_ALTER | PN_PWR_FAST | UARTx_PORTCTRL_CONFIG);

					//Inputs: A12 - RI;  A13 - DCD;  A14 - DSR;  A15 - CTS
					InitializePORTEx(PORT_A, PN12|PN13|PN14|PN15, 0,
							PN_ALTER | PN_INPUT | UARTx_PORTCTRL_CONFIG);
                                        break;
                                }
			}

                        switch(cfg->mapRxTx)
                        {
				case UART1_MAP_RXTX_B6B5:   { UARTRxTx(PORT_B, PN_ALTER, PN6, PN5);   break; }

				case UART1_MAP_RXTX_A6A7:   { UARTRxTx(PORT_A, PN_REMAP, PN6, PN7);   break; }

				case UART1_MAP_RXTX_D7D8:   { UARTRxTx(PORT_D, PN_REMAP, PN7, PN8);   break; }

				case UART1_MAP_RXTX_E12E13: { UARTRxTx(PORT_E, PN_REMAP, PN12, PN13); break; }

                case UART1_MAP_RXTX_D7E13:
                {
                    InitializePORTEx(PORT_D, PN7, 0,  PN_REMAP | PN_INPUT       | UARTx_PORTRXTX_CONFIG);
                    InitializePORTEx(PORT_E, PN13, 0, PN_REMAP | PN_PWR_FASTEST | UARTx_PORTRXTX_CONFIG);
                    return MCR_OK;
                }

				default: isRxTxDefined = 0; break;
			}

                        break;
                } //case UART_N1

		case UART_N2: //1986BE91T UART2
                {
                        //UART2 doesn't have control I/O at MCU 1986BE9x

                        switch(cfg->mapRxTx)
                        {
				case UART2_MAP_RXTX_D0D1: { UARTRxTx(PORT_D, PN_ALTER, PN0, PN1); break; }

				case UART2_MAP_RXTX_F0F1: { UARTRxTx(PORT_F, PN_REMAP, PN0, PN1); break; }

				default: isRxTxDefined = 0; break;
                        } //case UART_N2, switch(cfg->mapRxTx)

                        break;
                } // case UART_N2
		//--- End mapping 1986BE91T ---

		//--- Select mapping 1986BE1T ---
		#elif (TARGET_MCU == MCU_1986BE1x)

		case UART_N1: //1986BE1T UART1
                {
                        //UART1 doesn't have control I/O at MCU 1986BE1x

                        switch(cfg->mapRxTx)
                        {
				case UART1_MAP_RXTX_C4C3: { UARTRxTx(PORT_C, PN_BASIC, PN4, PN3); break; }

				default: isRxTxDefined = 0; break;
                        } //case UART_N1, switch(cfg->mapRxTx)

                        break;
		} //case UART_N1

		case UART_N2: //1986BE1T UART2
                {
                        switch(cfg->mapCtrl)
                        {
                                case UART2_MAP_CTRL_D6D11:
                                {
                                        //Outputs: D8 - DTR; D10 - RTS
                                        InitializePORTEx(PORT_D, PN8|PN10, 0,
							PN_ALTER | PN_PWR_FAST | UARTx_PORTCTRL_CONFIG);

					//Inputs: D6 - RI;  D7 - DCD;  D9 - DSR;  D11 - CTS
					InitializePORTEx(PORT_D, PN6|PN7|PN9|PN11, 0,
							PN_ALTER | PN_INPUT | UARTx_PORTCTRL_CONFIG);
                                        break;
                                }
			}

                        switch(cfg->mapRxTx)
                        {
				case UART2_MAP_RXTX_D14D13: { UARTRxTx(PORT_D, PN_BASIC, PN14, PN13); break; }

				case UART2_MAP_RXTX_C14C13: { UARTRxTx(PORT_C, PN_REMAP, PN14, PN13); break; }

				default: isRxTxDefined = 0; break;
			}

                        break;
                } // case UART_N2
		//--- End mapping 1986BE1T ---

		//--- Select mapping 1901BC1T
		#elif (TARGET_MCU == MCU_1901BC1x)
		case UART_N1: //1901BC1T UART1
		{
			switch(cfg->mapRxTx)
			{
				case UART1_MAP_RXTX_A6A7: { UARTRxTx(PORT_A, PN_ALTER, PN6, PN7); break; }

				case UART1_MAP_RXTX_C2C1: { UARTRxTx(PORT_C, PN_ALTER, PN2, PN1); break; }

				case UART1_MAP_RXTX_E5E4: { UARTRxTx(PORT_E, PN_REMAP, PN5, PN4); break; }

				default: isRxTxDefined = 0; break;
			} //case UART_N1, switch(cfg->mapRxTx)

			break;
		} // case UART_N1

		case UART_N2: //1901BC1T UART2
		{
			switch(cfg->mapRxTx)
			{
				case UART2_MAP_RXTX_B3B0:   { UARTRxTx(PORT_B, PN_ALTER, PN3, PN0);   break; }

				case UART2_MAP_RXTX_C7C6:   { UARTRxTx(PORT_C, PN_REMAP, PN7, PN6);   break; }

				case UART2_MAP_RXTX_E11E10: { UARTRxTx(PORT_E, PN_ALTER, PN11, PN10); break; }

				default: isRxTxDefined = 0; break;
			} //case UART_N2, switch(cfg->mapRxTx)

			break;
		} // case UART_N2

		case UART_N3: //1901BC1T UART3
		{
			switch(cfg->mapRxTx)
			{
				case UART3_MAP_RXTX_D8D7:   { UARTRxTx(PORT_D, PN_ALTER, PN8, PN7); break; }

				case UART3_MAP_RXTX_F0F1:   { UARTRxTx(PORT_F, PN_ALTER, PN0, PN1); break; }

				case UART3_MAP_RXTX_D14D13: { UARTRxTx(PORT_D, PN_BASIC, PN14, PN13); break; }

				default: isRxTxDefined = 0; break;
			} //case UART_N2, switch(cfg->mapRxTx)
			break;
		} // case UART_N3
		//--- End mapping 1901BC1T ---
                #endif


                default:
                        return MCR_INVALID_PORT;
        } //switch


        if (isRxTxDefined)
        {
            //Initialize port pins
            //RXD/TXD
            InitializePORTEx(portRxTx, pinRx, 0, pinFunc | PN_INPUT       | UARTx_PORTRXTX_CONFIG);
            InitializePORTEx(portRxTx, pinTx, 0, pinFunc | PN_PWR_FASTEST | UARTx_PORTRXTX_CONFIG);
            return MCR_OK;
        }
        else
            return MCR_INVALID_MAPPING;
}

#undef UARTRxTx

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

		#if (TARGET_MCU == MCU_1901BC1x)
		case UART_N3: destCfg = &cfgUART3; break;
		#endif

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
	if (UARTMapPortPins(destCfg) != MCR_OK)
                return MCR_INVALID_MAPPING;        

        //Enable UARTx clock
	switch(destCfg->uart)
        {
                case UART_N1: ClkEnable(CLK_UART1); break;

                case UART_N2: ClkEnable(CLK_UART2); break;

		#if (TARGET_MCU == MCU_1901BC1x)
		case UART_N3: ClkEnable(CLK_UART3); break;
		#endif
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
                //None        0      0      0   (1,2,7 bits already reset)
		case UART_PARITY_NONE:                              break;

                //Even bits   1      1      0
		case UART_PARITY_EVEN:   valLCR_H |= (PN1|PN2);     break;

                //Odd bits    1      0      0
		case UART_PARITY_ODD:    valLCR_H |= PN1;           break;

                //Mark   (1)  1      0      1
		case UART_PARITY_MARK:   valLCR_H |= (PN1|PN7);     break;

                //Space  (0)  1      1      1
		case UART_PARITY_SPACE:  valLCR_H |= (PN1|PN2|PN7); break;
        }
        //---------------------------------------------------------------


        //Set stop bits count -------------------------------------------
        //STP2[3]: 0 - 1 stop; 1 - 2 stop bits;
	switch(destCfg->opts & UART_STOPBITS_MASK)
        {
                case UART_STOPBITS_2: valLCR_H |= PN3; break;

		//Default 1 stop bit
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

			#if (TARGET_MCU == MCU_1901BC1x)
			case UART_N3: IRQEnable(IRQ_UART3); break;
			#endif
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

			#if (TARGET_MCU == MCU_1901BC1x)
			case UART_N3: IRQDisable(IRQ_UART3); break;
			#endif
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

				#if (TARGET_MCU == MCU_1901BC1x)
				case UART_N3:
				{
						if (RST_CLK->UART_CLOCK & PN26)
								brg = (u8)((RST_CLK->UART_CLOCK >> 16) & 0x07);
						else
								return 0;
						break;
				}
				#endif

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

				#if (TARGET_MCU == MCU_1901BC1x)
				//Reset UART3_CLK_EN[26] - disable UART3 clock
				case UART_N3: RST_CLK->UART_CLOCK &= ~PN26; break;
				#endif

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

				#if (TARGET_MCU == MCU_1901BC1x)
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
				#endif
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
        //None        0      0      0   (bits 1,2,7 already reset)
        case UART_PARITY_NONE:                              break;

        //Even bits   1      1      0
        case UART_PARITY_EVEN:   valLCR_H |= (PN1|PN2);     break;

        //Odd bits    1      0      0
        case UART_PARITY_ODD:    valLCR_H |= PN1;           break;

        //Mark   (1)  1      0      1
        case UART_PARITY_MARK:   valLCR_H |= (PN1|PN7);     break;

        //Space  (0)  1      1      1
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
void UARTWaitNotBusy(UARTBLOCK uart)
{
        //While BUSY[3] = 1
		while(get_uart(uart)->FR & PN3);
}



/* */
void TurnUARTBlock(UARTBLOCK uart, u8 turn)
{
		_uart *reg = get_uart(uart);

        if (turn)
                reg->CR |= PN0;        //Set UARTEN[0] - enable uart
        else
                reg->CR &= ~PN0;       //Reset UARTEN[0] - disable uart
}



/* */
void TurnUARTTx(UARTBLOCK uart, u8 turn)
{
		_uart *reg = get_uart(uart);

        if (turn)
                reg->CR |= PN8;        //Set TXE[8] - transmit enable
        else
                reg->CR &= ~PN8;       //Reset TXE[8] - transmit disable
}



/* */
void TurnUARTRx(UARTBLOCK uart, u8 turn)
{
		_uart *reg = get_uart(uart);

        if (turn)
                reg->CR |= PN9;        //Set RXE[9] - receive enable
        else
                reg->CR &= ~PN9;       //Reset RXE[9] - receive disable
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
u8 IsUARTTurn(UARTBLOCK uart)
{
	_uart *reg = get_uart(uart);

    //RXE[9], TXE[8], UARTEN[0] is set to 1
	return ((reg->CR & (PN9|PN8|PN0)) == (PN9|PN8|PN0)) ? 1 : 0;
}



/* */
u8 IsUARTInFifoEmpty(UARTBLOCK uart)
{
    //RXFE[4] == 1 - input fifo is empty
	return (get_uart(uart)->FR & PN4) ? 1 : 0;
}



/* */
u8 IsUARTInFifoFull(UARTBLOCK uart)
{
    //RXFF[6] == 1 - input fifo is full
	return (get_uart(uart)->FR & PN6) ? 1 : 0;
}



/* */
u8 IsUARTOutFifoEmpty(UARTBLOCK uart)
{
    //TXFE[7] == 1 - output fifo is empty
	return (get_uart(uart)->FR & PN7) ? 1 : 0;
}



/* */
u8 IsUARTOutFifoFull(UARTBLOCK uart)
{
    //TXFF[5] == 1 - output fifo is full
	return (get_uart(uart)->FR & PN5) ? 1 : 0;
}



/* Flush output data */
void UARTFlush(UARTBLOCK uart)
{
	_uart *reg = get_uart(uart);

	//TXFE[7]: 1/0 - output fifo empty/not empty
	while ((reg->FR & PN7) == 0) __NOP();
}



/* */
s32 UARTRead(UARTBLOCK uart, void *dest, s32 count)
{
        s32 i = 0;
        u8 *pdata = (u8*)dest;
        _uart *reg = get_uart(uart);

        //while (RXFE[4] == 0) - input fifo is not empty
        while(!(reg->FR & PN4) && i < count)
		pdata[i++] = (u8)reg->DR;

        //Return count of received bytes
        return i;
}



/* */
s32 UARTWrite(UARTBLOCK uart, const void *src, s32 count)
{	
	s32 i = 0, n;
	u8 *pdata = (u8*)src;
	_uart *reg = get_uart(uart);


        //V1.
        //TXFF[5] == 0 - output fifo is not full
        /*while(i < count)
                if (!(reg->FR & PN5))
		      reg->DR = pdata[i++];*/

        //V2.
        while(i < count)
        {
                //TXFE[7] == 1 - output fifo is empty
                if (reg->FR & PN7)
                {
                        n = 0;
                        while(i < count && n < UART_FIFO_TX_LENGTH)
                        {
				reg->DR = pdata[i++];
                                n++;
                        }
                }
        }

        return i;
}



/* */
void UARTSendXON(UARTBLOCK uart)
{	
	//_uart *reg = get_uart(uart);

        //TXFF[5] == 1 - output fifo is full
        //while (reg->FR & PN5);

        //Write XON char to output fifo
	//reg->DR = UART_CHAR_XON;

        //TXFE[7] == 1 - output fifo is empty
        //while((reg->FR & PN7) == 0);

	UARTWrite(uart, &chXON, 1);
}



void UARTSendXOFF(UARTBLOCK uart)
{
	//_uart *reg = get_uart(uart);

        //TXFF[5] == 1 - output fifo is full
        //while (reg->FR & PN5);

        //Write XOFF char to output fifo
	//reg->DR = UART_CHAR_XOFF;

        //TXFE[7] == 1 - output fifo is empty
        //while((reg->FR & PN7) == 0);

	UARTWrite(uart, &chXOFF, 1);
}


#ifndef __REPLACE_UART_IRQ_HANDLER
/* */
__IRQH_FATTR void UARTx_IRQHandler(_uart *reg, UARTConfig *cfg)
{
        //if RXMIS[4] or RTMIS[6]:
        //fifo filled or receiver timeout
        if (reg->MIS & (PN4 | PN6))
        {
                //Set RXIC[4], RTIC[6]  - cancel interrupt
                reg->ICR |= PN4 | PN6;

                if (cfg->received)
                        cfg->received();
        }
}



/* */
void UART1_IRQHandler(void)
{
        UARTx_IRQHandler(UART1, &cfgUART1);
}



/* */
void UART2_IRQHandler(void)
{
        UARTx_IRQHandler(UART2, &cfgUART2);
}


#if (TARGET_MCU == MCU_1901BC1x)
/* */
void UART3_IRQHandler(void)
{
	UARTx_IRQHandler(UART3, &cfgUART3);
}
#endif /* TARGET_MCU == MCU_1901BC1x */

#endif /* !__REPLACE_UART_IRQ_HANDLER */

//Indicates that value has zero value (char '0' or 0)
#define IsValOFF(v)		((v) == 0x00 || (v) == 0x30)

//#include <stdcf.h>

/* Sets option using specified option id and value */
s32 UARTSetOption(UARTBLOCK uart, u32 oid, const void *ival)
{
	u32 mask;
	u32 valreg;
	_uart *reg;
	u8 val;

	if (uart >= UART_COUNT)
		return -1;

	reg = get_uart(uart);
	val = *(u8*)ival;

	switch(oid)
	{
		case UART_OID_CLOCK:
		{
			if (IsValOFF(val))
				ClkDisable(get_uart_clknum(uart));
			else
				ClkEnable(get_uart_clknum(uart));
			return 1;
		}

		case UART_OID_TURN:
		{
			TurnUART(uart, IsValOFF(val) ? 0 : 1);
			return 1;
		}

		case UART_OID_TURNTX:
		{
			TurnUARTTx(uart, IsValOFF(val) ? 0 : 1);
			return 1;
		}

		case UART_OID_TURNRX:
		{
			TurnUARTRx(uart, IsValOFF(val) ? 0 : 1);
			return 1;
		}

		case UART_OID_SPEED:
		{
			if (UARTSetSpeed(uart, *((u32*)ival)) == MCR_OK)
				return 4;
			else
				return -1;
		}

		case UART_OID_DATABITS:
		{
			mask = 0;
			switch(*((u8*)ival))
			{
				case 5:  break; //mask = 0;
				case 6:  mask = PN5;       break;
				case 7:  mask = PN6;       break;
				default: mask = (PN5|PN6); break; //8 bits
			}

			//WLEN[6..5] values [0..3] -> [5..8] bits
			valreg = (u8)reg->LCR_H;
			reg->LCR_H = (u8)((valreg & ~(PN5|PN6)) | mask);
			return 1;
		}

		case UART_OID_PARITY:
		{
			mask = 0;
			switch(*((u8*)ival))
			{
				case UART_PARITY_NONE : break; //mask = 0;
				case UART_PARITY_EVEN : mask = (PN1|PN2);     break;
				case UART_PARITY_ODD  : mask = PN1;           break;
				case UART_PARITY_MARK : mask = (PN1|PN7);     break;
				case UART_PARITY_SPACE: mask = (PN1|PN2|PN7); break;
			}

			//Reset PEN[1], EPS[2], SPS[7] (no parity), add mask
			valreg = (u8)reg->LCR_H;
			reg->LCR_H = (u8)((valreg & ~(PN1|PN2|PN7)) | mask);
			return 1;
		}

		case UART_OID_STOPBITS:
		{
			//STP2[3]: 0/1 - 1/2 stop bits;
			valreg = (u8)reg->LCR_H;

			if (*((u8*)ival) == 2)
				valreg |= PN3;  //2 stop bits
			else
				valreg &= ~PN3; //1 stop bit

			reg->LCR_H = (u8)valreg;
			return 1;
		}

		case UART_OID_CTRL:
		{
			//Reset CTSEn[15], RTSEn[14]
			valreg = (u16)reg->CR;
			valreg &= ~(PN15|PN14);

			if (*((u8*)ival) & UART_RTS)
				valreg |= PN14;

			if (*((u8*)ival) & UART_CTS)
				valreg |= PN15;

			reg->CR = (u16)valreg;
			return 1;
		}

		case UART_OID_LOOPBACK:
		{
			valreg = (u16)reg->CR;
			if (IsValOFF(val))
				valreg &= ~PN7;
			else
				valreg |= PN7;
			reg->CR = (u16)valreg;
			return 1;
		}

		default: return -1;
	}
}



/* Gets option using specified option identifier */
s32 UARTGetOption(UARTBLOCK uart, u32 oid, void *oval)
{
	u32 valreg;
	_uart *reg;

	if (uart >= UART_COUNT)
		return -1;

	reg = get_uart(uart);

	switch(oid)
	{
		case UART_OID_CLOCK:
		{
			*((u8*)oval) = (u8)IsClkEnable(get_uart_clknum(uart));
			return 1;
		}

		case UART_OID_TURN:
		{
			//UARTEN[0]
			*((u8*)oval) = (reg->CR & PN0) ? 1 : 0;
			return 1;
		}

		case UART_OID_TURNTX:
		{
			//TXE[8]
			*((u8*)oval) = (reg->CR & PN8) ? 1 : 0;
			return 1;
		}

		case UART_OID_TURNRX:
		{
			//RXE[9]
			*((u8*)oval) = (reg->CR & PN9) ? 1 : 0;
			return 1;
		}

		case UART_OID_SPEED:
		{
			*((u32*)oval) = GetUARTSpeed(uart);
			return 4;
		}

		case UART_OID_DATABITS:
		{
			valreg = (u8)reg->LCR_H;
			*((u8*)oval) = 5 + ((valreg >> 5) & 0x03);
			return 1;
		}

		case UART_OID_PARITY:
		{
			valreg = (u8)reg->LCR_H;

			//PEN[1], EPS[2], SPS[7]
			if (valreg & PN1)
			{
				if (valreg & PN7)
				{
					*((u8*)oval) = (valreg & PN2) ?
									UART_PARITY_SPACE : UART_PARITY_MARK;
				}
				else
				{
					*((u8*)oval) = (valreg & PN2) ?
									UART_PARITY_EVEN : UART_PARITY_ODD;
				}
			}
			else
				*((u8*)oval) = UART_PARITY_NONE;

			return 1;
		}

		case UART_OID_STOPBITS:
		{
			valreg = (u8)reg->LCR_H;
			*((u8*)oval) = (valreg & PN3) ? 2 : 1;
			return 1;
		}

		case UART_OID_CTRL:
		{
			valreg = (u16)reg->CR;
			*((u8*)oval) = (valreg >> 9) & 0x60;
			return 1;
		}

		case UART_OID_LOOPBACK:
		{
			valreg = (u16)reg->CR;
			*((u8*)oval) = (valreg & PN7) ? 1 : 0;
			return 1;
		}

		default: return -1;
	}
}

/*===========================================================================
 * End of file 1986BE9x_UART.c
 *===========================================================================*/

