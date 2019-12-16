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
 *  File screen.c: Screen functions.
 *
 *===========================================================================*/

#include "../../inc/screen.h"
#include "screen-pr.h"
#include "../../../hw/inc/LCD.h"

#include <stdio.h>
#include <string.h>

#define CODE_TAB        0x09
#define CODE_CR         0x0D
#define CODE_LF         0x0A
#define TAB_OFFSET      3

/*===========================================================================
 * Common functions.
 *===========================================================================*/

void FillBuffer(ScreenParams *scp, u8 *buffer, color_t color);

/* */
void ClearDataBuffer(ScreenParams *scp)
{
    s32 length;

    if (!scp || !scp->buffer)
            return;

    if (scp->opts & SCREEN_OPTS_GRAPHIC)
    {
        //Graphic mode
        //Buffer size depends from color size
        length = GetScreenFuncsPtr(scp)->pfGetBufLength(scp->width, scp->height);

        //Always clear buffer with 0x00 at graphic mode
        memset(scp->buffer, 0x00, length);
    }
    else
    {
        //Text mode
        //Buffer size: WxH
        length = (scp->width * scp->height);

        //Clear with specified char
        memset(scp->buffer, EMPTYLINE_FILLCHAR, length);
    }
}



/* */
void ClearScreen(ScreenParams *scp)
{
    if ((scp->opts & SCREEN_OPTS_COLOR_MASK) == SCREEN_OPTS_COLOR_4b &&
        (scp->opts & SCREEN_OPTS_GRAPHIC) == 0)
    {
        //4-bit color
        s32 colBufSize = (scp->width * scp->height) >> 1;

        //Fill background colors with black color (0)
        if (scp->bColors)
            memset(scp->bColors, 0x00, colBufSize);

        //Fill foreground colors with gray color (7)
        if (scp->fColors)
            memset(scp->fColors, 0x77, colBufSize);

        //Clear border buffer (4 bits per char)
        if (scp->borders)
            memset(scp->borders, 0, colBufSize);
    }
    else
    {
        FillBuffer(scp, scp->bColors, COL_BLACK);
        FillBuffer(scp, scp->fColors, COL_WHITE);

        //Clear border buffer (4 bits per char)
        if (scp->borders)
            memset(scp->borders, 0, (scp->width * scp->height) >> 1);
    }    

    ClearDataBuffer(scp);
    scp->cur.x = 0;
    scp->cur.y = 0;
}



/* */
char *GetCharPtr(ScreenParams *scp, s16 x, s16 y)
{
        if (y >= 0 && x >= 0 &&
            y < scp->height && x < scp->width)
                return &scp->buffer[y * scp->width + x];
        else
                return &scp->buffer[0];
}



/* */
u32 GetLetterIndex(CODEPAGE *codePage, u32 charCode)
{
        s32 encIndex;

        if (codePage == 0)
                return charCode;

        //Only 7 bits used at KOI7-0, KOI7-1
        if (codePage->cpid == CPID_KOI7_0 ||
            codePage->cpid == CPID_KOI7_1)
                charCode &= 0x7F;

        if (charCode < codePage->offset)
                return charCode;
        else
        {
                encIndex = charCode - codePage->offset;

                if (encIndex < (s32)codePage->length)
                        return codePage->encoding[encIndex];
                else
                        return 0x00;
        }
}



/* Redraw screen */
void DrawScreen(ScreenParams *scp)
{
        if (scp->opts & SCREEN_OPTS_GRAPHIC)
                LCDDrawGraphic(scp);
        else
                LCDDrawText(scp);
}



/* Draw string at graphic and text modes with specified coordinates, font,
   background and foreground colors. */
