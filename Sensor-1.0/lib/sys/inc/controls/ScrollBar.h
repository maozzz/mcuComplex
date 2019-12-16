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
 *  File ScrollBar.h: Scroll bar definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __SCROLLBAR_H
#define __SCROLLBAR_H

#include "../../../bastypes.h"
#include "../screen.h"
#include "ctrltypes.h"

#define SCBARV_CHCODE_UP        0x1E
#define SCBARV_CHCODE_DOWN      0x1F
#define SCBARV_CHCODE_BODY      0x81
#define SCBARV_CHCODE_SCROLL    0xAC

#define SCBARH_CHCODE_LEFT      0x11
#define SCBARH_CHCODE_RIGHT     0x10
#define SCBARH_CHCODE_BODY      0x81
#define SCBARH_CHCODE_SCROLL    0xAC

typedef struct tag_ScrollBar
{
    //Scroll bar parameters
    CtrlParams control;

    //Scroll slider position
    s32 position;

    //Scroll slider length
    s32 length;
} ScrollBar;


/* Draw scroll bar */
void DrawScrollBar(ScreenParams *scp, CtrlParams *ctrl);

/* */
void SetScrollValue(ScrollBar *scrollBar, s32 position, s32 window, s32 count);

#endif /* __SCROLLBAR_H */

/*===========================================================================
 * End of file ScrollBar.h
 *===========================================================================*/

