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
 *  File 1986BE9x_ADC.c: ADC functions.
 *
 *===========================================================================*/

#include "../../inc/ADC.h"
#include <string.h>

#define ADC_CNANNEL_MASK        0x0FFF0000 //Mask: 12bit per channel
#define ADC_RESULT_MASK         0x00000FFF //Mask: 12bit per result
#define ADC_REGRES_MASK         0x0FFF0FFF //Full mask

static __io_reg *regADCCfg;
static __io_reg *regADCChSel;
static __io_reg *regADCStatus;
static __io_reg *regADCResult;

//ADC configuration
static ADCConfig cfgADCx;

//Last ADC sample for each channel
static u32 adcChannelValue[ADC_CHNL_COUNT];


/* */
typedef struct tag_ADCClkParams
{
				//Input parameters
		u32 adcClk;
		u32 clkSrc;

		//Output parameters
		u8 divc;
		u8 c1sel;
} ADCClkParams;



/* */
static MCRESULT TuneADCClkParams(ADCClkParams *p)
{
        u32 clk;

        if (!p)
                return MCR_SETUPCLK_FAIL;

        switch(p->clkSrc)
        {
                case CLKSRC_HCLK: clk = GetHCLK(); break;
                case CLKSRC_LSI:  clk = FREQ_LSI; break;
                case CLKSRC_LSE:  clk = FREQ_LSE; break;
                case CLKSRC_HSI:  clk = FREQ_HSI; break;
                default:
                        return MCR_SETUPCLK_FAIL;
        }

        p->divc = 0;
        while(p->divc <= 15)
        {
                u32 clki = clk / (1 << p->divc);

                if (clki <= p->adcClk &&
                    clki <= ADC_MAX_CLOCK)
                        return MCR_OK;

                p->divc++;
        }

        return MCR_SETUPCLK_FAIL;
}


#if (TARGET_MCU == MCU_1986BE9x)

//1986BE9x:
//ADC Vref+, Vref- : PD[0], PD[1]
#define ADC_VREF_PMASK          (PN0|PN1)
//ADC has 16 channels
#define ADC_CHS_MASK            0xFFFF
//ADC channel #0 at PD[0]
#define ADC_CHS0_PNUM           0

#elif (TARGET_MCU == MCU_1986BE1x)

//1986BE1x:
//ADC Vref+, Vref- : PD[7], PD[8]
#define ADC_VREF_PMASK          (PN7|PN8)
//ADC has 8 channels
#define ADC_CHS_MASK            0x00FF
//ADC channel #0 at PD[7]
#define ADC_CHS0_PNUM           7

#elif (TARGET_MCU == MCU_1901BC1x)

//1986BC1x:
//ADC Vref+, Vref- : PD[0], PD[1]
#define ADC_VREF_PMASK          (PN0|PN1)
//ADC has 16 channels
#define ADC_CHS_MASK            0xFFFF
//ADC channel #0 at PD[0]
#define ADC_CHS0_PNUM           0

#endif


static MCRESULT ADCMapPortPins(ADCConfig *cfg)
{
        switch(cfg->adc)
        {
                case ADC_N1:
                case ADC_N2:
                {
			//1986BEx ADC on PORTD
                        //Whether Vref used -
                        //initialize pins as analog inputs
                        //(ADC_VREF+, ADC_VREF-)
                        if (cfg->vRefSrc == ADC_VREFEXT)
                        {
				InitializePORTEx(PORT_D, ADC_VREF_PMASK, 0,
					PN_ANALOG   | PN_INPUT |
					PN_NOPULLUP | PN_NOPULLDOWN |
					PN_NOSHMIT  | PN_CTRLDRIVER |
                                        PN_NOINFILTER);

                                //Remove Vref+/- pins from channel mask
                                cfg->chMask &= ~ADC_VREF_PMASK;
                        }

			InitializePORTEx(PORT_D, (cfg->chMask & ADC_CHS_MASK) << ADC_CHS0_PNUM, 0,
				PN_ANALOG | PN_INPUT |
				PN_NOPULLUP | PN_NOPULLDOWN |
				PN_NOSHMIT  | PN_CTRLDRIVER |
                                PN_NOINFILTER);
                        break;
                }
        }

        return MCR_OK;
}



