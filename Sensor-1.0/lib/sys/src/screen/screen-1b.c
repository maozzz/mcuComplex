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
 *  File screen-1b.c: 1-bits screen functions.
 *
 *  Buffer mapping:
 *    X      0 2     width-1
 *  Y
 *  0        ||||...|| => '|' is 1 byte => column of 8 pixels: [x,y]-[x,y+7];
 *  8        ||||...||    each bit is color: 1 - white, 0 - black
 *           .........
 *  height-1 ||||...||
 *
 *===========================================================================*/

#include "../../inc/screen.h"
#include "screen-pr.h"

ScreenFunctions scrFuncs1b =
{
        &GetBufLength1b,
        &ColorToIndex1b,
        &IndexToColor1b,
        &SetPixelByOffset1b,
        &GetPixelByOffset1b,
        &SetPixel1b,
        &GetPixel1b,
        &DrawChar1b
};

/* */
s32 GetBufLength1b(s32 width, s32 height)
{
         return (width * ((height + 7) >> 3));
}


/* */
u32 ColorToIndex1b(const color_t *cTable, color_t color)
{
        return (color == COL_BLACK) ? 0 : 1;
}


/* */
color_t IndexToColor1b(const color_t *cTable, u32 colorIndex)
{
        return (colorIndex == 0) ? COL_BLACK : COL_WHITE;
}


/* */
void SetPixelByOffset1b(u8 *buffer, s32 offset, u32 colorIndex)
{
        if (colorIndex == 0)
                buffer[(offset >> 3)] &= ~(1 << (offset & 0x07));
        else
                buffer[(offset >> 3)] |= (1 << (offset & 0x07));
}


/* */
u32 GetPixelByOffset1b(u8 *buffer, s32 offset)
{
        return buffer[(offset >> 3)] & (1 << (offset & 0x07)) ? 1 : 0;
}


/* */
void SetPixel1b(u8 *buffer, s32 width, s32 x, s32 y, u32 colorIndex)
{
        u8 *p = &buffer[((y >> 3) * width + x)];

        if (colorIndex == 0)
                *p &= ~(1 << (y & 0x07));
        else
                *p |= (1 << (y & 0x07));
}


/* */
u32 GetPixel1b(u8 *buffer, s32 width, s32 x, s32 y)
{
        u8 *p = &buffer[((y >> 3) * width + x)];
        return *p & (1 << (y & 0x07)) ? 1 : 0;
}


/* Draw char on specified buffer. */
void DrawChar1b(u32 charIndex,
                u8 *buffer, s32 width, s32 x, s32 y,
                FONT *font, u32 fColorIndex, u32 bColorIndex)
{
        u32 fontW, rowCount, maskColumn;
        u32 coli = 0, rowi = 0, bufOffset = 0, column = 0;
        const u8 *psymbol; //pointer to symbol graphic data
        u8 *pbuffer;

        //Don't draw char 0x00 and chars with index > 255
        if ((charIndex == 0 && bColorIndex == 0) ||
             charIndex > 255)
                return;

        //Get pointer to 8-bit line
        pbuffer       = &buffer[width * (y >> 3) + x];

        //Font width, count of symbol's rows
        fontW         = font->width;
        rowCount      = font->byteRowCount;

        //Get pointer to symbol data
        psymbol       = &font->data[font->bytesPerSymbol * charIndex];

        //Symbol data column mask
        maskColumn    = (1 << font->height) - 1;

        //y = y mod 8
        y &= 0x07;

        if (bColorIndex == 0)
        {
                if (fColorIndex == 0)
                {
                        //back/fore: black
                        column = ~(maskColumn << y);
                        while (coli < fontW)
                        {
                                pbuffer[bufOffset]         &= (u8)column;
                                pbuffer[bufOffset + width] &= (u8)(column >> 8);
                                bufOffset++;
                                coli++;
                        } //while (coli < fontW)
                }
                else //fColorIndex != 0
                {
                        //back/fore: black/white (default)
                        while (coli < fontW)
                        {
                                column |= (psymbol[rowi * fontW + coli] << (rowi << 3));
                                rowi++;

                                if (rowi == rowCount)
                                {
                                        column <<= y;

                                        pbuffer[bufOffset]         |= (u8)column;
                                        pbuffer[bufOffset + width] |= (u8)(column >> 8);

                                        column = 0;
                                        rowi = 0;
                                        bufOffset++;
                                        coli++;
                                }
                        } //while (coli < fontW)
                } //fColorIndex != 0
        }
        else //bColorIndex != 0
        {
                if (fColorIndex == 0)
                {
                        //back/fore: white/black
                        while (coli < fontW)
                        {
                                column |= (psymbol[rowi * fontW + coli] << (rowi << 3));
                                rowi++;

                                if (rowi == rowCount)
                                {
                                        column = (~column) & maskColumn;

                                        column <<= y;

                                        pbuffer[bufOffset]         |= (u8)column;
                                        pbuffer[bufOffset + width] |= (u8)(column >> 8);

                                        column = 0;
                                        rowi = 0;
                                        bufOffset++;
                                        coli++;
                                }
                        } //while (coli < fontW)
                }
                else //fColorIndex != 0
                {
                        //back/fore: white
                        column = maskColumn << y;
                        while (coli < fontW)
                        {
                                pbuffer[bufOffset]         |= (u8)column;
                                pbuffer[bufOffset + width] |= (u8)(column >> 8);
                                bufOffset++;
                                coli++;
                        } //while (coli < fontW)
                } //fColorIndex != 0
        } //bColorIndex != 0
}

/*===========================================================================
 * End of file screen-1b.c
 *===========================================================================*/

