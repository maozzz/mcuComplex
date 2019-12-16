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
 *  File LCD_cmdset_2.h: LCD command set #2.
 *                       For LCDs compatible with
 *                       POWERTIP PC1602x (HD44780 controller)
 *
 *===========================================================================*/

#ifndef __LCD_CMDSET_2_H
#define __LCD_CMDSET_2_H

//LCD commands: RS=0; R/W=0
//Clear display (1.64ms). No arguments.
#define LCD_CMD_DISPLAY_CLEAR        (1 << 0)

//Return home (1.64ms) No arguments.
#define LCD_CMD_RETURN_HOME          (1 << 1)

//Entry mode set (40us). Arguments:
//ID[1]: 0 - decrement address (default); 1 - increment address
//SH[0]: 1 - shift screen when char added; 0 - no shift
#define LCD_CMD_MODESET              (1 << 2)
#define LCD_CMD_ID                   (1 << 1)
#define LCD_CMD_SH                   (1 << 0)

//Dispaly ON/OFF control (40us). Arguments:
//D[2]: 0/1 - OFF/ON display
//C[1]: 0/1 - OFF/ON cursor '_'
//B[0]: 0/1 - no blink/blink cursor
#define LCD_CMD_DISPLAY_CONTROL      (1 << 3)
#define LCD_CMD_D                    (1 << 2)
#define LCD_CMD_C                    (1 << 1)
#define LCD_CMD_B                    (1 << 0)

//Display or cursor shift (40us). Arguments:
//SC[3]: 1 - shift screen; 0 - shift cursor
//RL[2]: 1 - shift right;  0 - shift left
//bits[1..0]: unused
#define LCD_CMD_DISPLAY_SHIFT        (1 << 4)
#define LCD_CMD_SC                   (1 << 3)
#define LCD_CMD_RL                   (1 << 2)

//Function set (40us). Arguments:
//DL[4]: 1/0 - 8/4 data bits
//N[3] : 1/0 - 2/1 lines (4/2 lines for LCDs 2004)
//F[2] : 1/0 - 5x10/5x8 points
//bits[1..0]: unused
#define LCD_CMD_FUNCTION_SET         (1 << 5)
#define LCD_CMD_DL                   (1 << 4)
#define LCD_CMD_N                    (1 << 3)
#define LCD_CMD_F                    (1 << 2)

//Set CGRAM address (40us). Arguments:
//AC[5..0]: CGRAM address
#define LCD_CMD_SET_CGRAM_ADDR       (1 << 6)

//Set DDRAM address (40us). Arguments:
//AC[6..0]: DDRAM address
#define LCD_CMD_SET_DDRAM_ADDR       (1 << 7)


//Read busy flag and address: RS=0; R/W=1 (40us). Arguments:
//BF[7]:    busy flag;
//AC[6..0]: address counter
#define LCD_FLAG_BUSY           (1 << 7)


//Write data to RAM: RS=1; R/W=0
//DB[7..0]: data to write

//Read data to RAM: RS=1; R/W=1
//DB[7..0]: data that read

//Notes:
//PC1602x DDRAM mapping
//W - width  (count of chars per line)
//H - height (count of lines)
//          Visible part [0..W-1] | Invisible part [W..27h]
//          00h 01h 02h ... (W-1)   W ... 27h
//0000h     Line 0 data
//0040h     Line 1 data
//
//PC2004x DDRAM mapping
//0    Line 0 data
//40   Line 1 data
//20   Line 2 data
//84   Line 3 data

#endif /* __LCD_CMDSET_2_H */

/*===========================================================================
 * End of file LCD_cmdset_2.h
 *===========================================================================*/

