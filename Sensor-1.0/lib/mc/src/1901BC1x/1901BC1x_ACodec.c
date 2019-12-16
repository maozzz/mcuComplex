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
 *  File 1901BC1x_ACodec.c: Audio codec functions.
 *
 *===========================================================================*/

#include "../../inc/ACodec.h"
#include "../../inc/CPU.h"
#include <string.h>

#ifdef __USE_ACODEC_TEST_COUNTERS
u32 acADCFIFOFull     = 0;
u32 acDACFIFOEmpty    = 0;
u32 acDACShortCurcuit = 0;
u32 acIntCounter      = 0;

#define update_adc_fifo_overflow_count()	acADCFIFOFull++
#define update_dac_fifo_empty_count()       acDACFIFOEmpty++
#define update_dac_short_curcuit_count()    acDACShortCurcuit++
#define update_acodec_int_count()           acIntCounter++

#else

#define update_adc_fifo_overflow_count()
#define update_dac_fifo_empty_count()
#define update_dac_short_curcuit_count()
#define update_acodec_int_count()

#endif /* __NO_TEST_COUNTERS */


static ACodecConfig cfgACodec;

/* Initialize audio codec controller */
MCRESULT InitializeACodec(const ACodecConfig *cfg)
{
	u32 i;

	if (!cfg)
		return MCR_INVALID_CFG;

	//Save configuration
	memcpy(&cfgACodec, cfg, sizeof(ACodecConfig));

	//CDC[4] = 1 - enable clock for audio codec
	DSP_CLK->CLKMD |= PN4;

	//Turn OFF reset signal for DSP peripherals
	RST_CLK->DSP_CONTROL_STATUS &= ~PN3;

	//Enable clock for audio codec (ARM side)
	ClkEnable(CLK_DSP_ACODEC);
	
	//Turn OFF ACodec
	TurnACodec(ACODEC_N1, 0);

	//Setup Audio codec sample rate
	SetACodecRate(ACODEC_N1, cfg->rate, cfg->clkSrc);

	//Write zeros to initialize DAC FIFO
	i = 0;
	while(i++ < ACODEC_FIFO_TX_LENGTH)
		DSP_ACODEC->DACREG = 0;

	//Enable interrupts
	//OVERCUR[4] = 1 - Short circuit on DAC input
	//DACNSM[3]  = 1 - DAC FIFO is not full (can write sample)
	//ADCNSM[2]  = 1 - ADC FIFO is not empty (can read sample)
	//ADCVF[1]   = 1 - ADC FIFO is full
	//DAOVF[0]   = 1 - DAC FIFO is empty
	DSP_ACODEC->MASKCTL = PN4 | PN3 | PN2 | PN1 | PN0;

	//ADGAIN[5..0], DAGAIN[5..0]:
	//gain   = [ 0x00.. 0x3E], 0x3F - mute
	//gaindB = [-42dB..+20dB]
	//gaindB = gain - 42
	//gain   = gaindB - (-42)

	//AINSEL[9..8] = [0..3]:
	//0 - INP1/INM1;
	//1 - MICIN, internal offset 1.5V;
	//2 - MICIN, external offset;
	//3 - INP2/INM2.

	//DAC channel configuration
	//DACRES[15]   = 1 - work
	//SIDETONE[14..12] = 7 - no loopback. [0,1..6] -> [-3, -6,..,-21]dB
	//OVECBS[11]   = 0 - no detect short circuit on offset scheme
	//OVECBA[10]   = 0 - no detect short circuit on output buffer
	//ODDAC[9]     = 1 - turn on analog part of DAC
	//ODBIAS[8]    = 1 - on BIAS scheme
	//ODAMP[7]     = 1 - on OUTP1/OUTM1 amplifier
	//MUTE1[6]     = 0 - off mute OUTP1/OUTM1 outputs
	//DAGAIN[5..0] = <gain value> (see above).
	DSP_ACODEC->DACCTL = PN15 | (7 << 12) | PN9 | PN8 |
				PN7  | ((0 - ACODEC_GAIN_MIN_DB) & 0x3F);

	//ADC channel configuration
	//ICONT[10]    = 1 - turn on analog part of ADC
	//AINSEL[9..8] = <selected input> (see above)
	//INBG[7..6]   = 0 = +0dB preamplifier. [0..3] -> [+0, +6, +12, +24]dB
	//ADGAIN[5..0] = <gain value> (see above).
	DSP_ACODEC->ADCCTL &= ~(3 << 8);
	DSP_ACODEC->ADCCTL = PN10 | ((cfg->input & 3) << 8) |
				(0 << 6) | ((0 - ACODEC_GAIN_MIN_DB) & 0x3F);

	//Enable/disable interrupt from ACODEC
	if (cfg->inSample || cfg->outSample)
	    IRQEnable(IRQ_DSP_ACODEC);
	else
	    IRQDisable(IRQ_DSP_ACODEC);

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
		    srcClk   = FREQ_HSE;
		    TurnClkSrc(CLKSRC_HSE, 1);
		    break;
	    }

	    default: //HSI as default
	    {
		    auc_sel1 = 0;
		    srcClk   = FREQ_HSI;
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

    //AUC_CLK_EN[31]   = 1 - ADC_CLK = AUC_C3
    if ((RST_CLK->ADC_MCO_CLOCK & PN31) == 0 ||	auc_sel2 != 0)
		return 0;

    //AUC_SEL1[17..16] = [0, 2] -> AUC_C1 = [HSI, HSE];
    auc_sel1 = (RST_CLK->ADC_MCO_CLOCK >> 16) & 0x03;

    //AUC_SEL3[27..24] = [7..15]
    auc_sel3 = (RST_CLK->ADC_MCO_CLOCK >> 24) & 0x0F;

    srcClk = 0;
    switch(auc_sel1)
    {
		case 0: srcClk =  FREQ_HSI;       break;
		case 1: srcClk = (FREQ_HSI >> 1); break;
		case 2: srcClk =  FREQ_HSE;       break;
		case 3: srcClk = (FREQ_HSE >> 1); break;
    }

    return (u32)(srcClk >> (auc_sel3 + 1));
}