void DrawString(ScreenParams *scp,
                  s32 x, s32 y, const char *pstr, FONT *font,
                  color_t fColor, color_t bColor)
{
        u32 strLength, chIndex, chi, fCIndex, bCIndex;
        s32 xch, ych;
        s16 scrW, scrH;
        char code;
        ScreenFunctions *sf;

        if (!pstr || (strLength = strlen(pstr)) == 0)
                return;

        xch  = x;
        ych  = y;
        scrW  = scp->width;
        scrH  = scp->height;
        sf = GetScreenFuncsPtr(scp);

        fCIndex = sf->pfColorToIndex(scp->cTable, fColor);
        bCIndex = sf->pfColorToIndex(scp->cTable, bColor);

        //Draw string at graphic mode
        if (scp->opts & SCREEN_OPTS_GRAPHIC)
        {
                FONT *pfont;
                u16 fontW, fontH;

                pfont = font ? font : scp->font;

                if (!pfont)
                        return;

                fontW = pfont->width;
                fontH = pfont->height;

                chi = 0;
                while (chi < strLength)
                {
                        code = pstr[chi++];

                        //Control chars
                        switch(code)
                        {
                                case CODE_CR:
                                        xch = x;
                                continue;

                                case CODE_LF:
                                {
                                        xch = x;
                                        ych += fontH;
                                        continue;
                                }

                                case CODE_TAB:
                                        xch += TAB_OFFSET * fontW;
                                break;
                        }

                        if (ych + fontH > scrH)
                                break;

                        if (xch >= 0 && ych >= 0 &&
                            xch + fontW <= scrW)
                        {
                                //Graphic mode: convert char code to specified encoding
                                chIndex = GetLetterIndex(scp->cp, code);

                                //Draw char
                                sf->pfDrawChar(chIndex,
										 (u8*)scp->buffer, scrW, xch, ych,
                                        pfont, fCIndex, bCIndex);
                        }

                        xch += fontW;
                } //while
        } //End draw string at graphic mode
        //Draw string at text mode
        else
        {
                chi = 0;
                while (chi < strLength)
                {
                        code = pstr[chi++];

                        //Control codes
                        switch(code)
                        {
                                case CODE_CR:
                                        xch = x;
                                continue;

                                case CODE_LF:
                                {
                                        xch = x;
                                        ych++;
                                        continue;
                                }

                                case CODE_TAB:
                                        xch += TAB_OFFSET;
                                break;
                        }

                        if (ych >= scrH)
                                break;

                        if (xch >= 0 && ych >= 0 && xch < scrW)
                        {
                                //Text mode: do not convert char code here
                                //chIndex = GetLetterIndex(scp->cp, code);

                                //Put index to char buffer
                                scp->buffer[ych * scrW + xch] = code;

                                //Set foreground and background colors
                                if (scp->fColors)
                                        sf->pfSetPixel(scp->fColors, scrW,
                                                        xch, ych, fCIndex);

                                if (scp->bColors)
                                        sf->pfSetPixel(scp->bColors, scrW,
                                                        xch, ych, bCIndex);
                        }

                        xch++;
                } //while
        } //End draw string at text mode
}



/* Draw string at graphic mode with specified coordinates, font,
   background and foreground colors. Control chars will ignored */
void DrawGStringLine(ScreenParams *scp,
                  s32 x, s32 y, s32 width,
                  const char *pstr, FONT *font,
                  color_t fColor, color_t bColor)
{
        u8 *buffer;
        ScreenFunctions *sf;
        FONT *pfont;
        u32 strLength, chi;
        u32 fCIndex, bCIndex;
        s32 xe;
        s16 scrW;
        u16 fontW;

        if (pstr == 0 || (strLength = strlen(pstr)) == 0 ||
            !(scp->opts & SCREEN_OPTS_GRAPHIC))
                return;

		buffer = (u8*)scp->buffer;
        scrW  = scp->width;
        xe = (scrW < x + width) ? scrW : x + width;

        pfont = font ? font : scp->font;

        if (pfont == 0 ||
            y < 0 || y + pfont->height > scp->height)
                        return;

        fontW = pfont->width;

        sf = GetScreenFuncsPtr(scp);
        fCIndex = sf->pfColorToIndex(scp->cTable, fColor);
        bCIndex = sf->pfColorToIndex(scp->cTable, bColor);

        chi = 0;
        while (chi < strLength &&
                x + fontW <= xe)
        {
                //Graphic mode: convert char code to specified encoding
                if (x >= 0)
                        sf->pfDrawChar(GetLetterIndex(scp->cp, pstr[chi]),
                                        buffer, scrW, x, y,
                                        pfont, fCIndex, bCIndex);

                chi++;
                x += fontW;
        } //while
}



