/*===========================================================================
 *
 *  User interface library
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2019
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File PButton.h: Button definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __PBUTTON_H
#define __PBUTTON_H

#include "../../../bastypes.h"
#include "../../../std/inc/KeyCode.h"
#include "../screen.h"
#include "ctrltypes.h"

typedef struct tag_PButton PButton;

typedef struct tag_PButton
{
    //Basic control parameters
    CtrlParams control;

    //Button text
    const char *text;    

    //Hot key
    KeyCode hotkey;

    //Click event handler
    void (*click_event)(PButton *button, void *args);
} PButton;


/* Draw button */
void DrawPButton(ScreenParams *scp, CtrlParams *ctrl);

#endif /* __PBUTTON_H */

/*===========================================================================
 * End of file PButton.h
 *===========================================================================*/
