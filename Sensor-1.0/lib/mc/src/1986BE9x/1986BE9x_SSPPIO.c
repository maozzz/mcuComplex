/*===========================================================================
 *
 *  MCU library
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2017
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File 1986BE9x_SSPPIO.c. SSPPIO functions.
 *                          Software implementation of SSP controller.
 *                          Use for very low speed only (up to 200bps).
 *
 *===========================================================================*/

#include "../../inc/SSPPIO.h"
#include <string.h>

//Control register CR: SOD[5] MS[4] SSE[3] LBM[2] SPH[1] SPO[0]
#define SSPPIO_FLAG_SPO			PN0 //Clock polarity
#define SSPPIO_FLAG_SPH			PN1 //Clock phase
#define SSPPIO_FLAG_LBM			PN2 //1 - loopback
#define SSPPIO_FLAG_SSE			PN3 //1/0 - enable/disable SSPPIO request
#define SSPPIO_FLAG_MS			PN4 //1 - slave
#define SSPPIO_FLAG_SOD			PN5 //1 - disable Tx
#define SSPPIO_FLAG_WORK		PN6 //1 - work enable

//Status register SR: BSY[4] RFF[3] RNE[2] TNF[1] TFE[0]
#define SSPPIO_FLAG_TFE			PN0
#define SSPPIO_FLAG_TNF			PN1
#define SSPPIO_FLAG_RNE			PN2
#define SSPPIO_FLAG_RFF			PN3
#define SSPPIO_FLAG_BSY			PN4

/* */
typedef struct tag_SSPPIOCtrl
{
	//Clock counter
	volatile u32 clkCounter;

	//Count of timer ticks for half period of clock
	volatile u32 clkHalfT;

	//Clock level: 1/0
	volatile u32 clkLevel;

	//Tx frame bit counter
	volatile u32 txBitCounter;

	//Rx frame bit counter
	volatile u32 rxBitCounter;

	//Rx/Tx frame size minus 1, bits [3..15]
	volatile u16 DSS;

	//Control register
	volatile u16 CR;

	//Status register
	volatile u16 SR;

	//Tx data
	volatile u16 txDR;

	//Rx data
	volatile u16 rxDR;	

	//Pin mask: Tx
	u16 portTxMask;

	//Pin mask: Rx
	u16 portRxMask;

	//Pin mask: FSS
	u16 portFssMask;

	//Pin mask: CLK
	u16 portClkMask;	

	//Timer speed, Hz
	u32 timerSpeed;

	//Incoming data handler
	void (*received)(void);

	//Pointer to data port (Rx,Tx)
	_port *portData;

	//Pointer to control port (FSS,CLK)
	_port *portCtrl;

} SSPPIOCtrl;


//SSPPIO1,SSPPIO2 configurations
static SSPPIOCtrl cfgSSPPIO1;
static SSPPIOCtrl cfgSSPPIO2;

#if (TARGET_MCU == MCU_1986BE9x)
static SSPPIOCtrl *cfgSSPPIOs[2] = { &cfgSSPPIO1, &cfgSSPPIO2 };

#elif (TARGET_MCU == MCU_1986BE1x)
//SSPPIO3 configuration
static SSPPIOCtrl cfgSSPPIO3;
static SSPPIOCtrl *cfgSSPPIOs[3] = { &cfgSSPPIO1, &cfgSSPPIO2, &cfgSSPPIO3 };

#elif (TARGET_MCU == MCU_1901BC1x)
//SSPPIO3,SSPPIO4 configurations
static SSPPIOCtrl cfgSSPPIO3;
static SSPPIOCtrl cfgSSPPIO4;
static SSPPIOCtrl *cfgSSPPIOs[4] = { &cfgSSPPIO1, &cfgSSPPIO2, &cfgSSPPIO3, &cfgSSPPIO4 };

#endif

#define ssppio_get_ctrl_ptr(x) (cfgSSPPIOs[(x) & 3])


#define ssppio_set_fss_1(p)		(p)->portCtrl->RXTX |=  ((p)->portFssMask)
#define ssppio_set_fss_0(p)		(p)->portCtrl->RXTX &= ~((p)->portFssMask)

#define ssppio_set_clk_1(p)		(p)->portCtrl->RXTX |=  ((p)->portClkMask)
#define ssppio_set_clk_0(p)		(p)->portCtrl->RXTX &= ~((p)->portClkMask)