/* Draw string at text mode with specified coordinates, font,
   background and foreground colors. Control chars will ignored */
void DrawTStringLine(ScreenParams *scp,
                  s32 x, s32 y, s32 width,
                  const char *pstr)
{
    u8 *pline;
    s32 strLength, chi;

    if (pstr == 0 || (strLength = strlen(pstr)) == 0 ||
        y < 0 || y >= scp->height)
        return;

    //Pointer to start of screen line
    pline = (u8*)&scp->buffer[(y * scp->width)];
    chi = 0;

    if (width > strLength)
        width = strLength;

    while (chi < width && x < scp->width)
    {
        //Do not convert char codes here
        if (x >= 0)
            pline[x] = pstr[chi];

        chi++;
        x++;
    } //while
}



/* Indicating that speicifed point is visible at region */
u32 IsPointAtRegion(ScreenParams *scp,
                        s16 px, s16 py,
                        s16 x, s16 y, s16 width, s16 height)
{
        return
        (px >= x && px < (x + width) &&
         py >= y && py < (y + height));

}



/* Indicating that speicifed region is visible */
u32 IsRegionVisible(ScreenParams *scp, s16 x, s16 y, s16 width, s16 height)
{
        return
        !(((x + width)  < 0) || (x >= scp->width) ||
          ((y + height) < 0) || (y >= scp->height));

}



/* Fill region of graphic buffer with specified color */
void FillBufferRegion(ScreenParams *scp, u8 *buffer,
                      color_t color,
                      s16 x, s16 y, s16 width, s16 height)
{
        s32 xe, ye, xo, yo, xw, yh;
        s32 offset;
        ScreenFunctions *sf;
        u32 colorIndex;

        xe = scp->width;
        ye = scp->height;

        xw = x + width;
        yh = y + height;

        //Check that rectangle is fully invisible
        if (!buffer || (width <= 0) || (height <= 0) ||
            (xw <= 0) || (x >= xe) ||
            (yh <= 0) || (y >= ye))
            return;

        //Select begin/end coordinates
        xo = (x < 0) ? 0 : x;
        yo = (y < 0) ? 0 : y;
        if (xw < xe) xe = xw;
        if (yh < ye) ye = yh;

        //Get pointer to set of n-bit functions
        sf = GetScreenFuncsPtr(scp);

        //Convert color to color index
        colorIndex = sf->pfColorToIndex(scp->cTable, color);

        //Set current Y coordinate to initial Y coordinate
        y = yo;

        if (GetScreenColorSize(scp) == SCREEN_OPTS_COLOR_1b)
        {
                u8 *p;
                u8 mask;

                if (colorIndex == 0) //fill rectangle with black color
                while(y < ye)
                {
                        x = xo;
                        mask = 1 << (y & 0x07);
                        p = &buffer[((y >> 3) * scp->width + x)];
                        y++;

                        while(x++ < xe)
                                *p++ &= ~mask;
                }
                else                 //fill rectangle with white color
                while(y < ye)
                {
                        x = xo;
                        mask = 1 << (y & 0x07);
                        p = &buffer[((y >> 3) * scp->width + x)];
                        y++;

                        while(x++ < xe)
                                *p++ |= mask;
                }
        }
        else
        {
                while(y < ye)
                {
                        x = xo;
                        offset = y * scp->width + x;
                        y++;

                        while(x++ < xe)
                                sf->pfSetPixelByOffset(buffer,
                                                        offset++, colorIndex);
                }
        }
}



/* Fill graphic buffer with specified color */
void FillBuffer(ScreenParams *scp, u8 *buffer, color_t color)
{
        if (!buffer)
                return;

        if (GetScreenColorSize(scp) == SCREEN_OPTS_COLOR_1b)
        {
                memset(buffer,
                       (color == COL_BLACK) ? 0x00 : 0xFF,
                        GetScreenFuncsPtr(scp)->pfGetBufLength(
                                                scp->width, scp->height));
        }
        else
        {
                FillBufferRegion(scp, buffer, color,
                                 0, 0, scp->width, scp->height);
        }
}

