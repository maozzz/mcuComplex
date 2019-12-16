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
 *  File screen-32b.c: 32-bits screen functions.
 *
 *  Buffer mapping:
 *    X      0 2     width-1
 *  Y
 *  0        nnnn...nn => 'n' is 4 bytes => 1 pixel: [x,y];
 *  1        nnnn...nn     n[0,1,2,3] => [B,G,R,A]
 *           .........
 *  height-1 nnnn...nn
 *
 *===========================================================================*/

#include "../../inc/screen.h"
#include "screen-pr.h"


ScreenFunctions scrFuncs32b =
{
        &GetBufLength32b,
        &ColorToIndex32b,
        &IndexToColor32b,
        &SetPixelByOffset32b,
        &GetPixelByOffset32b,
        &SetPixel32b,
        &GetPixel32b,
        &DrawChar32b
};


/* */
s32 GetBufLength32b(s32 width, s32 height)
{
        return (width * height) << 2;
}


/* */
u32 ColorToIndex32b(const color_t *cTable, color_t color)
{
        return (u32)color;
}


/* */
color_t IndexToColor32b(const color_t *cTable, u32 colorIndex)
{
        return (color_t)colorIndex;
}


/* */
void SetPixelByOffset32b(u8 *buffer, s32 offset, u32 colorIndex)
{
        u8 *b = &buffer[(offset << 2)];
        u8 *ci = (u8*)&colorIndex;
        b[0] = ci[0];
        b[1] = ci[1];
        b[2] = ci[2];
        b[3] = ci[3];
}


/* */
u32 GetPixelByOffset32b(u8 *buffer, s32 offset)
{
        u32 colorIndex;
        u8 *b = &buffer[(offset << 2)];
        u8 *ci = (u8*)&colorIndex;
        ci[0] = b[0];
        ci[1] = b[1];
        ci[2] = b[2];
        ci[3] = b[3];
        return colorIndex;
}


/* */
void SetPixel32b(u8 *buffer, s32 width, s32 x, s32 y, u32 colorIndex)
{
        u8 *b = &buffer[(y * width + x) << 2];
        u8 *ci = (u8*)&colorIndex;
        b[0] = ci[0];
        b[1] = ci[1];
        b[2] = ci[2];
        b[3] = ci[3];
}


/* */
u32 GetPixel32b(u8 *buffer, s32 width, s32 x, s32 y)
{
        u32 colorIndex;
        u8 *b = &buffer[(y * width + x) << 2];
        u8 *ci = (u8*)&colorIndex;
        ci[0] = b[0];
        ci[1] = b[1];
        ci[2] = b[2];
        ci[3] = b[3];
        return colorIndex;
}



/* */
void DrawChar32b(u32 charIndex,
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
        pixOffset     = (y * width + x) << 2;

        while (coli < fontW)
        {
                column |= (psymbol[rowi * fontW + coli] << (rowi << 3));
                rowi++;

                if (rowi == rowCount)
                {
                        hi = 0;
                        curOffset = pixOffset + (coli << 2);

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
                                *b++ = *ci++;
                                *b   = *ci;

                                //Get offset to next line
                                curOffset += (width << 2);

                                column >>= 1;
                        }

                        column = 0;
                        rowi = 0;
                        coli++;
                }
        } //while (coli < fontW)
}

/*===========================================================================
 * End of file screen-32b.c
 *===========================================================================*/