/* */
MCRESULT InitializeADC(const ADCConfig *cfg)
{
	ADCConfig *destCfg;
        ADCClkParams p;

	memset(adcChannelValue, 0, ADC_CHNL_COUNT * sizeof(u32));

        if (!cfg)
                return MCR_INVALID_CFG;		

        //Select registers
        switch(cfg->adc)
        {
                case ADC_N1:
                {
                        regADCCfg    = (__io_reg *)&ADC->ADC1_CFG;
                        regADCChSel  = (__io_reg *)&ADC->ADC1_CHSEL;
                        regADCStatus = (__io_reg *)&ADC->ADC1_STATUS;
                        regADCResult = (__io_reg *)&ADC->ADC1_RESULT;
                        break;
                }

                case ADC_N2:
                {
                        regADCCfg    = (__io_reg *)&ADC->ADC2_CFG;
                        regADCChSel  = (__io_reg *)&ADC->ADC2_CHSEL;
                        regADCStatus = (__io_reg *)&ADC->ADC2_STATUS;
                        regADCResult = (__io_reg *)&ADC->ADC2_RESULT;
                        break;
                }

                default:
                        return MCR_INVALID_PORT;
        }

	destCfg = &cfgADCx;

	//Copy configuration
	memcpy(destCfg, cfg, sizeof(ADCConfig));

        //Check requested clock
	if (destCfg->adcClk > ADC_MAX_CLOCK)
	    destCfg->adcClk = ADC_MAX_CLOCK;

	p.adcClk = destCfg->adcClk;
	p.clkSrc = destCfg->clkSrc;
        //Tune clock parameters
        if (TuneADCClkParams(&p) != MCR_OK)
                return MCR_SETUPCLK_FAIL;

        //Disable interrupt from ADC
        IRQDisable(IRQ_ADC);

        //Cfg_REG_ADON[0] = 0 - turn of ADC
        *regADCCfg &= ~PN0;

        //Map port pins
	if (ADCMapPortPins(destCfg) != MCR_OK)
                return MCR_INVALID_MAPPING;        

        //Enable ADC clock
        ClkEnable(CLK_ADC);

        //Set Cfg_REG_DIVCLK [15..12]
        //ADC_CLK = clk / (1 << Cfg_REG_DIVCLK); ADC_CLK <= 24MHz
        *regADCCfg &= ~(0x0F << 12);
        *regADCCfg |= (p.divc & 0x0F) << 12;

        //Cfg_REG_CHS [8..4] - selected channel
        //ADC->ADC1_CFG |= 4 << 4;

        //Configure Vref
	switch(destCfg->vRefSrc)
        {
                //Internal Vref
                case ADC_VREFINT:
                {
                        *regADCCfg |= PN17;   //TS_EN [17] = 1 (use temp. sensor)
                        *regADCCfg &= ~PN11;  //Cfg_M_REF [11] = 0

                        //Next bits only at ADC2_CFG
			switch(destCfg->adc)
                        {
                                case ADC_N1:
				ADC->ADC2_CFG |= PN17;  //ADC1_OP [17] = 1 - use TS
				break;

                                case ADC_N2:
				ADC->ADC2_CFG |= PN18;  //ADC2_OP [18] = 1 - use TS
				break;
                        }

                        break;
                }

		//External Vref
                case ADC_VREFEXT:
                {
                        *regADCCfg &= ~PN17; //TS_EN [17] = 0,
                        *regADCCfg |= PN11;  //Cfg_M_REF [11] = 1 (use PD0, PD1)
                        break;
                }
        }


        //Reset Cfg_REG_RNGC [10], Cfg_REG_CHCH [9], Cfg_REG_SAMPLE [3]
        *regADCCfg &=~(PN10|PN9|PN3);

        //Next bits only at ADC1_CFG
        //Reset SEL_VREF [20], SEL_TS [19], TS_BUF_EN [18]
        ADC->ADC1_CFG &= ~(PN20|PN19|PN18);

        //Synchronize work
        //Cfg_Sync_Conver [16]
	if (destCfg->sync)
                ADC->ADC1_CFG |= PN16;
        else
                ADC->ADC1_CFG &= ~PN16;

        //Next bits only at ADC1_CFG
        //Set Delay_ADC[31..28]
        ADC->ADC1_CFG &= ~(0x0F << 28);
	ADC->ADC1_CFG |= (destCfg->delayADC & 0x0F) << 28;

        //Set TR [24..21] - trim Vref
        ADC->ADC1_CFG &= ~(0x0F << 21);
        ADC->ADC1_CFG |= (0x02 << 21);

        //Set selected channels
	*regADCChSel |= destCfg->chMask;

        //Set Cfg_REG_CHCH [9] = 1, Cfg_REG_SAMPLE [3] = 1
        *regADCCfg |= PN9 | PN3;


        //Set Delay_Go [27..25]
        *regADCCfg &= ~(0x07 << 25);
	*regADCCfg |= (destCfg->delayGo & 0x07) << 25;


        //Setup clocks
        //ADC_C2_SEL[5..4]: 0 - LSE; 1-LSI; 2-ADC_C1; 3-HSI_C1
        //Reset ADC_C2_SEL[5..4]
        RST_CLK->ADC_MCO_CLOCK &= ~(3 << 4);

        //ADC_C3_SEL[11..8]: ADC_C3 = ADC_C2 / (ADC_C3_SEL + 1)
        //Reset ADC_C3_SEL[11..8]
        RST_CLK->ADC_MCO_CLOCK &= ~(0x0F << 8);


	if (destCfg->clkSrc != CLKSRC_HCLK &&
	    !TurnClkSrc(destCfg->clkSrc, 1))
                return MCR_SETUPCLK_FAIL;

	switch(destCfg->clkSrc)
        {
                case CLKSRC_LSE:
                {
                        //0 - select LSE
                        RST_CLK->ADC_MCO_CLOCK &= ~(3 << 4);
                        break;
                }

                case CLKSRC_LSI:
                {
                        //1 - select LSI
                        RST_CLK->ADC_MCO_CLOCK |= 1 << 4;
                        break;
                }

                case CLKSRC_HSI:
                {
                        //HSI_SEL[7..4]: HSI_C1 = HSI / (HSI_SEL + 1)
                        //3 - select HSI_C1
                        RST_CLK->ADC_MCO_CLOCK |= 3 << 4;
                        break;
                }

                /* case ADC_C1 is not implemented */
        }

	if (destCfg->clkSrc == CLKSRC_HCLK)
        {
                //ADC_CLK = HCLK
		//ADC_CLK_EN[13]: 0 - disable ADC_C3
		//Cfg_REG_CLKS[2]: 0 - HCLK; 1 - ADC_C3
		RST_CLK->ADC_MCO_CLOCK &= ~PN13;
		*regADCCfg &= ~PN2;
        }
        else
        {
                //ADC_CLK = ADC_C3
		//ADC_C3_SEL [11..8]: ADC_C3 = ADC_C2 / (ADC_C3_SEL + 1)
		//RST_CLK->ADC_MCO_CLOCK |= (0 << 8);

		//ADC_CLK_EN[13]: 1 - enable ADC_C3
		//Cfg_REG_CLKS[2]: 1 - ADC_C3; 0 - HCLK;
		RST_CLK->ADC_MCO_CLOCK |= PN13;
		*regADCCfg |= PN2;
        }

	//Whether handler is defined: enable interrupts from ADC
	//ECOIF_IE[4] = 1/0 - interrupt enable/disable
	//Enable/disable interrupt at CPU core
	if (destCfg->result)
        {                
                *regADCStatus |= PN4;
                IRQEnable(IRQ_ADC);
        }
        else
        {				
                *regADCStatus &= ~PN4;
                IRQDisable(IRQ_ADC);
        }

        return MCR_OK;
}



