/*===========================================================================
 *
 *  User interface library
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
 *  File ctrltypes.h: Common controls definitions.
 *
 *===========================================================================*/

#ifndef __CONTROLTYPES_H
#define __CONTROLTYPES_H

#include "../../../bastypes.h"
#include "../../std/inc/colors.h"
#include "../screen.h"

/*===========================================================================
 * Basic definitions
 *===========================================================================*/

//Control types (CtrlParams::type)
#define CTRL_TYPE_FORM                  0
#define CTRL_TYPE_PROGRESSBAR           1
#define CTRL_TYPE_SLIDERBAR             2
#define CTRL_TYPE_SCROLLBAR             3
#define CTRL_TYPE_TEXTBOX               4
#define CTRL_TYPE_MENUBAR               5
#define CTRL_TYPE_MENUITEM              6
#define CTRL_TYPE_BUTTON                7
#define CTRL_TYPE_RADIOBUTTON           8
#define CTRL_TYPE_CHECKBOX              9
//#define CTRL_TYPE_LABEL                 10 //Currently not implemented
#define CTRL_TYPE_LISTBOX               11
#define CTRL_TYPE_COMBOBOX              12
//#define CTRL_TYPE_COUNTER               13 //Currently not implemented
//#define CTRL_TYPE_CALENDAR              14 //Currently not implemented
//#define CTRL_TYPE_GRID                  15 //Currently not implemented
#define CTRL_TYPE_TASKBAR               16
#define CTRL_TYPE_FILELISTBOX           17

//Control options (CtrlParams::opts)
#define CTRL_OPTS_VISIBLE              (1 <<  0) //Visible (all controls)
#define CTRL_OPTS_ENABLED              (1 <<  1) //Enabled (all controls)
#define CTRL_OPTS_FOCUSED              (1 <<  2) //Focused (all controls)
#define CTRL_OPTS_VERTICAL             (1 <<  3) //Vertical (progress/slider bars)
#define CTRL_OPTS_SCBARV               (1 <<  4) //Show vertical scrollbar (text/listbox)
#define CTRL_OPTS_SCBARH               (1 <<  5) //Show horizontal scrollbar (textbox)
#define CTRL_OPTS_WORDWRAP             (1 <<  6) //Word wrap (textbox)
#define CTRL_OPTS_RDONLY               (1 <<  7) //Read only (textbox)
#define CTRL_OPTS_NOCURSOR             (1 <<  8) //Hide cursor (textbox)
#define CTRL_OPTS_DRAW_SHADOW          (1 <<  9) //Draw shadow (dialog, menubar, combobox)
#define CTRL_OPTS_CBLVISIBLE           (1 << 10) //Combobox list visible (combobox)
#define CTRL_OPTS_NONEINPUT            (1 << 11) //None input control (label/progress bar)
#define CTRL_OPTS_AUTOSIZE             (1 << 12) //Autosize (menubar, combobox)
#define CTRL_OPTS_CASCADE_MENU         (1 << 13) //Draw cascade menu (menubar)
#define CTRL_OPTS_IGNORE_PARENT_SIZE   (1 << 14) //Ignore parent size (scrollbar)
#define CTRL_OPTS_DLG_SHOWON_TASKBAR   (1 << 15) //Show dialog on taskabar
#define CTRL_OPTS_DLG_MINIMIZED        (1 << 16) //Dialog minimized
#define CTRL_OPTS_DLG_KEEP_ONTOP       (1 << 17) //Keep dialog on top
#define CTRL_OPTS_DLG_MODAL            (1 << 18) //Modal dialog: can't select other dialog
                                                 //while modal dialog opened
#define CTRL_OPTS_DLG_DESKTOP          (1 << 19) //Desktop dialog: can't minimized, moved
#define CTRL_OPTS_PWDBOX               (1 << 20) //Password textbox:
                                                 //hide text using specified char (*)

//Border types (CtrlParams::opts, bits [31..28])
#define CTRL_OPTS_BORDER_NONE          (0 << 28)  //No border
#define CTRL_OPTS_BORDER_SINGLE1       (1 << 28)  //Single (pseudo graphic)
#define CTRL_OPTS_BORDER_SINGLE2       (2 << 28)  //Single ('+-|' chars)
#define CTRL_OPTS_BORDER_TWIN          (3 << 28)  //Twin (pseudo graphic)
#define CTRL_OPTS_BORDER_BOLD1         (4 << 28)  //Bold #1
#define CTRL_OPTS_BORDER_BOLD2         (5 << 28)  //Bold #2
#define CTRL_OPTS_BORDER_BOLD3         (6 << 28)  //Bold #3
#define CTRL_OPTS_BORDER_BOLD4         (7 << 28)  //Bold #4
#define CTRL_OPTS_BORDER_MASK          (0x0F << 28) //mask
#define CTRL_OPTS_BORDER_BITS          (28)         //bits



//Default options: visible and enabled
#define CTRL_OPTS_DEFAULT       (CTRL_OPTS_VISIBLE | CTRL_OPTS_ENABLED | CTRL_OPTS_FOCUSED)

//ListBox/ComboBox draw item codes (ListBoxItemEventArgs::retcode)
#define LISTBOX_CODE_OK                 0 //OK, continue drawing
#define LISTBOX_CODE_SKIP               1 //Skip item (don't show)
#define LISTBOX_CODE_BREAKDRAW          2 //Skip item and break drawing


