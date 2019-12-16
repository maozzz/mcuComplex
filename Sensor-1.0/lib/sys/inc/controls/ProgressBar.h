/*===========================================================================
 *
 *  User interface library
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
 *  File ProgressBar.h: Progress bar definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __PROGRESSBAR_H
#define __PROGRESSBAR_H

#include "../../../bastypes.h"
#include "../screen.h"
#include "ctrltypes.h"

//Vertical bar defintions
#ifndef PRBARV_CHCODE_PROGRESS
#define PRBARV_CHCODE_PROGRESS   0xAC
#endif

#ifndef PRBARV_CHCODE_EMPTY
#define PRBARV_CHCODE_EMPTY      0x81
#endif


//Horizontal bar defintions
#ifndef PRBARH_CHCODE_PROGRESS
#define PRBARH_CHCODE_PROGRESS   0xAC
#endif

#ifndef PRBARH_CHCODE_EMPTY
#define PRBARH_CHCODE_EMPTY      0x81
#endif

typedef struct tag_ProgressBar ProgressBar;

/* */
typedef struct tag_ProgressBar
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
    void (*position_changed_event)(ProgressBar *progressBar, void *args);
} ProgressBar;


/* Draw progress bar */
void DrawProgressBar(ScreenParams *scp, CtrlParams *ctrl);

/* */
void ScrollProgressBar(ProgressBar *progressBar, s32 value);

#endif /* __PROGRESSBAR_H */

/*===========================================================================
 * End of file ProgressBar.h
 *===========================================================================*/