#define ssppio_set_tx_1(p)		(p)->portData->RXTX |=  ((p)->portTxMask)
#define ssppio_set_tx_0(p)		(p)->portData->RXTX &= ~((p)->portTxMask)

#define ssppio_get_fss(p)		(((p)->portCtrl->RXTX) & ((p)->portFssMask))
#define ssppio_get_clk(p)		(((p)->portCtrl->RXTX) & ((p)->portClkMask))
#define ssppio_get_rx(p)		(((p)->portData->RXTX) & ((p)->portRxMask))


#define SSPx_PORTCTRL_CONFIG    (PN_NOPULLUP   | PN_NOPULLDOWN | PN_NOSHMIT |\
								 PN_CTRLDRIVER | PN_NOINFILTER)

#define SSPx_PORTRXTX_CONFIG    (PN_NOPULLUP   | PN_NOPULLDOWN | PN_NOSHMIT |\
								 PN_CTRLDRIVER | PN_NOINFILTER)

#define SSPRxTx(port,prx,ptx)   portRxTx = port; pinRx = prx; pinTx = ptx;

#define SSPCtrl(port,pfss,pclk) portCtrl = port; pinFSS = pfss; pinCLK = pclk;

/* */
static MCRESULT SSPPIOMapPortPins(const SSPConfig *cfg)
{
		SSPPIOCtrl *p;
		PORTBLOCK portRxTx, portCtrl;
		u32 pinRx, pinTx, pinFSS, pinCLK;
		u32 isPinsDefined = 1;

		switch(cfg->ssp)
		{
		//--- Select mapping 1986BE91T ---
		#if (TARGET_MCU == MCU_1986BE9x)
		case SSP_N1: //1986BE91T SSP1
				{
						switch(cfg->mapRxTx)
						{
							case SSP1_MAP_RXTX_B14B15: { SSPRxTx(PORT_B, PN14, PN15); break; }

							case SSP1_MAP_RXTX_F3F0:   { SSPRxTx(PORT_F, PN3, PN0);   break; }

							case SSP1_MAP_RXTX_D11D12: { SSPRxTx(PORT_D, PN11, PN12); break; }

							default: isPinsDefined = 0; break;
						} //case SSP_N1, switch(cfg->mapRxTx)


						switch(cfg->mapCtrl)
						{
							case SSP1_MAP_CTRL_B12B13: { SSPCtrl(PORT_B, PN12, PN13);  break; }

							case SSP1_MAP_CTRL_F2F1:   { SSPCtrl(PORT_F, PN2, PN1);    break; }

							case SSP1_MAP_CTRL_D9D10:  { SSPCtrl(PORT_D, PN9, PN10);   break; }

							default: isPinsDefined = 0; break;
						} //case SSP_N1, switch(cfg->mapCtrl)

						break;
				} //case SSP_N1

		case SSP_N2: //1986BE91T SSP2
				{
						switch(cfg->mapRxTx)
						{
							case SSP2_MAP_RXTX_D2D6:   { SSPRxTx(PORT_D, PN2, PN6);   break; }

							case SSP2_MAP_RXTX_B14B15: { SSPRxTx(PORT_B, PN14, PN15); break; }

							case SSP2_MAP_RXTX_C2C3:   { SSPRxTx(PORT_C, PN2, PN3);   break; }

							case SSP2_MAP_RXTX_F14F15: { SSPRxTx(PORT_F, PN14, PN15); break; }

							default: isPinsDefined = 0; break;
						} //case SSP_N2, switch(cfg->mapRxTx)


						switch(cfg->mapCtrl)
						{
							case SSP2_MAP_CTRL_D3D5:   { SSPCtrl(PORT_D, PN3, PN5);   break; }

							case SSP2_MAP_CTRL_B12B13: { SSPCtrl(PORT_B, PN12, PN13); break; }

							case SSP2_MAP_CTRL_C0C1:   { SSPCtrl(PORT_C, PN0, PN1);   break; }

							case SSP2_MAP_CTRL_F12F13: { SSPCtrl(PORT_F, PN12, PN13); break; }

							default: isPinsDefined = 0; break;
						} //case SSP_N2, switch(cfg->mapCtrl)

						break;
				} //case SSP_N2
		//--- End mapping 1986BE91T ---

		//--- Select mapping 1986BE1T ---
		#elif (TARGET_MCU == MCU_1986BE1x)
		case SSP_N1: //1986BE1T SSP1
				{
						switch(cfg->mapRxTx)
						{
							case SSP1_MAP_RXTX_C6C5: { SSPRxTx(PORT_C, PN6, PN5); break; }

							case SSP1_MAP_RXTX_D3D2: { SSPRxTx(PORT_D, PN3, PN2); break; }

							case SSP1_MAP_RXTX_C5C6: { SSPRxTx(PORT_C, PN5, PN6); break; }

							default: isPinsDefined = 0; break;
						} //case SSP_N1, switch(cfg->mapRxTx)

						switch(cfg->mapCtrl)
						{
							case SSP1_MAP_CTRL_C8C7: { SSPCtrl(PORT_C, PN8, PN7); break; }

							case SSP1_MAP_CTRL_D5D4: { SSPCtrl(PORT_D, PN5, PN4); break; }

							default: isPinsDefined = 0; break;
						} //case SSP_N1, switch(cfg->mapCtrl)

						break;
				} //case SSP_N1

				case SSP_N2: //1986BE1T SSP2
				{
						switch(cfg->mapRxTx)
						{
							case SSP2_MAP_RXTX_C10C9: { SSPRxTx(PORT_C, PN10, PN9); break; }

							case SSP2_MAP_RXTX_D8D7:  { SSPRxTx(PORT_D, PN8, PN7);  break; }

							default: isPinsDefined = 0; break;
						} //case SSP_N2, switch(cfg->mapRxTx)

						switch(cfg->mapCtrl)
						{
							case SSP2_MAP_CTRL_C12C11: { SSPCtrl(PORT_C, PN12, PN11); break; }

							case SSP2_MAP_CTRL_D10D9:  { SSPCtrl(PORT_D, PN10, PN9);  break; }

							default: isPinsDefined = 0; break;
						} //case SSP_N2, switch(cfg->mapCtrl)

						break;
				} //case SSP_N2
		//--- End mapping 1986BE1T ---

		//Select mapping 1901BC1T
		#elif (TARGET_MCU == MCU_1901BC1x)
		case SSP_N1: //1901BC1T SSP1REMAP
		{
			switch(cfg->mapRxTx)
			{
				case SSP1_MAP_RXTX_A12A13: { SSPRxTx(PORT_A, PN12, PN13);	break; }

				case SSP1_MAP_RXTX_D4D2:   { SSPRxTx(PORT_D, PN4, PN2);   break; }

				case SSP1_MAP_RXTX_E12E13: { SSPRxTx(PORT_E, PN12, PN13);	break; }

				default: isPinsDefined = 0;	break;
			}

			switch(cfg->mapCtrl)
			{
				case SSP1_MAP_CTRL_A15A14: { SSPCtrl(PORT_A, PN15, PN14); break; }

				case SSP1_MAP_CTRL_D3D5:   { SSPCtrl(PORT_D, PN3, PN5);   break; }

				case SSP1_MAP_CTRL_E15E14: { SSPCtrl(PORT_E, PN15, PN14); break; }

				default: isPinsDefined = 0;	break;
			}

			break;
		} //case SSP_N1

		case SSP_N2: //1901BC1T SSP2
		{
			switch(cfg->mapRxTx)
			{
				case SSP2_MAP_RXTX_A11A10: { SSPRxTx(PORT_A, PN11, PN10); break; }

				case SSP2_MAP_RXTX_B15B14: { SSPRxTx(PORT_B, PN15, PN14); break; }

				case SSP2_MAP_RXTX_D13D15: { SSPRxTx(PORT_D, PN13, PN15); break; }

				case SSP2_MAP_RXTX_E6E4:   { SSPRxTx(PORT_E, PN6, PN4);   break; }

				default: isPinsDefined = 0; break;
			}

			switch(cfg->mapCtrl)
			{
				case SSP2_MAP_CTRL_A9A8:   { SSPCtrl(PORT_A, PN9, PN8);   break; }

				case SSP2_MAP_CTRL_B13B12: { SSPCtrl(PORT_B, PN13, PN12); break; }

				case SSP2_MAP_CTRL_D12D14: { SSPCtrl(PORT_D, PN12, PN14); break; }

				case SSP2_MAP_CTRL_E5E7:   { SSPCtrl(PORT_E, PN5, PN7);   break; }

				default: isPinsDefined = 0; break;
			}

			break;
		} //case SSP_N2

		case SSP_N3: //1901BC1T SSP3
		{
			switch(cfg->mapRxTx)
			{
				case SSP3_MAP_RXTX_C10C8:  { SSPRxTx(PORT_C, PN10, PN8);  break; }

				case SSP3_MAP_RXTX_F14F12: { SSPRxTx(PORT_F, PN14, PN12); break; }

				default: isPinsDefined = 0; break;
			}

			switch(cfg->mapCtrl)
			{
				case SSP3_MAP_CTRL_C9C11:  { SSPCtrl(PORT_C, PN9, PN11);  break; }

				case SSP3_MAP_CTRL_F13F15: { SSPCtrl(PORT_F, PN13, PN15); break; }

				default: isPinsDefined = 0;	break;
			}

			break;
		} //case SSP_N3

		case SSP_N4: //1901BC1T SSP4
		{
			switch(cfg->mapRxTx)
			{
				case SSP4_MAP_RXTX_C3C4:   { SSPRxTx(PORT_C, PN3, PN4);   break; }

				case SSP4_MAP_RXTX_C15C14: { SSPRxTx(PORT_C, PN15, PN14); break; }

				case SSP4_MAP_RXTX_E3E2:   { SSPRxTx(PORT_E, PN3, PN2);   break; }

				case SSP4_MAP_RXTX_F2F3:   { SSPRxTx(PORT_F, PN2, PN3);   break; }

				default: isPinsDefined = 0; break;
			}

			switch(cfg->mapCtrl)
			{
				case SSP4_MAP_CTRL_C0C5:   { SSPCtrl(PORT_C, PN0, PN5);   break; }

				case SSP4_MAP_CTRL_C12C13: { SSPCtrl(PORT_C, PN12, PN13); break; }

				case SSP4_MAP_CTRL_E0E1:   { SSPCtrl(PORT_E, PN0, PN1);   break; }

				case SSP4_MAP_CTRL_F5F4:   { SSPCtrl(PORT_F, PN5, PN4);   break; }

				default: isPinsDefined = 0; break;
			}

			break;
		} //case SSP_N4
		//--- End mapping 1901BC1T ---
				#endif

				default:
						return MCR_INVALID_PORT;
		}


		//Configure ports
		if (isPinsDefined)
		{
				u32 pwr;

				pwr = (cfg->mode == SSP_MODE_MASTER) ? PN_PWR_FASTEST : PN_INPUT;

						//Initialize port pins
				//FSS/CLK
				InitializePORTEx(portCtrl, pinFSS, 0, PN_PORTIO | pwr | SSPx_PORTCTRL_CONFIG);
				InitializePORTEx(portCtrl, pinCLK, 0, PN_PORTIO | pwr | SSPx_PORTCTRL_CONFIG);

				//RXD/TXD
				InitializePORTEx(portRxTx, pinRx,  0, PN_PORTIO | PN_INPUT       | SSPx_PORTRXTX_CONFIG);
				InitializePORTEx(portRxTx, pinTx,  0, PN_PORTIO | PN_PWR_FASTEST | SSPx_PORTRXTX_CONFIG);

				p = ssppio_get_ctrl_ptr(cfg->ssp);

				if (p)
				{
					switch(portCtrl)
					{
						case PORT_A: p->portCtrl = PORTA; break;
						case PORT_B: p->portCtrl = PORTB; break;
						case PORT_C: p->portCtrl = PORTC; break;
						case PORT_D: p->portCtrl = PORTD; break;
						case PORT_E: p->portCtrl = PORTE; break;
						case PORT_F: p->portCtrl = PORTF; break;
						default: return MCR_INVALID_MAPPING;
					}

					switch(portRxTx)
					{
						case PORT_A: p->portData = PORTA; break;
						case PORT_B: p->portData = PORTB; break;
						case PORT_C: p->portData = PORTC; break;
						case PORT_D: p->portData = PORTD; break;
						case PORT_E: p->portData = PORTE; break;
						case PORT_F: p->portData = PORTF; break;
						default: return MCR_INVALID_MAPPING;
					}

					p->portFssMask = pinFSS;
					p->portClkMask = pinCLK;
					p->portRxMask  = pinRx;
					p->portTxMask  = pinTx;
				}

				return MCR_OK;
		}
		else
				return MCR_INVALID_MAPPING;
}

