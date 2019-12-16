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
 *  File ACodec.h: Audio codec controller definitions 
 *                 and function prototypes.
 *
 *===========================================================================*/

#ifndef __ACODEC_H
#define __ACODEC_H

#include "types.h"
#include "CPU.h"

//#define __NO_TEST_COUNTERS

//Audo codec number
#define ACODEC_N1	    0

//Audio codec inputs
#define ACODEC_IN_IN1	    0 //Input #1
#define ACODEC_IN_MIC1	    1 //Microphone input, internal offset
#define ACODEC_IN_MIC2	    2 //Microphone input, external offset
#define ACODEC_IN_IN2	    3 //Input #2

//Audio codec outputs
#define ACODEC_OUT_OUT1	    0 //Output #1

//Audio codec options (ACodecConfig::opts)
//#define ACODEC_OPTS_xxx

//DEfault options
#define ACODEC_OPTS_DEFAULT	    (0)


typedef u8		    ACODECBLOCK;


/* Represents BSP configuration */
typedef struct tag_ACodecConfig
{
	//Block number
	ACODECBLOCK ac;

	//Clock source (see CLKSRC_x)
	u8 clkSrc;

    //Selected input (see ACODEC_IN_x)
    u8 input;

    //Selected output (see ACODEC_OUT_x)
    u8 output;

	//Sample rate, Hz
    u32 rate;

	//Options (see ACODEC_OPTS_x)
	u32 opts;

	//ADC received sample
	void (*inSample)(s16 sample);

	//DAC requested sample
	s16 (*outSample)();
} ACodecConfig;


/* Initialize audio codec controller */
MCRESULT InitializeACodec(const ACodecConfig *cfg);

/* Setup Audio codec sample rate */
MCRESULT SetACodecRate(ACODECBLOCK ac, u32 rate, u32 clkSrc);

/* Get audio codec sample rate */
u32 GetACodecRate(ACODECBLOCK ac);


/* Turn audio codec */
void TurnACodec(ACODECBLOCK ac, u8 turn);

/* Turn audio codec loopback */
void TurnACodecLB(ACODECBLOCK ac, u8 on);

/* Turn audio codec BIAS scheme */
void TurnACodecBIAS(ACODECBLOCK ac, u8 on);


/* Select ADC input.
   in is one of ACODEC_IN_x values */
void ACodecSetADCInput(ACODECBLOCK ac, u32 in);

/* Select DAC output.
   out is one of ACODEC_OUT_x values */
void ACodecSetDACOutput(ACODECBLOCK ac, u32 out);


/* Set analog preamlifier level */
void ACodecSetPreAmp(ACODECBLOCK ac, s32 gaindB);

/* Get analog preamlifier level (dB) */
s32 ACodecGetPreAmp(ACODECBLOCK ac);



/* Set digital amplifier level for ADC channel */
void ACodecSetADCGain(ACODECBLOCK ac, s32 gaindB);

/* Set digital amplifier level for DAC channel */
void ACodecSetDACGain(ACODECBLOCK ac, s32 gaindB);


/* Get digital amplifier level for ADC channel (dB) */
s32 ACodecGetADCGain(ACODECBLOCK ac);

/* Get digital amplifier level for DAC channel (dB) */
s32 ACodecGetDACGain(ACODECBLOCK ac);


/* Set digital amplifier relative level for ADC channel */
void ACodecSetADCGainChange(ACODECBLOCK ac, s32 gaindB);

/* Set digital amplifier relative level for DAC channel */
void ACodecSetDACGainChange(ACODECBLOCK ac, s32 gaindB);

#endif /* !__ACODEC_H */

/*===========================================================================
 * End of file ACodec.h
 *===========================================================================*/
