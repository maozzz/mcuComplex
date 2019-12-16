/*===========================================================================
 *
 *  User interface library.
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2014-2019
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File SliderBar.h. Slider bar definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __SLIDERBAR_H
#define __SLIDERBAR_H

#include "../../../bastypes.h"
#include "../../std/inc/KeyCode.h"
#include "../screen.h"
#include "ctrltypes.h"

//Horizontal slider defintions
#ifndef SLBARH_CHCODE_CARET
#define SLBARH_CHCODE_CARET      0xAC
#endif

#ifndef SLBARH_CHCODE_CARET_SEL
#define SLBARH_CHCODE_CARET_SEL  0x81
#endif

#ifndef SLBARH_CHCODE_TRACK
#define SLBARH_CHCODE_TRACK      0x9D
#endif


//Vertical slider definitions
#ifndef SLBARV_CHCODE_CARET
#define SLBARV_CHCODE_CARET      0xAC
#endif

#ifndef SLBARV_CHCODE_CARET_SEL
#define SLBARV_CHCODE_CARET_SEL  0x81
#endif

#ifndef SLBARV_CHCODE_TRACK
#define SLBARV_CHCODE_TRACK      0x8A
#endif

typedef struct tag_SliderBar SliderBar;

/* */
typedef struct tag_SliderBar
{
    //Control parameters
    CtrlParams control;

    //Minimum value
    s32 minimum;

    //Maximum value
    s32 maximum;

    //Current value
    s32 value;

    //Scroll step
    s32 step;

    //Event handler
    void (*position_changed_event)(SliderBar *sliderBar, void *args);
} SliderBar;

/* Draw slider bar */
void DrawSliderBar(ScreenParams *scp, CtrlParams *ctrl);

/* Key handler: scroll sliderbar */
u32 SliderBarKeyHandler(SliderBar *sliderBar, KeyCode key, u32 locks);

/* Scroll sliderbar */
void ScrollSliderBar(SliderBar *sliderBar, s32 value);

#endif

/*===========================================================================
 * End of file SliderBar.h
 *===========================================================================*/

