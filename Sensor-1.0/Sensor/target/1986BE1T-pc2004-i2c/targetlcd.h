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
 *  File targetlcd.h: LCD PC1602LRS-FNH-x. I2C interface.
 *                    MCU 1986BE91T evaluation board is used.
 *                    It's not recommended change any definitions at this file.
 *
 *===========================================================================*/

#ifndef __TARGETLCD_H
#define __TARGETLCD_H

//Select target LCD (see LCD_x at "lib/hw/inc/LCD.h")
#define TARGET_LCD      LCD_PC2004A

//Select I2C interface, PCF8574 converter used
#define __LCD_INTERFACE		2

#define LCD_BASEADDR    (0)

//Command: RS=0
#define LCD_CMD1        (0)
//Data: RS=1
#define LCD_DATA1       (1)

//LCD_RES_{Act/Off}: sets active/inactive LCD reset (optional)
//[C9] -> [RESET]
#define LCD_RES_Act()         //PC_Reset(PN9)
#define LCD_RES_Off()         //PC_Set(PN9)							 

#define LCD_INV_CLKCYC			0.000160F

__STATIC_INLINE void InitializeLCDPorts()
{
    //No actions, assumed that I2C already initialized
}

#endif /* __TARGETLCD_H */

/*===========================================================================
 * End of file targetlcd.h
 *===========================================================================*/