//Selected GUI style:
//0 - DOS style (default)
//1 - Windows style
#ifndef CTRL_SELECTED_STYLE
#define CTRL_SELECTED_STYLE             0
#endif


//Forecolor of shadowed region
#ifndef CTRL_SHADOW_FORECOLOR
#define CTRL_SHADOW_FORECOLOR           COL_GRAY
#endif

//Backcolor of shadowed region
#ifndef CTRL_SHADOW_BACKCOLOR
#define CTRL_SHADOW_BACKCOLOR           COL_BLACK
#endif

//Foreground color of disabled control
#ifndef CTRL_DISABLED_FORECOLOR
#define CTRL_DISABLED_FORECOLOR         COL_GRAY
#endif


#if (CTRL_SELECTED_STYLE == 1)
//Style=1 (Windows)
//Dialog title foreground color
#ifndef CTRL_DIALOG_TITLE_FORECOLOR
#define CTRL_DIALOG_TITLE_FORECOLOR         COL_WHITE
#endif

//Unfocused Dialog border/header color
#ifndef CTRL_DIALOG_UNFOCUSED_BORDERCOLOR
#define CTRL_DIALOG_UNFOCUSED_BORDERCOLOR   COL_GRAY
#endif

//Scrollbar foreground color
#ifndef CTRL_SCROLLBAR_FORECOLOR
#define CTRL_SCROLLBAR_FORECOLOR            COL_GRAY
#endif

#else
//Style=0 (DOS)
//No definitions here

#endif /* CTRL_SELECTED_STYLE == 1 */

/*===========================================================================
 * Enable/disable controls implementation
 *===========================================================================*/

#ifndef CTRLMOD_EN_PROGRESSBAR
#define CTRLMOD_EN_PROGRESSBAR           1
#endif

#ifndef CTRLMOD_EN_SLIDERBAR
#define CTRLMOD_EN_SLIDERBAR             1
#endif

#ifndef CTRLMOD_EN_SCROLLBAR
#define CTRLMOD_EN_SCROLLBAR             1
#endif

#ifndef CTRLMOD_EN_TEXTBOX
#define CTRLMOD_EN_TEXTBOX               1
#endif

#ifndef CTRLMOD_EN_MENUBAR
#define CTRLMOD_EN_MENUBAR               1
#endif

#ifndef CTRLMOD_EN_BUTTON
#define CTRLMOD_EN_BUTTON                1
#endif

#ifndef CTRLMOD_EN_RADIOBUTTON
#define CTRLMOD_EN_RADIOBUTTON           1
#endif

#ifndef CTRLMOD_EN_CHECKBOX
#define CTRLMOD_EN_CHECKBOX              1
#endif

#ifndef CTRLMOD_EN_LISTBOX
#define CTRLMOD_EN_LISTBOX               1
#endif

#ifndef CTRLMOD_EN_COMBOBOX
#define CTRLMOD_EN_COMBOBOX              1
#endif

#ifndef CTRLMOD_EN_TASKBAR
#define CTRLMOD_EN_TASKBAR               1
#endif

#ifndef CTRLMOD_EN_FILELISTBOX
#define CTRLMOD_EN_FILELISTBOX           1
#endif

/*===========================================================================
 * Basic control structures
 *===========================================================================*/

/* Control location */
typedef struct tag_CtrlLocation
{
    //Upper-left corner position by X-axis
    s16 x;

    //Upper-left corner position by Y-axis
    s16 y;

    //Width
    s16 width;

    //Height
    s16 height;
} CtrlLocation;


typedef struct tag_CtrlParams CtrlParams;

//Control dimensions and position
typedef struct tag_CtrlParams
{
    //Control type (see CTRL_TYPE_x)
    u16 type;

    //Control identifier (user-defined)
    u16 id;

    //Upper-left corner position by X-axis
    s16 x;

    //Upper-left corner position by Y-axis
    s16 y;

    //Width
    s16 width;

    //Height
    s16 height;

    //Foreground color (for colors see COL_x at "colors.h")
    color_t foreColor;

    //Background color
    color_t backColor;

    //Selection fore color
    color_t selForeColor;

    //Selection back color
    color_t selBackColor;

    //Disabled fore color
    color_t disForeColor;

    //Border fore color
    color_t borderColor;

    //Control options (see CTRL_OPTS_x)
    u32 opts;

    //Pointer to parent control parameters
    CtrlParams *parent;

    //User-defined data associated with this control
    void *tag;

    //Pointer to function to draw
    void (*pfDrawCtrl)(ScreenParams *scp, CtrlParams *ctrl);
} CtrlParams;


/* */
typedef struct tag_CtrlChainItem
{
    //Pointer to current control
    CtrlParams *control;
} CtrlChainItem;


/* */
typedef struct tag_ListBoxItemEventArgs
{
    //Index of item to redraw (in)
    s32 index;

    //Pointer to item (in)
    const void *pItem;

    //Pointer to string buffer that stored string representation of item (in/out)
    char *strbuf;

    //User-defined fore color of current item
    color_t itemForeColor;

    //User-defined background color of current item
    color_t itemBackColor;

    //Code that controls redrawing action (out, see LISTBOX_CODE_x)
    u32 retcode;

} ListBoxItemEventArgs;



/* Gets full offset of an element on the screen. */
void GetFullOffset(CtrlParams *ctrlpar, s16 *xo, s16 *yo);

#endif /* __CONTROLTYPES_H */

/*===========================================================================
 * End of file ctrltypes.h
 *===========================================================================*/
