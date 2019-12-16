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
 *  File Dialog.c: Dialog functions.
 *
 *===========================================================================*/

#include "../../inc/controls/Dialog.h"
#include "../../inc/controls.h"
#include "controls-pr.h"
#include <string.h>

#ifndef __CONTROLS_DBG_MSG
#define __CONTROLS_DBG_MSG 0
#endif

#if (__CONTROLS_DBG_MSG == 1)
//Debug messages enabled
#include <stdio.h>
//Print to stdout
#define PrintDbg(...)       printf(__VA_ARGS__);

#else
//Debug messages disabled
#define PrintDbg(...)
#endif

/* */
static void UpdateChildControls(GUIDialog *pDialog);


//Control/dialog can accept user input: must visible, enabled, focused and input enabled
#define CTRL_OPTS_EDITABLE        (CTRL_OPTS_VISIBLE | CTRL_OPTS_ENABLED | CTRL_OPTS_FOCUSED)
#define CTRL_OPTS_EDITABLE_MASK   (CTRL_OPTS_NONEINPUT | CTRL_OPTS_EDITABLE)

//Control can focused: must visible, enabled and input enabled
#define CTRL_OPTS_CANFOCUSED        (CTRL_OPTS_VISIBLE | CTRL_OPTS_ENABLED)
#define CTRL_OPTS_CANFOCUSED_MASK   (CTRL_OPTS_NONEINPUT | CTRL_OPTS_CANFOCUSED)

//Global GUI state
GUIGlobalState guiGlobalState;


/* Save dialog size and position */
static void SaveDialogLocation(GUIDialog *pDialog)
{
    if (!pDialog)
        return;

    pDialog->location.x = pDialog->control.x;
    pDialog->location.y = pDialog->control.y;
    pDialog->location.width = pDialog->control.width;
    pDialog->location.height = pDialog->control.height;
}

/*===========================================================================
 * DIALOG MENU FUNCTIONS ADN DEFINITIONS (ITS NOT USER-DEFINED MENU)
 *===========================================================================*/

#if (CTRLMOD_EN_MENUBAR == 1 && GUIDIALOG_EN_DIALOGMENU == 1)

//Dialog menu IDs
#define MIDLG_ID_MOVE       0
#define MIDLG_ID_RESIZE     1
#define MIDLG_ID_KEEPONTOP  2
#define MIDLG_ID_MINIMIZE   3
#define MIDLG_ID_MAXIMIZE   4
#define MIDLG_ID_CLOSE      5

//Count of dialog menu items
#define DLGMENU_ITEMS_COUNT 6

//Dialog menu position on parent dialog, chars
#define DLGMENU_POSITION_X  0
#define DLGMENU_POSITION_Y  1
//Width and height (+2: space to border)
#define DLGMENU_WIDTH       12
#define DLGMENU_HEIGHT      (DLGMENU_ITEMS_COUNT + 2)


static MenuItem dialogMenuItems[];

//Root item (don't set text)
static MenuItem dialogMenuRoot =
{ 0, MI_TYPE_TEXT, VIRK_ModCtrl|VIRK_F12, "\0", 0, dialogMenuItems, DLGMENU_ITEMS_COUNT, 0 };

//Dialog menu items
static MenuItem dialogMenuItems[DLGMENU_ITEMS_COUNT] =
{
    { MIDLG_ID_MOVE,      MI_TYPE_TEXT, VIRK_NoKey, "Move",     &dialogMenuRoot, 0, 0, 0 },
    { MIDLG_ID_RESIZE,    MI_TYPE_TEXT, VIRK_NoKey, "Resize",   &dialogMenuRoot, 0, 0, 0 },
    { MIDLG_ID_KEEPONTOP, MI_TYPE_TEXT, VIRK_NoKey, "On top",   &dialogMenuRoot, 0, 0, 0 },
    { MIDLG_ID_MINIMIZE,  MI_TYPE_TEXT, VIRK_NoKey, "Minimize", &dialogMenuRoot, 0, 0, 0 },
    { MIDLG_ID_MAXIMIZE,  MI_TYPE_TEXT, VIRK_NoKey, "Maximize", &dialogMenuRoot, 0, 0, 0 },
    { MIDLG_ID_CLOSE,     MI_TYPE_TEXT, VIRK_NoKey, "Close",    &dialogMenuRoot, 0, 0, 0 }
};


/* Restore dialog size and position */
static void RestoreDialogLocation(GUIDialog *pDialog)
{
    if (!pDialog)
        return;

    pDialog->control.x      = pDialog->location.x;
    pDialog->control.y      = pDialog->location.y;
    pDialog->control.width  = pDialog->location.width;
    pDialog->control.height = pDialog->location.height;
}



/* Dialog menu click handler */
static void DialogMenuItem_Clicked(MenuItem *mi)
{
    GUIDialog *pDialog = (GUIDialog *)guiGlobalState.dialogMenu.control.parent;

    if (!mi || !pDialog)
        return;

    pDialog->modState = DLG_STATE_DEFAULT;    

    if (mi->id == MIDLG_ID_MINIMIZE)
    {
        pDialog->control.opts |= CTRL_OPTS_DLG_MINIMIZED;

        GUISelectNextDialog(1);
    }
    else
    {
        pDialog->control.opts &= ~CTRL_OPTS_DLG_MINIMIZED;
    }


    switch(mi->id)
    {
        case MIDLG_ID_MOVE: //"Move" item
        {            
            pDialog->modState = DLG_STATE_MOVING;
            break;
        }

        case MIDLG_ID_RESIZE: //"Resize" item
        {            
            pDialog->modState = DLG_STATE_RESIZING;
            break;
        }

        case MIDLG_ID_KEEPONTOP: //"Keep on top" item
        {
            u32 opts = pDialog->control.opts;
            opts ^= CTRL_OPTS_DLG_KEEP_ONTOP;
            pDialog->control.opts = opts;
            break;
        }        

        case MIDLG_ID_MAXIMIZE: //"Maximize" item
        {            
            if (pDialog->locState == DLG_STATE_DEFAULT)
            {
                //Normal -> Maximized: save size and position
                SaveDialogLocation(pDialog);

                pDialog->locState = DLG_STATE_MAXIMIZED;

                //Call event handler
                if (pDialog->state_change_event)
                {
                    //Dialog maximized: set size on full screen
                    pDialog->control.x = 0;
                    pDialog->control.y = 0;
                    pDialog->control.width  = guiGlobalState.pScreen->width;
                    pDialog->control.height = guiGlobalState.pScreen->height;

                    if (IsTaskBarVisible())
                        pDialog->control.height--;

                    pDialog->state_change_event(pDialog, 0);
                }

                UpdateChildControls(pDialog);
            }
            else if (pDialog->locState == DLG_STATE_MAXIMIZED)
            {
                //Maximized -> Normal: restore size and position
                RestoreDialogLocation(pDialog);
                pDialog->locState = DLG_STATE_DEFAULT;

                //Call event handler
                if (pDialog->state_change_event)
                    pDialog->state_change_event(pDialog, 0);

                UpdateChildControls(pDialog);
            }

            break;
        }

        case MIDLG_ID_CLOSE: //"Close" item
        {            
            GUICloseDialog(pDialog, 0);
            break;
        }
    } //switch
}



/* Open dialog menu */
static void OpenDialogMenu(GUIDialog *pDialog)
{    
    MenuBar *pDialogMenu = &guiGlobalState.dialogMenu;

    if (pDialog &&
        pDialog->dlgMenuItems &&
        (pDialog->control.opts & CTRL_OPTS_DLG_MINIMIZED) == 0)
    {
        s32 i;
        MenuItem *pMenuItems = pDialogMenu->root->sub;
        u16 itemmask = (pDialog->dlgMenuItems & GUIDIALOG_MENU_MASK);

        //Can't move/resize dialog when it maximized
        if (pDialog->locState == DLG_STATE_MAXIMIZED ||
            pDialog->control.opts & (CTRL_OPTS_DLG_DESKTOP))
            itemmask &= ~(GUIDIALOG_MENU_MOVE|GUIDIALOG_MENU_RESIZE);

        //Can't close/minimize modal dialog
        if (pDialog->control.opts & (CTRL_OPTS_DLG_MODAL|CTRL_OPTS_DLG_DESKTOP))
            itemmask &= ~(GUIDIALOG_MENU_CLOSE|GUIDIALOG_MENU_MINIMIZE);

        if (itemmask == 0)
        {
            //No dialog menu
            pDialogMenu->control.parent = 0;
            pDialogMenu->current        = 0;
            return;
        }

        //--------------------------
        //Menu items
        for(i = 0; i < DLGMENU_ITEMS_COUNT; i++)
        {
            if (pMenuItems[i].id == MIDLG_ID_KEEPONTOP)
            {
                if (pDialog->control.opts & CTRL_OPTS_DLG_KEEP_ONTOP)
                    pMenuItems[i].opts |= MI_OPTS_ISCHECKED;
                else
                    pMenuItems[i].opts &= ~MI_OPTS_ISCHECKED;
            }

            if (itemmask & 1)
                pMenuItems[i].opts &= ~MI_OPTS_DISABLED;
            else
                pMenuItems[i].opts |= MI_OPTS_DISABLED;

            itemmask >>= 1;
        }

        pDialogMenu->control.parent = &pDialog->control;
        pDialogMenu->current        = 0;

        //--------------------------
        //Dialog menu opened near top-left corner of dialog
        //and always visible on screen
        //Coordinates by default (relative to parent dialog)
        pDialogMenu->control.x = DLGMENU_POSITION_X;
        pDialogMenu->control.y = DLGMENU_POSITION_Y;

        //Check dialog menu coordinates
        if (pDialog->control.x < 0)
            pDialogMenu->control.x = -pDialog->control.x;

        if (pDialog->control.y < 0)
            pDialogMenu->control.y = -pDialog->control.y;

        if (guiGlobalState.pScreen)
        {
            s16 screenWidth  = guiGlobalState.pScreen->width;
            s16 screenHeight = guiGlobalState.pScreen->height;
            s16 w1;

            w1 = pDialog->control.x + pDialogMenu->control.width;
            if (w1 > screenWidth)
                pDialogMenu->control.x = screenWidth - w1;


            w1 = pDialog->control.y + pDialogMenu->control.height + DLGMENU_POSITION_Y;

            if (IsTaskBarVisible())
            {
                if (w1 > (screenHeight - 1))
                    pDialogMenu->control.y = screenHeight - w1;
            }
            else
            {
                if (w1 > screenHeight)
                    pDialogMenu->control.y = screenHeight - w1;
            }
        }

        //--------------------------
        //Open dialog menu
        SelectMenuItem(pDialogMenu, MBDIR_SUBMENU);
    }
    else
    {
        //No dialog menu
        pDialogMenu->control.parent = 0;
        pDialogMenu->current        = 0;
    }
}

