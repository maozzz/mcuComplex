/*===========================================================================
 *
 *  Screen library.
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
 *  File screen-24b.c: 24-bits screen functions.
 *
 *  Buffer mapping:
 *    X      0 2     width-1
 *  Y
 *  0        nnnn...nn => 'n' is 3 bytes => 1 pixel: [x,y];
 *  1        nnnn...nn     n[0,1,2] => [B,G,R]
 *           .........
 *  height-1 nnnn...nn
 *
 *===========================================================================*/

#include "../../inc/screen.h"
#include "screen-pr.h"


ScreenFunctions scrFuncs24b =
{
        &GetBufLength24b,
        &ColorToIndex24b,
        &IndexToColor24b,
        &SetPixelByOffset24b,
        &GetPixelByOffset24b,
        &SetPixel24b,
        &GetPixel24b,
        &DrawChar24b
};


/* */
s32 GetBufLength24b(s32 width, s32 height)
{
        return (width * height) * 3;
}


/* */
u32 ColorToIndex24b(const color_t *cTable, color_t color)
{
        return (u32)(color & 0x00FFFFFF);
}


/* */
color_t IndexToColor24b(const color_t *cTable, u32 colorIndex)
{
        return (color_t)(colorIndex & 0x00FFFFFF);
}


/* */
void SetPixelByOffset24b(u8 *buffer, s32 offset, u32 colorIndex)
{
        u8 *b = &buffer[offset * 3];
        u8 *ci = (u8*)&colorIndex;
        b[0] = ci[0];
        b[1] = ci[1];
        b[2] = ci[2];
}


/* */
u32 GetPixelByOffset24b(u8 *buffer, s32 offset)
{
        u32 colorIndex;
        u8 *b = &buffer[offset * 3];
        u8 *ci = (u8*)&colorIndex;
        ci[0] = b[0];
        ci[1] = b[1];
        ci[2] = b[2];
        return colorIndex;
}


/* */
void SetPixel24b(u8 *buffer, s32 width, s32 x, s32 y, u32 colorIndex)
{
        u8 *b = &buffer[(y * width + x) * 3];
        u8 *ci = (u8*)&colorIndex;
        b[0] = ci[0];
        b[1] = ci[1];
        b[2] = ci[2];
}


/* */
u32 GetPixel24b(u8 *buffer, s32 width, s32 x, s32 y)
{
        u32 colorIndex;
        u8 *b = &buffer[(y * width + x) * 3];
        u8 *ci = (u8*)&colorIndex;
        ci[0] = b[0];
        ci[1] = b[1];
        ci[2] = b[2];
        return colorIndex;
}



/* */
void DrawChar24b(u32 charIndex,
                u8 *buffer, s32 width, s32 x, s32 y,
                FONT *font, u32 fColorIndex, u32 bColorIndex)
{
        u32 fontW, fontH, rowCount;
        u32 coli = 0, rowi = 0, column = 0, hi;
        s32 pixOffset, curOffset;
        u8 *ci, *b;
        const u8 *psymbol; //pointer to symbol graphic data

        //Don't draw char 0x00 and chars with index > 255
        if (charIndex > 255)
                return;

        //Font width, count of symbol's rows
        fontW         = font->width;
        fontH         = font->height;
        rowCount      = font->byteRowCount;

        //Get pointer to symbol data
        psymbol       = &font->data[font->bytesPerSymbol * charIndex];

        //Get offset of upper-left corner
        pixOffset     = (y * width + x) * 3;

        while (coli < fontW)
        {
                column |= (psymbol[rowi * fontW + coli] << (rowi << 3));
                rowi++;

                if (rowi == rowCount)
                {
                        hi = 0;
                        curOffset = pixOffset + coli * 3;

                        while(hi++ < fontH)
                        {
                                //Select fore/back color
                                ci = (column & 1) ?
                                     (u8*)&fColorIndex :
                                     (u8*)&bColorIndex;

                                //Get pointer to current pixel
                                b = &buffer[curOffset];

                                //Copy color to buffer
                                *b++ = *ci++;
                                *b++ = *ci++;
                                *b = *ci;

                                //Get offset to next line
                                curOffset += width * 3;

                                column >>= 1;
                        }

                        column = 0;
                        rowi = 0;
                        coli++;
                }
        } //while (coli < fontW)
}

/*===========================================================================
 * End of file screen-24b.c
 *===========================================================================*/

