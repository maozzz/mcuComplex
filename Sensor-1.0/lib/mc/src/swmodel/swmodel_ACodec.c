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
 *  File swmodel_ACodec.c: Audio codec functions.
 *
 *===========================================================================*/

#include "../../inc/ACodec.h"
#include <string.h>

static ACodecConfig cfgACodec;

u32 acIntCounter;

u32 FREQ_HSE_VAL = 8000000;
u32 FREQ_HSI_VAL = 8000000;


/* Initialize audio codec controller */
MCRESULT InitializeACodec(const ACodecConfig *cfg)
{
	//Save configuration
	memcpy(&cfgACodec, cfg, sizeof(ACodecConfig));
	return MCR_OK;
}



/* Setup audio codec sample rate */
MCRESULT SetACodecRate(ACODECBLOCK ac, u32 rate, u32 clkSrc)
{
    u32 smpRate, diff, srcClk, auc_sel1, auc_sel3;

    //Select clock source
    //AUC_SEL1[17..16] = [0, 2] -> AUC_C1 = [HSI, HSE];
    switch (clkSrc)
    {
	    case CLKSRC_HSE: //HSE
	    {
		    auc_sel1 = 2;
		    srcClk   = FREQ_HSE_VAL;
		    TurnClkSrc(CLKSRC_HSE, 1);
		    break;
	    }

	    default: //HSI as default
	    {
		    auc_sel1 = 0;
		    srcClk   = FREQ_HSI_VAL;
		    TurnClkSrc(CLKSRC_HSI, 1);
		    break;
	    }
    }

    //smpRate = srcClk /  (2 ^ (AUC_SEL3 - 7)) / 256 =>
    //smpRate = srcClk / ((2 ^ (AUC_SEL3 - 7)) * (2 ^ 8)) =>
    //smpRate = srcClk /  (2 ^ (AUC_SEL3 + 1)) =>
    //smpRate = srcClk >> (AUC_SEL3 + 1), AUC_SEL3 = [7..15]
    auc_sel3 = 7;
    diff = rate;
    while(auc_sel3 <= 15)
    {
	smpRate = srcClk >> (auc_sel3 + 1);

	if (smpRate <= rate)
	{
	    if (diff < (rate - smpRate))
		auc_sel3--;
	    break;
	}

	diff = smpRate - rate;
	auc_sel3++;
    }    

    //Set clock register
    //ADC_CLK = AUC_C3 = AUC_C2 / (2 ^ (AUC_SEL3 - 7))
    //AUC_CLK_EN[31]   = 1 - ADC_CLK = AUC_C3
    //AUC_SEL3[27..24] = [7..15]
    //AUC_SEL2[21..20] = 0 - AUC_C2 = AUC_C1
    //AUC_SEL1[17..16] = [0, 2] -> AUC_C1 = [HSI, HSE];
    RST_CLK->ADC_MCO_CLOCK &= ~(PN31 | (15 << 24) | (3 << 20) | (3 << 16));
    RST_CLK->ADC_MCO_CLOCK |= PN31 | (auc_sel3 << 24) | (0 << 20) | (auc_sel1 << 16);

    return MCR_OK;
}



/* Get audio codec sample rate */
u32 GetACodecRate(ACODECBLOCK ac)
{
    u32 srcClk, auc_sel1, auc_sel2, auc_sel3;

    //AUC_SEL2[21..20] = 0 - AUC_C2 = AUC_C1
    auc_sel2 = (RST_CLK->ADC_MCO_CLOCK >> 20) & 0x03;

    if ((RST_CLK->ADC_MCO_CLOCK & PN31) == 0 ||	auc_sel2 != 0)
	return 0;

    //AUC_SEL1[17..16] = [0, 2] -> AUC_C1 = [HSI, HSE];
    auc_sel1 = (RST_CLK->ADC_MCO_CLOCK >> 16) & 0x03;

    //AUC_SEL3[27..24] = [7..15]
    auc_sel3 = (RST_CLK->ADC_MCO_CLOCK >> 24) & 0x0F;

    srcClk = 0;
    switch(auc_sel1)
    {
	case 0: srcClk = FREQ_HSI_VAL;        break;
	case 1: srcClk = (FREQ_HSI_VAL >> 1); break;
	case 2: srcClk = FREQ_HSE_VAL;        break;
	case 3: srcClk = (FREQ_HSE_VAL >> 1); break;
    }

    return (u32)(srcClk >> (auc_sel3 + 1));
}



