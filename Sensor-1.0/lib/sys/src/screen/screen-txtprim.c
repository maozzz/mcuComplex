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
 *  File screen-txtprim.c: Text primitives.
 *
 *===========================================================================*/

#include "../../inc/screen.h"
#include "screen-pr.h"

#include <string.h>

/* */
void PrintLine(ScreenParams *scp,
                s32 x1, s32 y1, s32 x2, s32 y2,
                char lineChar,
                color_t color)
{
        s32 dX = x2 - x1;
        s32 dY = y2 - y1;

        s32 x = x1, y = y1;
        s32 stepY = 1, stepX = 1;
        s32 E, er1, er2;

        s32 width = scp->width;
        u32 colorIndex;
        ScreenFunctions *sf;

        sf = GetScreenFuncsPtr(scp);

        if (dY < 0)
        {
                stepY = -1;
                dY = -dY;
        }

        if (dX < 0)
        {
                stepX = -1;
                dX = -dX;
        }


        colorIndex = sf->pfColorToIndex(scp->cTable, color);

        if (IsPointVisible(scp, x, y))
        {
                scp->buffer[y * width + x] = lineChar;

                if (scp->fColors)
                        sf->pfSetPixel(scp->fColors,
                                       scp->width,
                                       x, y, colorIndex);
        }

        if (dY <= dX) //width >= height
        {
                E = 2 * dY - dX;

                er1 = (dY - dX) << 1;
                er2 = dY << 1;

                while (x != x2)
                {
                        if (E > 0)
                        {
                                y += stepY;
                                E += er1;
                        }
                        else
                        {
                                E += er2;
                        }

                        x += stepX;

                        if (IsPointVisible(scp, x, y))
                        {
                                scp->buffer[y * width + x] = lineChar;

                                if (scp->fColors)
                                        sf->pfSetPixel(scp->fColors,
                                                       scp->width,
                                                       x, y, colorIndex);
                        }
                }
        }
        else  //height > width
        {
                E = 2 * dX - dY;

                er1 = (dX - dY) << 1;
                er2 = dX << 1;

                while (y != y2)
                {
                        if (E > 0)
                        {
                                x += stepX;
                                E += er1;
                        }
                        else
                        {
                                E += er2;
                        }

                        y += stepY;

                        if (IsPointVisible(scp, x, y))
                        {
                                scp->buffer[y * width + x] = lineChar;

                                if (scp->fColors)
                                        sf->pfSetPixel(scp->fColors,
                                                       scp->width,
                                                       x, y, colorIndex);
                        }
                }
        }
}



/* Draws filled rectangle with specified coordinates of upper left corner,
   width and height */
void PrintFillRectangle(ScreenParams *scp,
                        s32 x, s32 y, s32 width, s32 height,
                        color_t bColor,
                        u8 fillchar)
{

        s32 xe, ye, xo, yo, xw, yh;
        s32 offset;

        xe = scp->width;
        ye = scp->height;

        xw = x + width;
        yh = y + height;

        //Check that rectangle is fully invisible
        if ((width <= 0) || (height <= 0) ||
            (xw <= 0) || (x >= xe) ||
            (yh <= 0) || (y >= ye))
            return;

        //Set background color
        if (scp->bColors)
        {
            FillBufferRegion(scp, scp->bColors, bColor,
                             x, y, width, height);
        }

        //Select begin/end coordinates
        xo = (x < 0) ? 0 : x;
        yo = (y < 0) ? 0 : y;
        if (xw < xe) xe = xw;
        if (yh < ye) ye = yh;

        //Fill region with specified char
        y = yo;
        while(y < ye)
        {
                x = xo;
                offset = y * scp->width + x;
                y++;

                while(x++ < xe)
                        scp->buffer[offset++] = fillchar;
        }        
}


#define IND_UPL_CORNER         0
#define IND_UPR_CORNER         1
#define IND_BTML_CORNER        2
#define IND_BTMR_CORNER        3
#define IND_UP_LINE            4
#define IND_BTM_LINE           5
#define IND_LEFT_LINE          6
#define IND_RIGHT_LINE         7