#endif /* CTRLMOD_EN_MENUBAR && GUIDIALOG_EN_DIALOGMENU */



#if (CTRLMOD_EN_MENUBAR == 1 && GUITASKBAR_EN_STARTUPMENU == 1)
/* Open startup menu */
static void OpenStartupMenu()
{
    MenuBar *pStartupMenu = &guiGlobalState.startupMenu;

    if (IsTaskBarVisible() && pStartupMenu->root)
    {
        pStartupMenu->control.opts |= CTRL_OPTS_FOCUSED;
        pStartupMenu->current = 0;

        //Open menu
        SelectMenuItem(pStartupMenu, MBDIR_SUBMENU);
    }
}

#endif /* GUITASKBAR_EN_STARTUPMENU */

/*===========================================================================
 * TOP LEVEL FUNCTIONS
 *===========================================================================*/

/* */
void InitializeGUISubsystem()
{
    #if (CTRLMOD_EN_MENUBAR == 1 && (GUIDIALOG_EN_DIALOGMENU == 1 || GUITASKBAR_EN_STARTUPMENU == 1))
    MenuBar *pMenuBar;
    #endif

    //---------------------------------------------------------------
    //Control structure
    memset(&guiGlobalState, 0, sizeof(GUIGlobalState));    

    //---------------------------------------------------------------
    #if (CTRLMOD_EN_TASKBAR == 1)
    //Task bar (not visible by default, no parent control)
    InitializeControlParameters(&guiGlobalState.taskBar.control,
           CTRL_TYPE_TASKBAR, 0,
           0, 0, 1, 1, COL_BLACK, COL_GRAY, 0, 0);
    #endif

    //---------------------------------------------------------------
    #if (CTRLMOD_EN_MENUBAR == 1)
    #if (GUIDIALOG_EN_DIALOGMENU == 1)
    //Dialog menu (common for all dialogs), its not user-defined menu
    pMenuBar = &guiGlobalState.dialogMenu;

    InitializeControlParameters(&pMenuBar->control,
           CTRL_TYPE_MENUBAR, 0,
           DLGMENU_POSITION_X, DLGMENU_POSITION_Y,
           DLGMENU_WIDTH, DLGMENU_HEIGHT,
           COL_BLACK, COL_SILVER,
           CTRL_OPTS_VISIBLE|CTRL_OPTS_ENABLED|CTRL_OPTS_FOCUSED|CTRL_OPTS_CASCADE_MENU|
           CTRL_OPTS_BORDER_SINGLE1,
           0);

    pMenuBar->root  = &dialogMenuRoot;
    pMenuBar->click = &DialogMenuItem_Clicked;

    #endif /* GUIDIALOG_EN_DIALOGMENU */

    //---------------------------------------------------------------
    #if (CTRLMOD_EN_TASKBAR == 1 && GUITASKBAR_EN_STARTUPMENU == 1)
    //Starup menu
    pMenuBar = &guiGlobalState.startupMenu;

    InitializeControlParameters(&pMenuBar->control,
           CTRL_TYPE_MENUBAR, 0,
           0, 0, 21, 10,
           COL_BLACK, COL_SILVER,
           CTRL_OPTS_VISIBLE|CTRL_OPTS_ENABLED|CTRL_OPTS_CASCADE_MENU|
           CTRL_OPTS_BORDER_SINGLE1|CTRL_OPTS_AUTOSIZE,
           0);        

    //No startup menu by default (user-defined)
    //pMenuBar->root  = 0; //root item is user-defined
    //pMenuBar->click = 0; //click handler is user-defined
    #endif /* CTRLMOD_EN_TASKBAR && GUITASKBAR_EN_STARTUPMENU */
    #endif /* CTRLMOD_EN_MENUBAR */
}



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
                                 CtrlParams *parent) //Pointer to parent control parameters
{
    s32 sz = sizeof(CtrlParams);
    switch(type)
    {        
        case CTRL_TYPE_FORM:        sz = sizeof(GUIDialog);   break;        

        #if (CTRLMOD_EN_BUTTON == 1)
        case CTRL_TYPE_BUTTON:      sz = sizeof(PButton);     break;
        #endif

        #if (CTRLMOD_EN_TEXTBOX == 1)
        case CTRL_TYPE_TEXTBOX:     sz = sizeof(TextBox);     break;
        #endif

        #if (CTRLMOD_EN_LISTBOX == 1)
        case CTRL_TYPE_LISTBOX:     sz = sizeof(ListBox);     break;
        #endif

        #if (CTRLMOD_EN_COMBOBOX == 1)
        case CTRL_TYPE_COMBOBOX:    sz = sizeof(ComboBox);    break;
        #endif

        #if (CTRLMOD_EN_CHECKBOX == 1)
        case CTRL_TYPE_CHECKBOX:    sz = sizeof(CheckBox);    break;
        #endif

        #if (CTRLMOD_EN_RADIOBUTTON == 1)
        case CTRL_TYPE_RADIOBUTTON: sz = sizeof(RadioButton); break;
        #endif

        #if (CTRLMOD_EN_SLIDERBAR == 1)
        case CTRL_TYPE_SLIDERBAR:   sz = sizeof(SliderBar);   break;
        #endif

        #if (CTRLMOD_EN_PROGRESSBAR == 1)
        case CTRL_TYPE_PROGRESSBAR: sz = sizeof(ProgressBar); break;
        #endif

        #if (CTRLMOD_EN_SCROLLBAR == 1)
        case CTRL_TYPE_SCROLLBAR:   sz = sizeof(ScrollBar);   break;
        #endif

        #if (CTRLMOD_EN_MENUBAR == 1)
        case CTRL_TYPE_MENUBAR:     sz = sizeof(MenuBar);     break;
        #endif

        #if (CTRLMOD_EN_TASKBAR == 1)
        case CTRL_TYPE_TASKBAR:     sz = sizeof(GUITaskBar);  break;
        #endif

        #if (CTRLMOD_EN_FILELISTBOX == 1)
        case CTRL_TYPE_FILELISTBOX: sz = sizeof(FileListBox); break;               
        #endif
    }

    //Clear structure
    memset(control, 0, sz);

    //Set structure fields
    control->type   = type;
    control->id     = id;
    control->x      = x;
    control->y      = y;
    control->width  = width;
    control->height = height;
    control->backColor = backColor;
    control->foreColor = foreColor;    
    control->opts      = opts;
    control->parent    = parent;

    //Selection fore/back colors: use fore/back colors inversion by default
    control->selBackColor = control->foreColor;
    control->selForeColor = control->backColor;

    //Border color = fore color by default
    control->borderColor = control->foreColor;

    //Set disabled color by default
    control->disForeColor = CTRL_DISABLED_FORECOLOR;

    //Select draw function
    switch(type)
    {
        #if (CTRLMOD_EN_BUTTON == 1)
        case CTRL_TYPE_BUTTON:      control->pfDrawCtrl = &DrawPButton;     break;
        #endif

        #if (CTRLMOD_EN_TEXTBOX == 1)
        case CTRL_TYPE_TEXTBOX:     control->pfDrawCtrl = &DrawTextBox;     break;
        #endif

        #if (CTRLMOD_EN_LISTBOX == 1)
        case CTRL_TYPE_LISTBOX:     control->pfDrawCtrl = &DrawListBox;     break;
        #endif

        #if (CTRLMOD_EN_COMBOBOX == 1)
        case CTRL_TYPE_COMBOBOX:    control->pfDrawCtrl = &DrawComboBox;    break;
        #endif

        #if (CTRLMOD_EN_CHECKBOX == 1)
        case CTRL_TYPE_CHECKBOX:    control->pfDrawCtrl = &DrawCheckBox;    break;
        #endif

        #if (CTRLMOD_EN_RADIOBUTTON == 1)
        case CTRL_TYPE_RADIOBUTTON: control->pfDrawCtrl = &DrawRadioButton; break;
        #endif

        #if (CTRLMOD_EN_SLIDERBAR == 1)
        case CTRL_TYPE_SLIDERBAR:   control->pfDrawCtrl = &DrawSliderBar;   break;
        #endif

        #if (CTRLMOD_EN_PROGRESSBAR == 1)
        case CTRL_TYPE_PROGRESSBAR: control->pfDrawCtrl = &DrawProgressBar; break;
        #endif

        #if (CTRLMOD_EN_SCROLLBAR == 1)
        case CTRL_TYPE_SCROLLBAR:   control->pfDrawCtrl = &DrawScrollBar;   break;
        #endif

        #if (CTRLMOD_EN_MENUBAR == 1)
        case CTRL_TYPE_MENUBAR:     control->pfDrawCtrl = &DrawMenuBar;     break;
        #endif

        #if (CTRLMOD_EN_FILELISTBOX == 1)
        case CTRL_TYPE_FILELISTBOX:
        {
            FileListBox *fileListBox = (FileListBox *)control;

            //Directory fore color = fore color by default
            fileListBox->dirForeColor = control->foreColor;

            control->pfDrawCtrl = &DrawFileListBox;
            break;
        }
        #endif

        case CTRL_TYPE_FORM:
        {
            GUIDialog *pDialog = (GUIDialog *)control;

            //No dialog menu by default
            pDialog->dlgMenuItems = 0;

            pDialog->locState     = DLG_STATE_DEFAULT;
            pDialog->modState     = DLG_STATE_DEFAULT;

            //Save current location
            SaveDialogLocation(pDialog);
            break;
        }        
    }
}



