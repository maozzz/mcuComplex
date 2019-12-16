/*===========================================================================
 *
 *  Standard library
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2014
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File KeyCode.h: "QWERTY" keyboard key codes.
 *
 *===========================================================================*/

#ifndef __KEYCODE_H
#define __KEYCODE_H

#include "../../bastypes.h"

#define __VIRK_2ND_LAYOUT_RUS       1
//#define __VIRK_2ND_LAYOUT_xxx     2 //define other layout (if need)

//Selected second layout: russian by default
#ifndef __VIRK_2ND_LAYOUT
#define __VIRK_2ND_LAYOUT           __VIRK_2ND_LAYOUT_RUS
#endif

//Lock flags (VirtualKeyToCharCode function, argument locks)
#define VIRK_LOCK_CAPS_ON            0x01 //Caps Lock is ON
#define VIRK_LOCK_NUM_ON             0x02 //Num Lock is ON
#define VIRK_LOCK_SCROLL_ON          0x04 //Scroll Lock is ON
#define VIRK_LOCK_2ND_LAYOUT         0x08 //Second keyboard layout (not latin)

//The bitmask to extract a key code from a key value.
#define VIRK_KEYCODE_MASK			0xFFFF

//The bitmask to extract modifiers from a key value.
#define VIRK_MODIFIER_MASK			0xFFFF0000

