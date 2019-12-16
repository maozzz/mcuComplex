/*===========================================================================
 *
 *  Hardware library
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2014-2019
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File LCD_cmdset_1.h: LCD command set #1. 
 *                       For LCD controllers compatible with
 *                       SAMSUNG KS0108
 *                       ANGSTREM K145VG10 (LCD MT-12864x)
 *
 *===========================================================================*/

#ifndef __LCD_CMDSET_1_H
#define __LCD_CMDSET_1_H

//LCD commands
#define LCD_CMD_ON              0x3F
#define LCD_CMD_OFF             0x3E
#define LCD_CMD_STARTLINE       0xC0
#define LCD_CMD_SETPAGE         0xB8
#define LCD_CMD_SETADDR         0x40

//LCD status flags
#define LCD_FLAG_RESET          0x10
#define LCD_FLAG_OFF            0x20
#define LCD_FLAG_BUSY           0x80

#endif /* __LCD_CMDSET_1_H */

/*===========================================================================
 * End of file LCD_cmdset_1.h
 *===========================================================================*/