#undef SSPCtrl
#undef SSPRxTx

/* */
MCRESULT InitializeSSPPIO(const SSPConfig *cfg, u32 timerSpeed)
{
	SSPPIOCtrl *reg;
	u16 valCR;

	if (!cfg)
		return MCR_INVALID_CFG;
	else if (cfg->ssp >= SSP_COUNT)
		return MCR_INVALID_PORT;

	reg = ssppio_get_ctrl_ptr(cfg->ssp);

	memset(reg, 0, sizeof(SSPPIOCtrl));

	//Map port
	if (SSPPIOMapPortPins(cfg) != MCR_OK)
		return MCR_INVALID_PORT;

	//Turn OFF selected SSP
	TurnSSPPIO(cfg->ssp, 0);	

	valCR = 0;

	//LBM: 1/0 - enable/disable loopback
	if (cfg->opts & SSP_OPTS_LOOPBACK)
		valCR |= SSPPIO_FLAG_LBM;
	else
		valCR &= ~SSPPIO_FLAG_LBM;

	//Set MS: 0/1 - master/slave
	if (cfg->mode == SSP_MODE_MASTER)
		valCR &= ~SSPPIO_FLAG_MS;
	else
		valCR |= SSPPIO_FLAG_MS;

	switch (cfg->proto)
	{
		//SPI protocol options
		case SSP_PROTO_SPI:
		{
			//Set/reset SPO - clock polarity
			if (cfg->opts & SSP_OPTS_SPI_POLHIGH)
				valCR |= SSPPIO_FLAG_SPO;
			else
				valCR &= ~SSPPIO_FLAG_SPO;

			//Set/reset SPH - register data by first or second edge
			if (cfg->opts & SSP_OPTS_SPI_PHASE2)
				valCR |= SSPPIO_FLAG_SPH;
			else
				valCR &= ~SSPPIO_FLAG_SPH;

			break;
		}
	}

	//Set DSS: frame length, bits [4..16]
	if (cfg->dataBits < 4)
		reg->DSS = 4 - 1;
	else if (cfg->dataBits > 16)
		reg->DSS = 16 - 1;
	else
		reg->DSS = cfg->dataBits - 1;

	//Set control register value
	reg->CR = valCR;

	//Tx FIFO empty and not full: TFE and TNF is set to 1
	reg->SR = (SSPPIO_FLAG_TFE | SSPPIO_FLAG_TNF);

	//Save pointer to user defined function
	reg->received   = cfg->received;

	//Save timer speed
	reg->timerSpeed = timerSpeed;

	//Setup SSPPIO speed
	SSPPIOSetSpeed(cfg->ssp, cfg->speed);

	//Set clock level
	reg->clkLevel = (reg->CR & SSPPIO_FLAG_SPO) ? 1 : 0;

	return MCR_OK;
}