/* */
void TurnADC(ADCBLOCK adc, u8 turn)
{
        __io_reg *regCfg;

        switch(adc)
        {
                case ADC_N1: regCfg = (__io_reg *)&ADC->ADC1_CFG; break;
                case ADC_N2: regCfg = (__io_reg *)&ADC->ADC2_CFG; break;
                default: return;
        }

        if (turn)
        {
                *regCfg |= PN0; //Cfg_REG_ADON[0]
                //*regCfg |= PN1; //Cfg_REG_GO[1]
        }
        else
                *regCfg &= ~PN0;
}



/* */
void ADC_IRQHandler(void)
{
        ADCResult r;
        u32 adcr;

        //Was:
        //*((u32*)&r) = (*regADCResult) & ADC_REGRES_MASK;

        //Now:
        adcr = (*regADCResult);
        r.channel = (adcr >> 16) & 0xFFF;
        r.value   = adcr & 0xFFF;

        //Reset Flg_REG_OVERWRITE[0]: write 0
        //if (*regADCStatus & PN0)
        //    *regADCStatus &= ~PN0;

        //Set Cfg_REG_GO[1] = 1
        //not need: Cfg_REG_SAMPLE[3] is set to 1
        // *regADCCfg |= PN1;

        //Set ECOIF_IE[4] - cancel interrupt
         *regADCStatus |= PN4;

		//ADC sample handler is defined
		if (cfgADCx.result)
		{
			 //Save sample value
			 adcChannelValue[r.channel] = r.value;

			 //Call handler
			 cfgADCx.result(&r);
		}
}



