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
 *  File screen-16b.c: 16-bits screen functions.
 *
 *  Buffer mapping:
 *    X      0 2     width-1
 *  Y
 *  0        nnnn...nn => 'n' is 2 bytes => 1 pixel: [x,y];
 *  1        nnnn...nn    'n' is packed 32-bit color:
 *           .........    0xAARRGGBB[31..0] => 0xA[15]R[14..10]G[9..5]B[4..0]
 *  height-1 nnnn...nn    (R,G,B: 5 major bits of each 8-bit component)
 *
 *===========================================================================*/

#include "../../inc/screen.h"
#include "screen-pr.h"

ScreenFunctions scrFuncs16b =
{
        &GetBufLength16b,
        &ColorToIndex16b,
        &IndexToColor16b,
        &SetPixelByOffset16b,
        &GetPixelByOffset16b,
        &SetPixel16b,
        &GetPixel16b,
        &DrawChar16b
};

/* */
s32 GetBufLength16b(s32 width, s32 height)
{
        return (width * height) << 1;
}


/* */
u32 ColorToIndex16b(const color_t *cTable, color_t color)
{
        //color: 0xAARRGGBB
        //Convert 8 bits to 5 bits per component
        return
         ((color & 0xFF000000) ? (1 << 15) : 0) | //A
         ((color >> 9) & (0x1F << 10)) |          //R
         ((color >> 6) & (0x1F << 5))  |          //G
         ((color >> 3) & 0x1F);                   //B
}


/* */
color_t IndexToColor16b(const color_t *cTable, u32 colorIndex)
{
        return
        ((colorIndex & (1 << 15)) ? (0xFF << 24) : 0) | //A
        ((colorIndex & (0x1F << 10)) << 9)  | //R
        ((colorIndex & (0x1F << 5))  << 6)  | //G
        ((colorIndex & 0x1F)         << 3);   //B
}


/* */
void SetPixelByOffset16b(u8 *buffer, s32 offset, u32 colorIndex)
{
        u8 *p = &buffer[(offset << 1)];
        p[0] = (u8)colorIndex;
        p[1] = (u8)(colorIndex >> 8);
}


/* */
u32 GetPixelByOffset16b(u8 *buffer, s32 offset)
{
        u8 *p = &buffer[(offset << 1)];
        return ((p[1] << 8) | p[0]);
}


/* */
void SetPixel16b(u8 *buffer, s32 width, s32 x, s32 y, u32 colorIndex)
{
        u8 *p = &buffer[(y * width + x) << 1];
        p[0] = (u8)colorIndex;
        p[1] = (u8)(colorIndex >> 8);
}


/* */
u32 GetPixel16b(u8 *buffer, s32 width, s32 x, s32 y)
{
        u8 *p = &buffer[(y * width + x) << 1];
        return ((p[1] << 8) | p[0]);
}



/* */
void DrawChar16b(u32 charIndex,
                u8 *buffer, s32 width, s32 x, s32 y,
                FONT *font, u32 fColorIndex, u32 bColorIndex)
{
        u32 fontW, fontH, rowCount;
        u32 coli = 0, rowi = 0, column = 0, hi;
        s32 pixOffset, curOffset;
        u8 *ci;
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
        pixOffset     = (y * width + x) << 1;

        while (coli < fontW)
        {
                column |= (psymbol[rowi * fontW + coli] << (rowi << 3));
                rowi++;

                if (rowi == rowCount)
                {
                        hi = 0;
                        curOffset = pixOffset + (coli << 1);

                        while(hi++ < fontH)
                        {
                                //Select fore/back color
                                ci = (column & 1) ?
                                     (u8*)&fColorIndex :
                                     (u8*)&bColorIndex;

                                //Copy color to buffer
                                buffer[curOffset]     = ci[0];
                                buffer[curOffset + 1] = ci[1];

                                //Get offset to next line
                                curOffset += (width << 1);

                                column >>= 1;
                        }

                        column = 0;
                        rowi = 0;
                        coli++;
                }
        } //while (coli < fontW)
}

/*===========================================================================
 * End of file screen-16b.c
 *===========================================================================*/

