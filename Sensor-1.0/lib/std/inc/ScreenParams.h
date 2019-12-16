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
 *  File ScreenParams.h. Screen parameters.
 *
 *===========================================================================*/

#ifndef __SCREENPARAMS_H
#define __SCREENPARAMS_H

#include "../../bastypes.h"
#include "../../std/inc/fonts.h"

 //Graphic buffer sizes
#define SCREEN_GRBUFSIZE_1b(w, h)          ((w) * (((h) + 7) >> 3))
#define SCREEN_GRBUFSIZE_4b(w, h)         (((w) * (h)) >> 1)
#define SCREEN_GRBUFSIZE_8b(w, h)          ((w) * (h))
#define SCREEN_GRBUFSIZE_16b(w, h)        (((w) * (h)) << 1)
#define SCREEN_GRBUFSIZE_24b(w, h)        (((w) * (h)) * 3)
#define SCREEN_GRBUFSIZE_32b(w, h)        (((w) * (h)) << 2)

//ScreenParams::opts
//Graphic mode
#define SCREEN_OPTS_GRAPHIC     0x01

//Default screen options
#define SCREEN_OPTS_DEFAULT     (0x00)

//Color size.
//MUST DEFINED at text and graphic modes (ScreenParams::opts):
//1 bit color
#define SCREEN_OPTS_COLOR_1b    0x00000000
//4 bits color
#define SCREEN_OPTS_COLOR_4b    0x01000000
//8 bits color
#define SCREEN_OPTS_COLOR_8b    0x02000000
//16 bits color
#define SCREEN_OPTS_COLOR_16b   0x03000000
//24 bits color
#define SCREEN_OPTS_COLOR_24b   0x04000000
//32 bits color
#define SCREEN_OPTS_COLOR_32b   0x05000000
//Mask
#define SCREEN_OPTS_COLOR_MASK  0x0F000000

//
#define GetScreenColorSize(scp)    (scp->opts & SCREEN_OPTS_COLOR_MASK)

/* Represents screen cursor parameters */
typedef struct tag_Cursor
{
        s16 x;
        s16 y;
        u32 opts;
} Cursor;


/* Represents screen parameters */
typedef struct tag_ScreenParams
{
        FONT *font;
        CODEPAGE *cp;
        const color_t *cTable;
        u8 *bColors;
        u8 *fColors;
        u8 *borders;
        char *buffer;
        s16 width;
        s16 height;
        u32 opts;
		Cursor cur;
} ScreenParams;

/* Convert char from one encoding to another */
u32 GetLetterIndex(CODEPAGE *codePage, u32 charCode);

#endif /* __SCREENPARAMS_H */

/*===========================================================================
 * End of file ScreenParams.h
 *===========================================================================*/

