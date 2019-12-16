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
 *  File 1901BC1x_DSP.c: DSP core functions.
 *
 *===========================================================================*/

#include "../../inc/DSP.h"

typedef struct tag_DSPCLKCoreParams
{
	//Input parameters
	u32 iclk;     //Requested DSP core clock, Hz
	u32 clkSrc;   //Clock source (see CLKSRC_x)

	//Output parameters
	u32 oclk;      //Solved output DSP clock, Hz
	u8  mulc;      //PLL_DSP_MUL
	u8  divc;      //DSP_C3_SEL

	u16 res;       //Reserved
} DSPCLKCoreParams;


/* */
static MCRESULT TuneDSPCLKParams(DSPCLKCoreParams *p)
{
	DSPCLKCoreParams tmp;
	u32 diffDSPCLK;
	u32 selClock;

	if (!p)
		return MCR_SETUPCLK_FAIL;

	//Check requested dspclk
	if (p->iclk > MAX_DSPCLK)
		p->iclk = MAX_DSPCLK;

	p->oclk = 0;
	p->mulc = 0;
	p->divc = 0;

	switch(p->clkSrc)
	{
		case CLKSRC_HSI: selClock = FREQ_HSI; break;
		case CLKSRC_HSE: selClock = FREQ_HSE; break;
		default: return MCR_SETUPCLK_BADSRC;
	}

	//Set max difference
	diffDSPCLK = p->iclk;

	tmp.iclk = 0;

	//For each mul value [0..15]
	tmp.mulc = 0;
	while(tmp.mulc <= 15)
	{
		//For each div value [0..1]
		tmp.divc = 0;
		while(tmp.divc <= 1)
		{
			u32 df;

			//Solve result hclk with current parameters
			tmp.iclk = (u32)selClock * (tmp.mulc + 1) / (1 << tmp.divc);

			//Check dspclk
			if (tmp.iclk > MAX_VALID_DSPCLK)
			{
				tmp.divc++;
				continue;
			}

			//Solve difference
			df = tmp.iclk >= p->iclk ? (tmp.iclk - p->iclk) : (p->iclk - tmp.iclk);

			//Save params
			if (df < diffDSPCLK)
			{
			      diffDSPCLK = df;
			      p->oclk = tmp.iclk;
			      p->divc = tmp.divc;
			      p->mulc = tmp.mulc;
			}

			if (df == 0)
				break;

			//Select next div
			tmp.divc++;
		} //while

		if (diffDSPCLK == 0)
			break;

		//Select next mul
		tmp.mulc++;
	} //while

	return p->oclk < MAX_VALID_DSPCLK ? MCR_OK : MCR_SETUPCLK_FAIL;
}



/* Setup DSP core speed */
MCRESULT SetDSPCLK(u32 dspclk, u32 clkSrc)
{
	DSPCLKCoreParams hp;

	hp.iclk   = dspclk;
	hp.clkSrc = clkSrc;

	if (TuneDSPCLKParams(&hp) != MCR_OK)
		return MCR_SETUPCLK_FAIL;

	//Reset DSP_C1_SEL[1..0]
	//0 - HSI; 1 - HSI/2; 2 - HSE; 3 - HSE/2
	RST_CLK->DSP_CLOCK &= ~0x0003;

	if (!TurnClkSrc(clkSrc, 1))
		return MCR_SETUPCLK_FAIL;

	switch(clkSrc)
	{
		case CLKSRC_HSI:
		{
			//DSP_C1_SEL[1..0] = 0 (HSI) this bits already reset
			//RST_CLK->DSP_CLOCK &= ~0x0003;
			break;
		}

		case CLKSRC_HSE:
		{
			//DSP_C1_SEL[1..0] = 2 (HSE)
			RST_CLK->DSP_CLOCK |= 0x0002;
			break;
		}

		default:
			return MCR_SETUPCLK_BADSRC;
	}

	//DSPCLK = (PLL_DSP_MUL + 1) * HSE
	//PLL_DSP_MUL[23..20] = <mul val>
	//PLL_DSP_RLD[17] = 1 - reload dps pll
	//PLL_DSP_ON[16]  = 1 - dsp pll on
	RST_CLK->PLL_CONTROL &= ~(0x0F << 20);
	RST_CLK->PLL_CONTROL |= (hp.mulc << 20) | PN16; //| PN17;

	//While PLL_DSP_RDY[3] == 0 - DSP PLL is not ready
	while(!(RST_CLK->CLOCK_STATUS & PN3));

	//DSP_CLK_EN[8] = 1 - enable DSP clock
	//DSP_C3_SEL[4] = hp.divc
	//DSP_C2_SEL[2] = 1 - use PLLDSPo
	RST_CLK->DSP_CLOCK &= ~(1 << 4);
	RST_CLK->DSP_CLOCK |= PN8 | PN2 | ((hp.divc & 1) << 4);

	return MCR_OK;
}



