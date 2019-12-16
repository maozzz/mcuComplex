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
 *  File FileListBox.h: File list box definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __FILELISTBOX_H
#define __FILELISTBOX_H

#include "../../../bastypes.h"
#include "../../std/inc/KeyCode.h"
#include "../screen.h"
#include "../filesys.h"
#include "ctrltypes.h"
#include "ScrollBar.h"

//File listbox definitions
//Char code that indicates that current item is a directory
#ifndef FILELISTBOX_CHCODE_DIRECTORY
#define FILELISTBOX_CHCODE_DIRECTORY      '/' //slash by default
#endif

//Char code that indicates that current item is a file
#ifndef FILELISTBOX_CHCODE_FILE
#define FILELISTBOX_CHCODE_FILE           ' ' //space by default
#endif

typedef struct tag_FileListBox FileListBox;

/* Listbox control structure */
typedef struct tag_FileListBox
{
    //Basic control parameters
    CtrlParams control;   

    //Header string (column names).
    //Set to 0 to disable
    const char *pHeaderStr;

    //Directory item foreground color
    color_t dirForeColor;

    //Offset of first visible record
    foff_t fvisOffset;

    //Offset of selected record
    foff_t selOffset;

    //Pointer to directory stream
    FSDIR *hfDir;        

    //Key handler
    u32 (*key_pressed_event)(FileListBox *fileListBox, KeyCode key, u32 locks);

    //Redraw item event handler
    void (*redraw_event)(FileListBox *fileListBox, ListBoxItemEventArgs *args);

    //Click event handler
    void (*click_event)(FileListBox *fileListBox, ListBoxItemEventArgs *args);

    //Selection changed event handler
    void (*selection_changed_event)(FileListBox *fileListBox, ListBoxItemEventArgs *args);

} FileListBox;


/* Draw file listbox */
void DrawFileListBox(ScreenParams *scp, CtrlParams *ctrl);

/* Key handler: scroll file listbox, select item */
u32 FileListBoxKeyHandler(FileListBox *fileListBox, KeyCode key, u32 locks);

/* Scroll file listbox to one item up or down */
u32 ScrollFileListBoxStepV(FileListBox *fileListBox, s32 step);

/* Scroll file listbox vertically */
void ScrollFileListBoxV(FileListBox *fileListBox, s32 value);

/* Update file listbox */
void UpdateFileListBox(FileListBox *fileListBox);

#endif /* __FILELISTBOX_H */

/*===========================================================================
 * End of file FileListBox.h
 *===========================================================================*/