/* Turn audio codec */
void TurnACodec(ACODECBLOCK ac, u8 turn)
{

}



/* Turn audio codec loopback */
void TurnACodecLB(ACODECBLOCK ac, u8 on)
{

}



/* Turn audio codec BIAS scheme */
void TurnACodecBIAS(ACODECBLOCK ac, u8 on)
{

}



/* Select ADC input.
   in is one of ACODEC_IN_x values */
void ACodecSetADCInput(ACODECBLOCK ac, u32 in)
{

}



/* Select DAC output.
   out is one of ACODEC_OUT_x values */
void ACodecSetDACOutput(ACODECBLOCK ac, u32 out)
{

}



/* Set analog preamlifier level */
void ACodecSetPreAmp(ACODECBLOCK ac, s32 gaindB)
{

}



/* Get analog preamlifier level */
s32 ACodecGetPreAmp(ACODECBLOCK ac)
{
    return 0;
}



/* Set digital amplifier level for ADC channel */
void ACodecSetADCGain(ACODECBLOCK ac, s32 gaindB)
{

}



/* Set digital amplifier level for DAC channel */
void ACodecSetDACGain(ACODECBLOCK ac, s32 gaindB)
{

}



/* Get digital amplifier level for ADC channel */
s32 ACodecGetADCGain(ACODECBLOCK ac)
{
    return 0;
}



/* Get digital amplifier level for DAC channel */
s32 ACodecGetDACGain(ACODECBLOCK ac)
{
    return 0;
}



/* Set digital amplifier relative level for ADC channel */
void ACodecSetADCGainChange(ACODECBLOCK ac, s32 gaindB)
{

}



/* Set digital amplifier relative level for DAC channel */
void ACodecSetDACGainChange(ACODECBLOCK ac, s32 gaindB)
{

}



/* Audio codec IRQ handler */
void DSP_ACODEC_IRQHandler()
{
	u32 flag;
	s16 smp;

	//Read IRQ flags
	flag = DSP_ACODEC->IRQFLAG;

	//Check IRQ flags
	//ADCNS[2]: ADC FIFO is not empty
	if (flag & PN2)
	{
		//Read sample from ADC FIFO
		smp = (s16)DSP_ACODEC->ADCREG;

		//Call user-defined handler
		if (cfgACodec.inSample)
		    cfgACodec.inSample(smp);
	}

	//DACNS[3]: DAC FIFO is not full
	if (flag & PN3)
	{
		//Call user-defined handler to get sample and write it to DAC FIFO
		DSP_ACODEC->DACREG = cfgACodec.outSample ? cfgACodec.outSample() : 0;
	}

	//ADCVF[1] = 1: ADC FIFO is full
	if (flag & PN1)
	{
		//Reset flag by writing 1
		DSP_ACODEC->IRQFLAG |= PN1;

		//To do handler here (if need)
	}

	//DACVF[0] = 1: DAC FIFO is empty
	if (flag & PN0)
	{
		//Reset flag by writing 1
		DSP_ACODEC->IRQFLAG |= PN0;
	}

	//OVERCUR[4] = 1: short circuit on DAC input
	if (flag & PN4)
	{
	}

	acIntCounter++;
}

/*===========================================================================
 * End of file swmodel_ACodec.c
 *===========================================================================*/
