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
 *  File ListBox.h: List box definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __LISTBOX_H
#define __LISTBOX_H

#include "../../../bastypes.h"
#include "../../std/inc/KeyCode.h"
#include "../screen.h"
#include "ctrltypes.h"
#include "ScrollBar.h"


typedef struct tag_ListBox ListBox;

/* Listbox control structure */
typedef struct tag_ListBox
{
    //Basic control parameters
    CtrlParams control;

    //Vertical scrollbar
    ScrollBar sbv;

    //Horizontal scrollbar
    ScrollBar sbh;

    //Header string (column names).
    //Set to 0 to disable
    const char *pHeaderStr;

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

    //Scroll by horizontal, count of chars
    s32 scrollH;

    //Maximum length of item string representation (need for hor.scrolling)
    s32 maxItemStrWidth;

    //Key handler
    u32 (*key_pressed_event)(ListBox *listBox, KeyCode key, u32 locks);

    //Redraw item event handler
    void (*redraw_event)(ListBox *listBox, ListBoxItemEventArgs *args);

    //Click event handler
    void (*click_event)(ListBox *listBox, ListBoxItemEventArgs *args);

    //Selection changed event handler
    void (*selection_changed_event)(ListBox *listBox, ListBoxItemEventArgs *args);
} ListBox;


/* Draw listbox */
void DrawListBox(ScreenParams *scp, CtrlParams *ctrl);

/* Key handler: scroll listbox, select item */
u32 ListBoxKeyHandler(ListBox *listBox, KeyCode key, u32 locks);

/* Scroll listbox by vertically */
void ScrollListBoxV(ListBox *listBox, s32 value);

/* Scroll listbox by horizontally */
void ScrollListBoxH(ListBox *listBox, s32 value);

/* Scroll listbox to selected item */
void ScrollListBoxToSelectedItem(ListBox *listBox);

/* Update listbox */
void UpdateListBox(ListBox *listBox);

#endif /* __LISTBOX_H */

/*===========================================================================
 * End of file ListBox.h
 *===========================================================================*/
