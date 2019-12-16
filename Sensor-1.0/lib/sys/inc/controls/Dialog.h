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
 *  File Dialog.h: Dialog definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __GUIDIALOG_H
#define __GUIDIALOG_H

#include "../../../bastypes.h"
#include "../../std/inc/KeyCode.h"
#include "../screen.h"
#include "ctrltypes.h"
#include "PButton.h"
#include "MenuBar.h"

//Dialog title maximum length, chars
#ifndef GUIDIALOG_TITLE_MAXLENGTH
#define GUIDIALOG_TITLE_MAXLENGTH       64
#endif /* GUIDIALOG_TITLE_MAXLENGTH */

//Dialog menu char (mark at top left corner)
#ifndef GUIDIALOG_BTNMENU_CHAR
#define GUIDIALOG_BTNMENU_CHAR          0xBE
#endif

//Minimize dialog char
#ifndef GUIDIALOG_BTNMINIMIZE_CHAR
#define GUIDIALOG_BTNMINIMIZE_CHAR      '_'
#endif

//Maximize dialog char
#ifndef GUIDIALOG_BTNMAXIMIZE_CHAR
#define GUIDIALOG_BTNMAXIMIZE_CHAR      0xBE
#endif

//Close dialog char
#ifndef GUIDIALOG_BTNCLOSE_CHAR
#define GUIDIALOG_BTNCLOSE_CHAR         'x'
#endif

//Maximum width of taskbar button, chars
#ifndef GUITASKBAR_BUTTON_WIDTH
#define GUITASKBAR_BUTTON_WIDTH         14
#endif /* GUITASKBAR_BUTTON_WIDTH */

//Taskbar buttons separator
#ifndef GUITASKBAR_BUTTON_SEPARATOR
#define GUITASKBAR_BUTTON_SEPARATOR     0x83 //Variants: 0x8A, '|'
#endif /* GUITASKBAR_BUTTON_SEPARATOR */

//Default dialog title on taskbar
#ifndef GUITASKBAR_DEFAULT_TITLE
#define GUITASKBAR_DEFAULT_TITLE         "Dialog"
#endif /* GUITASKBAR_DEFAULT_TITLE */


//Enable dialog menu module
#ifndef GUIDIALOG_EN_DIALOGMENU
#define GUIDIALOG_EN_DIALOGMENU         1
#endif

//Enable startup menu module
#ifndef GUITASKBAR_EN_STARTUPMENU
#define GUITASKBAR_EN_STARTUPMENU       1
#endif


//Dialog menu items (GUIDialog::dlgMenuItems)
#define GUIDIALOG_MENU_MOVE         (1 << 0) //Move dialog
#define GUIDIALOG_MENU_RESIZE       (1 << 1) //Resize dialog
#define GUIDIALOG_MENU_KEEPONTOP    (1 << 2) //Keep on top
#define GUIDIALOG_MENU_MINIMIZE     (1 << 3) //Minimize
#define GUIDIALOG_MENU_MAXIMIZE     (1 << 4) //Maximize
#define GUIDIALOG_MENU_CLOSE        (1 << 5) //Close dialog
#define GUIDIALOG_MENU_MASK         (0x3F)   //mask


/* Dialog input state */
enum GUIDialogState
{
    //Access to dialog controls, default size/position
    DLG_STATE_DEFAULT   = 0,

    //Access to dialog controls, maximized size (full screen)
    DLG_STATE_MAXIMIZED = 1,

    //No access to dialog controls, move dialog
    DLG_STATE_MOVING    = 2,

    //No access to dialog controls, resize dialog
    DLG_STATE_RESIZING  = 3
};



/* Message box buttons variants */
enum MessageBoxButtons
{
    MsgBoxBtn_Ok               = 0,
    MsgBoxBtn_OkCancel         = 1,
    MsgBoxBtn_YesNo            = 2,
    MsgBoxBtn_YesNoCancel      = 3,
    MsgBoxBtn_AbortRetryIgnore = 4,
    MsgBoxBtn_Abort            = 5
};


/* Type of message box */
enum MessageBoxType
{
    //Message box with text message
    MsgBoxType_Default  = 0,

    //Message box with input field
    MsgBoxType_Input    = 1,

    //Message box with text message and progress bar
    MsgBoxType_Progress = 2,
};


/* Dialog result code */
enum DialogResult
{
    //Nothing is returned from the dialog box
    DialogResult_None = 0,

    //The dialog box return value is OK (usually sent from a button "OK")
    DialogResult_OK = 1,

    //The dialog box return value is Cancel (usually sent from a button "Cancel")
    DialogResult_Cancel = 2,

    //The dialog box return value is Abort (usually sent from a button "Abort")
    DialogResult_Abort = 3,

    //The dialog box return value is Retry (usually sent from a button "Retry")
    DialogResult_Retry = 4,

    //The dialog box return value is Ignore (usually sent from a button "Ignore")
    DialogResult_Ignore = 5,

    //The dialog box return value is Yes (usually sent from a button "Yes")
    DialogResult_Yes = 6,

    //The dialog box return value is No (usually sent from a button "No")
    DialogResult_No = 7
};



/* Closig event arguments */
typedef struct tag_ClosingEventArgs
{
    //Indicates that closing operation canceled by user
    u32 cancel;    

    //Dialog return code
    enum DialogResult result;

    //Dialog return data
    const char *data;
} ClosingEventArgs;


typedef struct tag_GUIDialog GUIDialog;