#define RECT_BORDER_TYPE_COUNT      8

#if (SCREEN_CHARSET_VERSION == 1)

//Version 1: updated pseudo graphic
//Rectangle border char codes (charset Windows-1251):
//up-left, up-right, btm-left, btm-right corners;
//up, btm, left, right lines.
const u8 rectBorderChars[(RECT_BORDER_TYPE_COUNT * 8)] =
{
        //No border (spaces)
        0x20, 0x20, 0x20, 0x20,  0x20, 0x20, 0x20, 0x20,

        //Single #1 (pseudo graphic, updated charset)
        0xAB, 0x8F, 0x90, 0xAA,  0x92, 0x91, 0x93, 0x84,

        //Single #2 ('+-|' chars)
        0x2B, 0x2B, 0x2B, 0x2B,  0x2D, 0x2D, 0x7C, 0x7C,

        //Twin (updated charset)
        0xB4, 0xB1, 0xB2, 0xB3,  0xB0, 0xAD, 0xAE, 0xAF,

        //Bold #1
        0xAC, 0xAC, 0xAC, 0xAC,  0xAC, 0xAC, 0xAC, 0xAC,

        //Bold #2
        0x82, 0x82, 0x82, 0x82,  0x82, 0x82, 0x82, 0x82,

        //Bold #3
        0x81, 0x81, 0x81, 0x81,  0x81, 0x81, 0x81, 0x81,

        //Bold #4
        0x80, 0x80, 0x80, 0x80,  0x80, 0x80, 0x80, 0x80
};

#else
//Default version: standard pseudo graphic
//Rectangle border char codes (charset Windows-1251):
//up-left, up-right, btm-left, btm-right corners;
//up, btm, left, right lines.
const u8 rectBorderChars[(RECT_BORDER_TYPE_COUNT * 8)] =
{
        //No border (spaces)
        0x20, 0x20, 0x20, 0x20,  0x20, 0x20, 0x20, 0x20,

        //Single #1 (pseudo graphic)
        0xAB, 0x8F, 0x90, 0xAA,  0x94, 0x94, 0x83, 0x83,

        //Single #2 ('+-|' chars)
        0x2B, 0x2B, 0x2B, 0x2B,  0x2D, 0x2D, 0x7C, 0x7C,

        //Twin
        0x99, 0x8B, 0x98, 0x8C,  0x9D, 0x9D, 0x8A, 0x8A,

        //Bold #1
        0xAC, 0xAC, 0xAC, 0xAC,  0xAC, 0xAC, 0xAC, 0xAC,

        //Bold #2
        0x82, 0x82, 0x82, 0x82,  0x82, 0x82, 0x82, 0x82,

        //Bold #3
        0x81, 0x81, 0x81, 0x81,  0x81, 0x81, 0x81, 0x81,

        //Bold #4
        0x80, 0x80, 0x80, 0x80,  0x80, 0x80, 0x80, 0x80
};

#endif /* SCREEN_CHARSET_VERSION */


/* Draws rectangle with specified coordinates of upper left corner,
   width and height */
