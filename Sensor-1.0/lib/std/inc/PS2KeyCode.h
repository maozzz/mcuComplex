/*===========================================================================
 *
 *  Standard library
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
 *  File PS2KeyCode.h: PS/2 "QWERTY" keyboard key codes.
 *
 *===========================================================================*/

#ifndef __PS2KEYCODE_H
#define __PS2KEYCODE_H

#include "../../bastypes.h"
#include "KeyCode.h"

/* PS/2 key codes */
typedef enum tag_PS2KeyCode
{                
        //No key pressed.
        PS2K_NoKey = 0,

        //The BACKSPACE key.
        PS2K_Backspace = 0x66,
        //The TAB key.
        PS2K_Tabulation = 0x0D,
        //The ENTER key.
        PS2K_Enter = 0x5A,

        //The CAPS LOCK key.
        PS2K_CapsLock = 0x58,

		//The ESC key.
        PS2K_Esc = 0x76,

        //The SPACEBAR key.
        PS2K_Space = 0x29,

        //The INS key.
        PS2K_Insert = 0xE070,
        //The DEL key.
        PS2K_Delete = 0xE071,

        //The HOME key.
        PS2K_Home = 0xE06C,
        //The END key.
        PS2K_End = 0xE069,

        //The PAGE UP key.
        PS2K_PageUp = 0xE07D,
        //The PAGE DOWN key.
        PS2K_PageDown = 0xE07A,

        //The LEFT ARROW key.
        PS2K_Left = 0xE06B,
        //The UP ARROW key.
        PS2K_Up = 0xE075,
        //The RIGHT ARROW key.
        PS2K_Right = 0xE074,
        //The DOWN ARROW key.
        PS2K_Down = 0xE072,

        //The PRINT SCREEN key.
        //When down and than up: [E0 12] [E0 7C] [E0 F0 7C] [E0 F0 12]
        PS2K_PrintScreen = 0xE07C,

        //Pause/Break
        PS2K_Pause = 0xE11477,

        //The 0 key.
        PS2K_D0 = 0x45,
        //The 1 key.
        PS2K_D1 = 0x16,
        //The 2 key.
        PS2K_D2 = 0x1E,
        //The 3 key.
        PS2K_D3 = 0x26,
        //The 4 key.
        PS2K_D4 = 0x25,
        //The 5 key.
        PS2K_D5 = 0x2E,
        //The 6 key.
        PS2K_D6 = 0x36,
        //The 7 key.
        PS2K_D7 = 0x3D,
        //The 8 key.
        PS2K_D8 = 0x3E,
        //The 9 key.
        PS2K_D9 = 0x46,

        //The A key.
        PS2K_A = 0x1C,
        //The B key.
        PS2K_B = 0x32,
        //The C key.
        PS2K_C = 0x21,
        //The D key.
        PS2K_D = 0x23,
        //The E key.
        PS2K_E = 0x24,
        //The F key.
        PS2K_F = 0x2B,
        //The G key.
        PS2K_G = 0x34,
        //The H key.
        PS2K_H = 0x33,
        //The I key.
        PS2K_I = 0x43,
        //The J key.
        PS2K_J = 0x3B,
        //The K key.
        PS2K_K = 0x42,
        //The L key.
        PS2K_L = 0x4B,
        //The M key.
        PS2K_M = 0x3A,
        //The N key.
        PS2K_N = 0x31,
        //The O key.
        PS2K_O = 0x44,
        //The P key.
        PS2K_P = 0x4D,
        //The Q key.
        PS2K_Q = 0x15,
        //The R key.
        PS2K_R = 0x2D,
        //The S key.
        PS2K_S = 0x1B,
        //The T key.
        PS2K_T = 0x2C,
        //The U key.
        PS2K_U = 0x3C,
        //The V key.
        PS2K_V = 0x2A,
        //The W key.
        PS2K_W = 0x1D,
        //The X key.
        PS2K_X = 0x22,
        //The Y key.
        PS2K_Y = 0x35,
        //The Z key.
        PS2K_Z = 0x1A,        

        //The 0 key on the numeric keypad.
        PS2K_NumPad0 = 0x70,
        //The 1 key on the numeric keypad.
        PS2K_NumPad1 = 0x69,
        //The 2 key on the numeric keypad.
        PS2K_NumPad2 = 0x72,
        //The 3 key on the numeric keypad.
        PS2K_NumPad3 = 0x7A,
        //The 4 key on the numeric keypad.
        PS2K_NumPad4 = 0x6B,
        //The 5 key on the numeric keypad.
        PS2K_NumPad5 = 0x73,
        //The 6 key on the numeric keypad.
        PS2K_NumPad6 = 0x74,
        //The 7 key on the numeric keypad.
        PS2K_NumPad7 = 0x6C,
        //The 8 key on the numeric keypad.
        PS2K_NumPad8 = 0x75,
        //The 9 key on the numeric keypad.
        PS2K_NumPad9 = 0x7D,

        //The multiply key, numpad '*'.
        PS2K_Multiply = 0x7C,
        //The add key, numpad '+'.
        PS2K_Add = 0x79,
        //The separator key, numpad 'Enter'.
        PS2K_Separator = 0xE05A,
        //The subtract key, numpad '-'.
        PS2K_Subtract = 0x7B,
        //The decimal key, numpad '.'.
        PS2K_Decimal = 0x71,
        //The divide key, numpad '/'.
        PS2K_Divide = 0xE04A,        

        //The NUM LOCK key.
        PS2K_NumLock = 0x77,
        //The SCROLL LOCK key.
        PS2K_Scroll = 0x7E,

        //The left SHIFT key.
        PS2K_LShift = 0x12,
        //The right SHIFT key.
        PS2K_RShift = 0x59,

        //The left CTRL key.
        PS2K_LCtrl = 0x14,
        //The right CTRL key.
        PS2K_RontrolKey = 0xE014,

        //The left ALT key.
        PS2K_LAlt = 0x11,
        //The right ALT key.
        PS2K_RAltKey = 0xE011,

        //The left Windows logo key.
        PS2K_LWin = 0xE01F,
        //The right Windows logo key.
        PS2K_RWin = 0xE027,
        //The application key (key between right Win and right Ctrl).
        PS2K_Apps = 0xE02F,

        //The OEM tilde key.
        PS2K_OemTilde = 0x0E,

        //The OEM minus key.
        PS2K_OemMinus = 0x4E,
        //The OEM plus key.
        PS2K_OemPlus = 0x55,
        //The OEM pipe key.
        PS2K_OemPipe = 0x5D,

        //The OEM open bracket key.
        PS2K_OemOpenBrackets = 0x54,
        //The OEM close bracket key.
        PS2K_OemCloseBrackets = 0x5B,

        //The OEM semicolon key.
        PS2K_OemSemicolon = 0x4C,
        //The OEM singled/double quote key.
        PS2K_OemQuotes = 0x52,

        //The OEM comma key.
        PS2K_OemComma = 0x41,
        //The OEM period key.
        PS2K_OemPeriod = 0x49,
        //The OEM question key.
        PS2K_OemQuestion = 0x4A,

        //The F1 key.
        PS2K_F1 = 0x05,
        //The F2 key.
        PS2K_F2 = 0x06,
        //The F3 key.
        PS2K_F3 = 0x04,
        //The F4 key.
        PS2K_F4 = 0x0C,
        //The F5 key.
        PS2K_F5 = 0x03,
        //The F6 key.
        PS2K_F6 = 0x0B,
        //The F7 key.
        PS2K_F7 = 0x83,
        //The F8 key.
        PS2K_F8 = 0x0A,
        //The F9 key.
        PS2K_F9 = 0x01,
        //The F10 key.
        PS2K_F10 = 0x09,
        //The F11 key.
        PS2K_F11 = 0x78,
        //The F12 key.
        PS2K_F12 = 0x72
} PS2KeyCode;


//PS2ToVirtualKeyCode function, argument oflag
#define PS2K_FLAG_KEY_UP            0x01


/* PS/2 keyboard control structure */
typedef struct tag_PS2KbrdState
{
    //Stored key modifiers
    KeyCode modifiers;

    //Converter state
    u16 state;

    //Locks (see VIRK_LOCK_x)
    u8 locks;

    //(see PS2K_FLAG_x)
    u8 oflags;    
} PS2KbrdState;


/* Convert PS/2 key codes to virtual key codes */
KeyCode PS2ToVirtualKeyCode(PS2KbrdState *ps2st, u8 inbyte);

#endif /* __PS2KEYCODE_H */

/*===========================================================================
 * End of file PS2KeyCode.h
 *===========================================================================*/