/*===========================================================================
 * Text mode functions.
 *===========================================================================*/

/* Shift up screen buffer by one line */
static void ShiftScreenBuffer(ScreenParams *scp)
{
	int i = 0;
	ScreenParams *sp = scp;
	char *pdest = &sp->buffer[0];
	char *psrc  = &sp->buffer[sp->width];

	//Shift lines
	while(i < (sp->height - 1))
	{
		memcpy(pdest, psrc, sp->width);
		pdest = pdest + sp->width;
		psrc  = psrc  + sp->width;
		i++;
	}

	//Last line is empty
	memset(pdest, EMPTYLINE_FILLCHAR, sp->width);

	//Update cursor position
	//scp->curpos = sp->width * (sp->height - 1);
	scp->cur.x = 0;
	scp->cur.y = sp->height - 1;
}



/* Write char to screen buffer (and shift lines) */
static void WriteScreenBuffer(ScreenParams *scp, char c)
{
	scp->buffer[(scp->cur.y * scp->width + scp->cur.x++)] = c;

	if (scp->cur.x == scp->width)
	{
		scp->cur.x = 0;
		scp->cur.y++;
	}

	if (scp->cur.y == scp->height)
		ShiftScreenBuffer(scp);
}



/* Write char to screen stream */
void WriteScreenStream(ScreenParams *scp, char c)
{
	if (!scp)
		return;

	switch(c)
	{
		//NULL, CR: no actions
		case 0x00:
		case 0x0D: break;

		//LF: move to new line (and shift buffer)
		case 0x0A:
		{
			scp->cur.y++;

			if (scp->cur.y == scp->height)
				ShiftScreenBuffer(scp);
			else
				scp->cur.x = 0;
			break;
		}

		//BS
		case 0x08:
		{
			if (scp->cur.y == 0 && scp->cur.x == 0)
				break;
			else if (scp->cur.x == 0)
			{
				scp->cur.y--;
				scp->cur.x = scp->width - 1;
			}
			else
			{
				scp->cur.x--;
			}

			scp->buffer[(scp->cur.y * scp->width + scp->cur.x)] = EMPTYLINE_FILLCHAR;
			break;
		}

		//TAB
		case 0x09:
		{
			int i = TAB_WIDTH;
			while(i--) WriteScreenBuffer(scp, TAB_FILLCHAR);
			break;
		}

		//Default: put char to screen buffer
		default: WriteScreenBuffer(scp, c); break;
	}
}



/* Draw cursor */
char DrawTCursor(ScreenParams *scp, char chcur)
{
	char *pchcur;
	char chtmp;
	pchcur = &scp->buffer[(scp->cur.y * scp->width + scp->cur.x)];
	chtmp   = *pchcur;
	*pchcur = chcur;
	return chtmp;
}



/* Clear specified line of chars */
void ClearScreenLine(ScreenParams *scp, s16 y)
{
        if (!scp || !scp->buffer || (scp->opts & SCREEN_OPTS_GRAPHIC))
                return;

        memset(&scp->buffer[scp->width * y],
                0x20, //space
                scp->width);
}



/* Set foreground color using specified char coordinates */
void SetForeColor(ScreenParams *scp, color_t color, s16 x, s16 y)
{
        if (scp->fColors)
        {
                ScreenFunctions *sf = GetScreenFuncsPtr(scp);

                sf->pfSetPixel(scp->fColors, scp->width, x, y,
                               sf->pfColorToIndex(scp->cTable, color));
        }
}



/* Set background color using specified char coordinates */
void SetBackColor(ScreenParams *scp, color_t color, s16 x, s16 y)
{
        if (scp->bColors)
        {
                ScreenFunctions *sf = GetScreenFuncsPtr(scp);

                sf->pfSetPixel(scp->bColors, scp->width, x, y,
                               sf->pfColorToIndex(scp->cTable, color));
        }
}



/* Set foreground colors at specifed rectangle */
void SetForeColors(ScreenParams *scp, color_t color,
                        s16 x, s16 y, s16 width, s16 height)
{
        if (scp->fColors)
                FillBufferRegion(scp, scp->fColors, color,
                                x, y, width, height);
}