void PrintRectangle(ScreenParams *scp,
                   s32 x, s32 y, s32 width, s32 height,
                   u32 borderType,
                   color_t fColor,
                   color_t bColor)
{
        s32 xe, ye, xo, yo, xw, yh, crd, offset;
        s32 scwidth = scp->width;
        u32 fColorIndex, bColorIndex;
        ScreenFunctions *sf;
		u8 *buffer = (u8*)scp->buffer;
        u8 *fScreenColors = scp->fColors;
        u8 *bScreenColors = scp->bColors;
        u8 border[8];

        xe = scp->width;
        ye = scp->height;

        xw = x + width;
        yh = y + height;

        //Check that rectangle is fully invisible
        if ((width <= 0) || (height <= 0) ||
            (xw <= 0) || (x >= xe) ||
            (yh <= 0) || (y >= ye))
            return;

        //Select begin/end coordinates
        xo = (x < 0) ? 0 : x;
        yo = (y < 0) ? 0 : y;
        if (xw < xe) xe = xw;
        if (yh < ye) ye = yh;

        //Select border type or use user-defined border
        if (borderType < RECT_BORDER_TYPE_COUNT)
                memcpy(border, &rectBorderChars[(borderType * 8)], 8);
        else
                memset(border, borderType, 8);

        sf = GetScreenFuncsPtr(scp);

        fColorIndex = sf->pfColorToIndex(scp->cTable, fColor);

        if (bColor != COL_TRANSPARENT)
            bColorIndex = sf->pfColorToIndex(scp->cTable, bColor);
        else
        {
            bColorIndex   = 0;
            bScreenColors = 0;
        }


        //Top line [x, y] - [x + w, y]
        if (y == yo)
        {
                offset = y * scwidth + xo;
                crd = xo;
                while(crd++ < xe)
                        buffer[offset++] = border[IND_UP_LINE];

                if (fScreenColors)
                {
                        crd = xo;
                        while(crd < xe)
                        {
                                sf->pfSetPixel(fScreenColors, scwidth,
                                                crd++, y, fColorIndex);
                        }
                }

                if (bScreenColors)
                {
                        crd = xo;
                        while(crd < xe)
                        {
                                sf->pfSetPixel(bScreenColors, scwidth,
                                                crd++, y, bColorIndex);
                        }
                }
        }


        //Bottom line  [x, y + h] - [x + w, y + h]
        yh--;
        if (yh < scp->height)
        {
                offset = yh * scwidth + xo;
                crd = xo;
                while(crd++ < xe)
                        buffer[offset++] = border[IND_BTM_LINE];

                if (fScreenColors)
                {
                        crd = xo;
                        while(crd < xe)
                        {
                                sf->pfSetPixel(fScreenColors, scwidth,
                                                crd++, yh, fColorIndex);
                        }
                }

                if (bScreenColors)
                {
                        crd = xo;
                        while(crd < xe)
                        {
                                sf->pfSetPixel(bScreenColors, scwidth,
                                                crd++, yh, bColorIndex);
                        }
                }
        }


        //Left side  [x, y] - [x, y + h]
        if (x == xo)
        {
                offset = yo * scwidth + x;
                crd = yo;
                while(crd++ < ye)
                {
                        buffer[offset] = border[IND_LEFT_LINE];
                        offset += scwidth;
                }

                if (fScreenColors)
                {
                        crd = yo;
                        while(crd < ye)
                        {
                                sf->pfSetPixel(fScreenColors, scwidth,
                                                x, crd++, fColorIndex);
                        }
                }

                if (bScreenColors)
                {
                        crd = yo;
                        while(crd < ye)
                        {
                                sf->pfSetPixel(bScreenColors, scwidth,
                                                x, crd++, bColorIndex);
                        }
                }

                //Print left corners
                if (y == yo)
                        buffer[y * scwidth + x] = border[IND_UPL_CORNER];

                if (yh < scp->height)
                        buffer[yh * scwidth + x] = border[IND_BTML_CORNER];
        }

        //Right side [x + w, y] - [x + w, y + h]
        xw--;
        if (xw < scp->width)
        {
                offset = yo * scwidth + xw;
                crd = yo;
                while(crd++ < ye)
                {
                        buffer[offset] = border[IND_RIGHT_LINE];
                        offset += scwidth;
                }

                if (fScreenColors)
                {
                        crd = yo;
                        while(crd < ye)
                        {
                                sf->pfSetPixel(fScreenColors, scwidth,
                                                xw, crd++, fColorIndex);
                        }
                }

                if (bScreenColors)
                {
                        crd = yo;
                        while(crd < ye)
                        {
                                sf->pfSetPixel(bScreenColors, scwidth,
                                                xw, crd++, bColorIndex);
                        }
                }

                //Print right corners
                if (y == yo)
                        buffer[y * scwidth + xw] = border[IND_UPR_CORNER];

                if (yh < scp->height)
                        buffer[yh * scwidth + xw] = border[IND_BTMR_CORNER];
        }
}



