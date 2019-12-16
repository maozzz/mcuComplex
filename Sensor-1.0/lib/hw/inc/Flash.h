/*===========================================================================
 *
 *  Hardware library
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
 *  File Flash.h: Flash memory definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __FLASH_H
#define __FLASH_H

//Available flash memory types:
//1636RR1x. Size: 512Kx8. Sectors: 8x64K
#define FM_1636RR1x     163610

//1636RR2x. Size: 2Mx8. Sectors: 32x64K
#define FM_1636RR2x     163620

//Add other flash memory types here

//Available flash memory models
//1636RR1x models:
#define FMM_1636RR1AU   163611
#define FMM_1636RR1BU   163612

//1636RR2x models:
#define FMM_1636RR2AU   163621
#define FMM_1636RR2BU   163622

//Add other flash memory models here

#include "../../mc/inc/CPU.h"
#include <targetfm.h>

#if (TARGET_FM == FM_1636RR2x)

#include "FM_1636RR2x_defs.h"
#define FM_SIZE         FM_1636RR2x_SIZE
#define FM_CELLSIZE     FM_1636RR2x_CELLSIZE
#define FM_SECCNT       FM_1636RR2x_SECCNT

#elif (TARGET_FM == FM_1636RR1x)

#include "FM_1636RR1x_defs.h"
#define FM_SIZE         FM_1636RR1x_SIZE
#define FM_CELLSIZE     FM_1636RR1x_CELLSIZE
#define FM_SECCNT       FM_1636RR1x_SECCNT

/*
//Add other flash memory definitions
#elif (TARGET_FM == FM_x)

//Other flash type definitions
#include "FM_x_defs.h"
#define FM_SIZE         FM_x_SIZE
#define FM_CELLSIZE     FM_x_CELLSIZE
#define FM_SECCNT       FM_x_SECCNT
*/

#else

#error "Unknown flash memory type. Check 'TARGET_FM' at 'targetfm.h'"

#endif

/* Initialize flash memory */
void InitializeFM(u32 hclk);

/* Setup hclk */
extern void (*FMSetHCLK)(u32 hclk);

/* Reset flash memory */
extern void (*FMReset)(u32 addr);

/* Erase chip */
extern s32  (*FMEraseChip)(u32 addr);

/* Erase flash sector */
extern s32  (*FMEraseSector)(u32 addr);

/* Write flash cell */
extern s32  (*FMWriteCell)(u32 addr, u32 value);

/* Read flash cell */
extern u32  (*FMReadCell)(u32 addr);

/* Write buffer to flash */
extern s32  (*FMWrite)(u32 addr, u8 *data, s32 length);

/* Read buffer from flash */
extern s32  (*FMRead)(u32 addr, u8 *data, s32 length);

/* Gets vendor ID */
extern u32  (*GetFMVendorID)(u32 addr);

/* Gets device ID */
extern u32  (*GetFMDeviceID)(u32 addr);

/* Indicating that specified sector is protected */
extern u32  (*IsFMSectorProtected)(u32 addr);

/* Turn ON/OFF sector protection */
extern u32 (*FMProtectSector)(u32 addr, u32 turn);

#endif /* !__FLASH_H */

/*===========================================================================
 * End of file Flash.h
 *===========================================================================*/

