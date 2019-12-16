/*===========================================================================
 *
 *  Standard library
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
 *  File esccodes.h: ANSI escape sequences.
 *
 *===========================================================================*/

#ifndef __ESCCODES_H
#define __ESCCODES_H

#include "../../bastypes.h"
#include "KeyCode.h"

//Move cursor direction
#define ESC_UP           0x41 //Move up
#define ESC_DOWN         0x42 //Move down
#define ESC_FWD          0x43 //Move forward
#define ESC_BACK         0x44 //Move back

//Clear screen mode
#define ESC_BOTTOM           0 //Clear bottom from cursor
#define ESC_TOP              1 //Clear top from cursor
#define ESC_CLRALL           2 //Clear all screen or line

//Clear line mode
#define ESC_END              0 //Clear from cursor to end
#define ESC_BEG              1 //Clear from cursor to start

//Graphic attributes
#define ESC_ATTROFF          0x01
#define ESC_BOLD             0x02
#define ESC_UNDER            0x04
#define ESC_BLINK            0x08
#define ESC_REVERSE          0x10
#define ESC_CONCEALED        0x20

//Colors
#define ESC_BLACK            30
#define ESC_RED              31
#define ESC_GREEN            32
#define ESC_YELLOW           33
#define ESC_BLUE             34
#define ESC_MAGENTA          35
#define ESC_CYAN             36
#define ESC_WHITE            37

/* */
typedef void (*KeyCodeHandler)(KeyCode key);
typedef KeyCodeHandler LpKeyCodeHandler;

/* Clears the screen */
void ESCClearScreen(char *buf, u8 mode);

/* Clears all characters from the cursor position to the end of the line */
void ESCClearLine(char *buf, u8 mode);

/* Moves the cursor up by the specified number of lines */
void ESCCurMove(char *buf, u8 curdir, u16 count);

/* Moves the cursor to the specified position */
void ESCCurPos(char *buf, u16 x, u16 y);

/* Hide cursor */
void ESCCurHide(char *buf);

/* Show cursor */
void ESCCurShow(char *buf);

/* Saves the current cursor position */
void ESCCurSave(char *buf);

/* Returns the cursor to the position */
void ESCCurRestore(char *buf);

/* Set graphic mode */
void ESCGraphicMode(char *buf, u16 mode, u8 fColor, u8 bColor);

/* Reset graphic mode to default values */
void ESCGraphicReset(char *buf);

/* Initialize escape handler */
void InitializeESCHandler();

/* Escape sequence handler */
void ESCHandler(const u8 *buffer, s32 count);

/* */
void SetESCKeyHandler(LpKeyCodeHandler handler);

/* */
s32 CheckSequenceT(u8 *seq, u32 count);

/* */
KeyCode GetKeyCode(u8 b);

#endif /* __ESCCODES_H */

/*===========================================================================
 * End of file esccodes.h
 *===========================================================================*/

