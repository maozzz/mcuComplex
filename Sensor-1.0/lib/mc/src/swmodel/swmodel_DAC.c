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
 *  File swmodel_DAC.c: DAC functions.
 *
 *===========================================================================*/

#include "../../inc/DAC.h"

/* Convert signed 16-bit sample to DAC N-bit unsigned vaue */
#define SmpToValue(smp)		((u32)((s16)DAC_HALFVALUE + (smp >> (16 - DAC_VALSIZE))))

//Last DAC value
u16 swmodel_dac0_value = 0;
u16 swmodel_dac1_value = 0;

static u16 isDACTurnON = 0;

/* */
MCRESULT InitializeDAC(const DACConfig *cfg)
{
	return MCR_OK;
}



/* */
u8 IsTurnDAC(DACBLOCK dac)
{
	return isDACTurnON;
}
 


/* */
void TurnDAC(DACBLOCK dac, u8 turn)
{
	isDACTurnON = (turn & 1);
}



/* Write value */
void DACWrite(DACBLOCK dac, u32 value)
{
	if (!isDACTurnON)
		return;

	if (dac & 1)
		swmodel_dac1_value = (u16)value;
	else
		swmodel_dac0_value = (u16)value;
}



/* Write values synchronously */
void DACWriteSync(u32 *values)
{
	if (!isDACTurnON)
		return;

	swmodel_dac0_value = (u16)values[0];
	swmodel_dac1_value = (u16)values[1];
}



/* Write sample */
void DACWriteSmp(DACBLOCK dac, s16 sample)
{
	if (!isDACTurnON)
		return;

	if (dac & 1)
		swmodel_dac1_value = SmpToValue(sample);
	else
		swmodel_dac0_value = SmpToValue(sample);
}



/* Write samples synchronously */
void DACWriteSyncSmp(DACBLOCK dac, s16 *samples)
{
	if (!isDACTurnON)
		return;

	swmodel_dac0_value = SmpToValue(samples[0]);
	swmodel_dac1_value = SmpToValue(samples[1]);
}

/*===========================================================================
 * End of file swmodel_DAC.c
 *===========================================================================*/
