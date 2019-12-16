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
 *  File CheckBox.h: Check box definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __CHECKBOX_H
#define __CHECKBOX_H

#include "../../../bastypes.h"
#include "../screen.h"
#include "ctrltypes.h"

//Checkbox definitons
//Char that indicates that checkbox is checked
#ifndef CHECKBOX_CHCODE_SELECT
#define CHECKBOX_CHCODE_SELECT      0x78 //'x' char
#endif

#ifndef CHECKBOX_CHCODE_NOSELECT
#define CHECKBOX_CHCODE_NOSELECT    0x20 //' ' char
#endif

typedef struct tag_CheckBox CheckBox;

/* */
typedef struct tag_CheckBox
{
    //Basic control parameters
    CtrlParams control;

    //Text
    const char *text;

    //Check state changed event handler
    void (*check_changed_event)(CheckBox *checkBox, void *args);

    //Indicates that state is checked
    u8 checked;
} CheckBox;


/* Draw checkbox */
void DrawCheckBox(ScreenParams *scp, CtrlParams *ctrl);

/* Toggle checkbox state */
void ToggleCheckBox(CheckBox *checkBox);

#endif /* __CHECKBOX_H */

/*===========================================================================
 * End of file CheckBox.h
 *===========================================================================*/