/* Turn audio codec */
void TurnACodec(ACODECBLOCK ac, u8 turn)
{
	//ACodec control
	//IIREN[2] = 1/0 - enable/disable IIR filter
	//DACEN[1] = 1/0 - enable/disable DAC channel
	//ADCEN[0] = 1/0 - enable/disable ADC channel
	if (turn)
	    DSP_ACODEC->POWCTL = PN1 | PN0;
	else
	    DSP_ACODEC->POWCTL = 0;
}



/* Turn audio codec loopback */
void TurnACodecLB(ACODECBLOCK ac, u8 on)
{
	//SIDETONE[14..12] = 7 - no loopback. [0,1..6] -> [-3, -6,..,-21]dB
	if (on)	
	    DSP_ACODEC->DACCTL &= ~(7 << 12); //loopback: -3dB	
	else	
	    DSP_ACODEC->DACCTL |= (7 << 12); //no loopback	
}



/* Turn audio codec BIAS scheme */
void TurnACodecBIAS(ACODECBLOCK ac, u8 on)
{
    //ODBIAS[8] = 1/0 - on/off BIAS scheme
    if (on)
		DSP_ACODEC->DACCTL |= PN8;
    else
		DSP_ACODEC->DACCTL &= ~PN8;
}



/* Select ADC input.
   in is one of ACODEC_IN_x values */
void ACodecSetADCInput(ACODECBLOCK ac, u32 in)
{
    u32 regval;

    regval = DSP_ACODEC->ADCCTL;

	//AINSEL[9..8]:
	//0 - INP1/INM1,
	//1 - MICIN, internal offset 1.5V,
	//2 - MICIN, external offset,
	//3 - INP2/INM2.
	regval = (regval & ~(3 << 8)) | ((in & 3) << 8);

	DSP_ACODEC->ADCCTL = regval;
}



/* Select DAC output.
   out is one of ACODEC_OUT_x values */
void ACodecSetDACOutput(ACODECBLOCK ac, u32 out)
{
	//Audio codec has one output OUTP1/OUTM1:
	//no actions
}



/* Set analog preamlifier level */
void ACodecSetPreAmp(ACODECBLOCK ac, s32 gaindB)
{
	u32 regval;

	if (gaindB < 3)
	    gaindB = 0; //+0dB
	else if (gaindB < 9)
	    gaindB = 1; //+6dB
	else if (gaindB < 18)
	    gaindB = 2; //+12dB
	else
	    gaindB = 3; //+24dB

	regval = DSP_ACODEC->ADCCTL;

	//INBG[7..6]: [0..3] -> [+0, +6, +12, +24]dB
	regval = (regval & ~(3 << 6)) | (gaindB << 6);

	DSP_ACODEC->ADCCTL = regval;
}



/* Get analog preamlifier level */
s32 ACodecGetPreAmp(ACODECBLOCK ac)
{
    s32 regval;

    regval = DSP_ACODEC->ADCCTL;

    regval = (regval >> 6) & 3;

    if (regval == 3)
        return 24;
    else
        return (regval * 6);
}



/* Set digital amplifier level for ADC channel */
void ACodecSetADCGain(ACODECBLOCK ac, s32 gaindB)
{
	u32 regval;

	//ADGAIN[5..0]:
	//gain   = [ 0x00.. 0x3E], 0x3F - mute
	//gaindB = [-42dB..+20dB]
	//gain   = gaindB - (-42)
	if (gaindB < ACODEC_GAIN_MIN_DB)
	    gaindB = ACODEC_GAIN_MIN_DB;
	else if (gaindB > ACODEC_GAIN_MAX_DB)
	    gaindB = ACODEC_GAIN_MAX_DB;

	regval = DSP_ACODEC->ADCCTL;

	//ADGAIN[5..0] = <gain value> (see above)
	regval = (regval & ~0x3F) | (gaindB - ACODEC_GAIN_MIN_DB);

	DSP_ACODEC->ADCCTL = regval;
}



