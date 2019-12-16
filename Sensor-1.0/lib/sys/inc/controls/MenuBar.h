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
 *  File MenuBar.h: Menu definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __MENUBAR_H
#define __MENUBAR_H

#include "../../../bastypes.h"
#include "../../std/inc/colors.h"
#include "../../../std/inc/KeyCode.h"
#include "../screen.h"
#include "ctrltypes.h"

//Menu item options (MenuItem::opts)
//Item types
#define MI_TYPE_TEXT            0x00   //text
#define MI_TYPE_NUMERIC         0x01   //number
#define MI_TYPE_COUNTER         0x02   //counter
#define MI_TYPE_FLAG            0x03   //flag
#define MI_TYPE_CHECKBOX        0x04   //checkbox
#define MI_TYPE_SPLITTER        0x05   //splitter
#define MI_TYPE_MASK            0x0F   //item type mask
//Item options
#define MI_OPTS_PWDCHARS        0x0010   //Password chars
#define MI_OPTS_RDONLY          0x0020   //Read only value
#define MI_OPTS_ISCHECKED       0x0040   //Checked item
#define MI_OPTS_DISABLED        0x0080   //Disabled item
#define MI_OPTS_NOMENUBUTTON    0x0100   //Don't draw menu button
#define MI_OPTS_NOSHOWHOTKEY    0x0200   //Don't draw menu button hotkey

//Default item options
#define MI_OPTS_DEFAULT         (MI_TYPE_TEXT)

//Menu selection directions (SelectMenuItem function)
#define MBDIR_PREVIOUS          0
#define MBDIR_NEXT              1
#define MBDIR_PARENT            2
#define MBDIR_SUBMENU           3


//Char code that indicates that current menu item has submenu
#ifndef MI_INDICCHAR_SUBMENU
#define MI_INDICCHAR_SUBMENU      0x10
#endif /* MI_INDICCHAR_SUBMENU */

//Char code that indicates that current menu item is checked
#ifndef MI_INDICCHAR_CHECKED
#define MI_INDICCHAR_CHECKED      0xBB //Variants: 0xB9, 0xBE, '+', '*'
#endif /* MI_INDICCHAR_CHECKED */


typedef struct tag_MenuItem MenuItem;

/* Represents menu item */
typedef struct tag_MenuItem
{
        //Menu item identifier (any 16-bit number)
        u16 id;

        //Menu item options (see MI_OPTS_x)
        u16 opts;

        //Hot key
        KeyCode hotkey;

        //Pointer to char buffer with item text
        const char *text;

        //Pointer to parent menu item
        MenuItem *parent;

        //Pointer to array of sub items
        MenuItem *sub;        

        //Count of sub items
        u16 subLength;

        //Index of currently selected item at
        //MenuItem::sub array
        u16 subSel;
} MenuItem;


typedef void         (*MenuOpen) (MenuItem *mi);
typedef void         (*MenuClick) (MenuItem *mi);
typedef const char * (*MenuPrint)(MenuItem *mi, u32 mindex, char *txtBuffer);

typedef MenuOpen        LpMenuOpen;
typedef MenuPrint       LpMenuPrint;

/* Represents menu bar control */
typedef struct tag_MenuBar
{
    //Size and position
    CtrlParams control;

    //Pointer to root element
    MenuItem *root;

    //Pointer to currently selected item
    MenuItem *current;    

    //Called when submenu is open
    LpMenuOpen   open;

    //Called when menu item is click
    LpMenuOpen   click;

    //Called when submenu is printed
    LpMenuPrint  print;    
} MenuBar;

/* Draw menu using specified screen. */
void DrawMenuBar(ScreenParams *scp, CtrlParams *ctrl);

/* Perform recursive search at submenu */
u32 MenuBarHotkeyHandler(MenuBar *menuBar, MenuItem *mi, KeyCode hotkey);

/* Key handler: scroll listbox, select item */
u32 MenuBarKeyHandler(MenuBar *menuBar, KeyCode key, u32 locks);

/* Change current menu item using specified direction mbdir. (see MBDIR_x) */
MenuItem *SelectMenuItem(MenuBar *mb, s32 mbdir);

#endif /* __MENUBAR_H */

/*===========================================================================
 * End of file MenuBar.h
 *===========================================================================*/

