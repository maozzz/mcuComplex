/*===========================================================================
 *
 *  Hardware library
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2019
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File LCD_PC1602LRSFNHx_defs.h: LCD PC1602LRS-FNHx defintions.
 *
 *===========================================================================*/

#ifndef __LCD_PC1602LRSFNHX_DEFS_H
#define __LCD_PC1602LRSFNHX_DEFS_H

//LCD full size, bytes: 2 lines * 16 bytes
#define LCD_SIZE                (2 * 16)

//LCD cell size, bit
#define LCD_CELLSIZE            8

//LCD count of pages
#define LCD_PAGECNT             1

//LCD width/height, pixels (assume font 6x8 pixels for text display)
#define LCD_WIDTH               (6 * 16) //16 chars per line
#define LCD_HEIGHT              (8 * 2)  //2 lines

//One crystal width/height, pixels
#define LCD_CR_WIDTH            LCD_WIDTH
#define LCD_CR_HEIGHT           LCD_HEIGHT

//LCD PC1602LRS-FNH-x time intervals, ns
#define LCD_tcyce_ns            (500+200)
#define LCD_tas_ns              (60+20)       //Address setup
#define LCD_tah_ns              (10+20)       //Address hold
#define LCD_PWeh_ns             (300+200)     //E (CLOCK) duration
#define LCD_Pause_ns            (40000+20000) //Pause after command

//Include command set of this type LCD
#include "LCD_cmdset_2.h"

#endif /* __LCD_PC1602LRSFNHX_DEFS_H */

/*===========================================================================
 * End of file LCD_PC1602LRSFNHx_defs.h
 *===========================================================================*/