/* */
static void UpdateChildControls(GUIDialog *pDialog)
{
    s32 i;
    CtrlParams *control;

    //Second phase of control initialization
    for(i = 0; i < pDialog->childCount; i++)
    {
        control = pDialog->childs[i].control;

        switch(control->type)
        {
            #if (CTRLMOD_EN_PROGRESSBAR == 1)
            case CTRL_TYPE_PROGRESSBAR:            
            {
                //Progress bar, label cannot accept user input:
                //set "NONEINPUT" flag
                control->opts |= CTRL_OPTS_NONEINPUT;
                break;
            }
            #endif

            #if (CTRLMOD_EN_TEXTBOX == 1)
            case CTRL_TYPE_TEXTBOX:
            {
                TextBox *textBox = (TextBox*)control;

                UpdateTextBox(textBox);
                break;
            }
            #endif

            #if (CTRLMOD_EN_BUTTON == 1)
            case CTRL_TYPE_BUTTON:
            {
                //PButton *button = (PButton *)control;
                //currently no actions
                break;
            }
            #endif

            #if (CTRLMOD_EN_CHECKBOX == 1)
            case CTRL_TYPE_CHECKBOX:
            {
                //CheckBox *checkBox = (CheckBox *)control;
                //currently no actions
                break;
            }
            #endif

            #if (CTRLMOD_EN_RADIOBUTTON == 1)
            case CTRL_TYPE_RADIOBUTTON:
            {
                //RadioButton *radioButton = (RadioButton *)control;
                //currently no actions
                break;
            }
            #endif

            #if (CTRLMOD_EN_LISTBOX == 1)
            case CTRL_TYPE_LISTBOX:
            {
                ListBox *listBox = (ListBox*)control;

                UpdateListBox(listBox);
                break;
            }
            #endif

            #if (CTRLMOD_EN_COMBOBOX == 1)
            case CTRL_TYPE_COMBOBOX:
            {
                ComboBox *comboBox = (ComboBox*)control;

                UpdateComboBox(comboBox);
                break;
            }
            #endif

            #if (CTRLMOD_EN_FILELISTBOX == 1)
            case CTRL_TYPE_FILELISTBOX:
            {
                FileListBox *fileListBox = (FileListBox *)control;

                UpdateFileListBox(fileListBox);
                break;
            }
            #endif

            default: break;
        } //switch(type)
    } //for
}



/* Set child controls to specified dialog */
void SetChildControls(GUIDialog *pDialog, CtrlChainItem *child, s32 count)
{
    //Save pointer to array of child controls
    if (child && count)
    {
        pDialog->childs     = child;
        pDialog->childCount = count;
    }
    else
    {
        pDialog->childs     = 0;
        pDialog->childCount = 0;
    }

    UpdateChildControls(pDialog);
}



/* Remove focus from all controls at selected dialog */
void RemoveControlFocus(GUIDialog *pDialog)
{
    s32 i;
    CtrlChainItem *child = pDialog->childs;

    if (!child)
        return;

    for(i = 0; i < pDialog->childCount; i++)
    {
        if (child[i].control)
            child[i].control->opts &= ~CTRL_OPTS_FOCUSED;
    }
}



/* Set focus to first acceptable control element */
static CtrlParams *UpdateControlFocus(GUIDialog *pDialog)
{
    s32 i;
    u32 opts;
    CtrlChainItem *childs;
    CtrlChainItem *childCanFocused;
    CtrlChainItem *childNowFocused;

    if (!pDialog ||
        (pDialog->control.opts & (CTRL_OPTS_VISIBLE)) != (CTRL_OPTS_VISIBLE))
        return 0;

    //Check that some control already focused and
    //try find visible and enabled control to set focus
    childNowFocused = 0;
    childCanFocused = 0;    
    childs = pDialog->childs;
    for(i = 0; i < pDialog->childCount; i++)
    {
        if (childs[i].control)
        {            
            opts = childs[i].control->opts;
            if ((opts & CTRL_OPTS_EDITABLE_MASK) == CTRL_OPTS_EDITABLE)
            {                
                //Some control already focused: save pointer, break
                childNowFocused = &childs[i];
                break;
            }
            else if (childCanFocused == 0 &&
                     (opts & CTRL_OPTS_CANFOCUSED_MASK) == CTRL_OPTS_CANFOCUSED)
            {
                //This control can focused: save pointer
                childCanFocused = &childs[i];
            }
        }
    } //for

    if (childNowFocused)
    {
        //Remove focus from all controls
        //to prevent that case when focused two or more controls
        for(i = 0; i < pDialog->childCount; i++)
        {
            if (childs[i].control)
                childs[i].control->opts &= ~CTRL_OPTS_FOCUSED;
        }

        //Set focus to specified control
        childNowFocused->control->opts |= CTRL_OPTS_FOCUSED;
        return (childNowFocused->control);
    }
    else if (childCanFocused)
    {
        //No one child control doesn't have focus:
        //set focus to first found control
        childCanFocused->control->opts |= CTRL_OPTS_FOCUSED;
        return (childCanFocused->control);
    }
    else
        return 0;
}



/* Open dialog */
void GUIOpenDialog(GUIDialog *pDialog)
{    
    GUIDialog *dialogs = guiGlobalState.pDialogs;
    s32 count          = guiGlobalState.dialogsCount;
    s32 i;

    //Null pointer: return
    if (!pDialog)
        return;

    //Reset focus flag for all dialogs
    for(i = 0; i < count; i++)
    {
        dialogs[i].control.opts &= ~CTRL_OPTS_FOCUSED;        
    }

    //Clear minimized flag
    pDialog->control.opts &= ~CTRL_OPTS_DLG_MINIMIZED;

    if (pDialog->control.opts & CTRL_OPTS_VISIBLE)
    {
        //Dialog alredy opened: set focus flag
        pDialog->control.opts |= CTRL_OPTS_FOCUSED;

        UpdateControlFocus(pDialog);
    }
    else
    {
        //Dialog not opened: set visible and focus flags, call event handler
        pDialog->control.opts |= (CTRL_OPTS_VISIBLE | CTRL_OPTS_FOCUSED);

        UpdateControlFocus(pDialog);

        if (pDialog->load_event)
            pDialog->load_event(pDialog, 0);
    }
}



/* Close dialog */
void GUICloseDialog(GUIDialog *pDialog, ClosingEventArgs *args)
{    
    GUIDialog *dialogs = guiGlobalState.pDialogs;
    s32 count          = guiGlobalState.dialogsCount;
    s32 i, isIWasFocused;
    CtrlParams *pCtrlParent;
    GUIDialog *pFocusedDialog;


    //Null pointer or dialog already closed: return
    if (!pDialog || (pDialog->control.opts & CTRL_OPTS_VISIBLE) == 0)
        return;


    //Current dialog was focused
    isIWasFocused   = (pDialog->control.opts & CTRL_OPTS_FOCUSED) ? 1 : 0;

    //Reset flags
    pDialog->control.opts &= ~(CTRL_OPTS_VISIBLE | CTRL_OPTS_FOCUSED);

    //Pointer to parent control
    pCtrlParent = pDialog->control.parent;


    //Call event handler
    if (pDialog->closing_event)
    {
        ClosingEventArgs locargs;

        if (args == 0)
        {
            locargs.cancel = 0;
            locargs.result   = 0;
            locargs.data   = 0;
            args = &locargs;
        }

        pDialog->closing_event(pDialog, args);

        //User canceled dialog closing or reopen same dialog
        if (args->cancel || (pDialog->control.opts & CTRL_OPTS_VISIBLE))
        {
            //Restore dialog flags
            pDialog->control.opts |= CTRL_OPTS_VISIBLE;

            if (isIWasFocused)
                pDialog->control.opts |= CTRL_OPTS_FOCUSED;
            return;
        }
    }

    if (isIWasFocused)
    {
        //Try search any dialog with focus
        pFocusedDialog = 0;
        for(i = (count - 1); i >= 0; i--)
        {
            if ((dialogs[i].control.opts & (CTRL_OPTS_VISIBLE|CTRL_OPTS_FOCUSED)) ==
                 (CTRL_OPTS_VISIBLE|CTRL_OPTS_FOCUSED))
            {
                pFocusedDialog = &dialogs[i];
                break;
            }
        }

        if (pCtrlParent && pCtrlParent->type == CTRL_TYPE_FORM &&
            (pCtrlParent->opts & CTRL_OPTS_VISIBLE))
        {
            //Current dialog has parent visible dialog: set focus to parent dialog
            if (pFocusedDialog == 0)
            {
                pCtrlParent->opts |= CTRL_OPTS_FOCUSED;
                UpdateControlFocus((GUIDialog*)pCtrlParent);
            }
        }
        else
        {
            //Current dialog doesn't has parent dialog           
            //Set focus to first visible dialog
            if (pFocusedDialog == 0)
            {
                for(i = (count - 1); i >= 0; i--)
                {
                    if (dialogs[i].control.opts & CTRL_OPTS_VISIBLE)
                    {
                        dialogs[i].control.opts |= CTRL_OPTS_FOCUSED;
                        UpdateControlFocus(&dialogs[i]);
                        break;
                    }
                }
            }
        }
    }
}



