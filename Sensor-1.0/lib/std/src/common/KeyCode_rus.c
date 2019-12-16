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
 *  File KeyCode_rus.c: Virtual keyboard functions, russian layout.
 *
 *  Notes:
 *  1. This file at Windows-1251 encoding
 *  2. Commands to convert from UTF-8 to Windows-1251:
 *     $ iconv -f UTF-8 -t WINDOWS-1251 KeyCode_rus.c > KeyCode_rus_1251.c
 *     $ rm -f KeyCode_rus.c
 *     $ mv KeyCode_rus_1251.c KeyCode_rus.c
 *
 *===========================================================================*/

#include "../../inc/KeyCode.h"

//QWERTY keyboard: ðóññêàÿ ðàñêëàäêà, symbols
static const char *kbrd_qwerty_digit_symbols_rus = ")!\"¹;%:?*(";

//QWERTY keyboard: russian letters [A..Z] => [rus EF.. rus YA]
//except 6 rus letters on right buttons
static const char *kbrd_qwerty_letters_az_rus = "ÔÈÑÂÓÀÏÐØÎËÄÜÒÙÇÉÊÛÅÃÌÖ×Íß";

/* Convert virtual key code to char code (russian) */
static u8 VirtualKeyToCharCode_rus(KeyCode key, u32 locks)
{
    //Return char code, NULL by default
    u8 ch = 0x00;

    KeyCode modifier;

    modifier = (key & VIRK_MODIFIER_MASK); //Shift,Alt,Ctrl modifiers
    key      = (key & VIRK_KEYCODE_MASK);  //Key code

    if (key >= VIRK_D0 && key <= VIRK_D9)
    {
        //Russian
        if (modifier == VIRK_ModShift)
        {
            ch = kbrd_qwerty_digit_symbols_rus[(key - VIRK_D0)]; //russian symbols
            if (ch == 0xB9) //(N)umber
                return 0xBC;
            else
                return ch;
        }
        else
            return (u8)key;                                        //digits [0..9]
    }
    else if (key >= VIRK_NumPad0 && key <= VIRK_NumPad9)
    {
        return (u8)((key - VIRK_NumPad0) + 0x30); //numpad digits [0..9]
    }
    else if (key >= VIRK_A && key <= VIRK_Z)
    {
        //Russian
        if (locks & VIRK_LOCK_CAPS_ON)
            modifier ^= VIRK_ModShift;

        if (modifier == VIRK_ModShift)
            return (u8)(kbrd_qwerty_letters_az_rus[(key - VIRK_A)]); //Capital char
        else
            return (u8)(kbrd_qwerty_letters_az_rus[(key - VIRK_A)] + 0x20); //Small char
    }
    else
    {
        u32 isfound = 1;

        switch (key)
        {
            //Not sensitive to shift key (note that "return" used)
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


            //Sensitive to shift key (note that "return" used)
            case VIRK_OemComma:         ch = 'Á'; break;
            case VIRK_OemPeriod:        ch = 'Þ'; break;

            case VIRK_OemOpenBrackets:  ch = 'Õ'; break;
            case VIRK_OemCloseBrackets: ch = 'Ú'; break;

            case VIRK_OemSemicolon:     ch = 'Æ';  break;
            case VIRK_OemQuotes:        ch = 'Ý'; break;

            case VIRK_OemTilde:         ch = '¨'; break;
            default: isfound = 0; break;
        }

        if (isfound)
        {
            if (locks & VIRK_LOCK_CAPS_ON)
                modifier ^= VIRK_ModShift;

            if (modifier != VIRK_ModShift)
            {
                //Small char
                if (ch == 0xA8)
                    ch += 0x10; //Small rus IO
                else
                    ch += 0x20; //Other chars
            }
        }
        else if (modifier == VIRK_ModShift)
        {
            //shift+key
            switch (key)
            {
                case VIRK_OemMinus:    ch = '_'; break;
                case VIRK_OemPlus:     ch = '+'; break;
                case VIRK_OemPipe:     ch = '/'; break;
                case VIRK_OemQuestion: ch = ','; break;
                default: break;
            }
        }
        else
        {
            //no shift
            switch (key)
            {
                case VIRK_OemMinus:    ch = '-';  break;
                case VIRK_OemPlus:     ch = '=';  break;
                case VIRK_OemPipe:     ch = '\\'; break;
                case VIRK_OemQuestion: ch = '.';  break;
                default: break;
            }
        }
    }

    return ch;
}

/*===========================================================================
 * End of file KeyCode_rus.c
 *===========================================================================*/
