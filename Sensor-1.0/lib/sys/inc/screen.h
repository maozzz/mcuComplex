/*===========================================================================
 *
 *  Screen library
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
 *  File screen.h. Screen definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __SCREEN_H
#define __SCREEN_H

#include <globaldef.h>
#include "../../bastypes.h"
#include "../../std/inc/ScreenParams.h"
#include "../../std/inc/fonts.h"
#include "../../std/inc/colors.h"
#include "../../std/inc/stdcf.h"

//Enable/disable color functions (1/0 - enabled/disabled)
//1-bit color
#ifndef SCREEN_EN_COLOR_1b
#define SCREEN_EN_COLOR_1b      1
#endif /* SCREEN_EN_COLOR_1b */

//4-bit color
#ifndef SCREEN_EN_COLOR_4b
#define SCREEN_EN_COLOR_4b      0
#endif /* SCREEN_EN_COLOR_4b */

//8-bit color
#ifndef SCREEN_EN_COLOR_8b
#define SCREEN_EN_COLOR_8b      0
#endif /* SCREEN_EN_COLOR_8b */

//16-bit color
#ifndef SCREEN_EN_COLOR_16b
#define SCREEN_EN_COLOR_16b     0
#endif /* SCREEN_EN_COLOR_16b */

//24-bit color
#ifndef SCREEN_EN_COLOR_24b
#define SCREEN_EN_COLOR_24b     0
#endif /* SCREEN_EN_COLOR_24b */

//32-bit color
#ifndef SCREEN_EN_COLOR_32b
#define SCREEN_EN_COLOR_32b     0
#endif /* SCREEN_EN_COLOR_32b */

//Add graphic primitives library
#ifndef SCREEN_EN_GRPRIM
#define SCREEN_EN_GRPRIM        0
#endif /* SCREEN_EN_GRPRIM */

//Add text primitives library
#ifndef SCREEN_EN_TXTPRIM
#define SCREEN_EN_TXTPRIM        0
#endif /* SCREEN_EN_TXTPRIM */

//Screen charset version:
//0 - standard pseudo graphic
//1 - updated pseudo graphic
#ifndef SCREEN_CHARSET_VERSION
#define SCREEN_CHARSET_VERSION      0
#endif

//Default 8-bit color table: [colorTableWin8bit, colorTableMac8bit]
#define SCREEN_DEFAULT_CTABLE_8b        colorTableWin8bit

//TAB char width, spaces
#define TAB_WIDTH						4
#define TAB_FILLCHAR					0x20

//When screen clearing this char is used for fill buffer.
//Space (0x20) or  Null (0x00) is recommended
#ifndef EMPTYLINE_FILLCHAR
#define EMPTYLINE_FILLCHAR				0x20
#endif

/*===========================================================================
 * Common screen functions. Text and graphic modes
 *===========================================================================*/

/* Indicating that specified point is visible on screen */
#define IsPointVisible(scp, px, py) ((px) >= 0 && (px) < scp->width &&\
                                     (py) >= 0 && (py) < scp->height)

/* Indicating that speicifed point is visible at region */
u32 IsPointAtRegion(ScreenParams *scp, s16 px, s16 py,
                    s16 x, s16 y, s16 width, s16 height);

/* Indicating that speicifed region is visible on screen */
u32 IsRegionVisible(ScreenParams *scp,
                    s16 x, s16 y, s16 width, s16 height);

/* Clear all data at buffer */
void ClearDataBuffer(ScreenParams *scp);

/* Clear screen */
void ClearScreen(ScreenParams *scp);

/* Redraw screen */
void DrawScreen(ScreenParams *scp);

/* Draw string with specified coordinates, font,
   foreground and background colors. */
void DrawString(ScreenParams *scp,
                s32 x, s32 y,
                const char *pstr, FONT *font,
                color_t fColor, color_t bColor);

/*===========================================================================
 * Graphic mode functions.
 * "SCREEN_OPTS_GRAPHIC" must SET at SreenParams::opts.
 * ScreenParams::buffer contains graphic data.
 * ScreenParams::width,height: width and height at pixels
 *===========================================================================*/

/* Draw string at graphic mode ignoring control chars
   with specified coordinates, width, font,
   foreground and background colors. */
void DrawGStringLine(ScreenParams *scp,
                  s32 x, s32 y, s32 width,
                  const char *pstr, FONT *font,
                  color_t fColor, color_t bColor);

/*===========================================================================
 * Graphic primitives (SCREEN_EN_GRPRIM mist set to 1).
 *===========================================================================*/

/* Sets pixel's color using specified coordinates */
void SetPixel(ScreenParams *scp, s32 x, s32 y, color_t color);