/* Turn ON/OFF selected SSPPIO */
void TurnSSPPIO(SSPBLOCK ssp, u8 turn)
{
	SSPPIOCtrl *reg = ssppio_get_ctrl_ptr(ssp);
	if (turn)
		reg->CR |= SSPPIO_FLAG_SSE;
	else
		reg->CR &= ~SSPPIO_FLAG_SSE;
}



/* */
MCRESULT SSPPIOSetSpeed(SSPBLOCK ssp, u32 speed)
{
	SSPPIOCtrl *reg = ssppio_get_ctrl_ptr(ssp);

	reg->clkHalfT = (u32)(reg->timerSpeed / (speed << 1));

	if (reg->clkHalfT > 0)
		return MCR_OK;
	else
		return MCR_SETUPSPEED_FAIL;
}



/* Indicating that output fifo is full */
u8 IsSSPPIOOutFifoFull(SSPBLOCK ssp)
{	
	return ((ssppio_get_ctrl_ptr(ssp)->SR & SSPPIO_FLAG_TNF) ? 0 : 1);
}



/* Write data to SSPPIO */
s32 SSPPIOWrite(SSPBLOCK ssp, const void *src, s32 count)
{
	s32 i = 0;
	SSPPIOCtrl *reg = ssppio_get_ctrl_ptr(ssp);
	const u8 *pdata8 = (u8*)src;

	while(i < count)
	{
		//TNF = 1: Tx FIFO not full
		if (reg->SR & SSPPIO_FLAG_TNF)
		{
			reg->txDR = *pdata8++;
			reg->SR &= ~(SSPPIO_FLAG_TNF | SSPPIO_FLAG_TFE);
			i++;
		}
	}

	return i;
}



