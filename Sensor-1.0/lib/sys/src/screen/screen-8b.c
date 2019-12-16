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
 *  File screen-8b.c: 8-bits screen functions.
 *
 *  Buffer mapping:
 *    X      0 2     width-1
 *  Y
 *  0        nnnn...nn => 'n' is 1 byte => 1 pixel: [x,y];
 *  1        nnnn...nn    'n' is index at 8-bit color table [0..255]
 *           .........     (see color tables at "colors.c")
 *  height-1 nnnn...nn
 *
 *===========================================================================*/

#include "../../inc/screen.h"
#include "screen-pr.h"

ScreenFunctions scrFuncs8b =
{
        &GetBufLength8b,
        &ColorToIndex8b,
        &IndexToColor8b,
        &SetPixelByOffset8b,
        &GetPixelByOffset8b,
        &SetPixel8b,
        &GetPixel8b,
        &DrawChar8b
};

/* */
s32 GetBufLength8b(s32 width, s32 height)
{
        return (width * height);
}


/* */
u32 ColorToIndex8b(const color_t *cTable, color_t color)
{
        return ColorToIndexXb((cTable ? cTable : SCREEN_DEFAULT_CTABLE_8b),
                              256,
                              color);
}


/* */
color_t IndexToColor8b(const color_t *cTable, u32 colorIndex)
{
        const color_t *ct = cTable ? cTable : SCREEN_DEFAULT_CTABLE_8b;
        return ct[(colorIndex & 0xFF)];
}


/* */
void SetPixelByOffset8b(u8 *buffer, s32 offset, u32 colorIndex)
{
        buffer[offset] = (u8)colorIndex;
}


/* */
u32 GetPixelByOffset8b(u8 *buffer, s32 offset)
{
        return buffer[offset];
}


/* */
void SetPixel8b(u8 *buffer, s32 width, s32 x, s32 y, u32 colorIndex)
{
        buffer[y * width + x] = (u8)colorIndex;
}


/* */
u32 GetPixel8b(u8 *buffer, s32 width, s32 x, s32 y)
{
        return buffer[y * width + x];
}


/* */
void DrawChar8b(u32 charIndex,
                u8 *buffer, s32 width, s32 x, s32 y,
                FONT *font, u32 fColorIndex, u32 bColorIndex)
{
        u32 fontW, fontH, rowCount;
        u32 coli = 0, rowi = 0, column = 0, hi;
        s32 pixOffset, curOffset;
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
        pixOffset     = y * width + x;

        while (coli < fontW)
        {
                column |= (psymbol[rowi * fontW + coli] << (rowi << 3));
                rowi++;

                if (rowi == rowCount)
                {
                        hi = 0;
                        curOffset = pixOffset + coli;

                        while(hi++ < fontH)
                        {
                                //Select fore/back color
                                buffer[curOffset] = (column & 1) ?
                                                      (u8)fColorIndex :
                                                      (u8)bColorIndex;


                                //Get offset to next line
                                curOffset += width;

                                column >>= 1;
                        }

                        column = 0;
                        rowi = 0;
                        coli++;
                }
        } //while (coli < fontW)
}

/*===========================================================================
 * End of file screen-8b.c
 *===========================================================================*/