/* Clears border buffer at specified region */
void ClearBorderRegion(ScreenParams *scp,
                        s32 x, s32 y, s32 width, s32 height)
{
    s32 xe, ye, xo, yo, xw, yh;
    s32 offset;
    u8 *borders = (u8*)scp->borders;
    u8 tmp;

    if (!borders)
        return;

    xe = scp->width;
    ye = scp->height;

    xw = x + width;
    yh = y + height;

    //Check that rectangle is fully invisible
    if ((width <= 0) || (height <= 0) ||
        (xw <= 0) || (x >= xe) ||
        (yh <= 0) || (y >= ye))
        return;

    //Select begin/end coordinates
    xo = (x < 0) ? 0 : x;
    yo = (y < 0) ? 0 : y;
    if (xw < xe) xe = xw;
    if (yh < ye) ye = yh;

    //Clear border region
    y = yo;
    while(y < ye)
    {
        x = xo;
        offset = y * scp->width + x;
        y++;

        while(x++ < xe)
        {
            tmp = borders[(offset >> 1)];

            if (offset & 1)
                tmp &= 0xF0;
            else
                tmp &= 0x0F;

            borders[(offset >> 1)] = tmp;
            offset++;
        }
    }
}



/* Add border bits (bit OR operation).
   4 bits per each char: bits[3..0] => [top, left, bottom, right] */
#define add_border_mask(bmask) \
    tmp = borders[(offset >> 1)];\
    if (offset & 1)\
        tmp |= (bmask);\
    else\
        tmp |= ((bmask) << 4);\
    borders[(offset >> 1)] = tmp


/* Draws border rectangle (ScreenParams::borders)
   with specified coordinates of upper left corner, width and height */
void PrintBorderRectangle(ScreenParams *scp,
                          s32 x, s32 y, s32 width, s32 height)
{
    s32 xe, ye, xo, yo, xw, yh, crd, offset;
    s32 scwidth;
    u8 *borders = (u8*)scp->borders;
    u8 tmp;

    if (!borders)
        return;

    xe = scp->width;
    ye = scp->height;

    xw = x + width;
    yh = y + height;

    //Check that rectangle is fully invisible
    if ((width <= 0) || (height <= 0) ||
        (xw <= 0) || (x >= xe) ||
        (yh <= 0) || (y >= ye))
        return;

    //Select begin/end coordinates
    xo = (x < 0) ? 0 : x;
    yo = (y < 0) ? 0 : y;
    if (xw < xe) xe = xw;
    if (yh < ye) ye = yh;

    scwidth = scp->width;

    //Top line [x, y] - [x + w, y]
    if (y == yo)
    {
            offset = y * scwidth + xo;
            crd = xo;
            while(crd++ < xe)
            {
                add_border_mask(0x08);
                offset++;
            }
    }

    //Bottom line  [x, y + h] - [x + w, y + h]
    yh--;
    if (yh < scp->height)
    {
            offset = yh * scwidth + xo;
            crd = xo;
            while(crd++ < xe)
            {
                add_border_mask(0x02);
                offset++;
            }
    }

    //Left side  [x, y] - [x, y + h]
    if (x == xo)
    {
            offset = yo * scwidth + x;
            crd = yo;
            while(crd++ < ye)
            {
                add_border_mask(0x04);
                offset += scwidth;
            }
    }

    //Right side [x + w, y] - [x + w, y + h]
    xw--;
    if (xw < scp->width)
    {
        offset = yo * scwidth + xw;
        crd = yo;
        while(crd++ < ye)
        {
            add_border_mask(0x01);
            offset += scwidth;
        }
    }
}

/*===========================================================================
 * End of file screen-txtprim.c
 *===========================================================================*/