/* Gets pixel's color using specified coordinates */
color_t GetPixel(ScreenParams *scp, s32 width, s32 x, s32 y);

/* Draws line with specified coordinates begin and end */
void DrawLine(ScreenParams *scp,
                s32 x1, s32 y1, s32 x2, s32 y2, color_t color);

/* Draws circle with specified coordinates of center and radius */
void DrawCircle(ScreenParams *scp, s32 xc, s32 yc, s32 r, color_t color);

/* Draws rectangle with specified coordinates of upper left corner,
   width and height */
void DrawRectangle(ScreenParams *scp,
                        s32 x, s32 y, s32 width, s32 height, color_t color);

/* Draws filled rectangle with specified coordinates of upper left corner,
   width and height */
void DrawFillRectangle(ScreenParams *scp,
                        s32 x, s32 y, s32 width, s32 height, color_t color);

/*===========================================================================
 * Text mode functions.
 * "SCREEN_OPTS_GRAPHIC" must RESET at SreenParams::opts.
 * ScreenParams::buffer contains char codes
 * ScreenParams::width,height: width and height at chars
 *===========================================================================*/

/* Print string with specified corrdinates.
   Screen must at text mode. */
#define PrintfString(scp, x, y, ...)\
        std_sprintf(GetCharPtr(scp, x, y), __VA_ARGS__);

/* Write char to screen stream */
void WriteScreenStream(ScreenParams *scp, char c);

/* Draw cursor */
char DrawTCursor(ScreenParams *scp, char chcur);

/* Draw string at text mode ignoring control chars
   with specified coordinates and width. */
void DrawTStringLine(ScreenParams *scp,
                  s32 x, s32 y, s32 width,
                  const char *pstr);

/* Get pointer to char with specified char coordinates (x: column, y: line) */
char *GetCharPtr(ScreenParams *scp, s16 x, s16 y);

/* Clear pecified line of chars */
void ClearScreenLine(ScreenParams *scp, s16 y);

/* Sets foreground color of specified char */
void SetForeColor(ScreenParams *scp, color_t color, s16 x, s16 y);

/* Sets background color of specified char */
void SetBackColor(ScreenParams *scp, color_t color, s16 x, s16 y);

/* Gets foreground color of specified char */
color_t GetForeColor(ScreenParams *scp, s16 x, s16 y);

/* Gets background color of specified char */
color_t GetBackColor(ScreenParams *scp, s16 x, s16 y);

/* Sets foreground color at specified region */
void SetForeColors(ScreenParams *scp, color_t color,
                   s16 x, s16 y, s16 width, s16 height);

/* Sets background color at specified region */
void SetBackColors(ScreenParams *scp, color_t color,
                   s16 x, s16 y, s16 width, s16 height);

/* Sets foreground and background colors of specified char */
void SetForeBackColor(ScreenParams *scp,
                        color_t fColor, color_t bColor,
                        s16 x, s16 y);

/* Sets foreground and background colors at specified region */
void SetForeBackColors(ScreenParams *scp,
                        color_t fColor, color_t bColor,
                        s16 x, s16 y, s16 width, s16 height);

/* Fill screen with specified foreground color */
void FillForeColor(ScreenParams *scp, color_t color);

/* Fill screen with specified background color */
void FillBackColor(ScreenParams *scp, color_t color);

/*===========================================================================
 * Text primitives (SCREEN_EN_TXTPRIM mist set to 1).
 *===========================================================================*/

/* Print line of specified chars */
void PrintLine(ScreenParams *scp,
                s32 x1, s32 y1, s32 x2, s32 y2,
                char lineChar,
                color_t color);

/* Print filled rectangle with specified coordinates of upper left corner,
   width and height */
void PrintFillRectangle(ScreenParams *scp,
                        s32 x, s32 y, s32 width, s32 height,
                        color_t bColor, u8 fillchar);

/* Draws rectangle with specified coordinates of upper left corner,
   width and height */
void PrintRectangle(ScreenParams *scp,
                   s32 x, s32 y, s32 width, s32 height,
                   u32 borderType,
                   color_t fColor, color_t bColor);

/* Clears border buffer at specified region */
void ClearBorderRegion(ScreenParams *scp,
                        s32 x, s32 y, s32 width, s32 height);

/* Draws border rectangle (using ScreenParams::borders buffer)
   with specified coordinates of upper left corner, width and height */
void PrintBorderRectangle(ScreenParams *scp,
                          s32 x, s32 y, s32 width, s32 height);

#endif /* __SCREEN_H */

/*===========================================================================
 * End of file screen.h
 *===========================================================================*/