/* Draw dialog */
void GUIDrawDialog(ScreenParams *scp, GUIDialog *pDialog)
{    
    CtrlParams *ctrl;
    #if (CTRLMOD_EN_COMBOBOX == 1)
    CtrlParams *pComboBoxCtrl;
    #endif

    #if (CTRLMOD_EN_MENUBAR == 1)
    CtrlParams *pUserMenuCtrl;
    #endif

    s32 i, dlgX, dlgY, dlgWidth, dlgHeight;
    u32 dlgOpts;

    dlgOpts = pDialog->control.opts;

    //Dialog minimized: no actions, return
    if (dlgOpts & CTRL_OPTS_DLG_MINIMIZED)
        return;    

    //Dialog size and position
    dlgX      = pDialog->control.x;
    dlgY      = pDialog->control.y;
    dlgWidth  = pDialog->control.width;
    dlgHeight = pDialog->control.height;

    if (dlgWidth <= 0 || dlgHeight <= 0)
        return;

    //Clear dialog region with specified background color and filling char
    PrintFillRectangle(scp, dlgX, dlgY, dlgWidth, dlgHeight,
                       pDialog->control.backColor, 0x20);

    //Set foreground color for dialog region
    SetForeColors(scp,
                  pDialog->control.foreColor,
                  dlgX, dlgY, dlgWidth, dlgHeight);

    //Clear border region
    ClearBorderRegion(scp, dlgX, dlgY, dlgWidth, dlgHeight);

    //Draw shadows
    if (dlgOpts & CTRL_OPTS_DRAW_SHADOW)
    {
        //Bottom-horizontal
        SetForeBackColors(scp, CTRL_SHADOW_FORECOLOR, CTRL_SHADOW_BACKCOLOR,
                      dlgX + 1, dlgY + dlgHeight, dlgWidth, 1);

        //Right-vertical
        SetForeBackColors(scp, CTRL_SHADOW_FORECOLOR, CTRL_SHADOW_BACKCOLOR,
                      dlgX + dlgWidth, dlgY + 1, 1, dlgHeight);
    }    


    //Call redraw event handler
    //(draw user-defined data on dialog, possibly static text)
    if (pDialog->redraw_event)
        pDialog->redraw_event(pDialog, 0);

    //Draw child controls
    #if (CTRLMOD_EN_COMBOBOX == 1)
    pComboBoxCtrl = 0;
    #endif

    #if (CTRLMOD_EN_MENUBAR == 1)
    pUserMenuCtrl = 0;
    #endif

    for(i = 0; i < pDialog->childCount; i++)
    {
        ctrl = pDialog->childs[i].control;
        if (!ctrl)
            continue;

        if (ctrl->type == CTRL_TYPE_MENUBAR &&
            ((MenuBar*)ctrl)->current != 0)
        {
            #if (CTRLMOD_EN_MENUBAR == 1)
            pUserMenuCtrl = ctrl;
            #endif
        }
        else if (ctrl->type == CTRL_TYPE_COMBOBOX &&
                 (ctrl->opts & CTRL_OPTS_CBLVISIBLE))
        {
            #if (CTRLMOD_EN_COMBOBOX == 1)
            pComboBoxCtrl = ctrl;
            #endif
        }
        else
        {
            //Draw child control
            if (ctrl->pfDrawCtrl)
                ctrl->pfDrawCtrl(scp, ctrl);
        }
    }    

    //Draw border
    if ((dlgOpts & CTRL_OPTS_BORDER_MASK) != CTRL_OPTS_BORDER_NONE)
    {
        #if (CTRL_SELECTED_STYLE == 1)
        //Windows style
        color_t borderColor;

        if (dlgOpts & CTRL_OPTS_FOCUSED)
            borderColor = pDialog->control.borderColor;
        else
            borderColor = CTRL_DIALOG_UNFOCUSED_BORDERCOLOR;

        //Dialog border
        PrintRectangle(scp,
                       dlgX, dlgY, dlgWidth, dlgHeight,
                       (dlgOpts & CTRL_OPTS_BORDER_MASK) >> CTRL_OPTS_BORDER_BITS,
                        borderColor, pDialog->control.backColor);

        //Dialog header
        PrintFillRectangle(scp, dlgX, dlgY, dlgWidth, 1, borderColor, 0x20);

        //Set title foreground color
        SetForeColors(scp, CTRL_DIALOG_TITLE_FORECOLOR, dlgX, dlgY, dlgWidth, 1);

        //Print dialog title
        if (pDialog->title)
        {
            DrawTStringLine(scp, dlgX + 2, dlgY, dlgWidth - 5, pDialog->title);
        }

        #else
        //DOS style
        s32 titleLen;

        //Dialog border
        PrintRectangle(scp,
                       dlgX, dlgY, dlgWidth, dlgHeight,
                       (dlgOpts & CTRL_OPTS_BORDER_MASK) >> CTRL_OPTS_BORDER_BITS,
                        pDialog->control.borderColor, pDialog->control.backColor);

        //Print dialog title
        if (pDialog->title && ((titleLen = strlen(pDialog->title)) > 0))
        {
            const char *pTitle;
            char titlestr[GUIDIALOG_TITLE_MAXLENGTH + 8];

            if (titleLen > GUIDIALOG_TITLE_MAXLENGTH)
            {
                pTitle = pDialog->title;
            }
            else
            {
                std_sprintf(titlestr, " %s ", pDialog->title);
                pTitle = titlestr;
            }

            DrawTStringLine(scp, dlgX + 2, dlgY, dlgWidth - 5, pTitle);
        }

        #endif

        #if (CTRLMOD_EN_MENUBAR == 1 && GUIDIALOG_EN_DIALOGMENU == 1)
        //Marks that current dialog has dialog menu
        if (pDialog->dlgMenuItems & GUIDIALOG_MENU_MASK)
        {
            char str[4];
            str[0] = GUIDIALOG_BTNMENU_CHAR;
            str[1] = 0;
            DrawTStringLine(scp, dlgX, dlgY, 1, str);
        }
        #endif

        //Marks that current dialog can closed
        if ((dlgOpts & CTRL_OPTS_DLG_MODAL) == 0)
        {
            s32 dlgBtnsCount = 0;
            char str[4];            

            if (pDialog->dlgMenuItems & GUIDIALOG_MENU_MINIMIZE)
                str[dlgBtnsCount++] = GUIDIALOG_BTNMINIMIZE_CHAR;

            if (pDialog->dlgMenuItems & GUIDIALOG_MENU_MAXIMIZE)
                str[dlgBtnsCount++] = GUIDIALOG_BTNMAXIMIZE_CHAR;

            str[dlgBtnsCount++] = GUIDIALOG_BTNCLOSE_CHAR;
            str[dlgBtnsCount] = 0;

            DrawTStringLine(scp, dlgX + dlgWidth - dlgBtnsCount, dlgY, dlgBtnsCount, str);
        }
    }    

    #if (CTRLMOD_EN_COMBOBOX == 1)
    //Draw opened combobox as last
    if (pComboBoxCtrl)
        DrawComboBox(scp, pComboBoxCtrl);
    #endif /* CTRLMOD_EN_COMBOBOX */

    #if (CTRLMOD_EN_MENUBAR == 1)
    //User menu
    if (pUserMenuCtrl)
        DrawMenuBar(scp, pUserMenuCtrl);
    #endif /* CTRLMOD_EN_MENUBAR */

    #if (CTRLMOD_EN_MENUBAR == 1 && GUIDIALOG_EN_DIALOGMENU == 1)
    if (dlgOpts & CTRL_OPTS_FOCUSED)
    {
        //Current dialog focused and dialog menu opened: draw menu
        MenuBar *pDialogMenu = &guiGlobalState.dialogMenu;
        if (pDialogMenu->current)
            DrawMenuBar(scp, &pDialogMenu->control);
    }
    #endif /* CTRLMOD_EN_MENUBAR && GUIDIALOG_EN_DIALOGMENU */
}



/* */
static u32 GUIAcceptButtonClick(GUIDialog *pDialog)
{
    #if (CTRLMOD_EN_BUTTON)
    //Button module enabled
    if (pDialog && (pDialog->control.opts & CTRL_OPTS_ENABLED) &&
        pDialog->acceptButton != 0 &&
        pDialog->acceptButton->click_event &&
       (pDialog->acceptButton->control.opts & (CTRL_OPTS_VISIBLE|CTRL_OPTS_ENABLED)) ==
            (CTRL_OPTS_VISIBLE|CTRL_OPTS_ENABLED))
    {
        //Call click event handler
        pDialog->acceptButton->click_event(pDialog->acceptButton, 0);
        return 1;
    }
    else
    {
        //No accept button or event handler not defined: return 0
        return 0;
    }

    #else
    //Button module disabled: return 0
    return 0;

    #endif /* CTRLMOD_EN_BUTTON */
}