/* Set background colors at specifed rectangle */
void SetBackColors(ScreenParams *scp, color_t color,
                        s16 x, s16 y, s16 width, s16 height)
{
        if (scp->bColors)
                FillBufferRegion(scp, scp->bColors, color,
                                x, y, width, height);
}



/* Get foreground color using specified char coordinates */
color_t GetForeColor(ScreenParams *scp, s16 x, s16 y)
{
        if (scp->fColors)
        {
                ScreenFunctions *sf = GetScreenFuncsPtr(scp);

                return sf->pfIndexToColor(
                        scp->cTable,
                        sf->pfGetPixel(scp->fColors, scp->width, x, y));
        }
        else
                return COL_WHITE;
}



/* Get background color using specified char coordinates */
color_t GetBackColor(ScreenParams *scp, s16 x, s16 y)
{
        if (scp->bColors)
        {
                ScreenFunctions *sf = GetScreenFuncsPtr(scp);

                return sf->pfIndexToColor(
                        scp->cTable,
                        sf->pfGetPixel(scp->bColors, scp->width, x, y));
        }
        else
                return COL_BLACK;
}



/* */
void SetForeBackColor(ScreenParams *scp,
                      color_t fColor, color_t bColor,
                      s16 x, s16 y)
{
    ScreenFunctions *sf = GetScreenFuncsPtr(scp);
    u32 colorIndex;

    //Convert color to color index
    //Forecolor
    if (scp->fColors)
    {
        colorIndex = sf->pfColorToIndex(scp->cTable, fColor);
        sf->pfSetPixel(scp->fColors, scp->width, x, y, colorIndex);
    }

    //Backcolor
    if (scp->bColors)
    {
        colorIndex = sf->pfColorToIndex(scp->cTable, bColor);
        sf->pfSetPixel(scp->bColors, scp->width, x, y, colorIndex);
    }
}



/* Set specified foreground and background colors for region */
void SetForeBackColors(ScreenParams *scp,
                        color_t fColor, color_t bColor,
                        s16 x, s16 y, s16 width, s16 height)
{
        ScreenFunctions *sf;
        u8 *fColBuffer, *bColBuffer;
        s32 xe, ye, xo, yo, xw, yh, offset;
        u32 fCIndex, bCIndex;

        fColBuffer = scp->fColors;
        bColBuffer = scp->bColors;

        xe = scp->width;
        ye = scp->height;

        xw = x + width;
        yh = y + height;        

        //Check that rectangle is fully invisible
        if (!fColBuffer || !bColBuffer ||
            (width <= 0) || (height <= 0) ||
            (xw <= 0) || (x >= xe) ||
            (yh <= 0) || (y >= ye))
            return;

        //Select begin/end coordinates
        xo = (x < 0) ? 0 : x;
        yo = (y < 0) ? 0 : y;
        if (xw < xe) xe = xw;
        if (yh < ye) ye = yh;

        //Get pointer to set of n-bit functions
        sf = GetScreenFuncsPtr(scp);

        //Convert color to color index
        fCIndex = sf->pfColorToIndex(scp->cTable, fColor);
        bCIndex = sf->pfColorToIndex(scp->cTable, bColor);

        //Set current Y coordinate to initial Y coordinate
        y = yo;

        if (GetScreenColorSize(scp) == SCREEN_OPTS_COLOR_1b)
        {
                u8 *p0, *p1;
                u8 mask;

                while(y < ye)
                {
                        x = xo;
                        mask = 1 << (y & 0x07);
                        offset = ((y >> 3) * scp->width + x);
                        p0 = &fColBuffer[offset];
                        p1 = &bColBuffer[offset];
                        y++;

                        if (fCIndex == 0)
                                while(x++ < xe)
                                        *p0++ &= ~mask;
                        else
                                while(x++ < xe)
                                        *p0++ |= mask;

                        x = xo;
                        if (bCIndex == 0)
                                while(x++ < xe)
                                        *p1++ &= ~mask;
                        else
                                while(x++ < xe)
                                        *p1++ |= mask;
                }
        }
        else
        {
                while(y < ye)
                {
                        x = xo;
                        offset = y * scp->width + x;
                        y++;

                        while(x++ < xe)
                        {
                                sf->pfSetPixelByOffset(
                                        fColBuffer, offset, fCIndex);

                                sf->pfSetPixelByOffset(
                                        bColBuffer, offset, bCIndex);

                                offset++;
                        }
                }
        }
}



