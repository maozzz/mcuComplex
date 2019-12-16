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
 *  File DSP.h: DSP core functions.
 *
 *===========================================================================*/

#ifndef __DSP_H
#define __DSP_H

#include "CPU.h"

/* Setup DSP core speed */
MCRESULT SetDSPCLK(u32 hclk, u32 clkSrc);

/* Gets current DSP core speed, Hz */
u32 GetDSPCLK(void);

/* Load DSP data/code to DSP data/code memory */
void DSPMemWrite(const u16 *src, u32 destArmAddr, s32 wSize);

/* Clear DSP data memory */
void DSPMemDataClear(void);

/* Set/clear reset signal for DSP peripherals */
void DSPPerReset(u8 on);

/* Set/clear reset signal for DSP memory */
void DSPMemReset(u8 on);

/* Set/clear reset signal for DSP core */
void DSPCoreReset(u8 on);

/* Set/clear reset signal for DSP subsystem */
void DSPBlockReset(u8 on);

#endif /* !__DSP_H */

/*===========================================================================
 * End of file DSP.h
 *===========================================================================*/