/* Dialog control structure */
typedef struct tag_GUIDialog
{
    //Parameters of current control (dialog)
    CtrlParams control;

    //Saved location
    CtrlLocation location;

    //Pointer to acceptable button
    PButton *acceptButton;

    //Pointer to child controls array
    CtrlChainItem *childs;

    //Count of child controls
    s32 childCount;

    //Dialog menu items (see GUIDIALOG_MENU_x)
    u32 dlgMenuItems;

    //Dialog title
    const char *title;

    //Modification state: move or resize
    enum GUIDialogState modState;

    //Size state: minimized, maximized or defined size
    enum GUIDialogState locState;

    //Pointer to load event handler
    void (*load_event)(GUIDialog *pDialog, void *args);

    //Pointer to closing event handler
    void (*closing_event)(GUIDialog *pDialog, ClosingEventArgs *args);

    //Pointer to redraw event handler
    void (*redraw_event)(GUIDialog *pDialog, void *args);

    //Pointer to state change event handler
    void (*state_change_event)(GUIDialog *pDialog, void *args);

    //Key handler
    u32 (*key_pressed_event)(GUIDialog *pDialog, KeyCode key, u32 locks);
} GUIDialog;



typedef struct tag_GUITaskBar GUITaskBar;

/* Taskbar control structure */
typedef struct tag_GUITaskBar
{
    //Parameters of current control (Taskbar)
    CtrlParams control;

    //
    s16 userInfoWidth;

    //
    s16 reserved;

    //Pointer to redraw event handler
    void (*redraw_event)(GUITaskBar *pTaskBar, void *args);
} GUITaskBar;


/* Types of data that stored at clipboard */
enum GUIClipboardDataType
{
    //Plain text
    ClipData_Text = 0,
    ClipData_Path = 1
};


/* Global state */
typedef struct tag_GUIGlobalState
{
    //Pointer to clipboard buffer
    u8 *pClipboard;

    //Length of data that stored at clipboard buffer
    s32 clipboarLength;

    //Maximum clipboard buffer length, bytes
    s32 clipboarMaxLength;

    //Type of data that stored at clipboard
    enum GUIClipboardDataType clipboarDataType;

    //Pointer to redraw event handler
    GUITaskBar taskBar;

    //Dialog menu (common for all dialogs)
    MenuBar dialogMenu;

    //Startup menu
    MenuBar startupMenu;

    //Pointer to current screen
    ScreenParams *pScreen;

    //Pointer to dialogs array
    GUIDialog *pDialogs;

    //Count of dialogs at "dialogs" array
    s32 dialogsCount;
} GUIGlobalState;

extern GUIGlobalState guiGlobalState;



/* */
void InitializeGUISubsystem();

/* */
void InitializeControlParameters(CtrlParams *control,
                                 u16 type,   //Control type (see CTRL_TYPE_x)
                                 u16 id,     //Control identifier
                                 s16 x,      //Upper-left corner position by X-axis
                                 s16 y,      //Upper-left corner position by Y-axis
                                 s16 width,  //Width
                                 s16 height, //Height
                                 color_t foreColor,  //Foreground color (see "colors.h")
                                 color_t backColor,  //Background color (see "colors.h")
                                 u32 opts,           //Control options (see CTRL_OPTS_x)
                                 CtrlParams *parent); //Pointer to parent control parameters

/* */
void SetChildControls(GUIDialog *pDialog, CtrlChainItem *child, s32 count);


/* Open dialog */
void GUIOpenDialog(GUIDialog *pDialog);

/* Close dialog */
void GUICloseDialog(GUIDialog *pDialog, ClosingEventArgs *args);

/* Keyboard input handler
   "locks" indicates that Caps, Scroll, Num locks is on (see GUI_LOCK_x) */
void GUIInputHandler(KeyCode key, u32 locks);


/* Draw dialog region */
void GUIDrawDialog(ScreenParams *scp, GUIDialog *pDialog);

/* Redraw dialogs array */
void GUIDrawDialogs();


/* Move focus to next/previous control.
   dir > 0: next; dir < 0: previous */
CtrlParams *GUISelectNextControl(s32 dir);

/* Get pointer to focused control  */
CtrlParams *GUIGetFocusedControl();

/* Set focus to specified control */
void GUISetFocusedControl(CtrlParams *ctrl);


/* Move focus to next dialog */
GUIDialog *GUISelectNextDialog(u32 isSkipMinimized);

/* Get pointer to focused dialog */
GUIDialog *GUIGetFocusedDialog();

/* */
void GUICopyFromControl(CtrlParams *src);

/* */
void GUIPasteToControl(CtrlParams *dest);

/* Cascade dialogs */
void GUICascadeDialogs();

/* Minimize all dialogs */
void GUIMinimizeAllDialogs();

/* */
#if defined(__GNUC__)
#define _FMTFUNC_ATTR(attrs) __attribute__ (attrs)
#else
#define _FMTFUNC_ATTR(attrs)
#endif

/* Draw string line on dialog using dialog colors */
_FMTFUNC_ATTR((__format__(__printf__, 6, 7)))
void DrawDialogString(ScreenParams *scp, GUIDialog *pDialog,
                      s32 x, s32 y, s32 width,
                      const char *format, ...);

/* Draw string line on dialog using specified foreground and background colors */
_FMTFUNC_ATTR((__format__(__printf__, 8, 9)))
void DrawDialogColorString(ScreenParams *scp, GUIDialog *pDialog,
                      s32 x, s32 y, s32 width,
                      color_t fColor, color_t bColor,
                      const char *format, ...);

#endif /* __GUIDIALOG_H */

/*===========================================================================
 * End of file Dialog.h
 *===========================================================================*/