/* Fill screen with specified foreground color */
void FillForeColor(ScreenParams *scp, color_t color)
{
        FillBuffer(scp, scp->fColors, color);
}



/* Fill screen with specified background color */
void FillBackColor(ScreenParams *scp, color_t color)
{
        FillBuffer(scp, scp->bColors, color);
}



/* Find nearest color at color table using specified color */
u32 ColorToIndexXb(const color_t *cTable, u32 length, color_t color)
{
        s32 r, g, b, rd, gd, bd, c;
        s32 minDifp2, difp2;
        u32 i, index;

        r = (color >> 16) & 0xFF;
        g = (color >> 8) & 0xFF;
        b = color & 0xFF;

        index = 0;
        minDifp2 = 0x7FFFFFFF;

        i = 0;
        while(i < length)
        {
                c = cTable[i];

                rd = r - ((c >> 16) & 0xFF);
                gd = g - ((c >> 8) & 0xFF);
                bd = b - (c & 0xFF);

                difp2 = rd * rd + gd * gd + bd * bd;

                if (difp2 < minDifp2)
                {
                        minDifp2 = difp2;
                        index = i;
                }

                i++;
        }

        return index;
}

/*===========================================================================
 * Include other files of source code
 *===========================================================================*/

ScreenFunctions *scrFuncs[] =
{
        #if (SCREEN_EN_COLOR_1b == 1)
        &scrFuncs1b,
        #else
        0,
        #endif /* SCREEN_EN_COLOR_1b != 1 */

        #if (SCREEN_EN_COLOR_4b == 1)
        &scrFuncs4b,
        #else
        0,
        #endif /* SCREEN_EN_COLOR_4b != 1 */

        #if (SCREEN_EN_COLOR_8b == 1)
        &scrFuncs8b,
        #else
        0,
        #endif /* SCREEN_EN_COLOR_8b != 1 */

        #if (SCREEN_EN_COLOR_16b == 1)
        &scrFuncs16b,
        #else
        0,
        #endif /* SCREEN_EN_COLOR_16b != 1 */

        #if (SCREEN_EN_COLOR_24b == 1)
        &scrFuncs24b,
        #else
        0,
        #endif /* SCREEN_EN_COLOR_24b != 1 */

        #if (SCREEN_EN_COLOR_32b == 1)
        &scrFuncs32b,
        #else
        0
        #endif /* SCREEN_EN_COLOR_32b != 1 */
};


#if (SCREEN_EN_COLOR_1b == 1)
#include "screen-1b.c"
#endif /* SCREEN_EN_COLOR_1b != 1 */

#if (SCREEN_EN_COLOR_4b == 1)
#include "screen-4b.c"
#endif /* SCREEN_EN_COLOR_4b != 1 */

#if (SCREEN_EN_COLOR_8b == 1)
#include "screen-8b.c"
#endif /* SCREEN_EN_COLOR_8b != 1 */

#if (SCREEN_EN_COLOR_16b == 1)
#include "screen-16b.c"
#endif /* SCREEN_EN_COLOR_16b != 1 */

#if (SCREEN_EN_COLOR_24b == 1)
#include "screen-24b.c"
#endif /* SCREEN_EN_COLOR_24b != 1 */

#if (SCREEN_EN_COLOR_32b == 1)
#include "screen-32b.c"
#endif /* SCREEN_EN_COLOR_32b != 1 */

#if (SCREEN_EN_GRPRIM == 1)
#include "screen-grprim.c"
#endif /* SCREEN_EN_GRPRIM */

#if (SCREEN_EN_TXTPRIM == 1)
#include "screen-txtprim.c"
#endif /* SCREEN_EN_TXTPRIM */

/*===========================================================================
 * End of file screen.c
 *===========================================================================*/