/* Get pointer to focused control */
static u32 GUIHotkeyHandler(GUIDialog *pDialog, KeyCode hotkey)
{
    s32 i;
    CtrlParams *ctrl;

    #if (CTRLMOD_EN_MENUBAR == 1)
    #if (GUITASKBAR_EN_STARTUPMENU == 1)
    MenuBar *pStartupMenu = &guiGlobalState.startupMenu;

    if (pStartupMenu->root && IsTaskBarVisible())
    {
        //Find hotkey at startup menu
        if (MenuBarHotkeyHandler(pStartupMenu, pStartupMenu->root, hotkey))
            return 1;
    }
    #endif /* GUITASKBAR_EN_STARTUPMENU */
    #endif /* CTRLMOD_EN_MENUBAR */

    //pDialog now have flags: (CTRL_OPTS_VISIBLE|CTRL_OPTS_FOCUSED)
    if (pDialog && (pDialog->control.opts & CTRL_OPTS_ENABLED))
    {
        for(i = 0; i < pDialog->childCount; i++)
        {
            ctrl = pDialog->childs[i].control;

            //Control flags: must visible and enabled (not need be focused)
            if (ctrl && (ctrl->opts & (CTRL_OPTS_NONEINPUT|CTRL_OPTS_VISIBLE|CTRL_OPTS_ENABLED)) !=
                                                          (CTRL_OPTS_VISIBLE|CTRL_OPTS_ENABLED))
                continue;

            if (ctrl->type == CTRL_TYPE_BUTTON)
            {
                #if (CTRLMOD_EN_BUTTON == 1)
                //Button hotkeys
                PButton *button = (PButton *)ctrl;
                if (button->hotkey == hotkey)
                {
                    if (button->click_event)
                        button->click_event(button, 0);

                    return 1;
                }
                #endif /* CTRLMOD_EN_BUTTON */
            }
            else if (ctrl->type == CTRL_TYPE_MENUBAR)
            {
                #if (CTRLMOD_EN_MENUBAR == 1)
                //Menu hotkeys
                MenuBar *menuBar = (MenuBar *)ctrl;

                if (menuBar->root)
                {
                    if (menuBar->root->hotkey == hotkey)
                    {
                        GUISetFocusedControl(&menuBar->control);

                        if (menuBar->current == 0)
                            SelectMenuItem(menuBar, MBDIR_SUBMENU);

                        return 1;
                    }
                    else
                    {
                        if (MenuBarHotkeyHandler(menuBar, menuBar->root, hotkey))
                            return 1;
                    }
                }
                #endif /* CTRLMOD_EN_MENUBAR */
            }
        } //for each child control

        return 0;
    }
    else
    {
        return 0;
    }
}


#if (CTRLMOD_EN_MENUBAR == 1)
#if (GUIDIALOG_EN_DIALOGMENU == 1)
/* Move or resize specified dialog */
static u32 DialogKeyHandler(GUIDialog *pDialog, KeyCode key, u32 locks)
{
    s16 changeVal;

    if (!pDialog || pDialog->locState == DLG_STATE_MAXIMIZED)
    {
        pDialog->modState = DLG_STATE_DEFAULT;
        return 0;
    }

    if (pDialog->modState == DLG_STATE_MOVING)
    {
        //Move dialog                
        changeVal = ((key & VIRK_MODIFIER_MASK) == VIRK_ModCtrl) ? 4 : 1;
        key       =  (key & VIRK_KEYCODE_MASK);

        switch(key)
        {
            case VIRK_Up:
            {
                pDialog->control.y -= changeVal;
                break;
            }

            case VIRK_Down:
            {
                pDialog->control.y += changeVal;
                break;
            }

            case VIRK_Left:
            {
                pDialog->control.x -= changeVal;
                break;
            }

            case VIRK_Right:
            {
                pDialog->control.x += changeVal;
                break;
            }

            //End of move mode
            case VIRK_Enter:
            case VIRK_Separator:
            {
                pDialog->modState = DLG_STATE_DEFAULT;
                break;
            }

            default: break;
        } //switch     

        //In move mode: return 1
        return 1;
    }
    else if (pDialog->modState == DLG_STATE_RESIZING)
    {
        //Change dialog size
        changeVal = ((key & VIRK_MODIFIER_MASK) == VIRK_ModCtrl) ? 4 : 1;
        key       =  (key & VIRK_KEYCODE_MASK);

        switch(key)
        {
            case VIRK_Up:
            {
                if (pDialog->control.height - changeVal > 2)
                    pDialog->control.height -= changeVal;
                break;
            }

            case VIRK_Left:
            {
                if (pDialog->control.width - changeVal > 9)
                    pDialog->control.width -= changeVal;
                break;
            }

            case VIRK_Down:
            {
                pDialog->control.height += changeVal;
                break;
            }

            case VIRK_Right:
            {
                pDialog->control.width += changeVal;
                break;
            }

            //End of resize mode
            case VIRK_Enter:
            case VIRK_Separator:
            {
                pDialog->modState = DLG_STATE_DEFAULT;
                return 1;
            }

            //In resize mode: return 1
            default: return 1;
        } //switch

        //Call event handler
        if (pDialog->state_change_event)
            pDialog->state_change_event(pDialog, 0);

        //Update child controls (in resize mode only)
        UpdateChildControls(pDialog);

        //In resize mode: return 1
        return 1;
    }

    //Not in Move/Resize mode: return 0
    return 0;
}
#endif /* GUIDIALOG_EN_DIALOGMENU */
#endif /* CTRLMOD_EN_MENUBAR */


