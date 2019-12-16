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
 *  File ComboBox.h: Combobox definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __COMBOBOX_H
#define __COMBOBOX_H

#include "../../../bastypes.h"
#include "../../std/inc/KeyCode.h"
#include "../screen.h"
#include "ctrltypes.h"
#include "ScrollBar.h"

typedef struct tag_ComboBox ComboBox;

/* Combobox control structure */
typedef struct tag_ComboBox
{
    //Basic control parameters
    CtrlParams control;

    //Scroll bar vertical
    ScrollBar sbv;    

    //Pointer to first item
    //Must: pFirstItem <= pEndItem
    const void *pFirstItem;

    //Pointer to item that next after last item
    //Must: pFirstItem <= pEndItem
    const void *pEndItem;

    //Size of list item, bytes
    s32 itemSize;

    //Selected index (zero-based)
    s32 selIndex;

    //Scroll by vertical, count of items (zero-based, index of first visible item)
    s32 scrollV;

    //Redraw combobox item event handler
    void (*redraw_event)(ComboBox *comboBox, ListBoxItemEventArgs *args);

    //Click event handler
    void (*click_event)(ComboBox *comboBox, ListBoxItemEventArgs *args);

    //Selection changed
    void (*selection_changed_event)(ComboBox *comboBox, ListBoxItemEventArgs *args);
} ComboBox;


/* Draw combobox */
void DrawComboBox(ScreenParams *scp, CtrlParams *ctrl);

/* Key handler: scroll combobox, select item */
u32 ComboBoxKeyHandler(ComboBox *comboBox, KeyCode key, u32 locks);

/* Toggle combobox list */
void ToggleComboBoxList(ComboBox *comboBox);

/* Scroll combobox vertically */
void ScrollComboBoxV(ComboBox *comboBox, s32 value);

/* Scroll combobox to selected item */
void ScrollComboBoxToSelectedItem(ComboBox *comboBox);

/* Update combobox */
void UpdateComboBox(ComboBox *comboBox);

#endif /* __COMBOBOX_H */

/*===========================================================================
 * End of file ComboBox.h
 *===========================================================================*/