/* Read data from SSPPIO */
s32 SSPPIORead(SSPBLOCK ssp, void *dest, s32 count)
{
	s32 i = 0;
	SSPPIOCtrl *reg = ssppio_get_ctrl_ptr(ssp);
	u8 *pdata8 = (u8*)dest;

	//RNE = 1 - rx fifo not empty
	while((reg->SR & SSPPIO_FLAG_RNE) && i < count)
	{
		*pdata8++ = reg->rxDR;
		reg->SR &= ~(SSPPIO_FLAG_RNE | SSPPIO_FLAG_RFF);
		i++;
	}

	return i;
}



/* */
__STATIC_INLINE void SSPPIOStandbyActions_slave(SSPPIOCtrl *reg)
{
	reg->txBitCounter = 0;
	reg->rxBitCounter = 0;
	reg->clkLevel = (reg->CR & SSPPIO_FLAG_SPO)	? 1 : 0;

	//Reset BUSY flag
	reg->SR &= ~SSPPIO_FLAG_BSY;

	//Disable work
	if ((reg->CR & SSPPIO_FLAG_SSE) == 0)
		reg->CR &= ~SSPPIO_FLAG_WORK;
}



/* */
__STATIC_INLINE void SSPPIOStandbyActions_master(SSPPIOCtrl *reg)
{
	//Tx FIFO is empty: set CLK and FSS to standby
	if (reg->CR & SSPPIO_FLAG_SPO)
	{
		//Clock polarity: high
		reg->clkLevel = 1;
		ssppio_set_clk_1(reg);
	}
	else
	{
		//Clock polarity: low
		reg->clkLevel = 0;
		ssppio_set_clk_0(reg);
	}

	reg->txBitCounter = 0;
	reg->rxBitCounter = 0;

	//Reset BUSY flag
	reg->SR &= ~SSPPIO_FLAG_BSY;

	//Disable work
	if ((reg->CR & SSPPIO_FLAG_SSE) == 0)
		reg->CR &= ~SSPPIO_FLAG_WORK;

	//Set FSS
	ssppio_set_fss_1(reg);
}