/* Keyboard input handler */
void GUIInputHandler(KeyCode key, u32 locks)
{            
    GUIDialog *dialogs = guiGlobalState.pDialogs;
    s32 count          = guiGlobalState.dialogsCount;

    CtrlParams *pFocusedControl;
    GUIDialog *pFocusedDialog;

    #if (CTRLMOD_EN_MENUBAR == 1)
    #if (GUIDIALOG_EN_DIALOGMENU == 1)
    MenuBar *pDialogMenu  = &guiGlobalState.dialogMenu;
    #endif
    #if (GUITASKBAR_EN_STARTUPMENU == 1)
    MenuBar *pStartupMenu = &guiGlobalState.startupMenu;
    #endif
    #endif /* CTRLMOD_EN_MENUBAR */

    //No key: no actions, return
    if (key == VIRK_NoKey)
        return;    

    pFocusedDialog = GUIGetFocusedDialog(dialogs, count);

    if (pFocusedDialog && pFocusedDialog->key_pressed_event)
    {
        //Process keycodes with user-defined handler
        if (pFocusedDialog->key_pressed_event(pFocusedDialog, key, locks))
            return;
    }

    if (GUIHotkeyHandler(pFocusedDialog, key))
    {
        //Hotkey found: return
        return;
    }
    else if (key == VIRK_Tabulation)
    {
        #if (CTRLMOD_EN_TEXTBOX == 1)
        if ((pFocusedControl = GUIGetFocusedControl(dialogs, count)) != 0 &&
             pFocusedControl->type == CTRL_TYPE_TEXTBOX &&
             pFocusedControl->height > 1)
        {
            //Add TAB char to multiline textbox
            TextBox *textBox = (TextBox *)pFocusedControl;

            TextBoxKeyHandler(textBox, key, locks);
        }
        else
        #endif /* CTRLMOD_EN_MENUBAR */
        {
            //Set focus to next control at current dialog
            GUISelectNextControl(1);
        }

        return;
    }   
    else if (key == VIRK_Esc)
    {
        //Escape: close opened menu or close dialog

        #if (CTRLMOD_EN_MENUBAR == 1)
        #if (GUITASKBAR_EN_STARTUPMENU == 1)
        //Close startup menu, clear flag, return
        if (pStartupMenu->current)
        {            
            pStartupMenu->current = 0;                       
            pStartupMenu->control.opts &= ~CTRL_OPTS_FOCUSED;
            return;
        }
        else
        #endif /* GUITASKBAR_EN_STARTUPMENU */
        #if (GUIDIALOG_EN_DIALOGMENU == 1)
        //Close dialog menu, return
        if (pDialogMenu->current)
        {
            pDialogMenu->current = 0;
            return;
        }
        #endif /* GUIDIALOG_EN_DIALOGMENU */

        if ((pFocusedControl = GUIGetFocusedControl(dialogs, count)) != 0 &&
             pFocusedControl->type == CTRL_TYPE_MENUBAR)
        {
            //Close opened menu, return
            MenuBar *menuBar = (MenuBar *)pFocusedControl;

            if (menuBar->current)
            {
                menuBar->current = 0;
                return;
            }
        }
        #endif /* CTRLMOD_EN_MENUBAR */

        //Close current dialog (except main or modal dialog)
        if (pFocusedDialog &&
            (pFocusedDialog->control.opts & (CTRL_OPTS_DLG_MODAL|CTRL_OPTS_DLG_DESKTOP)) == 0)
            GUICloseDialog(pFocusedDialog, 0);

        return;
    }
    else if (key == (VIRK_ModAlt|VIRK_Tabulation))
    {
        //Alt+Tab: select next opened dialog

        if (pFocusedDialog &&
            (pFocusedDialog->control.opts & CTRL_OPTS_DLG_MODAL) == 0)
        {
            //End of move/resize mode of current dialog
            pFocusedDialog->modState = DLG_STATE_DEFAULT;

            //Set focus to next dialog
            pFocusedDialog = GUISelectNextDialog(0);
        }

        return;
    }
    #if (CTRLMOD_EN_MENUBAR == 1)
    #if (GUITASKBAR_EN_STARTUPMENU == 1)
    //Try open startup menu
    else if (pStartupMenu->root &&
             key == pStartupMenu->root->hotkey)
    {
        OpenStartupMenu();
        return;
    }
    #endif /* GUITASKBAR_EN_STARTUPMENU */
    #if (GUIDIALOG_EN_DIALOGMENU == 1)
    //Try open dialog menu
    else if (pDialogMenu->root &&
             key == pDialogMenu->root->hotkey)
    {        
        OpenDialogMenu(pFocusedDialog);
        return;
    }
    #endif /* GUIDIALOG_EN_DIALOGMENU */
    #endif /* CTRLMOD_EN_MENUBAR */


    #if (CTRLMOD_EN_MENUBAR == 1)
    #if (GUITASKBAR_EN_STARTUPMENU == 1)
    //Startup menu handler (user defined menu)
    if (pStartupMenu->current)
    {
        MenuBarKeyHandler(pStartupMenu, key, locks);

        //Clear startup menu focus flag
        if (pStartupMenu->current == 0)
            pStartupMenu->control.opts &= ~CTRL_OPTS_FOCUSED;
        return;
    }
    else
    #endif /* GUITASKBAR_EN_STARTUPMENU */

    #if (GUIDIALOG_EN_DIALOGMENU == 1)
    //Dialog menu handler (not user defined menu)
    if (pDialogMenu->current)
    {
        MenuBarKeyHandler(pDialogMenu, key, locks);
        return;
    }
    //Move/resize dialog mode: return
    else if (DialogKeyHandler(pFocusedDialog, key, locks))
    {
        return;
    }
    else
    #endif /* GUIDIALOG_EN_DIALOGMENU */
    #endif /* CTRLMOD_EN_MENUBAR */

    //Get focused control: no focused control - update focus and return
    if ((pFocusedControl = GUIGetFocusedControl(dialogs, count)) == 0)
    {
        if (!(key == VIRK_Enter || key == VIRK_Separator))
            UpdateControlFocus(pFocusedDialog);
        return;
    }

    //Next lines not need:
    //flags already checked at function "GUIGetFocusedControl"
    //if ((focusedControl->opts & CTRL_OPTS_EDITABLE) != CTRL_OPTS_EDITABLE)
    //    return;

    //Copy/paste actions
    if (key == (VIRK_ModCtrl|VIRK_Insert) || //Ctrl+Insert
        key == (VIRK_ModCtrl|VIRK_C))        //Ctrl+C
    {
        //Copy to clipboard
        GUICopyFromControl(pFocusedControl);
        return;
    }
    else if (key == (VIRK_ModShift|VIRK_Insert) || //Shift+Insert
             key == (VIRK_ModCtrl|VIRK_V))         //Ctrl+V
    {
        //Paste from clipboard
        GUIPasteToControl(pFocusedControl);
        return;
    }


    switch(pFocusedControl->type)
    {
        #if (CTRLMOD_EN_MENUBAR == 1 || CTRLMOD_EN_COMBOBOX == 1)
        case CTRL_TYPE_MENUBAR:
        case CTRL_TYPE_COMBOBOX:
        {
            if (pFocusedControl->type == CTRL_TYPE_MENUBAR)
            {
                #if (CTRLMOD_EN_MENUBAR)
                //Dialog-specified menu key handler
                if (MenuBarKeyHandler((MenuBar *)pFocusedControl, key, locks))
                    break;
                #endif
            }
            else
            {
                #if (CTRLMOD_EN_COMBOBOX == 1)
                //Combobox key handler
                if (ComboBoxKeyHandler((ComboBox *)pFocusedControl, key, locks))
                    break;
                #endif
            }

            //No accept button click here

            //Select previous/next control
            if (key == VIRK_Left)
                GUISelectNextControl(-1);
            else if (key == VIRK_Right)
                GUISelectNextControl(1);

            break;
        } //CTRL_TYPE_MENUBAR, CTRL_TYPE_COMBOBOX
        #endif /* CTRLMOD_EN_MENUBAR || CTRLMOD_EN_COMBOBOX */

        #if (CTRLMOD_EN_BUTTON == 1)
        case CTRL_TYPE_BUTTON:
        {
            PButton *button = (PButton *)pFocusedControl;

            //Call click handler
            if (key == VIRK_Enter || key == VIRK_Separator)
            {
                if (button->click_event)
                    button->click_event(button, 0);
            }
            //Select previous/next control
            else if (key == VIRK_Up || key == VIRK_Left)
                GUISelectNextControl(-1);
            else if (key == VIRK_Down || key == VIRK_Right)            
                GUISelectNextControl(1);

            break;
        } //CTRL_TYPE_BUTTON
        #endif /* CTRLMOD_EN_BUTTON */


        #if (CTRLMOD_EN_CHECKBOX == 1 || CTRLMOD_EN_RADIOBUTTON == 1)
        case CTRL_TYPE_CHECKBOX:
        case CTRL_TYPE_RADIOBUTTON:
        {
            if (key == VIRK_Space)
            {
                if (pFocusedControl->type == CTRL_TYPE_CHECKBOX)
                {
                    #if (CTRLMOD_EN_CHECKBOX == 1)
                    ToggleCheckBox((CheckBox*)pFocusedControl);
                    #endif
                }
                else
                {
                    #if (CTRLMOD_EN_RADIOBUTTON == 1)
                    ToggleRadioButton((RadioButton *)pFocusedControl);
                    #endif
                }
            }
            //Click accept button
            else if (key == VIRK_Enter || key == VIRK_Separator)
                GUIAcceptButtonClick(pFocusedDialog);
            //Select previous/next control
            else if (key == VIRK_Up || key == VIRK_Left)
                GUISelectNextControl(-1);
            else if (key == VIRK_Down || key == VIRK_Right)
                GUISelectNextControl(1);

            break;
        } //CTRL_TYPE_CHECKBOX, CTRL_TYPE_RADIOBUTTON
        #endif /* CTRLMOD_EN_CHECKBOX || CTRLMOD_EN_RADIOBUTTON */


        #if (CTRLMOD_EN_TEXTBOX == 1)
        case CTRL_TYPE_TEXTBOX:
        {
            //Ctrl+Tab: select next control, when textbox is multiline
            //the Tab key inserts Tab char (code 0x09)
            if (key == (VIRK_ModCtrl|VIRK_Tabulation))
                GUISelectNextControl(1);
            //Edit text box buffer
            else if (TextBoxKeyHandler((TextBox *)pFocusedControl, key, locks))
                break;

            //Click accept button
            if (key == VIRK_Enter || key == VIRK_Separator)
                GUIAcceptButtonClick(pFocusedDialog);
            //Select previous/next control
            else if (key == VIRK_Up)
                GUISelectNextControl(-1);
            else if (key == VIRK_Down)
                GUISelectNextControl(1);

            break;
        } //CTRL_TYPE_TEXTBOX        
        #endif /* CTRLMOD_EN_TEXTBOX */


        #if (CTRLMOD_EN_LISTBOX == 1 || CTRLMOD_EN_FILELISTBOX == 1 || CTRLMOD_EN_SLIDERBAR == 1)
        case CTRL_TYPE_LISTBOX:
        case CTRL_TYPE_FILELISTBOX:
        case CTRL_TYPE_SLIDERBAR:
        {
            if (pFocusedControl->type == CTRL_TYPE_LISTBOX)
            {
                #if (CTRLMOD_EN_LISTBOX == 1)
                if (ListBoxKeyHandler((ListBox *)pFocusedControl, key, locks))
                    break;
                #endif
            }
            else if (pFocusedControl->type == CTRL_TYPE_FILELISTBOX)
            {
                #if (CTRLMOD_EN_FILELISTBOX == 1)
                if (FileListBoxKeyHandler((FileListBox *)pFocusedControl, key, locks))
                    break;
                #endif
            }
            else
            {
                #if (CTRLMOD_EN_SLIDERBAR == 1)
                if (SliderBarKeyHandler((SliderBar *)pFocusedControl, key, locks))
                    break;
                #endif
            }

            //Click accept button
            if (key == VIRK_Enter || key == VIRK_Separator)
                GUIAcceptButtonClick(pFocusedDialog);
            //Select previous/next control
            else if (key == VIRK_Left)
                GUISelectNextControl(-1);
            else if (key == VIRK_Right)
                GUISelectNextControl(1);

            break;
        } //CTRL_TYPE_LISTBOX, CTRL_TYPE_FILELISTBOX, CTRL_TYPE_SLIDERBAR
        #endif /* CTRLMOD_EN_LISTBOX || CTRLMOD_EN_FILELISTBOX || CTRLMOD_EN_SLIDERBAR */


        //case CTRL_TYPE_PROGRESSBAR:
        //Progress bar cannot accept user input, no actions

        default: break;
    } //switch(pFocusedControl->type)
}