/* Virtual key codes and modifiers */
typedef enum tag_KeyCode
{                
        //No key pressed.
        VIRK_NoKey = 0,
        //The left mouse button.
        VIRK_LButton = 1,
        //The right mouse button.
        VIRK_RButton = 2,
        //The CANCEL key.
        VIRK_Cancel = 3,
        //The middle mouse button (three-button mouse).
        VIRK_MButton = 4,
        //The first x mouse button (five-button mouse).
        VIRK_XButton1 = 5,
        //The second x mouse button (five-button mouse).
        VIRK_XButton2 = 6,

        //The IME Kana/Hangul mode key.
        VIRK_KanaMode  = 21,
        //The IME Junja mode key.
        VIRK_JunjaMode = 23,
        //The IME final mode key.
        VIRK_FinalMode = 24,
        //The IME Kanji/Hanja mode key.
        VIRK_KanjiMode = 25,
        //The IME convert key.
        VIRK_IMEConvert = 28,
        //The IME nonconvert key.
        VIRK_IMENonconvert = 29,
        //The IME accept key.
        VIRK_IMEAccept = 30,
        //The IME mode change key.
        VIRK_IMEModeChange = 31,
        //The HELP key.
        VIRK_Help = 47,


        //The BACKSPACE key.
        VIRK_Backspace = 8,
        //The TAB key.
        VIRK_Tabulation = 9,
        //The LINEFEED key.
        VIRK_LineFeed = 10,
        //The CLEAR key.
        VIRK_Clear = 12,
        //The SHIFT key.
        VIRK_ShiftKey = 16,
        //The CTRL key.
        VIRK_ControlKey = 17,
        //The ALT key.
        VIRK_AltKey = 18,


        //The ENTER key.
        VIRK_Enter = 13,
        //The ESC key.
        VIRK_Esc = 27,
        //The SPACEBAR key.
        VIRK_Space = 32,

		//The PAGE UP key.
        VIRK_PageUp = 33,
        //The PAGE DOWN key.
        VIRK_PageDown = 34,
        //The END key.
        VIRK_End = 35,
        //The HOME key.
        VIRK_Home = 36,

        //The INS key.
        VIRK_Insert = 45,
        //The DEL key.
        VIRK_Delete = 46,

        //The LEFT ARROW key.
        VIRK_Left = 37,
        //The UP ARROW key.
        VIRK_Up = 38,
        //The RIGHT ARROW key.
        VIRK_Right = 39,
        //The DOWN ARROW key.
        VIRK_Down = 40,

        //The SELECT key.
        VIRK_Select = 41,
        //The PRINT key.
        VIRK_Print = 42,
        //The EXECUTE key.
        VIRK_Execute = 43,

        //The PRINT SCREEN key.
        VIRK_PrintScreen = 44,
        //The PAUSE key.
        VIRK_Pause = 19,

        //The CAPS LOCK key.
        VIRK_CapsLock = 20,
        //The NUM LOCK key.
        VIRK_NumLock = 144,
        //The SCROLL LOCK key.
        VIRK_Scroll = 145,

        //The 0 key.
        VIRK_D0 = 48,
        //The 1 key.
        VIRK_D1 = 49,
        //The 2 key.
        VIRK_D2 = 50,
        //The 3 key.
        VIRK_D3 = 51,
        //The 4 key.
        VIRK_D4 = 52,
        //The 5 key.
        VIRK_D5 = 53,
        //The 6 key.
        VIRK_D6 = 54,
        //The 7 key.
        VIRK_D7 = 55,
        //The 8 key.
        VIRK_D8 = 56,
        //The 9 key.
        VIRK_D9 = 57,

        //The A key.
        VIRK_A = 65,
        //The B key.
        VIRK_B = 66,
        //The C key.
        VIRK_C = 67,
        //The D key.
        VIRK_D = 68,
        //The E key.
        VIRK_E = 69,
        //The F key.
        VIRK_F = 70,
        //The G key.
        VIRK_G = 71,
        //The H key.
        VIRK_H = 72,
        //The I key.
        VIRK_I = 73,
        //The J key.
        VIRK_J = 74,
        //The K key.
        VIRK_K = 75,
        //The L key.
        VIRK_L = 76,
        //The M key.
        VIRK_M = 77,
        //The N key.
        VIRK_N = 78,
        //The O key.
        VIRK_O = 79,
        //The P key.
        VIRK_P = 80,
        //The Q key.
        VIRK_Q = 81,
        //The R key.
        VIRK_R = 82,
        //The S key.
        VIRK_S = 83,
        //The T key.
        VIRK_T = 84,
        //The U key.
        VIRK_U = 85,
        //The V key.
        VIRK_V = 86,
        //The W key.
        VIRK_W = 87,
        //The X key.
        VIRK_X = 88,
        //The Y key.
        VIRK_Y = 89,
        //The Z key.
        VIRK_Z = 90,        

        //The 0 key on the numeric keypad.
        VIRK_NumPad0 = 96,
        //The 1 key on the numeric keypad.
        VIRK_NumPad1 = 97,
        //The 2 key on the numeric keypad.
        VIRK_NumPad2 = 98,
        //The 3 key on the numeric keypad.
        VIRK_NumPad3 = 99,
        //The 4 key on the numeric keypad.
        VIRK_NumPad4 = 100,
        //The 5 key on the numeric keypad.
        VIRK_NumPad5 = 101,
        //The 6 key on the numeric keypad.
        VIRK_NumPad6 = 102,
        //The 7 key on the numeric keypad.
        VIRK_NumPad7 = 103,
        //The 8 key on the numeric keypad.
        VIRK_NumPad8 = 104,
        //The 9 key on the numeric keypad.
        VIRK_NumPad9 = 105,

        //The multiply key, numpad '*'.
        VIRK_Multiply = 106,
        //The add key, numpad '+'.
        VIRK_Add = 107,
        //The separator key, numpad 'Enter'.
        VIRK_Separator = 108,
        //The subtract key, numpad '-'.
        VIRK_Subtract = 109,
        //The decimal key, numpad '. Del'.
        VIRK_Decimal = 110,
        //The divide key, numpad '/'.
        VIRK_Divide = 111,        

        //The left Windows logo key.
        VIRK_LWin = 91,
        //The right Windows logo key.
        VIRK_RWin = 92,
        //The application key (key between right Win and right Ctrl).
        VIRK_Apps = 93,
        //The computer sleep key.
        VIRK_BSleep = 95,

        //The left SHIFT key.
        VIRK_LShiftKey = 160,
        //The right SHIFT key.
        VIRK_RShiftKey = 161,
        //The left CTRL key.
        VIRK_LControlKey = 162,
        //The right CTRL key.
        VIRK_RControlKey = 163,
        //The left ALT key.
        VIRK_LAltKey = 164,
        //The right ALT key.
        VIRK_RAltKey = 165,

        //The OEM semicolon key ';:'.
        VIRK_OemSemicolon = 186,
        //The OEM plus key '=+'.
        VIRK_OemPlus = 187,
        //The OEM comma key ',<'.
        VIRK_OemComma = 188,
        //The OEM minus key '-_'.
        VIRK_OemMinus = 189,
        //The OEM period key '.>'.
        VIRK_OemPeriod = 190,
        //The OEM question key '/?'.
        VIRK_OemQuestion = 191,
        //The OEM tilde key '`~'.
        VIRK_OemTilde = 192,
        //The OEM open bracket key '[{'.
        VIRK_OemOpenBrackets = 219,
        //The OEM pipe key '\|'.
        VIRK_OemPipe = 220,
        //The OEM close bracket key ']}'.
        VIRK_OemCloseBrackets = 221,
        //The OEM singled/double quote key ''"'.
        VIRK_OemQuotes = 222,

        //The OEM 8 key.
        VIRK_Oem8 = 223,
        //The OEM angle bracket or backslash key on the RT 102 key keyboard.
        VIRK_OemBackslash = 226,

        //The F1 key.
        VIRK_F1 = 112,
        //The F2 key.
        VIRK_F2 = 113,
        //The F3 key.
        VIRK_F3 = 114,
        //The F4 key.
        VIRK_F4 = 115,
        //The F5 key.
        VIRK_F5 = 116,
        //The F6 key.
        VIRK_F6 = 117,
        //The F7 key.
        VIRK_F7 = 118,
        //The F8 key.
        VIRK_F8 = 119,
        //The F9 key.
        VIRK_F9 = 120,
        //The F10 key.
        VIRK_F10 = 121,
        //The F11 key.
        VIRK_F11 = 122,
        //The F12 key.
        VIRK_F12 = 123,
        //The F13 key.
        VIRK_F13 = 124,
        //The F14 key.
        VIRK_F14 = 125,
        //The F15 key.
        VIRK_F15 = 126,
        //The F16 key.
        VIRK_F16 = 127,
        //The F17 key.
        VIRK_F17 = 128,
        //The F18 key.
        VIRK_F18 = 129,
        //The F19 key.
        VIRK_F19 = 130,
        //The F20 key.
        VIRK_F20 = 131,
        //The F21 key.
        VIRK_F21 = 132,
        //The F22 key.
        VIRK_F22 = 133,
        //The F23 key.
        VIRK_F23 = 134,
        //The F24 key.
        VIRK_F24 = 135,


        //The browser back key.
        VIRK_BrowserBack = 166,
        //The browser forward key.
        VIRK_BrowserForward = 167,
        //The browser refresh key.
        VIRK_BrowserRefresh = 168,
        //The browser stop key.
        VIRK_BrowserStop = 169,
        //The browser search key.
        VIRK_BrowserSearch = 170,
        //The browser favorites key.
        VIRK_BrowserFavorites = 171,
        //The browser home key.
        VIRK_BrowserHome = 172,
        //The volume mute key.
        VIRK_VolumeMute = 173,
        //The volume down key.
        VIRK_VolumeDown = 174,
        //The volume up key.
        VIRK_VolumeUp = 175,
        //The media next track key.
        VIRK_MediaNextTrack = 176,
        //The media previous track key.
        VIRK_MediaPreviousTrack = 177,
        //The media Stop key.
        VIRK_MediaStop = 178,
        //The media play pause key.
        VIRK_MediaPlayPause = 179,
        //The launch mail key.
        VIRK_LaunchMail = 180,
        //The select media key.
        VIRK_SelectMedia = 181,
        //The start application one key.
        VIRK_LaunchApplication1 = 182,
        //The start application two key.
        VIRK_LaunchApplication2 = 183,               

        //The PROCESS KEY key.
        VIRK_ProcessKey = 229,
        //Used to pass Unicode characters as if they were keystrokes. The Packet key
        //value is the low word of a 32-bit virtual-key value used for non-keyboard
        //input methods.
        VIRK_Packet = 231,
        //The ATTN key.
        VIRK_Attn = 246,
        //The CRSEL key.
        VIRK_Crsel = 247,
        //The EXSEL key.
        VIRK_Exsel = 248,
        //The ERASE EOF key.
        VIRK_EraseEof = 249,
        //The PLAY key.
        VIRK_Play = 250,
        //The ZOOM key.
        VIRK_Zoom = 251,
        //A constant reserved for future use.
        VIRK_NoName = 252,
        //The PA1 key.
        VIRK_Pa1 = 253,
        //The CLEAR key.
        VIRK_OemClear = 254,

        //The SHIFT modifier key.
        VIRK_ModShift = 0x010000, //65536,
        //The CTRL modifier key.
        VIRK_ModCtrl  = 0x020000, //131072,
        //The ALT modifier key.
        VIRK_ModAlt   = 0x040000  //262144
} KeyCode;

/* Convert virtual key code to char code */
u8 VirtualKeyToCharCode(KeyCode key, u32 locks);

/* Convert virtual key code to it string representation */
char *VirtualKeyToString(KeyCode key, char *dest);

#endif /* __KEYCODE_H */

/*===========================================================================
 * End of file KeyCode.h
 *===========================================================================*/

