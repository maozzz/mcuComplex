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
 *  File 1986BE9x_SSP.c. SSP functions.
 *
 *===========================================================================*/

#include "../../inc/SSP.h"
#include <string.h>

//SSP1,SSP2 configurations
static SSPConfig cfgSSP1;
static SSPConfig cfgSSP2;

#if (TARGET_MCU == MCU_1986BE9x)
//2 SSP blocks
static _ssp *regSSPs[2] = { SSP1, SSP2 };
static SSPConfig *cfgSSPs[2] = { &cfgSSP1, &cfgSSP2 };

#elif (TARGET_MCU == MCU_1986BE1x)
//3 SSP blocks
static SSPConfig cfgSSP3;
static _ssp *regSSPs[3] = { SSP1, SSP2, SSP3 };
static SSPConfig *cfgSSPs[3] = { &cfgSSP1, &cfgSSP2, &cfgSSP3 };

#elif (TARGET_MCU == MCU_1901BC1x)
//4 SSP blocks
static SSPConfig cfgSSP3;
static SSPConfig cfgSSP4;
static _ssp *regSSPs[4] = { SSP1, SSP2, SSP3, SSP4 };
static SSPConfig *cfgSSPs[4] = { &cfgSSP1, &cfgSSP2, &cfgSSP3, &cfgSSP4 };

#endif

#define GetRegSSPX(x) (regSSPs[(x)])
#define GetCfgSSPX(x) (cfgSSPs[(x)])


typedef struct tag_SSPClkParams
{
        //Input params
        u32 speed;
        u8 mode;

        //Output params
        u32 ospeed;
        u8 brg;
        u8 cpsr;
        u8 scr;
} SSPClkParams;



/* */
static MCRESULT TuneSSPClkParams(SSPClkParams *p)
{
	MCRESULT result;
	u32 scr, cpsr, brg;
	u32 minN, hclk, N;
	u32 dfSpeed, dfSpeedSaved;
	u32 ospeed;
	u8 isFound = 0;

	hclk = GetHCLK();

	//sspclk / speed >= 2 for master, >= 12 for slave mode
	minN = (p->mode == SSP_MODE_MASTER) ? 2 : 12;

	dfSpeedSaved = p->speed;

	brg = 1;
	//brg = [0..7], step 1
	while(brg < 8)
	{
		//scr = [0..255], step 1
		scr = 0;
		while(scr < 64)
		{
			//cpsr = [2..254], step 2
			cpsr = 2;
			while(cpsr < 255)
			{
				N = cpsr * (scr + 1);

				if (N < minN)
				{
					cpsr += 2;
					continue;
				}

				//SSPCLK = HCLK / (2^BRG) = HCLK >> BRG
				ospeed = (hclk >> brg) / N;

				dfSpeed = ospeed >= p->speed ?
						  ospeed - p->speed : p->speed - ospeed;

				if (dfSpeed < dfSpeedSaved)
				{
					dfSpeedSaved = dfSpeed;

					p->ospeed = ospeed;
					p->brg    = (u8)brg;
					p->cpsr   = (u8)cpsr;
					p->scr    = (u8)scr;
					isFound   = 1;
				}

				cpsr += 2;
			} //while cpsr

			scr++;
		} //while scr

		brg++;
	} //while brg

	if (isFound)
		result = MCR_OK;
	else
		result = MCR_SETUPCLK_FAIL;

	return (result);
}



#define SSPx_PORTCTRL_CONFIG    (PN_NOPULLUP   | PN_NOPULLDOWN | PN_NOSHMIT |\
				 PN_CTRLDRIVER | PN_NOINFILTER)

#define SSPx_PORTRXTX_CONFIG    SSPx_PORTCTRL_CONFIG

#define SSPRxTx(port,func,prx,ptx) portRxTx = port; funcRxTx = func;\
                                   pinRx = prx; pinTx = ptx;

#define SSPCtrl(port,func,pfss,pclk) portCtrl = port; funcCtrl = func;\
                                     pinFSS = pfss; pinCLK = pclk;

