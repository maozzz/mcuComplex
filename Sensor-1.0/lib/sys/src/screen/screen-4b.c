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
 *  File screen-4b.c: 4-bits screen functions.
 *
 *  Buffer mapping:
 *    X      0 2     width-1
 *  Y
 *  0        abab...ab => 'ab' is 1 byte => row of 2 pixels: [2*n,y]-[2*n+1,y];
 *  1        abab...ab    'a','b' - indexes at 4-bit color table: [0..15]
 *           .........    (see color tables at "colors.c")
 *  height-1 abab...ab
 *
 *===========================================================================*/

#include "../../../std/inc/colors.h"
#include "../../inc/screen.h"
#include "screen-pr.h"

ScreenFunctions scrFuncs4b =
{
        &GetBufLength4b,
        &ColorToIndex4b,
        &IndexToColor4b,
        &SetPixelByOffset4b,
        &GetPixelByOffset4b,
        &SetPixel4b,
        &GetPixel4b,
        &DrawChar4b
};

/* */
s32 GetBufLength4b(s32 width, s32 height)
{
        return (width * height) >> 1;
}


/* */
u32 ColorToIndex4b(const color_t *cTable, color_t color)
{
	return ColorToIndexXb((cTable ? cTable : colorTable4bit),
                              16,
                              color);
}


/* */
color_t IndexToColor4b(const color_t *cTable, u32 colorIndex)
{
	const color_t *ct = cTable ? cTable : colorTable4bit;
        return ct[(colorIndex & 0x0F)];
}


/* */
void SetPixelByOffset4b(u8 *buffer, s32 offset, u32 colorIndex)
{
        u8 *p = &buffer[(offset >> 1)];

        if (offset & 1)
                *p = (*p & 0xF0) | (colorIndex & 0x0F);
        else
                *p = (*p & 0x0F) | ((colorIndex & 0x0F) << 4);
}


/* */
u32 GetPixelByOffset4b(u8 *buffer, s32 offset)
{
        u8 *p = &buffer[(offset >> 1)];

        if (offset & 1)
                return (*p & 0x0F);
        else
                return ((*p >> 4) & 0x0F);
}


/* */
void SetPixel4b(u8 *buffer, s32 width, s32 x, s32 y, u32 colorIndex)
{
        u8 *p = &buffer[((y * width + x) >> 1)];

        if (x & 1)
                *p = (*p & 0xF0) | (colorIndex & 0x0F);
        else
                *p = (*p & 0x0F) | ((colorIndex & 0x0F) << 4);
}


/* */
u32 GetPixel4b(u8 *buffer, s32 width, s32 x, s32 y)
{
        u8 *p = &buffer[((y * width + x) >> 1)];

        if (x & 1)
                return (*p & 0x0F);
        else
                return ((*p >> 4) & 0x0F);
}


/* */
void DrawChar4b(u32 charIndex,
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
        pixOffset     = (y * width) >> 1;

        while (coli < fontW)
        {
                column |= (psymbol[rowi * fontW + coli] << (rowi << 3));
                rowi++;

                if (rowi == rowCount)
                {
                        hi = 0;
                        curOffset = pixOffset + ((x + coli) >> 1);

                        while(hi++ < fontH)
                        {
                                b = &buffer[curOffset];

                                //Select fore/back color
                                ci = (column & 1) ?
                                     (u8*)&fColorIndex :
                                     (u8*)&bColorIndex;

                                 if ((x + coli) & 1)
                                        *b = (*b & 0xF0) | (*ci & 0x0F);
                                 else
                                        *b = (*b & 0x0F) | ((*ci & 0x0F) << 4);

                                //Get offset to next line
                                curOffset += (width >> 1);

                                column >>= 1;
                        }

                        column = 0;
                        rowi = 0;
                        coli++;
                }
        } //while (coli < fontW)
}


/*===========================================================================
 * End of file screen-4b.c
 *===========================================================================*/