/*  */
u32 ADCReadAny(ADCBLOCK adc, u32 *chNumber)
{
        u32 adcr;

        //__io_reg *regCfg;
        __io_reg *regStatus, *regResult;

        //Select register
        if (adc == ADC_N1)
        {
                //regCfg    = &ADC->ADC1_CFG;
                regStatus = (__io_reg *)&ADC->ADC1_STATUS;
                regResult = (__io_reg *)&ADC->ADC1_RESULT;
        }
        else
        {
                //regCfg    = &ADC->ADC2_CFG;
                regStatus = (__io_reg *)&ADC->ADC2_STATUS;
                regResult = (__io_reg *)&ADC->ADC2_RESULT;
        }

        //Cfg_REG_GO [1] = 1  (start convertion)
        //not need: Cfg_REG_SAMPLE[3] is set to 1
        //*regCfg |= PN1;

        //Wait while convertion in progress:
        //Flg_REG_EOCIF[2] == 0
        while((*regStatus & PN2) == 0);

        //Was:
        //*((u32*)&r) = (*regResult) & ADC_REGRES_MASK;

        //Now:
        adcr = (*regResult);        

        //Reset Flg_REG_OVERWRITE[0]: write 0
        //if (*regStatus & PN0)
        //    *regStatus &= ~PN0;

	*chNumber = (adcr >> 16) & 0xFFF;
	return (u32)(adcr & 0xFFF);
}



/* */
u32 ADCReadSync(ADCBLOCK adc, u32 chNumber)
{
	u32 adcr;
        u32 chCur;

        //__io_reg *regCfg;
        __io_reg *regStatus, *regResult;

        //Select register
        if (adc == ADC_N1)
        {
                //regCfg    = &ADC->ADC1_CFG;
                regStatus = (__io_reg *)&ADC->ADC1_STATUS;
                regResult = (__io_reg *)&ADC->ADC1_RESULT;
        }
        else
        {
                //regCfg    = &ADC->ADC2_CFG;
                regStatus = (__io_reg *)&ADC->ADC2_STATUS;
                regResult = (__io_reg *)&ADC->ADC2_RESULT;
        }        

	do
        {
                //Cfg_REG_GO[1] = 1  (start convertion)
                //not need: Cfg_REG_SAMPLE[3] is set to 1
                //*regCfg |= PN1;

                //Wait while convertion in progress:
                //Flg_REG_EOCIF[2] == 0
                while((*regStatus & PN2) == 0);

                //Was:
                //*((u32*)&r) = (*regResult) & ADC_REGRES_MASK;

                //Now:
                adcr  = (*regResult);
                chCur = (adcr >> 16) & 0xFFF;

                //Reset Flg_REG_OVERWRITE[0]: write 0
                //if (*regStatus & PN0)
                //    *regStatus &= ~PN0;

	} while(chCur != chNumber);

	return (u32)(adcr & 0xFFF);
}



/* */
u32 ADCRead(ADCBLOCK adc, u32 chNumber)
{
	if (cfgADCx.result)
		return adcChannelValue[chNumber];
	else
		return ADCReadSync(adc, chNumber);
}



/* */
float ADCToVoltage(u32 sample)
{
	return (float)(ADC_REF_VOLTAGE * (float)sample / (float)ADC_MAXVALUE);
}



/* */
u32 ADCToSample(float voltage)
{
        if (voltage < 0)
                voltage = 0;
        else if (voltage > ADC_REF_VOLTAGE)
                voltage = ADC_REF_VOLTAGE;

        return (u32)(voltage * (float)ADC_MAXVALUE / ADC_REF_VOLTAGE);
}

/*===========================================================================
 * End of file 1986BE9x_ADC.c
 *===========================================================================*/

