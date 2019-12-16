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
 *  File LCD.h: LCD macros and functions prototypes.
 *
 *===========================================================================*/

#ifndef __LCD_H
#define __LCD_H

//Available LCD types
#define LCD_MT12864x         1 //LCD MT-128x64x
#define LCD_PC1602LRSFNHx    2 //LCD PC1602LRS-FNH-x
#define LCD_PC2004A          3 //LCD PC2004A

#include "../../mc/inc/CPU.h"
#include "../../std/inc/ScreenParams.h"
#include <targetlcd.h>


#if (TARGET_LCD == LCD_MT12864x)
#include "LCD_MT12864x_defs.h"

#elif (TARGET_LCD == LCD_PC1602LRSFNHx)
#include "LCD_PC1602LRSFNHx_defs.h"

#elif (TARGET_LCD == LCD_PC2004A)
#include "LCD_PC2004A_defs.h"

/*
#elif (TARGET_LCD == LCD_x)
//Other LCD type definitions
#include "LCD_x_defs.h"
*/

#else

#error "Unknown LCD type. Check 'TARGET_LCD' at 'targetlcd.h'"

#endif

/* Initialize LCD */
void InitializeLCD(u32 hclk);

/* Setup hclk */
void LCDSetHCLK(u32 hclk);

/* Reset LCD */
void LCDReset(u32 addr);

/* Clear LCD */
void LCDClear(u32 addr);

/* Write LCD cell */
s32  LCDWriteCell(u32 addr, u32 value);

/* Read LCD cell */
u32  LCDReadCell(u32 addr);

/* Write data starts from specified address */
s32 LCDWrite(u32 addr, u8 *data, s32 length);

/* Read data starts from specified address */
s32 LCDRead(u32 addr, u8 *data, s32 length);

/* Turn LCD ON/OFF */
void TurnLCD(u8 turn);

/* Select I2C address */
void LCDSelectI2CAddress(u8 i2caddr);

/* Write page to LCD using specified starting address and length */
s32  LCDWritePage(u32 page, u32 addr, u8 *data, s32 length);

/* Read page from LCD using specified starting address and length */
s32  LCDReadPage(u32 page, u32 addr, u8 *data, s32 length);

/* Draw text on LCD using specified parameters */
void LCDDrawText(ScreenParams *scp);

/* Draw graphic on LCD using specified parameters */
void LCDDrawGraphic(ScreenParams *scp);

/*===========================================================================
 * Test functions
 *===========================================================================*/

#ifdef __LCD_TEST_FUNC

/* LCD write/read test */
s32 LCDTest0();

#else /* __LCD_TEST_FUNC */

#define LCDTest0()   0

#endif /* !__LCD_TEST_FUNC */

#endif  /* __LCD_H */

/*===========================================================================
 * End of file LCD.h
 *===========================================================================*/