/* Set digital amplifier absolute level for DAC channel */
void ACodecSetDACGain(ACODECBLOCK ac, s32 gaindB)
{
	u32 regval;

	//DAGAIN[5..0]:
	//gain   = [ 0x00.. 0x3E], 0x3F - mute
	//gaindB = [-42dB..+20dB]
	//gain   = gaindB - (-42)
	if (gaindB < ACODEC_GAIN_MIN_DB)
	    gaindB = ACODEC_GAIN_MIN_DB;
	else if (gaindB > ACODEC_GAIN_MAX_DB)
	    gaindB = ACODEC_GAIN_MAX_DB;

	regval = DSP_ACODEC->DACCTL;

	//DAGAIN[5..0] = <gain value> (see above)
	regval = (regval & ~0x3F) | (gaindB - ACODEC_GAIN_MIN_DB);

	DSP_ACODEC->DACCTL = regval;
}



/* Get digital amplifier level for ADC channel */
s32 ACodecGetADCGain(ACODECBLOCK ac)
{
    s32 regval;

    regval = DSP_ACODEC->ADCCTL;

    regval = (regval & 0x3F) + ACODEC_GAIN_MIN_DB;

    return regval;
}



/* Get digital amplifier level for DAC channel */
s32 ACodecGetDACGain(ACODECBLOCK ac)
{
    s32 regval;

    regval = DSP_ACODEC->DACCTL;

    regval = (regval & 0x3F) + ACODEC_GAIN_MIN_DB;

    return regval;
}



/* Set digital amplifier relative level for ADC channel */
void ACodecSetADCGainChange(ACODECBLOCK ac, s32 gaindB)
{
	u32 regval;

	//Read ADC control register
	regval = DSP_ACODEC->ADCCTL;

	//New gain gain = old gain + change
	gaindB = (s32)(regval & 0x3F) + gaindB + ACODEC_GAIN_MIN_DB;

	//ADGAIN[5..0]:
	//gain   = [ 0x00.. 0x3E] = [0..62], 0x3F - mute
	//gaindB = [-42dB..+20dB]
	//gain   = gaindB - (-42)
	if (gaindB < ACODEC_GAIN_MIN_DB)
		gaindB = ACODEC_GAIN_MIN_DB;
	else if (gaindB > ACODEC_GAIN_MAX_DB)
		gaindB = ACODEC_GAIN_MAX_DB;

	//ADGAIN[5..0] = <gain value> (see above)
	regval = (regval & ~0x3F) | (u32)(gaindB - ACODEC_GAIN_MIN_DB);

	//Write ADC control register
	DSP_ACODEC->ADCCTL = regval;
}



/* Set digital amplifier relative level for DAC channel */
void ACodecSetDACGainChange(ACODECBLOCK ac, s32 gaindB)
{
	u32 regval;

	//Read DAC control register
	regval = DSP_ACODEC->DACCTL;

	//New gain = old gain + change
	gaindB = (s32)(regval & 0x3F) + gaindB + ACODEC_GAIN_MIN_DB;

	//DAGAIN[5..0]:
	//gain   = [ 0x00.. 0x3E] = [0..62], 0x3F - mute
	//gaindB = [-42dB..+20dB]
	//gain   = gaindB - (-42)
	if (gaindB < ACODEC_GAIN_MIN_DB)
		gaindB = ACODEC_GAIN_MIN_DB;
	else if (gaindB > ACODEC_GAIN_MAX_DB)
		gaindB = ACODEC_GAIN_MAX_DB;

	//DAGAIN[5..0] = <gain value> (see above)
	regval = (regval & ~0x3F) | (u32)(gaindB - ACODEC_GAIN_MIN_DB);

	//Write DAC control register
	DSP_ACODEC->DACCTL = regval;
}



/* Audio codec IRQ handler */
void DSP_ACODEC_IRQHandler()
{
	u32 flag;
	s16 smp;

	//Reset interrupt flag
	//DSP_IRQ->IRQ = AIRQ/DIRQ registers.
	//SNR[15]   = 0 - reset interrupt flag,
	//CDIRQ[11] = 1 - reset interrupt from ACodec
	DSP_IRQ->IRQ = PN11;

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
		update_adc_fifo_overflow_count();
	}
	
	//DACVF[0] = 1: DAC FIFO is empty
	if (flag & PN0)
	{
		//Reset flag by writing 1
		DSP_ACODEC->IRQFLAG |= PN0;		
		
		//To do handler here (if need)
		update_dac_fifo_empty_count();
	}	

	//OVERCUR[4] = 1: short circuit on DAC input
	if (flag & PN4)
	{
		//To do handler here (if need)		
		update_dac_short_curcuit_count();
	}	

	update_acodec_int_count();
}

/*===========================================================================
 * End of file 1901BC1x_ACodec.c
 *===========================================================================*/