#if (CTRLMOD_EN_TASKBAR == 1)
/* Draw taskbar */
static void DrawTaskBar()
{
    #define STRDLGBUTTONS_BUFFER_LENGTH     192    
    #if (CTRLMOD_EN_MENUBAR == 1 && GUITASKBAR_EN_STARTUPMENU == 1)
    MenuBar *pStartupMenu;
    #endif

    ScreenParams *scp  = guiGlobalState.pScreen;
    GUIDialog *dialogs = guiGlobalState.pDialogs;
    s32 count          = guiGlobalState.dialogsCount;

    CtrlParams *ctrl      = &guiGlobalState.taskBar.control;
    GUITaskBar *pTaskBar  = &guiGlobalState.taskBar;
    GUIDialog *pDialog;    

    s32 i, strOffset, dlgVisCount, dlgButtonWidth, titleLen;
    s16 ctrlWidth, ctrlX;
    const char *strTitle;
    char strDlgButtons[STRDLGBUTTONS_BUFFER_LENGTH];
    char strDefaultTitle[32];

    if (!ctrl || (ctrl->opts & CTRL_OPTS_VISIBLE) == 0 || !count)
        return;

    #if (CTRLMOD_EN_MENUBAR == 1 && GUITASKBAR_EN_STARTUPMENU == 1)
    pStartupMenu = &guiGlobalState.startupMenu;

    if (pStartupMenu->root &&
        pStartupMenu->root->text)
    {
        //Startup menu exist: decrement dialog button region
        s16 d     = 2 + strlen(pStartupMenu->root->text);
        ctrlX     = ctrl->x + d;
        ctrlWidth = ctrl->width - d;
    }
    else
    {
        //No startup menu
        ctrlX     = ctrl->x;
        ctrlWidth = ctrl->width;
    }

    //Draw startup menu
    if (pStartupMenu->root != 0)
    {
        pStartupMenu->control.x = 0;
        pStartupMenu->control.y = ctrl->y;
        DrawMenuBar(scp, &pStartupMenu->control);
    }    

    #else
    //Startup menu module disabled
    ctrlX     = ctrl->x;
    ctrlWidth = ctrl->width;

    #endif /* CTRLMOD_EN_MENUBAR && GUITASKBAR_EN_STARTUPMENU */

    //Draw taskbar rectangle with specified background color
    PrintFillRectangle(scp,
                       ctrlX, ctrl->y, ctrlWidth, ctrl->height,
                       ctrl->backColor, 0x20);

    //Set taskbar foreground color
    SetForeColors(scp,
                  pTaskBar->control.foreColor,
                  ctrlX, ctrl->y, ctrlWidth, ctrl->height);

    //Clear border
    ClearBorderRegion(scp, ctrl->x, ctrl->y, ctrl->width, ctrl->height);

    //Call event handler
    if (pTaskBar->redraw_event)
        pTaskBar->redraw_event(pTaskBar, 0);    


    //Count of visible dialogs
    dlgVisCount = 0;
    for(i = 0; i < count; i++)
    {
        if (dialogs[i].control.opts & CTRL_OPTS_VISIBLE)
            dlgVisCount++;
    }

    //Define width of dialog button on taskbar (without separator)
    if (dlgVisCount == 0)
        return;
    else if ((dlgVisCount * (GUITASKBAR_BUTTON_WIDTH + 2)) <= (ctrlWidth - pTaskBar->userInfoWidth))
        dlgButtonWidth = GUITASKBAR_BUTTON_WIDTH;
    else
        dlgButtonWidth = (s32)((ctrlWidth - pTaskBar->userInfoWidth) / dlgVisCount) - 2;

    if (dlgButtonWidth < 1)
        return;

    //Clear buffer with spaces
    memset(strDlgButtons, 0x20, STRDLGBUTTONS_BUFFER_LENGTH);

    strOffset = 0;
    strDlgButtons[strOffset++] = GUITASKBAR_BUTTON_SEPARATOR;

    for(i = 0; i < count; i++)
    {
        pDialog = &dialogs[i];

        if ((pDialog->control.opts & (CTRL_OPTS_VISIBLE|CTRL_OPTS_DLG_SHOWON_TASKBAR)) ==
             (CTRL_OPTS_VISIBLE|CTRL_OPTS_DLG_SHOWON_TASKBAR))
        {
            //Focused dialog: mark it with colors
            if (pDialog->control.opts & CTRL_OPTS_FOCUSED)
            {
                SetForeBackColors(scp, ctrl->selForeColor, ctrl->selBackColor,
                                  ctrlX + strOffset, ctrl->y,
                                  dlgButtonWidth, ctrl->height);
            }

            if (pDialog->title && ((titleLen = strlen(pDialog->title)) > 0))
            {
                strTitle = pDialog->title;
            }
            else
            {
                //Default title
                std_sprintf(strDefaultTitle, "%s%i", GUITASKBAR_DEFAULT_TITLE, i + 1);
                strTitle = strDefaultTitle;
                titleLen = strlen(strTitle);
            }

            //Copy specified count of title chars to common string buffer
            memcpy(&strDlgButtons[strOffset],
                   strTitle,
                   (dlgButtonWidth < titleLen) ? dlgButtonWidth : titleLen);

            //Add separator
            strOffset += dlgButtonWidth;
            strDlgButtons[strOffset++] = GUITASKBAR_BUTTON_SEPARATOR;
        }
    }

    //Zero terminate string
    strDlgButtons[strOffset] = 0;

    //Print button titles
    DrawTStringLine(scp, ctrlX, ctrl->y, ctrlWidth, strDlgButtons);

    #undef STRDLGBUTTONS_BUFFER_LENGTH
}
#endif /* CTRLMOD_EN_TASKBAR */



/* Redraw dialogs array */
void GUIDrawDialogs()
{
    s32 i;
    u32 opts;

    ScreenParams *scp  = guiGlobalState.pScreen;
    GUIDialog *dialogs = guiGlobalState.pDialogs;
    s32 count          = guiGlobalState.dialogsCount;

    GUIDialog *pDialog;
    GUIDialog *pDialogFocused;    
    GUIDialog *pDialogOnTop;

    //Clear screen: clear data and color buffers
    ClearScreen(scp);

    pDialogFocused = 0;
    pDialogOnTop   = 0;
    for(i = 0; i < count; i++)
    {
        pDialog = &dialogs[i];

        opts = pDialog->control.opts;

        if (opts & CTRL_OPTS_VISIBLE)
        {            
            if (opts & CTRL_OPTS_FOCUSED)
                pDialogFocused = pDialog;    //Focused dialog: save pointer
            else if (opts & CTRL_OPTS_DLG_KEEP_ONTOP)
                pDialogOnTop   = pDialog;    //Top dialog: save pointer
            else
                GUIDrawDialog(scp, pDialog); //Other dialogs: redraw
        }
    }

    //Draw focused dialog
    if (pDialogFocused)
    {
        //Focused dialog is modal
        if (pDialogOnTop && (pDialogFocused->control.opts & CTRL_OPTS_DLG_MODAL))
        {
            //Draw "on top" dialog before modal dialog
            GUIDrawDialog(scp, pDialogOnTop);

            //Reset pointer
            pDialogOnTop = 0;
        }

        GUIDrawDialog(scp, pDialogFocused);
    }

    //Draw "on top" dialog
    if (pDialogOnTop)
        GUIDrawDialog(scp, pDialogOnTop);    

    #if (CTRLMOD_EN_TASKBAR == 1)
    //Draw taskbar as last
    DrawTaskBar(scp, &guiGlobalState.taskBar.control, dialogs, count);
    #endif
}



/* Move focus to next/previous control.
   dir > 0: next; dir < 0: previous */
CtrlParams *GUISelectNextControl(s32 dir)
{
    GUIDialog *dialogs = guiGlobalState.pDialogs;
    s32 count          = guiGlobalState.dialogsCount;
    s32 i, j, k, childCount;
    CtrlChainItem *childs;
    GUIDialog *foundDialog = 0;

    //For each dialog
    for(i = 0; i < count; i++)
    {
        if ((dialogs[i].control.opts & CTRL_OPTS_EDITABLE_MASK) == CTRL_OPTS_EDITABLE)
        {
            //For each child control
            foundDialog = &dialogs[i];
            childs      = foundDialog->childs;
            childCount  = foundDialog->childCount;
            for(j = 0; j < childCount; j++)
            {
                if (  childs[j].control &&
                    ((childs[j].control->opts & CTRL_OPTS_EDITABLE_MASK) == CTRL_OPTS_EDITABLE))
                {
                    childs[j].control->opts &= ~CTRL_OPTS_FOCUSED;

                    //Search next/previuous child control
                    k = j;
                    do
                    {
                        if (dir < 0)
                        {
                            //Prev
                            k--;
                            if (k < 0)
                                k = childCount - 1;
                        }
                        else
                        {
                            //Next
                            k = (k + 1) % childCount;
                        }

                        if ((childs[k].control->opts & CTRL_OPTS_CANFOCUSED_MASK) == CTRL_OPTS_CANFOCUSED)                                                    
                            break;

                    } while(k != j);

                    #if (CTRLMOD_EN_FILELISTBOX == 1)
                    if (childs[k].control->type == CTRL_TYPE_FILELISTBOX)
                        UpdateFileListBox((FileListBox*)childs[k].control);
                    #endif

                    childs[k].control->opts |= CTRL_OPTS_FOCUSED;
                    return childs[k].control;
                }

            } //for

            break;
        }
    } //for

    return UpdateControlFocus(foundDialog);
}



