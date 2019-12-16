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
 *  File KeyCode.c: Virtual keyboard functions.
 *
 *===========================================================================*/

#include "../../inc/KeyCode.h"
#include "../../inc/stdcf.h"

#if (__VIRK_2ND_LAYOUT == __VIRK_2ND_LAYOUT_RUS)
#include "KeyCode_rus.c"
#define VirtualKeyToCharCode_2nd  VirtualKeyToCharCode_rus
//#elif (__VIRK_2ND_LAYOUT == __VIRK_2ND_LAYOUT_xxx)
//#include "KeyCode_xxx.c"
#else
#error "Unknown keyboard layout"
#endif

//Symbols at digit keys
static const char *kbrd_qwerty_digit_symbols_lat = ")!@#$%^&*(";

/* Convert virtual key code to char code (latin layout) */
static u8 VirtualKeyToCharCode_lat(KeyCode key, u32 locks)
{
    u8 ch = 0x00;
    KeyCode modifier;

    modifier = (key & VIRK_MODIFIER_MASK); //Shift,Alt,Ctrl modifiers
    key      = (key & VIRK_KEYCODE_MASK);  //Key code

    if (key >= VIRK_D0 && key <= VIRK_D9)
    {
        //Latin
        if (modifier == VIRK_ModShift)
            return kbrd_qwerty_digit_symbols_lat[(key - VIRK_D0)]; //latin symbols
        else
            return (u8)key;                                        //digits [0..9]
    }
    else if (key >= VIRK_NumPad0 && key <= VIRK_NumPad9)
    {
        return (u8)((key - VIRK_NumPad0) + 0x30); //numpad digits [0..9]
    }
    else if (key >= VIRK_A && key <= VIRK_Z)
    {               
        //Latin [A..Z] or [a..z]
        if (locks & VIRK_LOCK_CAPS_ON)
            modifier ^= VIRK_ModShift;

        if (modifier == VIRK_ModShift)
            return (u8)(key);        //Capital char
        else
            return (u8)(key + 0x20); //Small char
    }
    else
    {
        //Not sensitive to shift key (note that "return" used)
        switch (key)
        {            
            case VIRK_Separator:
            case VIRK_Enter:      return 0x0A; //Enter -> LF char (not CR)
            case VIRK_Backspace:  return 0x08;
            case VIRK_Delete:     return 0x7F;
            case VIRK_Space:      return 0x20;
            case VIRK_Tabulation: return 0x09;

            //Numpad keys
            case VIRK_Divide:    return '/';
            case VIRK_Multiply:  return '*';
            case VIRK_Subtract:  return '-';
            case VIRK_Add:       return '+';
            case VIRK_Decimal:   return '.';
            default: break;
        }

        //Sensitive to shift key
        if (modifier == VIRK_ModShift)
        {
            //shift+key
            switch (key)
            {
                case VIRK_OemComma:     ch = '<'; break;
                case VIRK_OemPeriod:    ch = '>'; break;
                case VIRK_OemQuestion:  ch = '?'; break;

                case VIRK_OemMinus:  ch = '_';  break;
                case VIRK_OemPlus:   ch = '+';  break;
                case VIRK_OemPipe:   ch = '|'; break;

                case VIRK_OemOpenBrackets:  ch = '{'; break;
                case VIRK_OemCloseBrackets: ch = '}'; break;

                case VIRK_OemSemicolon: ch = ':';  break;
                case VIRK_OemQuotes:    ch = '\"'; break;

                case VIRK_OemTilde:     ch = '~'; break;
                default: break;
            }
        }
        else
        {
            //no shift
            switch (key)
            {
                case VIRK_OemComma:    ch = ','; break;
                case VIRK_OemPeriod:   ch = '.'; break;
                case VIRK_OemQuestion: ch = '/'; break;

                case VIRK_OemMinus:  ch = '-';  break;
                case VIRK_OemPlus:   ch = '=';  break;
                case VIRK_OemPipe:   ch = '\\'; break;

                case VIRK_OemOpenBrackets:  ch = '['; break;
                case VIRK_OemCloseBrackets: ch = ']'; break;

                case VIRK_OemSemicolon: ch = ';';  break;
                case VIRK_OemQuotes:    ch = '\''; break;

                case VIRK_OemTilde:     ch = '`'; break;

                default: break;
            }
        }
    }

    return ch;
}



