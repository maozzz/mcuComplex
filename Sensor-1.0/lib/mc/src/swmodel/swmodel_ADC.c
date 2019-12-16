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
 *  File swmodel_ADC.c: ADC functions.
 *
 *===========================================================================*/

#include "../../inc/ADC.h"
#include <string.h>

//ADC noise amplitude
#define SWMODEL_ADC_NOISE		24

//Input adc value (channel 0)
u16 swmodel_adcch0_value        = ADC_HALFVALUE;

//
static u16 isADCTurnON          = 0;
static u16 isADCInterruptEnable = 0;

//ADC configuration
static ADCConfig cfgADCx;

//ADC samples for each channel
static u32 adcChannelValue[ADC_CHNL_COUNT];

//Function to generate noise
float randPM(void);

/* */
MCRESULT InitializeADC(const ADCConfig *cfg)
{
	//Copy configuration
	memcpy(&cfgADCx, cfg, sizeof(ADCConfig));	

	memset(adcChannelValue, 0, ADC_CHNL_COUNT * sizeof(u32));

	isADCInterruptEnable = (cfg->result) ? 1 : 0;

	return MCR_OK;
}
 


/* */
void TurnADC(ADCBLOCK adc, u8 turn)
{
	isADCTurnON = (turn & 1);
}



/* Returns current ADC sample + noise */
static u16 swmodel_getadcsample()
{	
	return (u16)((s16)swmodel_adcch0_value +
				(s16)(SWMODEL_ADC_NOISE * randPM() - (SWMODEL_ADC_NOISE >> 1))) & 0xFFF;
}



/* */
void ADC_IRQHandler(void)
{
	ADCResult r;

	if (!isADCInterruptEnable || !isADCTurnON)
		return;

	r.channel = 0;
	r.value   = swmodel_getadcsample();

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
	if (!isADCTurnON)
		return 0;

	*chNumber = 0;
	return adcChannelValue[0];
}



/* */
u32 ADCReadSync(ADCBLOCK adc, u32 chNumber)
{	
	if (!isADCTurnON)
		return 0;

	adcChannelValue[0] = swmodel_getadcsample();
	return adcChannelValue[chNumber];
}



/* */
u32 ADCRead(ADCBLOCK adc, u32 chNumber)
{
	if (!isADCTurnON)
		return 0;

	if (cfgADCx.result)
		return adcChannelValue[chNumber];
	else
		return ADCReadSync(adc, chNumber);
}

/*===========================================================================
 * End of file swmodel_ADC.c
 *===========================================================================*/
