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
 *  File screen-pr.h: Screen private definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __SCREEN_PR_H
#define __SCREEN_PR_H

#include "../../inc/screen.h"
#include "../../../bastypes.h"
#include "../../../std/inc/fonts.h"

/* Store pointers to color finctions */
typedef struct tag_ScreenFunctions
{
        s32 (*pfGetBufLength)(s32 width, s32 height);

        u32 (*pfColorToIndex)(const color_t *cTable, color_t color);

        color_t (*pfIndexToColor)(const color_t *cTable, u32 colorIndex);

        void (*pfSetPixelByOffset)(u8 *buffer, s32 offset, u32 color);

        u32 (*pfGetPixelByOffset)(u8 *buffer, s32 offset);

        void (*pfSetPixel)(u8 *buffer, s32 width, s32 x, s32 y, u32 colorIndex);

        u32 (*pfGetPixel)(u8 *buffer, s32 width, s32 x, s32 y);

        void (*pfDrawChar)(u32 charIndex, u8 *buffer, s32 width, s32 x, s32 y,
                             FONT *font, u32 fColorIndex, u32 bColorIndex);
} ScreenFunctions;


extern ScreenFunctions *scrFuncs[];

//ScreenParams::opts[27..24] - index at scrFuncs array
#define GetScreenFuncsPtr(scp)    (scrFuncs[(scp->opts >> 24)])


extern ScreenFunctions scrFuncs1b;
extern ScreenFunctions scrFuncs4b;
extern ScreenFunctions scrFuncs8b;
extern ScreenFunctions scrFuncs16b;
extern ScreenFunctions scrFuncs24b;
extern ScreenFunctions scrFuncs32b;

/*===========================================================================
 * 1-bit color functions
 *===========================================================================*/

/* */
s32 GetBufLength1b(s32 width, s32 height);

/* */
u32 ColorToIndex1b(const color_t *cTable, color_t color);

/* */
color_t IndexToColor1b(const color_t *cTable, u32 colorIndex);

/* */
void SetPixelByOffset1b(u8 *buffer, s32 offset, u32 colorIndex);

/* */
u32 GetPixelByOffset1b(u8 *buffer, s32 offset);

/* */
void SetPixel1b(u8 *buffer, s32 width, s32 x, s32 y, u32 colorIndex);

/* */
u32 GetPixel1b(u8 *buffer, s32 width, s32 x, s32 y);

/* */
void DrawChar1b(u32 charIndex,
                    u8 *buffer, s32 width, s32 x, s32 y,
                    FONT *font, u32 fColorIndex, u32 bColorIndex);

/*===========================================================================
 * 4-bit color functions
 *===========================================================================*/

/* */
s32 GetBufLength4b(s32 width, s32 height);

/* */
u32 ColorToIndex4b(const color_t *cTable, color_t color);

/* */
color_t IndexToColor4b(const color_t *cTable, u32 colorIndex);

/* */
void SetPixelByOffset4b(u8 *buffer, s32 offset, u32 colorIndex);

/* */
u32 GetPixelByOffset4b(u8 *buffer, s32 offset);

/* */
void SetPixel4b(u8 *buffer, s32 width, s32 x, s32 y, u32 colorIndex);

/* */
u32 GetPixel4b(u8 *buffer, s32 width, s32 x, s32 y);

/* */
void DrawChar4b(u32 charIndex,
                    u8 *buffer, s32 width, s32 x, s32 y,
                    FONT *font, u32 fColorIndex, u32 bColorIndex);

/*===========================================================================
 * 8-bit color functions
 *===========================================================================*/

/* */
s32 GetBufLength8b(s32 width, s32 height);

/* */
u32 ColorToIndex8b(const color_t *cTable, color_t color);

/* */
color_t IndexToColor8b(const color_t *cTable, u32 colorIndex);

/* */
void SetPixelByOffset8b(u8 *buffer, s32 offset, u32 colorIndex);

/* */
u32 GetPixelByOffset8b(u8 *buffer, s32 offset);

/* */
void SetPixel8b(u8 *buffer, s32 width, s32 x, s32 y, u32 colorIndex);

/* */
u32 GetPixel8b(u8 *buffer, s32 width, s32 x, s32 y);

/* */
void DrawChar8b(u32 charIndex,
                    u8 *buffer, s32 width, s32 x, s32 y,
                    FONT *font, u32 fColorIndex, u32 bColorIndex);

/*===========================================================================
 * 16-bit color functions
 *===========================================================================*/

/* */
s32 GetBufLength16b(s32 width, s32 height);

/* */
u32 ColorToIndex16b(const color_t *cTable, color_t color);

/* */
color_t IndexToColor16b(const color_t *cTable, u32 colorIndex);

/* */
void SetPixelByOffset16b(u8 *buffer, s32 offset, u32 colorIndex);

/* */
u32 GetPixelByOffset16b(u8 *buffer, s32 offset);

/* */
void SetPixel16b(u8 *buffer, s32 width, s32 x, s32 y, u32 colorIndex);

/* */
u32 GetPixel16b(u8 *buffer, s32 width, s32 x, s32 y);

/* */
void DrawChar16b(u32 charIndex,
                    u8 *buffer, s32 width, s32 x, s32 y,
                    FONT *font, u32 fColorIndex, u32 bColorIndex);

/*===========================================================================
 * 24-bit color functions
 *===========================================================================*/

/* */
s32 GetBufLength24b(s32 width, s32 height);

/* */
u32 ColorToIndex24b(const color_t *cTable, color_t color);

/* */
color_t IndexToColor24b(const color_t *cTable, u32 colorIndex);

/* */
void SetPixelByOffset24b(u8 *buffer, s32 offset, u32 colorIndex);

/* */
u32 GetPixelByOffset24b(u8 *buffer, s32 offset);

/* */
void SetPixel24b(u8 *buffer, s32 width, s32 x, s32 y, u32 colorIndex);

/* */
u32 GetPixel24b(u8 *buffer, s32 width, s32 x, s32 y);

/* */
void DrawChar24b(u32 charIndex,
                    u8 *buffer, s32 width, s32 x, s32 y,
                    FONT *font, u32 fColorIndex, u32 bColorIndex);

/*===========================================================================
 * 32-bit color functions
 *===========================================================================*/

/* */
s32 GetBufLength32b(s32 width, s32 height);

/* */
u32 ColorToIndex32b(const color_t *cTable, color_t color);

/* */
color_t IndexToColor32b(const color_t *cTable, u32 colorIndex);

/* */
void SetPixelByOffset32b(u8 *buffer, s32 offset, u32 colorIndex);

/* */
u32 GetPixelByOffset32b(u8 *buffer, s32 offset);

/* */
void SetPixel32b(u8 *buffer, s32 width, s32 x, s32 y, u32 colorIndex);

/* */
u32 GetPixel32b(u8 *buffer, s32 width, s32 x, s32 y);

/* */
void DrawChar32b(u32 charIndex,
                    u8 *buffer, s32 width, s32 x, s32 y,
                    FONT *font, u32 fColorIndex, u32 bColorIndex);

/*===========================================================================
 * Grpahic functions
 *===========================================================================*/

void FillBufferRegion(ScreenParams *scp, u8 *buffer,
                      color_t color,
                      s16 x, s16 y, s16 width, s16 height);

/*===========================================================================
 * Common functions
 *===========================================================================*/

/* Find nearest color at color table using specified color */
u32 ColorToIndexXb(const color_t *cTable, u32 length, color_t color);

#endif /* __SCREEN_PR_H */

/*===========================================================================
 * End of file screen-pr.h
 *===========================================================================*/

