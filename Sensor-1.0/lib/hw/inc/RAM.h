/*===========================================================================
 *
 *  Hardware library
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2014
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File RAM.h: RAM macros and functions prototypes.
 *
 *===========================================================================*/

#ifndef __RAM_H
#define __RAM_H

//Available RAM types
//1645RU4x. Size 1Mx16.
#define RAM_1645RU4x            164540

//1645RU4x models:
#define RAMM_1645RU4AU          164541
#define RAMM_1645RU4BU          164542

#include "../../mc/inc/CPU.h"
#include <targetram.h>

#if (TARGET_RAM == RAM_1645RU4x)

#include "RAM_1645RU4x_defs.h"
#define RAM_SIZE         RAM_1645RUx_SIZE
#define RAM_CELLSIZE     RAM_1645RUx_CELLSIZE

/*
#elif (TARGET_RAM == RAM_x)

//Other RAM type definitions
#include "RAM_x_defs.h"
#define RAM_SIZE         RAM_x_SIZE
#define RAM_CELLSIZE     RAM_x_CELLSIZE
*/

#else

#error "Unknown RAM type. Check 'TARGET_RAM' at 'targetram.h'"

#endif

#define RAM_SECCNT       1


/* Initialize RAM */
void InitializeRAM(u32 hclk);

/* Setup hclk */
extern void (*RAMSetHCLK)(u32 hclk);

#if (TARGET_RAM == RAM_1645RU4x)
/* No actions for 1645x */
#define RAMReset(x)
#else
/* Reset RAM */
extern void (*RAMReset)(u32 addr);
#endif

/* Clear RAM */
extern s32  (*RAMClear)(u32 addr);

/* Write RAM cell */
extern s32  (*RAMWriteCell)(u32 addr, u32 value);

/* Read RAM cell */
extern u32  (*RAMReadCell)(u32 addr);

/* Write buffer to RAM */
extern s32  (*RAMWrite)(u32 addr, u8 *data, s32 length);

/* Read buffer from RAM */
extern s32  (*RAMRead)(u32 addr, u8 *data, s32 length);

#endif  /* __RAM_H */

/*===========================================================================
 * End of file RAM.h
 *===========================================================================*/