/* Get pointer to focused control */
CtrlParams *GUIGetFocusedControl()
{
    GUIDialog *dialogs = guiGlobalState.pDialogs;
    s32 count          = guiGlobalState.dialogsCount;
    CtrlChainItem *childs;
    CtrlParams *ctrl;
    s32 i, j;    

    //Search at dialogs
    for(i = 0; i < count; i++)
    {
        if ((dialogs[i].control.opts & CTRL_OPTS_EDITABLE_MASK) == CTRL_OPTS_EDITABLE)
        {
            //Search at child controls
            childs = dialogs[i].childs;
            for(j = 0; j < dialogs[i].childCount; j++)
            {
                ctrl = childs[j].control;
                if (ctrl && ((ctrl->opts & CTRL_OPTS_EDITABLE_MASK) == CTRL_OPTS_EDITABLE))
                    return ctrl;
            } //while

            break;
        }
    } //for

    return 0;
}



/* Set focus to specified control */
void GUISetFocusedControl(CtrlParams *ctrl)
{
    s32 i;
    CtrlParams *chctrl;

    if (!ctrl ||
        (ctrl->opts & CTRL_OPTS_EDITABLE_MASK) != (CTRL_OPTS_VISIBLE|CTRL_OPTS_ENABLED))
        return;

    if (ctrl->parent)
    {
        //Parent exists and it is dialog (form)
        if (ctrl->parent->type == CTRL_TYPE_FORM)
        {
            //Clear focus for all child controls
            GUIDialog *pDialog = (GUIDialog *)ctrl->parent;

            for(i = 0; i < pDialog->childCount; i++)
            {
                chctrl = pDialog->childs[i].control;
                if (chctrl && (chctrl->opts & CTRL_OPTS_FOCUSED))
                {
                    if (chctrl->type == CTRL_TYPE_MENUBAR)
                    {
                        MenuBar *mb = (MenuBar *)chctrl;
                        mb->current = 0;
                    }
                    else if (chctrl->type == CTRL_TYPE_COMBOBOX)
                        chctrl->opts &= ~CTRL_OPTS_CBLVISIBLE;

                    chctrl->opts &= ~CTRL_OPTS_FOCUSED;
                }
            }

            //Set focus to specified control
            ctrl->opts |= CTRL_OPTS_FOCUSED;
        }
    }
    else
    {
        //No parent
        //Set focus to specified control
        ctrl->opts |= CTRL_OPTS_FOCUSED;
    }
}



/* Move focus to next dialog */
GUIDialog *GUISelectNextDialog(u32 isSkipMinimized)
{
    GUIDialog *dialogs = guiGlobalState.pDialogs;
    s32 count          = guiGlobalState.dialogsCount;
    GUIDialog *pDialog;
    s32 i, j, p;
    u32 opts;

    for(i = 0; i < count; i++)
    {
        if ((dialogs[i].control.opts & (CTRL_OPTS_VISIBLE|CTRL_OPTS_FOCUSED)) ==
                                       (CTRL_OPTS_VISIBLE|CTRL_OPTS_FOCUSED))
        {
            dialogs[i].control.opts &= ~CTRL_OPTS_FOCUSED;

            //Find next visible dialog
            p = i;
            for(j = 0; j < count; j++)
            {
                p = (p + 1) % count;
                opts = dialogs[p].control.opts;
                if (opts & CTRL_OPTS_VISIBLE)
                {
                    if (isSkipMinimized && (opts & CTRL_OPTS_DLG_MINIMIZED))
                    { }
                    else
                    {
                        i = p;
                        break;
                    }
                }
            }

            //Restore selected dialog from minimized state
            pDialog = &dialogs[i];

            if (isSkipMinimized == 0)
                pDialog->control.opts &= ~CTRL_OPTS_DLG_MINIMIZED;

            pDialog->control.opts |= CTRL_OPTS_FOCUSED;            

            UpdateControlFocus(pDialog);

            return pDialog;
        }
    }

    return 0;
}



/* Get pointer to focused dialog */
GUIDialog *GUIGetFocusedDialog()
{
    GUIDialog *dialogs = guiGlobalState.pDialogs;
    s32 count          = guiGlobalState.dialogsCount;
    s32 i;

    for(i = 0; i < count; i++)
    {
        if ((dialogs[i].control.opts & (CTRL_OPTS_VISIBLE|CTRL_OPTS_FOCUSED)) ==
                                       (CTRL_OPTS_VISIBLE|CTRL_OPTS_FOCUSED))
            return &dialogs[i];
    }

    return 0;
}



/* Copy selected data to clipboard */
void GUICopyFromControl(CtrlParams *src)
{
    if (src == 0 ||
        (src->opts & (CTRL_OPTS_VISIBLE|CTRL_OPTS_ENABLED)) != (CTRL_OPTS_VISIBLE|CTRL_OPTS_ENABLED))
        return;

    #if (CTRLMOD_EN_TEXTBOX == 1)
    if (src->type == CTRL_TYPE_TEXTBOX)
        CopyFromTextBox((TextBox*)src);
    #endif /* CTRLMOD_EN_TEXTBOX */

    /*else if (src->type == CTRL_TYPE_LISTBOX)
    {
        //Source: listbox
        ListBox *listBox = (ListBox *)src;

        //Convert listbox items to string representaion
    }*/
}



/* Paste clipboard data to focused control */
void GUIPasteToControl(CtrlParams *dest)
{
    //Check control flags
    if (dest == 0 ||
        (dest->opts & (CTRL_OPTS_RDONLY|CTRL_OPTS_NONEINPUT|CTRL_OPTS_VISIBLE|CTRL_OPTS_ENABLED)) !=
            (CTRL_OPTS_VISIBLE|CTRL_OPTS_ENABLED))
        return;

    //Destination: textbox
    #if (CTRLMOD_EN_TEXTBOX == 1)
    if (dest->type == CTRL_TYPE_TEXTBOX)    
        PasteToTextBox((TextBox*)dest);
    #endif

    //Destination: other control
    //else if (dest->type == CTRL_TYPE_x) { ... }
}



/* Cascade dialogs */
void GUICascadeDialogs()
{
    GUIDialog *pDialog;
    GUIDialog *dialogs = guiGlobalState.pDialogs;
    s32 count          = guiGlobalState.dialogsCount;
    s32 i;
    s16 xyScreen;    

    xyScreen = 0;
    for(i = 0; i < count; i++)
    {
        pDialog = &dialogs[i];

        if ((pDialog->control.opts & (CTRL_OPTS_VISIBLE|CTRL_OPTS_DLG_DESKTOP)) ==
             CTRL_OPTS_VISIBLE &&
             pDialog->locState != DLG_STATE_MAXIMIZED)
        {
            pDialog->control.opts &= ~CTRL_OPTS_DLG_MINIMIZED;
            pDialog->control.x = xyScreen;
            pDialog->control.y = xyScreen;
            xyScreen++;
        }
    }
}



/* Minimize all dialogs */
void GUIMinimizeAllDialogs()
{    
    GUIDialog *dialogs = guiGlobalState.pDialogs;
    s32 count          = guiGlobalState.dialogsCount;
    s32 i;

    GUIDialog *pDialog;

    for(i = 0; i < count; i++)
    {
        pDialog = &dialogs[i];

        if ((pDialog->control.opts & (CTRL_OPTS_VISIBLE|CTRL_OPTS_DLG_DESKTOP|CTRL_OPTS_DLG_MODAL)) ==
            CTRL_OPTS_VISIBLE)
        {
            //Minimize none-modal and none-desktop dialog
            pDialog->control.opts |= CTRL_OPTS_DLG_MINIMIZED;
        }
    }
}



/* Draw string line on dialog using dialog colors */
void DrawDialogString(ScreenParams *scp, GUIDialog *pDialog,
                      s32 x, s32 y, s32 width,
                      const char *format, ...)
{
    CtrlParams *ctrl;
    s32 stringLength;
    char dest[512];
    va_list argv;

    if (format == 0 || y < 0 || y >= pDialog->control.height)
        return;

    va_start(argv, format);

    subst_vsprintf(dest, format, argv);

    va_end(argv);

    stringLength = strlen(dest);

    if (stringLength < width)
        width = stringLength;

    ctrl = &pDialog->control;

    if ((x + width) > ctrl->width)
        width = ctrl->width - x;

    //SetForeColors(scp, 0, ctrl->x + x, ctrl->y + y, width, 1);

    DrawTStringLine(scp, ctrl->x + x, ctrl->y + y, width, dest);
}



/* Draw string line on dialog using specified foreground and background colors */
void DrawDialogColorString(ScreenParams *scp, GUIDialog *pDialog,
                      s32 x, s32 y, s32 width,
                      color_t fColor, color_t bColor,
                      const char *format, ...)
{
    CtrlParams *ctrl;
    s32 stringLength;
    char dest[512];
    va_list argv;

    if (format == 0 || y < 0 || y >= pDialog->control.height)
        return;

    va_start(argv, format);

    subst_vsprintf(dest, format, argv);

    va_end(argv);

    stringLength = strlen(dest);

    if (stringLength < width)
        width = stringLength;

    ctrl = &pDialog->control;

    if ((x + width) > ctrl->width)
        width = ctrl->width - x;

    x += ctrl->x;
    y += ctrl->y;

    SetForeBackColors(scp, fColor, bColor, x, y, width, 1);

    DrawTStringLine(scp, x, y, width, dest);
}

/*===========================================================================
 * End of file Dialog.c
 *===========================================================================*/
