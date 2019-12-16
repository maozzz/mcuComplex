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
 *  File swmodel_SSPPIO.c. SSPPIO functions.
 *
 *===========================================================================*/

#include "../../inc/SSPPIO.h"

/* */
MCRESULT InitializeSSPPIO(const SSPConfig *cfg, u32 timerSpeed)
{
	return MCR_OK;
}



/* Turn ON/OFF selected SSPPIO */
void TurnSSPPIO(SSPBLOCK ssp, u8 turn)
{
}



/* */
MCRESULT SSPPIOSetSpeed(SSPBLOCK ssp, u32 speed)
{
	return MCR_OK;
}



/* Indicating that output fifo is full */
u8 IsSSPPIOOutFifoFull(SSPBLOCK ssp)
{	
	return 0;
}



/* Write data to SSPPIO */
s32 SSPPIOWrite(SSPBLOCK ssp, const void *src, s32 count)
{
	return 0;
}



/* Read data from SSPPIO */
s32 SSPPIORead(SSPBLOCK ssp, void *dest, s32 count)
{
	return 0;
}



/* SSP line signal handler. Call this function with frequency "timerSpeed" Hz */
void SSPPIOLineHandler(SSPBLOCK ssp)
{	
}

/*===========================================================================
 * End of file swmodel_SSPPIO.c
 *===========================================================================*/ 
