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
 *  File SSPPIO.h: SSPPIO interface definitions and functions prototypes.
 *                 Software implementation of SSP controller.
 *                 Use for very low speed only (up to 200bps).
 *
 *===========================================================================*/

#ifndef __SSPPIO_H
#define __SSPPIO_H

#include "types.h"
#include "SSP.h"

/* Initialize SSPPIO */
MCRESULT InitializeSSPPIO(const SSPConfig *cfg, u32 timerSpeed);

/* Turn ON/OFF selected SSPPIO */
void TurnSSPPIO(SSPBLOCK ssp, u8 turn);

/* Set speed */
MCRESULT SSPPIOSetSpeed(SSPBLOCK ssp, u32 speed);

/* Indicating that output fifo is full */
u8 IsSSPPIOOutFifoFull(SSPBLOCK ssp);

/* Read data from SSP */
s32 SSPPIORead(SSPBLOCK ssp, void *dest, s32 count);

/* Write data to SSP */
s32 SSPPIOWrite(SSPBLOCK ssp, const void *src, s32 count);

/* SSP line signal handler. Call this function with frequency "timerSpeed" Hz */
void SSPPIOLineHandler(SSPBLOCK ssp);

#endif /* __SSPPIO_H */

/*===========================================================================
 * End of file SSPPIO.h
 *===========================================================================*/



