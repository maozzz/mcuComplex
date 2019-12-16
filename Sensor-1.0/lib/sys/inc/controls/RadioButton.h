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
 *  File RadioButton.h: Radio button definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __RADIOBUTTON_H
#define __RADIOBUTTON_H

#include "../../../bastypes.h"
#include "../screen.h"
#include "ctrltypes.h"

//Radio button definitons
#ifndef RADIOBUTTON_CHCODE_SELECT
#define RADIOBUTTON_CHCODE_SELECT      0x2A //'*' char
#endif

#ifndef RADIOBUTTON_CHCODE_NOSELECT
#define RADIOBUTTON_CHCODE_NOSELECT    0x20 //' ' char
#endif

typedef struct tag_RadioButton  RadioButton;

/* */
typedef struct tag_RadioButton
{
    //Basic control parameters
    CtrlParams control;

    //Text
    const char *text;

    //Check state changed
    void (*check_changed_event)(RadioButton *radioButton, void *args);

    //Group of radio buttons (pointer to array)
    CtrlChainItem *group;

    //Count of radio buttons at group
    s32 groupCount;

    //Check state
    u8 checked;
} RadioButton;

/* */
void DrawRadioButton(ScreenParams *scp, CtrlParams *ctrl);

/* */
void ToggleRadioButton(RadioButton *radioButton);

/* */
void SetRadioButtonGroup(RadioButton *radioButton, CtrlChainItem *group, s32 count);

#endif /* __RADIOBUTTON_H */

/*===========================================================================
 * End of file RadioButton.h
 *===========================================================================*/