/* Gets current DSP core speed, Hz */
u32 GetDSPCLK(void)
{
	u32 speed_DSP_C1  = 0;
	u32 speed_PLLDSPo = 0;
	u32 speed_DSP_C2;
	u32 speed_DSP_C3;
	u32 divc;

	//DSP_CLK_EN[8]: 1/0 - enable/disable DSP clock
	if (!(RST_CLK->DSP_CLOCK & PN8))
	    return 0;

	//Selected clock source
	//DSP_C1_SEL[1..0]: [0..3] -> [HSI, HSI/2, HSE, HSE/2]
	switch(RST_CLK->DSP_CLOCK & 0x03) //DSP_C1_SEL
	{
		case 0x00: { speed_DSP_C1 = IsClkSrcReady(CLKSRC_HSI) ? FREQ_HSI : 0;        break; }
		case 0x01: { speed_DSP_C1 = IsClkSrcReady(CLKSRC_HSI) ? (FREQ_HSI >> 1) : 0; break; }
		case 0x02: { speed_DSP_C1 = IsClkSrcReady(CLKSRC_HSE) ? FREQ_HSE : 0;        break; }
		case 0x03: { speed_DSP_C1 = IsClkSrcReady(CLKSRC_HSE) ? (FREQ_HSE >> 1) : 0; break; }
	}

	//PLL block
	//DSP_C2_SEL[2]: 1/0 -> pllDSPo/DSP_C1
	if (RST_CLK->DSP_CLOCK & PN2)
	{
		//PLL_DSP_ON[16]: 1/0 - PLL on/off
		if (RST_CLK->PLL_CONTROL & PN16)
		{
			//PLL_DSP_MUL[23..20]
			int pll_DSP_MUL = (RST_CLK->PLL_CONTROL & (0x0F << 20)) >> 20;
			speed_PLLDSPo = speed_DSP_C1 * (pll_DSP_MUL + 1);
		}

		//DSP_C2 = PLL out
		speed_DSP_C2 = speed_PLLDSPo;
	}
	else
	{
		//DSP_C2 = DSP_C1
		speed_DSP_C2 = speed_DSP_C1;
	}

	//Division block
	//DSP_C3_SEL[4]: [0..1] -> [/1../2]
	divc = (RST_CLK->DSP_CLOCK & 0x10) >> 4;

	speed_DSP_C3 = (u32)(speed_DSP_C2 / (float)(1 << divc));

	return speed_DSP_C3;
}



/* Load DSP data/code to DSP data/code memory */
void DSPMemWrite(const u16 *src, u32 destArmAddr, s32 wSize)
{
	u16 *pDest;
	s32 i;

	pDest = (u16*)destArmAddr;

	for(i = 0; i < wSize; i++)
		*pDest++ = *src++;
}



/* Clear DSP data memory */
void DSPMemDataClear(void)
{
	u16 *ptr;
	u32 i;

	//Clear all memory except core/peripherals registers
	ptr = (u16*)(DSP_DATA_ARM_ADDR + 0x0100);

	for(i = 0; i < DSP_DATA_MAX_SIZE; i++)
		*ptr++ = (u16)0;
}



/* Set/clear reset signal for DSP peripherals */
void DSPPerReset(u8 on)
{
	//RST_DSP_PER[3] = 1/0 - peripherals at reset/work mode
	if (on)
		RST_CLK->DSP_CONTROL_STATUS |=  PN3;
	else
		RST_CLK->DSP_CONTROL_STATUS &= ~PN3;
}



/* Set/clear reset signal for DSP memory */
void DSPMemReset(u8 on)
{
	//RST_DSP_MEM[2] = 1/0 - memory at reset/work mode
	if (on)
		RST_CLK->DSP_CONTROL_STATUS |=  PN2;
	else
		RST_CLK->DSP_CONTROL_STATUS &= ~PN2;
}



/* Set/clear reset signal for DSP core */
void DSPCoreReset(u8 on)
{
	//RD_Buff_EN[15] = 1/0 - on/off preread,
	//HOLD[4] = 0 - request to stop DSP convair; 1 - work mode
	//RST_DSP_CPU[1] = 0 - DSP core at work mode.
	if (on)
	{
	    RST_CLK->DSP_CONTROL_STATUS |=  PN1;
	}
	else
	{
		RST_CLK->DSP_CONTROL_STATUS |=  PN4;
		RST_CLK->DSP_CONTROL_STATUS &= ~PN1;
	}
}



/* Set/clear reset signal for DSP subsystem */
void DSPBlockReset(u8 on)
{
	u32 i;
	//RST_DSP_PER[3] = 1/0 - DSP peripherals at reset/work mode
	//RST_DSP_MEM[2] = 1/0 - DSP memory at reset/work mode
	//RST_DSP_CPU[1] = 1/0 - DSP core  at reset/work mode
	//RST_DSP[0]     = 1/0 - DSP block at reset/work mode
	if (on)
		RST_CLK->DSP_CONTROL_STATUS = 0x000F;
	else
		RST_CLK->DSP_CONTROL_STATUS &= ~PN0;

	i = 0;
	while(i < 100) { i += 2; i--; };
}

/*===========================================================================
 * End of file 1901BC1x_DSP.c
 *===========================================================================*/
