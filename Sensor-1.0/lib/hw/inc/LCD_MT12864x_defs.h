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
 *  File LCD_MT12864X_defs.h: LCD MT-12864x defintions.
 *
 *===========================================================================*/

#ifndef __LCD_MT12864X_DEFS_H
#define __LCD_MT12864X_DEFS_H

//LCD full size, bytes: 8 pages * 128 bytes
#define LCD_SIZE                (8 * 128)

//LCD cell size, bit
#define LCD_CELLSIZE            8

//LCD count of pages
#define LCD_PAGECNT             8

//LCD width/height, pixels
#define LCD_WIDTH               128
#define LCD_HEIGHT              64

//One crystal width/height, pixels
#define LCD_CR_WIDTH            64
#define LCD_CR_HEIGHT           64

//LCD MT-12864x time intervals, ns
#define LCD_tcyce_ns            1000
#define LCD_tas_ns              140
#define LCD_tah_ns              10
#define LCD_PWeh_ns             450
#define LCD_Pause_ns            8000
#define LCD_ResetAct_ns         1000
#define LCD_ResetRdy_ns         10000

//Include command set of this type LCD
#include "LCD_cmdset_1.h"

#endif

/*===========================================================================
 * End of file LCD_MT_12864X_defs.h
 *===========================================================================*/