/* Convert virtual key code to char code */
u8 VirtualKeyToCharCode(KeyCode key, u32 locks)
{
    if (locks & VIRK_LOCK_2ND_LAYOUT)
        return VirtualKeyToCharCode_2nd(key, locks);
    else
        return VirtualKeyToCharCode_lat(key, locks);
}



/* Convert virtual key code to it string representation */
char *VirtualKeyToString(KeyCode key, char *dest)
{
    KeyCode modifier;
    const char *pKeyName;
    char keyNameBuf[16];

    if (key == VIRK_NoKey)
    {
        dest[0] = 0;
        return dest;
    }

    modifier = (key & VIRK_MODIFIER_MASK); //Shift,Alt,Ctrl modifiers
    key      = (key & VIRK_KEYCODE_MASK);  //Key code

    pKeyName = keyNameBuf;

    if (key >= VIRK_A && key <= VIRK_Z)
    {
        keyNameBuf[0] = (char)(key);
        keyNameBuf[1] = 0;
    }
    else if (key >= VIRK_D0 && key <= VIRK_D9)
    {
        keyNameBuf[0] = (char)key;
        keyNameBuf[1] = 0;
    }
    else if (key >= VIRK_F1 && key <= VIRK_F24)
    {
        std_sprintf(keyNameBuf, "F%i", key - VIRK_F1 + 1);
    }
    else if (key >= VIRK_NumPad0 && key <= VIRK_NumPad9)
    {
        std_sprintf(keyNameBuf, "NumPad%i", key - VIRK_NumPad0);
    }
    else
    {
        switch(key)
        {
            case VIRK_Left:  pKeyName = "Left";  break;
            case VIRK_Right: pKeyName = "Right"; break;
            case VIRK_Down:  pKeyName = "Down";  break;
            case VIRK_Up:    pKeyName = "Up";    break;

            case VIRK_Esc:        pKeyName = "Esc";       break;
            case VIRK_Tabulation: pKeyName = "Tab";       break;
            case VIRK_Enter:      pKeyName = "Enter";     break;
            case VIRK_Backspace:  pKeyName = "Backspace"; break;

            case VIRK_Insert:   pKeyName = "Ins";    break;
            case VIRK_Home:     pKeyName = "Home";   break;
            case VIRK_Delete:   pKeyName = "Del";    break;
            case VIRK_End:      pKeyName = "End";    break;
            case VIRK_PageUp:   pKeyName = "PgUp";   break;
            case VIRK_PageDown: pKeyName = "PgDown"; break;

            case VIRK_NumLock:  pKeyName = "NumLck"; break;
            case VIRK_CapsLock: pKeyName = "CapLck"; break;
            case VIRK_Scroll:   pKeyName = "ScrLck"; break;

            case VIRK_LShiftKey:  pKeyName = "LShift"; break;
            case VIRK_RShiftKey:  pKeyName = "RShift"; break;

            case VIRK_LControlKey:  pKeyName = "LCtrl"; break;
            case VIRK_RControlKey:  pKeyName = "RCtrl"; break;

            case VIRK_LAltKey:  pKeyName = "LAlt"; break;
            case VIRK_RAltKey:  pKeyName = "RAlt"; break;

            case VIRK_OemTilde: pKeyName = "~"; break;

            case VIRK_LWin:  pKeyName = "LWin"; break;
            case VIRK_RWin:  pKeyName = "RWin"; break;
            case VIRK_Apps:  pKeyName = "Apps"; break;

            default:
            {
                std_sprintf(keyNameBuf, "Key%i", key);
                break;
            }
        }
    }

    std_sprintf(dest, "%s%s%s%s",
                (modifier & VIRK_ModCtrl)  ? "Ctrl+"  : "",
                (modifier & VIRK_ModAlt)   ? "Alt+"  : "",
                (modifier & VIRK_ModShift) ? "Shift+" : "",
                pKeyName);
    return dest;
}

/*===========================================================================
 * End of file KeyCode.c
 *===========================================================================*/