static MCRESULT SSPMapPortPins(const SSPConfig *cfg)
{
        PORTBLOCK portRxTx, portCtrl;
	u32 pinRx, pinTx, pinFSS, pinCLK;

	u32 funcRxTx, funcCtrl;
	u32 isPinsDefined = 1;

        switch(cfg->ssp)
        {
		//--- Select mapping 1986BE91T ---
		#if (TARGET_MCU == MCU_1986BE9x)
		case SSP_N1: //1986BE91T SSP1
                {
                        switch(cfg->mapRxTx)
                        {
				case SSP1_MAP_RXTX_B14B15: { SSPRxTx(PORT_B, PN_ALTER, PN14, PN15); break; }

				case SSP1_MAP_RXTX_F3F0:   { SSPRxTx(PORT_F, PN_ALTER, PN3, PN0);   break; }

				case SSP1_MAP_RXTX_D11D12: { SSPRxTx(PORT_D, PN_REMAP, PN11, PN12); break; }

				default: isPinsDefined = 0; break;
                        } //case SSP_N1, switch(cfg->mapRxTx)


                        switch(cfg->mapCtrl)
                        {
				case SSP1_MAP_CTRL_B12B13: { SSPCtrl(PORT_B, PN_ALTER, PN12, PN13);  break; }

				case SSP1_MAP_CTRL_F2F1:   { SSPCtrl(PORT_F, PN_ALTER, PN2, PN1);    break; }

				case SSP1_MAP_CTRL_D9D10:  { SSPCtrl(PORT_D, PN_REMAP, PN9, PN10);   break; }

				default: isPinsDefined = 0; break;
                        } //case SSP_N1, switch(cfg->mapCtrl)

                        break;
                } //case SSP_N1

		case SSP_N2: //1986BE91T SSP2
                {
                        switch(cfg->mapRxTx)
                        {
				case SSP2_MAP_RXTX_D2D6:   { SSPRxTx(PORT_D, PN_ALTER, PN2, PN6);   break; }

				case SSP2_MAP_RXTX_B14B15: { SSPRxTx(PORT_B, PN_REMAP, PN14, PN15); break; }

				case SSP2_MAP_RXTX_C2C3:   { SSPRxTx(PORT_C, PN_REMAP, PN2, PN3);   break; }

				case SSP2_MAP_RXTX_F14F15: { SSPRxTx(PORT_F, PN_REMAP, PN14, PN15); break; }

				default: isPinsDefined = 0; break;
                        } //case SSP_N2, switch(cfg->mapRxTx)


                        switch(cfg->mapCtrl)
                        {
				case SSP2_MAP_CTRL_D3D5:   { SSPCtrl(PORT_D, PN_ALTER, PN3, PN5);   break; }

				case SSP2_MAP_CTRL_B12B13: { SSPCtrl(PORT_B, PN_REMAP, PN12, PN13); break; }

				case SSP2_MAP_CTRL_C0C1:   { SSPCtrl(PORT_C, PN_REMAP, PN0, PN1);   break; }

				case SSP2_MAP_CTRL_F12F13: { SSPCtrl(PORT_F, PN_REMAP, PN12, PN13); break; }

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
				case SSP1_MAP_RXTX_C6C5: { SSPRxTx(PORT_C, PN_ALTER, PN6, PN5); break; }

				case SSP1_MAP_RXTX_D3D2: { SSPRxTx(PORT_D, PN_ALTER, PN3, PN2); break; }

				case SSP1_MAP_RXTX_C5C6: { SSPRxTx(PORT_C, PN_REMAP, PN5, PN6); break; }

				default: isPinsDefined = 0; break;
                        } //case SSP_N1, switch(cfg->mapRxTx)

                        switch(cfg->mapCtrl)
                        {
				case SSP1_MAP_CTRL_C8C7: { SSPCtrl(PORT_C, PN_ALTER, PN8, PN7); break; }

				case SSP1_MAP_CTRL_D5D4: { SSPCtrl(PORT_D, PN_ALTER, PN5, PN4); break; }

				default: isPinsDefined = 0; break;
                        } //case SSP_N1, switch(cfg->mapCtrl)

                        break;
                } //case SSP_N1

		case SSP_N2: //1986BE1T SSP2
                {
                        switch(cfg->mapRxTx)
                        {
				case SSP2_MAP_RXTX_C10C9: { SSPRxTx(PORT_C, PN_BASIC, PN10, PN9); break; }

				case SSP2_MAP_RXTX_D8D7:  { SSPRxTx(PORT_D, PN_BASIC, PN8, PN7);  break; }

				default: isPinsDefined = 0; break;
                        } //case SSP_N2, switch(cfg->mapRxTx)

                        switch(cfg->mapCtrl)
                        {
				case SSP2_MAP_CTRL_C12C11: { SSPCtrl(PORT_C, PN_BASIC, PN12, PN11); break; }

				case SSP2_MAP_CTRL_D10D9:  { SSPCtrl(PORT_D, PN_BASIC, PN10, PN9);  break; }

				default: isPinsDefined = 0; break;
                        } //case SSP_N2, switch(cfg->mapCtrl)

                        break;
                } //case SSP_N2

		case SSP_N3: //1986BE1T SSP3
                {
			//Rx/Tx pins at different ports
                        switch(cfg->mapRxTx)
                        {
                                case SSP3_MAP_RXTX_D12F15:
				{       //Rx/Tx
					InitializePORTEx(PORT_D, PN12, 0,
							 PN_REMAP | PN_INPUT | SSPx_PORTRXTX_CONFIG);

                                        InitializePORTEx(PORT_F, PN15, 0,
							 PN_REMAP | PN_PWR_FASTEST | SSPx_PORTRXTX_CONFIG);
					break;
                                }

                                case SSP3_MAP_RXTX_F15D12:
				{       //Rx/Tx
                                        InitializePORTEx(PORT_F, PN15, 0,
							 PN_BASIC | PN_INPUT | SSPx_PORTRXTX_CONFIG);

                                        InitializePORTEx(PORT_D, PN12, 0,
							 PN_BASIC | PN_PWR_FASTEST | SSPx_PORTRXTX_CONFIG);
					break;
                                }

				default: isPinsDefined = 0;	break;
			}

                        switch(cfg->mapCtrl)
                        {
                                case SSP3_MAP_CTRL_F13F14:
                                {                                        
					//FSS/CLK PINS
					u32 pwr;
					pwr = (cfg->mode == SSP_MODE_MASTER) ? PN_PWR_FASTEST : PN_INPUT;
					InitializePORTEx(PORT_F, PN13|PN14, 0,
							 pwr | PN_REMAP | SSPx_PORTCTRL_CONFIG);
                                        break;
                                }

				default: isPinsDefined = 0;	break;
			}

			//SSP pins already initialized: return
			return isPinsDefined ? MCR_OK : MCR_INVALID_MAPPING;
                } //case SSP_N3
		//--- End mapping 1986BE1T ---

		//Select mapping 1901BC1T
		#elif (TARGET_MCU == MCU_1901BC1x)
		case SSP_N1: //1901BC1T SSP1
		{
			switch(cfg->mapRxTx)
			{
			    case SSP1_MAP_RXTX_A12A13: { SSPRxTx(PORT_A, PN_ALTER, PN12, PN13);	break; }

			    case SSP1_MAP_RXTX_D4D2:   { SSPRxTx(PORT_D, PN_ALTER, PN4, PN2);   break; }

			    case SSP1_MAP_RXTX_E12E13: { SSPRxTx(PORT_E, PN_ALTER, PN12, PN13);	break; }

			    default: isPinsDefined = 0;	break;
			}

			switch(cfg->mapCtrl)
			{
			    case SSP1_MAP_CTRL_A15A14: { SSPCtrl(PORT_A, PN_ALTER, PN15, PN14);	break; }

			    case SSP1_MAP_CTRL_D3D5:   { SSPCtrl(PORT_D, PN_ALTER, PN3, PN5);   break; }

			    case SSP1_MAP_CTRL_E15E14: { SSPCtrl(PORT_E, PN_ALTER, PN15, PN14);	break; }

			    default: isPinsDefined = 0;	break;
			}

			break;
		} //case SSP_N1

		case SSP_N2: //1901BC1T SSP2
		{
			switch(cfg->mapRxTx)
			{
				case SSP2_MAP_RXTX_A11A10: { SSPRxTx(PORT_A, PN_ALTER, PN11, PN10); break; }

				case SSP2_MAP_RXTX_B15B14: { SSPRxTx(PORT_B, PN_ALTER, PN15, PN14); break; }

				case SSP2_MAP_RXTX_D13D15: { SSPRxTx(PORT_D, PN_ALTER, PN13, PN15); break; }

				case SSP2_MAP_RXTX_E6E4:   { SSPRxTx(PORT_E, PN_ALTER, PN6, PN4);   break; }

				default: isPinsDefined = 0; break;
			}

			switch(cfg->mapCtrl)
			{
				case SSP2_MAP_CTRL_A9A8:   { SSPCtrl(PORT_A, PN_ALTER, PN9, PN8);   break; }

				case SSP2_MAP_CTRL_B13B12: { SSPCtrl(PORT_B, PN_ALTER, PN13, PN12); break; }

				case SSP2_MAP_CTRL_D12D14: { SSPCtrl(PORT_D, PN_ALTER, PN12, PN14); break; }

				case SSP2_MAP_CTRL_E5E7:   { SSPCtrl(PORT_E, PN_ALTER, PN5, PN7);   break; }

				default: isPinsDefined = 0; break;
			}

			break;
		} //case SSP_N2

		case SSP_N3: //1901BC1T SSP3
		{
			switch(cfg->mapRxTx)
			{
				case SSP3_MAP_RXTX_C10C8:  { SSPRxTx(PORT_C, PN_ALTER, PN10, PN8);  break; }

				case SSP3_MAP_RXTX_F14F12: { SSPRxTx(PORT_F, PN_ALTER, PN14, PN12); break; }

				default: isPinsDefined = 0; break;
			}

			switch(cfg->mapCtrl)
			{
				case SSP3_MAP_CTRL_C9C11:  { SSPCtrl(PORT_C, PN_ALTER, PN9, PN11);  break; }

				case SSP3_MAP_CTRL_F13F15: { SSPCtrl(PORT_F, PN_ALTER, PN13, PN15); break; }

				default: isPinsDefined = 0;	break;
			}

			break;
		} //case SSP_N3

		case SSP_N4: //1901BC1T SSP4
		{
			switch(cfg->mapRxTx)
			{
				case SSP4_MAP_RXTX_C3C4:   { SSPRxTx(PORT_C, PN_ALTER, PN3, PN4);   break; }

				case SSP4_MAP_RXTX_C15C14: { SSPRxTx(PORT_C, PN_ALTER, PN15, PN14); break; }

				case SSP4_MAP_RXTX_E3E2:   { SSPRxTx(PORT_E, PN_ALTER, PN3, PN2);   break; }

				case SSP4_MAP_RXTX_F2F3:   { SSPRxTx(PORT_F, PN_ALTER, PN2, PN3);   break; }

				default: isPinsDefined = 0; break;
			}

			switch(cfg->mapCtrl)
			{
				case SSP4_MAP_CTRL_C0C5:   { SSPCtrl(PORT_C, PN_ALTER, PN0, PN5);   break; }

				case SSP4_MAP_CTRL_C12C13: { SSPCtrl(PORT_C, PN_ALTER, PN12, PN13); break; }

				case SSP4_MAP_CTRL_E0E1:   { SSPCtrl(PORT_E, PN_ALTER, PN0, PN1);   break; }

				case SSP4_MAP_CTRL_F5F4:   { SSPCtrl(PORT_F, PN_ALTER, PN5, PN4);   break; }

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
				InitializePORTEx(portCtrl, pinFSS, 0, funcCtrl | pwr | SSPx_PORTCTRL_CONFIG);
				InitializePORTEx(portCtrl, pinCLK, 0, funcCtrl | pwr | SSPx_PORTCTRL_CONFIG);

				//RXD/TXD
				InitializePORTEx(portRxTx, pinRx,  0, funcRxTx | PN_INPUT       | SSPx_PORTRXTX_CONFIG);
				InitializePORTEx(portRxTx, pinTx,  0, funcRxTx | PN_PWR_FASTEST | SSPx_PORTRXTX_CONFIG);

                return MCR_OK;
        }
        else
                return MCR_INVALID_MAPPING;		
}

#undef SSPCtrl
#undef SSPRxTx

/* */
MCRESULT SSPSetSpeedInternal(SSPBLOCK ssp, u32 speed, u8 mode);


/* */
MCRESULT InitializeSSP(const SSPConfig *cfg)
{
	//u16 sspInitBufferData[SSP_FIFO_TX_LENGTH] = {0, 0, 0, 0,  0, 0, 0, 0};
	SSPConfig *destCfg;
	_ssp *reg;
	u32 valCR1;

	if (!cfg)
	    return MCR_INVALID_CFG;
	else if (cfg->ssp >= SSP_COUNT)
	    return MCR_INVALID_PORT;

	//Pointer to configuration
	destCfg = cfgSSPs[(cfg->ssp)];

	//Copy configuration
	memcpy(destCfg, cfg, sizeof(SSPConfig));

	//Map port
	if (SSPMapPortPins(destCfg) != MCR_OK)
		return MCR_INVALID_PORT;

	if (SSPSetSpeedInternal(destCfg->ssp, destCfg->speed, destCfg->mode) != MCR_OK)
		return MCR_SETUPSPEED_FAIL;

	//Turn OFF selected SSP
	TurnSSP(destCfg->ssp, 0);

	//Get SSPx registers
	reg = GetRegSSPX(destCfg->ssp);

	//Set DSS[3..0] - frame length
	reg->CR0 &= ~0x000F;
	reg->CR0 |= ((destCfg->dataBits - 1) & 0x000F);

	//Set FRF[5..4] - frame format (protocol)
	reg->CR0 &= ~0x30;
	reg->CR0 |=  (destCfg->proto & 0x03) << 4;


	//Specified protocol options
	switch (destCfg->proto)
	{
		//SPI protocol options
		case SSP_PROTO_SPI:
		{
			//Set/reset SPO[6] - clock polarity
			if (destCfg->opts & SSP_OPTS_SPI_POLHIGH)
				reg->CR0 |= PN6;
			else
				reg->CR0 &= ~PN6;

			//Set/reset SPH[7] - register data
			if (destCfg->opts & SSP_OPTS_SPI_PHASE2)
				reg->CR0 |= PN7;
			else
				reg->CR0 &= ~PN7;

			break;
		}
	}


	valCR1 = (reg->CR1 & 0x0F);

	//LBM[0]: 1/0 - enable/disable loopback
	if (destCfg->opts & SSP_OPTS_LOOPBACK)
		valCR1 |= PN0;
	else
		valCR1 &= ~PN0;

	//Set MS[2]: 0/1 - master/slave
	if (destCfg->mode == SSP_MODE_MASTER)
		valCR1 &= ~PN2;
	else
		valCR1 |= PN2;

	reg->CR1 = (u8)(valCR1 & 0x0F);

    //IMSC register:
	//RORIM[0] - receiver overflow interrupt
	//RTIM[1]  - receiver timeout interrupt
	//RXIM[2]  - receiver interrupt
    //TXIM[3]  - tx FIFO is filled to 50% or less
    #if (__SSP_MODULE_VERSION == 2)
    //VERSION=2
    if (destCfg->received || destCfg->transmitted)
	{
        reg->IMSC = 0;

        //RXIM[2]=RTIM[1]=RORIM[0]=1
        if (destCfg->received)
            reg->IMSC |= (PN2 | PN1 | PN0);

        //TXIM[3]=1
        if (destCfg->transmitted)
            reg->IMSC |= PN3;

		//Enable SSPx interrupt
		switch(destCfg->ssp)
		{
			case SSP_N1: IRQEnable(IRQ_SSP1); break;

			case SSP_N2: IRQEnable(IRQ_SSP2); break;

			#if (TARGET_MCU == MCU_1986BE1x || TARGET_MCU == MCUM_1901BC1T)
			case SSP_N3: IRQEnable(IRQ_SSP3);  break;
			#endif

			#if (TARGET_MCU == MCUM_1901BC1T)
			case SSP_N4: IRQEnable(IRQ_SSP4); break;
			#endif
		}
	}

    #else
    //VERSION=1
    if (destCfg->received)
    {
        //RXIM[2]=RTIM[1]=RORIM[0]=1
        reg->IMSC |= (PN2 | PN1 | PN0);

        //Enable SSPx interrupt
        switch(destCfg->ssp)
        {
            case SSP_N1: IRQEnable(IRQ_SSP1); break;

            case SSP_N2: IRQEnable(IRQ_SSP2); break;

            #if (TARGET_MCU == MCU_1986BE1x || TARGET_MCU == MCUM_1901BC1T)
            case SSP_N3: IRQEnable(IRQ_SSP3);  break;
            #endif

            #if (TARGET_MCU == MCUM_1901BC1T)
            case SSP_N4: IRQEnable(IRQ_SSP4); break;
            #endif
        }
    }
    #endif /* __SSP_MODULE_VERSION */
	else
	{
        //Disable SSPx interrupt
        reg->IMSC &= ~(PN3 | PN2 | PN1 | PN0);

		switch(destCfg->ssp)
		{
			case SSP_N1: IRQDisable(IRQ_SSP1); break;

			case SSP_N2: IRQDisable(IRQ_SSP2); break;

			#if (TARGET_MCU == MCU_1986BE1x || TARGET_MCU == MCUM_1901BC1T)
			case SSP_N3: IRQDisable(IRQ_SSP3); break;
			#endif

			#if (TARGET_MCU == MCUM_1901BC1T)
			case SSP_N4: IRQDisable(IRQ_SSP4); break;
			#endif
		}
	}

	return MCR_OK;
}



/* Set SSP speed */
MCRESULT SSPSetSpeedInternal(SSPBLOCK ssp, u32 speed, u8 mode)
{
	_ssp *reg;
	SSPClkParams clkp;

	//Check ssp block index
	if (ssp >= SSP_COUNT)
		return MCR_INVALID_PORT;

	//Check requested speed
	if (speed > SSP_MAX_SPEED)
		speed = SSP_MAX_SPEED;

	clkp.speed = speed;
	clkp.mode  = mode;
	clkp.brg   = 0;
	clkp.cpsr  = 0;
	clkp.scr   = 0;

	//Tune clock parameters
	if (TuneSSPClkParams(&clkp) != MCR_OK)
		return MCR_SETUPCLK_FAIL;

	//Configure SSPx clock (FSSPCLK)
	switch(ssp)
	{
			case SSP_N1:
			{
					//Enable SSP1 clock
					ClkEnable(CLK_SSP1);

					//SSP1_CLK_EN[24] = 0
					RST_CLK->SSP_CLOCK &= ~PN24;

					//Setup SSP1_BRG[7..0] at [0..7]
					//FSSPCLK = HCLK / (1 << BRG)
					RST_CLK->SSP_CLOCK &= ~0xFF;
					RST_CLK->SSP_CLOCK |= clkp.brg;

					//SSP1_CLK_EN[24] = 1
					RST_CLK->SSP_CLOCK |= PN24;
					break;
			}

			case SSP_N2:
			{
					//Enable SSP2 clock
					ClkEnable(CLK_SSP2);

					//SSP2_CLK_EN[25] = 0
					RST_CLK->SSP_CLOCK &= ~PN25;

					//Setup SSP2_BRG[15..8] at [0..7]
					//FSSPCLK = HCLK / (1 << BRG)
					RST_CLK->SSP_CLOCK &= ~(0xFF << 8);
					RST_CLK->SSP_CLOCK |= clkp.brg << 8;

					//SSP2_CLK_EN[25] = 1
					RST_CLK->SSP_CLOCK |= PN25;
					break;
			}

			#if (TARGET_MCU == MCU_1986BE1x || TARGET_MCU == MCU_1901BC1x)
			case SSP_N3:
			{
					//Enable SSP2 clock
					ClkEnable(CLK_SSP3);

					//SSP3_CLK_EN[26] = 0
					RST_CLK->SSP_CLOCK &= ~PN26;

					//Setup SSP3_BRG[23..16] at [0..7]
					//FSSPCLK = HCLK / (1 << BRG)
					RST_CLK->SSP_CLOCK &= ~(0xFF << 16);
					RST_CLK->SSP_CLOCK |= clkp.brg << 16;

					//SSP3_CLK_EN[26] = 1
					RST_CLK->SSP_CLOCK |= PN26;
					break;
			}
			#endif

			#if (TARGET_MCU == MCU_1901BC1x)
			case SSP_N4:
			{
				//Enable SSP4 clock
				ClkEnable(CLK_SSP4);

				//SSP4_CLK_EN[24] = 0
				RST_CLK->SSP_CLOCK2 &= ~PN24;

				//Setup SSP4_BRG[7..0]
				//FSSPCLK = HCLK / (1 << BRG)
				RST_CLK->SSP_CLOCK2 &= ~0xFF;
				RST_CLK->SSP_CLOCK2 |= clkp.brg;

				//SSP4_CLK_EN[24] = 1
				RST_CLK->SSP_CLOCK2 |= PN24;
				break;
			}
			#endif
	}

	reg = GetRegSSPX(ssp);

	//Set SCR, CPSR: clock out (FSSPCLKOUT)
	//FSSPCLKOUT = FSSPCLK / (CPSR * (SCR + 1))
	//Set SCR[15..8]
	reg->CR0 &= ~(0xFF << 8);
	reg->CR0 |= (clkp.scr << 8);

	//Set CPSR[7..0]
	reg->CPSR &= ~0xFF;
	reg->CPSR = clkp.cpsr;

	return MCR_OK;
}



/* Set SSP speed */
MCRESULT SSPSetSpeed(SSPBLOCK ssp, u32 speed)
{
	u32 isWasON;
	_ssp *reg;
	SSPConfig *destCfg;
	MCRESULT result;

	//Check ssp block index
	if (ssp >= SSP_COUNT)
		return MCR_INVALID_PORT;

	reg = GetRegSSPX(ssp);

	destCfg = cfgSSPs[ssp];

	//SSE[1] = 1/0 - SSP ON/OFF
	isWasON = (reg->CR1 & PN1) ? 1 : 0;

	//Turn SSP OFF
	TurnSSP(ssp, 0);

	//Setup speed
	result = SSPSetSpeedInternal(ssp, speed, destCfg->mode);

	//Turn SSP ON
	if (isWasON && result == MCR_OK)
		TurnSSP(ssp, 1);

	return result;
}



/* Get SSP clock, Hz */
u32 GetSSPClk(SSPBLOCK ssp)
{
        u32 hclk = GetHCLK();
        u32 clk = 0;

        //FSSPCLK = HCLK / (1 << BRG)
        switch(ssp)
        {
                case SSP_N1:
                {
					//SSP1 enable and SSP1_CLK_EN[24]
					if (!(RST_CLK->PER_CLOCK & (1 << CLK_SSP1)) ||
						!(RST_CLK->SSP_CLOCK & PN24))
						break;

                        //SSP1_BRG[7..0]
                        clk = hclk / (1 << (u8)RST_CLK->SSP_CLOCK);
                        break;
                }

                case SSP_N2:
                {
					//SSP2 enable and SSP2_CLK_EN[25]
					if (!(RST_CLK->PER_CLOCK & (1 << CLK_SSP2)) ||
						!(RST_CLK->SSP_CLOCK & PN25))
						break;

					   //SSP2_BRG[15..8]
                        clk = hclk / (1 << ((u8)(RST_CLK->SSP_CLOCK >> 8)));
                        break;
                }

				#if (TARGET_MCU == MCU_1986BE1x || TARGET_MCU == MCU_1901BC1x)
                case SSP_N3:
                {
					//SSP3 enable and SSP3_CLK_EN[26]
					if (!(RST_CLK->PER_CLOCK & (1 << CLK_SSP3)) ||
						!(RST_CLK->SSP_CLOCK & PN26))
						break;

                        //SSP3_BRG[23..16]
                        clk = hclk / (1 << ((u8)(RST_CLK->SSP_CLOCK >> 16)));
                        break;
                }
                #endif

				#if (TARGET_MCU == MCU_1901BC1x)
				case SSP_N4:
				{
					 //SSP4 enable and SSP4_CLK_EN[24]
					if (!(RST_CLK->PER_CLOCK & (1 << CLK_SSP4)) ||
						!(RST_CLK->SSP_CLOCK2 & PN24))
						 break;

					//SSP4_BRG[7..0]
					clk = hclk / (1 << ((u8)(RST_CLK->SSP_CLOCK2)));
					break;
				}
				#endif
        }

        return clk;
}



/* Get SSP speed, bit/s */
u32 GetSSPSpeed(SSPBLOCK ssp)
{
		_ssp *reg = GetRegSSPX(ssp);

        //FSSPCLKOUT = FSSPCLK / (CPSR * (SCR + 1))
        //CPSR[7..0], SCR[15..8]
		//if SSE[1] is set SSP turned ON
        if (reg->CR1 & PN1)
                return (u32)(GetSSPClk(ssp) / ( (u8)reg->CPSR * ((u8)(reg->CR0 >> 8) + 1)) );
        else
                return 0;
}



/* */
void TurnSSP(SSPBLOCK ssp, u8 turn)
{
	_ssp *reg = GetRegSSPX(ssp);

        //Set/reset SSE[1] - turn ON/OFF SSP
        if (turn)
                reg->CR1 |= PN1;
        else
        {
                //while BSY[4] == 1 - while SSP is busy
                //while(reg->SR & PN4);

                reg->CR1 &= ~PN1;
        }
}



/* */
void TurnSSPTx(SSPBLOCK ssp, u8 turn)
{
	_ssp *reg = GetRegSSPX(ssp);

        //if MS[2] = 0 - its master mode
        //SOD[3] used only at slave mode
        if (!(reg->CR1 & PN2))
                return;

        if (turn)
                reg->CR1 &= ~PN3; //Reset SOD[3]: enable TxD
        else
                reg->CR1 |= PN3;  //Set SOD[3]: disable TxD
}



/* Turn ON/OFF SSP loopback */
void TurnSSPLB(SSPBLOCK ssp, u8 on)
{
	_ssp *reg;
	u32 valCR1;
		
	reg = GetRegSSPX(ssp);
	valCR1 = (reg->CR1 & 0x0F);

        //LBM[0]: 1/0 - enable/disable loopback
        if (on)
                valCR1 |= PN0;
        else
                valCR1 &= ~PN0;       

        reg->CR1 = (u8)(valCR1 & 0x0F);
}



/* */
void SSPWaitNotBusy(SSPBLOCK ssp)
{
	_ssp *reg = GetRegSSPX(ssp);

        //while BSY[4] == 1 - while SSP is busy
        while(reg->SR & PN4);
}



/* Indicating that input fifo is empty */
u8 IsSSPInFifoEmpty(SSPBLOCK ssp)
{
        //RNE[2]: 1/0 - input fifo not empty/empty
	return (GetRegSSPX(ssp)->SR & PN2) ? 0 : 1;
}



/* Indicating that input fifo is full */
u8 IsSSPInFifoFull(SSPBLOCK ssp)
{
        //RFF[3]: 1/0 - input fifo full/not full
	return (GetRegSSPX(ssp)->SR & PN3) ? 1 : 0;
}



/* Indicating that output fifo is empty */
u8 IsSSPOutFifoEmpty(SSPBLOCK ssp)
{
        //TFE[0]: 1/0 - output fifo empty/not empty
	return (GetRegSSPX(ssp)->SR & PN0) ? 1 : 0;
}



/* Indicating that output fifo is full */
u8 IsSSPOutFifoFull(SSPBLOCK ssp)
{
        //TNF[1]: 1/0 - output fifo not full/full
	return (GetRegSSPX(ssp)->SR & PN1) ? 0 : 1;
}



/* Indicates that SSP busy: tx/rx data or tx FIFO not empty */
u8 IsSSPBusy(SSPBLOCK ssp)
{
    //BSY[4]: 1 - SSP tx/rx data or tx FIFO not empty
    return (GetRegSSPX(ssp)->SR & PN4);
}


/* Fill output fifo cells with specified value */
void SSPInitOutFifo(SSPBLOCK ssp, u32 v)
{
	s32 i = 0;
	_ssp *reg = GetRegSSPX(ssp);
		
	while(i < SSP_FIFO_TX_LENGTH)
	{
	    reg->DR = (u16)v;
	    i++;
	}
}



/* Flush output data */
void SSPFlush(SSPBLOCK ssp)
{
    _ssp *reg = GetRegSSPX(ssp);

    //TFE[0]: 1/0 - output fifo empty/not empty
    while ((reg->SR & PN0) == 0) __NOP();
}



/* */
s32 SSPRead(SSPBLOCK ssp, void *dest, s32 count)
{
    s32 i = 0;
	_ssp *reg = GetRegSSPX(ssp);

    if (GetCfgSSPX(ssp)->dataBits != 8)
	{
	    //Frame size: 16 bit
	    u16 *pdata16 = (u16*)dest;

	    //RNE[2]: 1 - input fifo not empty
	    //read data
	    while((reg->SR & PN2) && i < count)
	    {
            *pdata16++ = reg->DR;
            i += 2;
	    }
	}
	else
	{
	    //Frame size: 8 bit
	    u8 *pdata8 = (u8*)dest;

	    //RNE[2]: 1 - input fifo not empty
	    //read data
	    while((reg->SR & PN2) && i < count)
	    {
            *pdata8++ = reg->DR;
            i++;
	    }
	}

    return i;
}



/* */
s32 SSPWrite(SSPBLOCK ssp, const void *src, s32 count)
{
    s32 i = 0, n;
	_ssp *reg = GetRegSSPX(ssp);

    if (GetCfgSSPX(ssp)->dataBits != 8)
	{
		//Frame size: 16 bit
		const u16 *pdata16 = (u16*)src;

		//V1. TNF[1]: 0 - output fifo is full.
		/* while(i < count)
		{
			while(!(reg->SR & PN1));
			reg->DR = *pdata16++;
			i += 2;
		} */

		//V2. TNF[1]: 1 - output fifo is not full.
		/* while(i < count)
		{
			if (reg->SR & PN1)
			{
				reg->DR = *pdata16++;
				i += 2;
			}
		}
		*/

		//V3. TFE[0]: 1 - output fifo is empty
		while(i < count)
		{
			if (reg->SR & PN0)
			{
				n = 0;
				while(i < count && n++ < SSP_FIFO_TX_LENGTH)
				{
					reg->DR = *pdata16++;
					i += 2;
				}
			}
		}

		//odd byte count
		if (count & 1)
		{
			//TFE[0]: 1 - output fifo is empty
			while (!(reg->SR & PN0));

			//Write last byte
			reg->DR = (u16)(((u8*)src)[count - 1]);
		}
	}
	else
	{
		//Frame size: 8 bit
		const u8 *pdata8 = (u8*)src;

		//V1. TNF[1]: 0 - output fifo is full.
		/* while(i < count)
		{
			while(!(reg->SR & PN1));
			reg->DR = *pdata8++;
			i++;
		} */

		//V2. TNF[1]: 1 - output fifo is not full.
		while(i < count)
		{
		    if (reg->SR & PN1)
		    {
                reg->DR = *pdata8++;
                i++;
		    }
		}

		//V3. TFE[0]: 1 - output fifo is empty
		//With errors: write SD card block (512 bytes)
		/*while(i < count)
		{
			if (reg->SR & PN0)
			{
			    n = 0;
			    while(i++ < count && n++ < SSP_FIFO_TX_LENGTH)
			    reg->DR = *pdata8++;
			}
		}*/
	}

        return i;
}



/* Read one element from SSP fifo */
s32 SSPReadFifo(SSPBLOCK ssp, u16 *data)
{
    _ssp *reg = GetRegSSPX(ssp);
    *data = reg->DR;
    return 1;
}



/* Write one element to SSP fifo */
s32 SSPWriteFifo(SSPBLOCK ssp, u16 data)
{
    _ssp *reg = GetRegSSPX(ssp);
    reg->DR = data;
    return 1;
}



/* */
__IRQH_FATTR void SSPx_IRQHandler(_ssp *reg, SSPConfig *cfg)
{
        #if (__SSP_MODULE_VERSION == 2)
        u32 mis = reg->MIS;

        //RXMIS[2]=1, RTMIS[1]=1 or RORMIS[0]=1
        if (mis & (PN2 | PN1 | PN0))
        {
            //Set RTIC[1], RORIC[0]: 1 - cancel interrupt
            reg->ICR |= PN1 | PN0;

            if (cfg->received)
                    cfg->received();
        }

        //TXMIS[3]=1
        if (mis & PN3)
        {
            if (cfg->transmitted)
                cfg->transmitted();
        }

        #else
        //RXMIS[2]=1, RTMIS[1]=1 or RORMIS[0]=1
        if (reg->MIS & (PN2 | PN1 | PN0))
        {
            //Set RTIC[1], RORIC[0]: 1 - cancel interrupt
            reg->ICR |= PN1 | PN0;

            if (cfg->received)
                    cfg->received();
        }

        #endif /* __SSP_MODULE_VERSION */
}



/* */
void SSP1_IRQHandler(void)
{        
        SSPx_IRQHandler(SSP1, &cfgSSP1);
}



/* */
void SSP2_IRQHandler(void)
{        
        SSPx_IRQHandler(SSP2, &cfgSSP2);
}


#if (TARGET_MCU == MCU_1986BE1x || TARGET_MCU == MCU_1901BC1x)
/* */
void SSP3_IRQHandler(void)
{        
        SSPx_IRQHandler(SSP3, &cfgSSP3);
}
#endif /* TARGET_MCU == MCU_1986BE1x || TARGET_MCU == MCU_1901BC1x */

#if (TARGET_MCU == MCU_1901BC1x)
/* */
void SSP4_IRQHandler(void)
{
	SSPx_IRQHandler(SSP4, &cfgSSP4);
}
#endif /* TARGET_MCU == MCU_1901BC1x */

/*===========================================================================
 * End of file 1986BE9x_SSP.c
 *===========================================================================*/ 