/* SSP line signal handler. Call this function with frequency "timerSpeed" Hz */
void SSPPIOLineHandler(SSPBLOCK ssp)
{
	SSPPIOCtrl *reg = ssppio_get_ctrl_ptr(ssp);
	u32 pin_clk;	
	u32 flagRW;

	//SSPPIO disabled: return
	if ((reg->CR & SSPPIO_FLAG_WORK) == 0)
	{
		if ((reg->CR & SSPPIO_FLAG_SSE) == 0)
			return;
		else
		{
			reg->SR &= ~SSPPIO_FLAG_BSY;
			reg->CR |= SSPPIO_FLAG_WORK;
		}
	}

	if (reg->CR & SSPPIO_FLAG_MS)
	{
		//Slave handler
		//FSS = 1: return
		if (ssppio_get_fss(reg) != 0)
		{
			SSPPIOStandbyActions_slave(reg);
			return;
		}

		//Set BUSY flag
		reg->SR |= SSPPIO_FLAG_BSY;

		pin_clk = ssppio_get_clk(reg);

		flagRW = 0;

		if (reg->CR & SSPPIO_FLAG_SPO)
		{
			//Clock polarity high: write bit by ON to OFF
			if (reg->clkLevel == 1 && pin_clk == 0)
			{
				flagRW = 1;
				reg->clkLevel = 0;
			}
			else if (reg->clkLevel == 0 && pin_clk != 0)
			{
				flagRW = 2;
				reg->clkLevel = 1;
			}
		}
		else
		{
			//Clock polarity low: write bit by OFF to ON
			if (reg->clkLevel == 1 && pin_clk == 0)
			{
				flagRW = 2;
				reg->clkLevel = 0;
			}
			else if (reg->clkLevel == 0 && pin_clk != 0)
			{
				flagRW = 1;
				reg->clkLevel = 1;
			}
		}

		if (flagRW == 1)
		{
			//Slave: write bit to line
			if ((reg->CR & SSPPIO_FLAG_SOD) == 0 &&
				(reg->SR & SSPPIO_FLAG_TFE) == 0)
			{
				//Put bit to line
				if (reg->txDR & (1 << (reg->DSS)))
				{
					ssppio_set_tx_1(reg);
				}
				else
				{
					ssppio_set_tx_0(reg);
				}

				reg->txDR <<= 1;
				reg->txBitCounter++;

				//Send frame completed
				if (reg->txBitCounter == (reg->DSS + 1U))
				{
					reg->txBitCounter = 0;
					reg->SR |= (SSPPIO_FLAG_TFE | SSPPIO_FLAG_TNF);

					//Disable work
					if ((reg->CR & SSPPIO_FLAG_SSE) == 0)
					{
						SSPPIOStandbyActions_slave(reg);
					}
				}
			}
		}
		else if (flagRW == 2)
		{
			//Read bit from line
			if ((reg->SR & SSPPIO_FLAG_RFF) == 0)
			{
				//Slave: read bit from line (MSB first)
				if (ssppio_get_rx(reg))
				{
					reg->rxDR = (reg->rxDR << 1) | 1;
				}
				else
				{
					reg->rxDR <<= 1;
				}

				reg->rxBitCounter++;

				//Reseive frame completed
				if (reg->rxBitCounter == (reg->DSS + 1U))
				{
					reg->rxBitCounter = 0;
					reg->rxDR &= ((1U << (reg->DSS + 1)) - 1U);
					reg->SR |= (SSPPIO_FLAG_RFF | SSPPIO_FLAG_RNE);

					if (reg->received)
						reg->received();
				}
			}
		}

		//End of slave handler
	}
	else
	{		
		//Master handler
		if ((reg->SR & SSPPIO_FLAG_TFE) == 0)
		{
			//Tx FIFO is not empty: tx data
			//Set BUSY flag
			reg->SR |= SSPPIO_FLAG_BSY;

			//FSS = 0
			ssppio_set_fss_0(reg);

			reg->clkCounter++;
			if (reg->clkCounter == reg->clkHalfT)
			{
				reg->clkCounter = 0;

				if (reg->CR & SSPPIO_FLAG_SPO)
				{
					flagRW = (reg->clkLevel == 1) ? 1 : 0;
				}
				else
				{
					flagRW = (reg->clkLevel == 0) ? 1 : 0;
				}

				reg->clkLevel ^= 1;

				//Change clock level
				if (reg->clkLevel)
				{
					ssppio_set_clk_1(reg);
				}
				else
				{
					ssppio_set_clk_0(reg);
				}

				if (flagRW)
				{
					//Master: write bit to line (MSB first)
					if (reg->txDR & (1 << (reg->DSS)))
					{
						ssppio_set_tx_1(reg);
					}
					else
					{
						ssppio_set_tx_0(reg);
					}

					reg->txDR <<= 1;
					reg->txBitCounter++;
				}
				else
				{
					//Master: read bit from line
					//Check that input FIFO is not full
					if ((reg->SR & SSPPIO_FLAG_RFF) == 0)
					{
						//Read bit from line: MSB first
						if (ssppio_get_rx(reg))
						{
							reg->rxDR = (reg->rxDR << 1) | 1;
						}
						else
						{
							reg->rxDR <<= 1;
						}

						reg->rxBitCounter++;

						//Reseive frame completed
						if (reg->rxBitCounter == (reg->DSS + 1U))
						{
							reg->rxBitCounter = 0;
							reg->rxDR &= ((1U << (reg->DSS + 1)) - 1U);
							reg->SR |= (SSPPIO_FLAG_RFF | SSPPIO_FLAG_RNE);

							if (reg->received)
								reg->received();
						}
					}

					//Frame send completed
					if (reg->txBitCounter == (reg->DSS + 1U))
					{
						reg->txBitCounter = 0;
						reg->SR |= (SSPPIO_FLAG_TFE | SSPPIO_FLAG_TNF);

						//Disable work
						if ((reg->CR & SSPPIO_FLAG_SSE) == 0)
						{
							SSPPIOStandbyActions_master(reg);
						}
					}
				}
			} //if (reg->clkCounter == reg->clkHalfT)
		}
		else
		{
			SSPPIOStandbyActions_master(reg);
		}

		//End of master handler
	}
}

/*===========================================================================
 * End of file 1986BE9x_SSPPIO.c
 *===========================================================================*/ 
