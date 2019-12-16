/*===========================================================================
 *
 *  Graphic library.
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
 *  File screen-gprim.c: Graphic primitives.
 *
 *===========================================================================*/

#include "../../inc/screen.h"
#include "screen-pr.h"

/* Sets pixel's color using specified coordinates */
void SetPixel(ScreenParams *scp, s32 x, s32 y, color_t color)
{
        ScreenFunctions *sf = GetScreenFuncsPtr(scp);

		sf->pfSetPixel((u8*)scp->buffer, scp->width, x, y,
                       sf->pfColorToIndex(scp->cTable, color));
}



/* Gets pixel's color using specified coordinates */
color_t GetPixel(ScreenParams *scp, s32 width, s32 x, s32 y)
{
        ScreenFunctions *sf = GetScreenFuncsPtr(scp);

        //Convert color index to color
        return sf->pfIndexToColor(
                        scp->cTable,
						sf->pfGetPixel((u8*)scp->buffer, scp->width, x, y));
}



/* Draws line with specified coordinates begin and end */
void DrawLine(ScreenParams *scp,
                s32 x1, s32 y1, s32 x2, s32 y2, color_t color)
{
        s32 dX = x2 - x1;
        s32 dY = y2 - y1;

        s32 x = x1, y = y1;
        s32 stepY = 1, stepX = 1;
        s32 E, er1, er2;

		u8 *buffer = (u8*)scp->buffer;
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
                sf->pfSetPixel(buffer, width, x, y, colorIndex);

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
                                sf->pfSetPixel(buffer, width, x, y, colorIndex);
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
                                sf->pfSetPixel(buffer, width, x, y, colorIndex);
                }
        }
}



/* Draws circle with specified coordinates of center and radius */
void DrawCircle(ScreenParams *scp, s32 xc, s32 yc, s32 r, color_t color)
{
        s32 x = 0, y = r, d = 3; // - 2 * y;
        s32 x1, x2, x3, x4, y1, y2, y3, y4;
		u8 *buffer = (u8*)scp->buffer;
        s32 width = scp->width;
        u32 colorIndex;
        ScreenFunctions *sf;

        if (r <= 0)
                return;

        sf = GetScreenFuncsPtr(scp);
        colorIndex = sf->pfColorToIndex(scp->cTable, color);

        while (x <= y)
        {
                if (d < 0)
                        d += (x << 2) + 6; //4 * x + 6;
                else
                {
                        d += ((x - y) << 2) + 10; //4 * (x - y) + 10;
                        y--;
                }

                x1 = xc + x;
                x2 = xc - x;
                x3 = xc + y;
                x4 = xc - y;

                y1 = yc + y;
                y2 = yc - y;
                y3 = yc + x;
                y4 = yc - x;


                if (IsPointVisible(scp, x1, y1))
                        sf->pfSetPixel(buffer, width, x1, y1, colorIndex);

                if (IsPointVisible(scp, x1, y2))
                        sf->pfSetPixel(buffer, width, x1, y2, colorIndex);

                if (IsPointVisible(scp, x2, y2))
                        sf->pfSetPixel(buffer, width, x2, y2, colorIndex);

                if (IsPointVisible(scp, x2, y1))
                        sf->pfSetPixel(buffer, width, x2, y1, colorIndex);

                if (IsPointVisible(scp, x3, y3))
                        sf->pfSetPixel(buffer, width, x3, y3, colorIndex);

                if (IsPointVisible(scp, x3, y4))
                        sf->pfSetPixel(buffer, width, x3, y4, colorIndex);

                if (IsPointVisible(scp, x4, y3))
                        sf->pfSetPixel(buffer, width, x4, y4, colorIndex);

                if (IsPointVisible(scp, x4, y3))
                        sf->pfSetPixel(buffer, width, x4, y3, colorIndex);

                x++;
        }
}



/* Draws rectangle with specified coordinates of upper left corner,
   width and height */
void DrawRectangle(ScreenParams *scp,
                   s32 x, s32 y, s32 width, s32 height, color_t color)
{
        s32 xe, ye, xo, yo, xw, yh, crd;
		u8 *buffer = (u8*)scp->buffer;
        s32 scwidth = scp->width;
        u32 colorIndex;
        ScreenFunctions *sf;

        xe = scp->width;
        ye = scp->height;

        xw = x + width;
        yh = y + height;

        //Check that rectangle is fully invisible
        if (width == 0 || height == 0 ||
            (x < 0 && xw < 0) || (x >= xe) ||
            (y < 0 && yh < 0) || (y >= ye))
            return;

        //Select begin/end coordinates
        xo = (x < 0) ? 0 : x;
        yo = (y < 0) ? 0 : y;
        if (xw < xe) xe = xw;
        if (yh < ye) ye = yh;

        sf = GetScreenFuncsPtr(scp);
        colorIndex = sf->pfColorToIndex(scp->cTable, color);

        //Top line [x, y] - [x + w, y]
        if (y == yo)
        {
                crd = xo;
                while(crd < xe)
                        sf->pfSetPixel(buffer, scwidth, crd++, y, colorIndex);
        }


        //Bottom line  [x, y + h] - [x + w, y + h]
        yh--;
        if (yh < scp->height)
        {
                crd = xo;
                while(crd < xe)
                        sf->pfSetPixel(buffer, scwidth, crd++, yh, colorIndex);
        }


        //Left side  [x, y] - [x, y + h]
        if (x == xo)
        {
                crd = yo;
                while(crd < ye)
                        sf->pfSetPixel(buffer, scwidth, x, crd++, colorIndex);
        }

        //Right side [x + w, y] - [x + w, y + h]
        xw--;
        if (xw < scp->width)
        {
                crd = yo;
                while(crd < ye)
                        sf->pfSetPixel(buffer, scwidth, xw, crd++, colorIndex);
        }
}



/* Draws filled rectangle with specified coordinates of upper left corner,
   width and height */
void DrawFillRectangle(ScreenParams *scp,
                        s32 x, s32 y, s32 width, s32 height, color_t color)
{
		FillBufferRegion(scp, (u8*)scp->buffer,
                        color,
                        x, y, width, height);
}

/*===========================================================================
 * End of file screen-grprim.c
 *===========================================================================*/

