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
 *  File LCD.c: LCD implementation selector.
 *
 *===========================================================================*/

#include "../inc/LCD.h"

#if (TARGET_LCD == LCD_MT12864x)
//LCD MT128x64x: graphic display, 128x64 pixel black/white;
//8-bit interface
#include "LCD_MT12864x.c"

#elif (TARGET_LCD == LCD_PC1602LRSFNHx || TARGET_LCD == LCD_PC2004A)
//LCD PC1602: char display 16 chars x 2 lines
//LCD PC2004: char display 20 chars x 4 lines
//One implementation for both displays
#if (defined(__LCD_INTERFACE))
#if (__LCD_INTERFACE == 4)
//4-bit interface [DB7..DB4,RS,RW,E]
#include "LCD_PC1602LRSFNHx_4bit.c"
#elif (__LCD_INTERFACE == 2)
//I2C interface [SDA,SCL], PCF8574 I2C<->8bit converter used
#include "LCD_PC1602LRSFNHx_i2c.c"
#else
//8-bit interface by default [DB7..DB0,RS,RW,E]
#include "LCD_PC1602LRSFNHx.c"
#endif

#else
//8-bit interface by default [DB7..DB0,RS,RW,E]
#include "LCD_PC1602LRSFNHx.c"
#endif /* __LCD_INTERFACE */

#else

#error "Unknown LCD type. Check 'TARGET_LCD' at 'targetlcd.h'"

#endif

/*===========================================================================
 * End of file LCD.c
 *===========================================================================*/

