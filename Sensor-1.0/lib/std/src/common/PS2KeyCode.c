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
 *  File PS2KeyCode.c: PS/2 keyboard functions.
 *
 *===========================================================================*/

#include "../../inc/PS2KeyCode.h"

//Converter state (state low byte)
#define PS2KBRD_STATE_INITIAL       0
#define PS2KBRD_STATE_EXT_DOWN      1
#define PS2KBRD_STATE_UP            2
#define PS2KBRD_STATE_EXT_UP        3

//"Print Screen" down OR left shift down/up
#define PS2KBRD_STATE_PRINT_DOWN    4

//"Pause/Break" key has 8 bytes code
#define PS2KBRD_STATE_PAUSE_DOWN    10

//"Pause/Break" code
static const u8 ps2_pause_break_code[8] =
{
    0xE1, 0x14, 0x77, 0xE1, 0xF0, 0x14, 0xF0, 0x77
};

//PS/2 to virtual key codes convertation table,
//one byte key codes [00..83]
#define table_ps2kToVKey_length       132
static const KeyCode table_ps2kToVKey[table_ps2kToVKey_length] =
{
    //00..0F
    VIRK_NoKey,       VIRK_F9,          VIRK_NoKey,       VIRK_F5,
    VIRK_F3,          VIRK_F1,          VIRK_F2,          VIRK_F12,
    VIRK_NoKey,       VIRK_F10,         VIRK_F8,          VIRK_F6,
    VIRK_F4,          VIRK_Tabulation,  VIRK_OemTilde,    VIRK_NoKey,

    //10..1F
    VIRK_NoKey,       VIRK_LAltKey,     VIRK_LShiftKey,   VIRK_NoKey,
    VIRK_LControlKey, VIRK_Q,           VIRK_D1,          VIRK_NoKey,
    VIRK_NoKey,       VIRK_NoKey,       VIRK_Z,           VIRK_S,
    VIRK_A,           VIRK_W,           VIRK_D2,          VIRK_NoKey,

    //20..2F
    VIRK_NoKey,       VIRK_C,           VIRK_X,           VIRK_D,
    VIRK_E,           VIRK_D4,          VIRK_D3,          VIRK_NoKey,
    VIRK_NoKey,       VIRK_Space,       VIRK_V,           VIRK_F,
    VIRK_T,           VIRK_R,           VIRK_D5,          VIRK_NoKey,

    //30..3F
    VIRK_NoKey,       VIRK_N,           VIRK_B,           VIRK_H,
    VIRK_G,           VIRK_Y,           VIRK_D6,          VIRK_NoKey,
    VIRK_NoKey,       VIRK_NoKey,       VIRK_M,           VIRK_J,
    VIRK_U,           VIRK_D7,          VIRK_D8,          VIRK_NoKey,

    //40..4F
    VIRK_NoKey,       VIRK_OemComma,    VIRK_K,           VIRK_I,
    VIRK_O,           VIRK_D0,          VIRK_D9,          VIRK_NoKey,
    VIRK_NoKey,       VIRK_OemPeriod,   VIRK_OemQuestion, VIRK_L,
    VIRK_OemSemicolon,VIRK_P,           VIRK_OemMinus,    VIRK_NoKey,

    //50..5F
    VIRK_NoKey,       VIRK_NoKey,       VIRK_OemQuotes,   VIRK_NoKey,
    VIRK_OemOpenBrackets,VIRK_OemPlus,  VIRK_NoKey,       VIRK_NoKey,
    VIRK_CapsLock,    VIRK_RShiftKey,   VIRK_Enter,       VIRK_OemCloseBrackets,
    VIRK_NoKey,       VIRK_OemPipe,     VIRK_NoKey,       VIRK_NoKey,

    //60..6F
    VIRK_NoKey,       VIRK_NoKey,       VIRK_NoKey,       VIRK_NoKey,
    VIRK_NoKey,       VIRK_NoKey,       VIRK_Backspace,   VIRK_NoKey,
    VIRK_NoKey,       VIRK_NumPad1,     VIRK_NoKey,       VIRK_NumPad4,
    VIRK_NumPad7,     VIRK_NoKey,       VIRK_NoKey,       VIRK_NoKey,

    //70..7F
    VIRK_NumPad0,     VIRK_Decimal,     VIRK_NumPad2,     VIRK_NumPad5,
    VIRK_NumPad6,     VIRK_NumPad8,     VIRK_Esc,         VIRK_NumLock,
    VIRK_F11,         VIRK_Add,         VIRK_NumPad3,     VIRK_Subtract,
    VIRK_Multiply,    VIRK_NumPad9,     VIRK_Scroll,      VIRK_NoKey,

    //80..83
    VIRK_NoKey,       VIRK_NoKey,       VIRK_NoKey,       VIRK_F7
};


//Extended codes, two bytes [E0 xx]
#define table_ps2kToVKey_ext_offset           0x69
#define table_ps2kToVKey_ext_length           21
static const KeyCode table_ps2kToVKey_ext[table_ps2kToVKey_ext_length] =
{
    //[E0 69]..[E0 7D]
    VIRK_End,         VIRK_NoKey,       VIRK_Left,        VIRK_Home,
    VIRK_NoKey,       VIRK_NoKey,       VIRK_NoKey,       VIRK_Insert,
    VIRK_Delete,      VIRK_Down,        VIRK_NoKey,       VIRK_Right,
    VIRK_Up,          VIRK_NoKey,       VIRK_NoKey,       VIRK_NoKey,
    VIRK_NoKey,       VIRK_PageDown,    VIRK_NoKey,       VIRK_PrintScreen,
    VIRK_PageUp
};


/* Convert numpad digits [0..9] to control keys (when "Num Lock" is OFF) */
static const KeyCode table_numpad_convert[10] =
{
    //0          1         2          3
    VIRK_Insert, VIRK_End, VIRK_Down,  VIRK_PageDown,

    //4          5         6           7
    VIRK_Left,   VIRK_D5,  VIRK_Right, VIRK_Home,

    //8          9
    VIRK_Up,     VIRK_PageUp
};


/* Convert PS/2 key codes to virtual key codes.
   Notes:
   1. Pause/Break idiots code:
      E1 14 77 E1 F0 14 F0 77 (fucking freaks!),
      no repeat, no up.
   2. Print Screen code:
      down   E0 12 E0 7C;
      repeat E0 7C;
      up     E0 F0 7C E0 F0 12 */
KeyCode PS2ToVirtualKeyCode(PS2KbrdState *ps2st, u8 inbyte)
{
    KeyCode vkey;
    KeyCode modstate;
    PS2KeyCode ps2key;
    u32 rxstate;
    u8 locks;

    //Init out flags: no flags
    ps2st->oflags = 0;

    ps2key = inbyte;

    //Output virtual key: initial no key
    vkey   = VIRK_NoKey;

    locks    =  ps2st->locks;
    modstate = (ps2st->modifiers);
    rxstate  = (ps2st->state);

    switch (rxstate)
    {
        case PS2KBRD_STATE_EXT_DOWN:
        {
            //Extended codes (KEY DOWN)
            if (ps2key == 0xF0)
            {
                //[E0 F0 xx]: key up, wait key code
                rxstate = PS2KBRD_STATE_EXT_UP;
            }
            else if (ps2key == 0x12)
            {
                //[E0 12]: go to initial state
                rxstate = PS2KBRD_STATE_INITIAL;
            }
            else
            {
                //Extended key code: [E0 xx]
                if (ps2key >= table_ps2kToVKey_ext_offset &&
                    ps2key < (table_ps2kToVKey_ext_offset + table_ps2kToVKey_ext_length))
                {
                    vkey = table_ps2kToVKey_ext[(ps2key - table_ps2kToVKey_ext_offset)];
                }
                else
                {
                    ps2key |= 0xE000;
                    switch(ps2key)
                    {
                        case PS2K_RAltKey:
                        {
                            vkey = VIRK_RAltKey;
                            modstate |= VIRK_ModAlt;
                            break;
                        }

                        case PS2K_RontrolKey:
                        {
                            vkey = VIRK_RControlKey;
                            modstate |= VIRK_ModCtrl;
                            break;
                        }

                        case PS2K_Divide:    vkey = VIRK_Divide;      break;
                        case PS2K_Separator: vkey = VIRK_Separator;   break;
                        case PS2K_Apps:      vkey = VIRK_Apps;        break;
                        case PS2K_LWin:      vkey = VIRK_LWin;        break;
                        case PS2K_RWin:      vkey = VIRK_RWin;        break;
                        default: break;
                    }
                }

                rxstate = PS2KBRD_STATE_INITIAL;
            }

            break;
        }

        case PS2KBRD_STATE_EXT_UP:
        {
            //Extended key codes (E0 F0 xx): KEY UP
            if (ps2key >= table_ps2kToVKey_ext_offset &&
                ps2key < (table_ps2kToVKey_ext_offset + table_ps2kToVKey_ext_length))
            {
                vkey = table_ps2kToVKey_ext[(ps2key - table_ps2kToVKey_ext_offset)];
                ps2st->oflags = PS2K_FLAG_KEY_UP;
            }
            else if (ps2key == 0x12)  //[E0 F0 12]
            {
                //No actions, go to initial state
            }
            else
            {
                //Extended key code: add E0 byte
                ps2key |= 0xE000;
                ps2st->oflags = PS2K_FLAG_KEY_UP;

                switch(ps2key)
                {
                    case PS2K_RAltKey:
                    {
                        vkey = VIRK_RAltKey;
                        modstate &= ~VIRK_ModAlt;
                        break;
                    }

                    case PS2K_RontrolKey:
                    {
                        vkey = VIRK_RControlKey;
                        modstate &= ~VIRK_ModCtrl;
                        break;
                    }

                    case PS2K_Divide:      vkey = VIRK_Divide;      break;
                    case PS2K_Separator:   vkey = VIRK_Separator;   break;
                    case PS2K_Apps:        vkey = VIRK_Apps;        break;
                    case PS2K_LWin:        vkey = VIRK_LWin;        break;
                    case PS2K_RWin:        vkey = VIRK_RWin;        break;

                    //Undefined key code: no flag
                    default: ps2st->oflags = 0; break;
                } //switch
            }

            rxstate = PS2KBRD_STATE_INITIAL;
            break;
        } //PS2KBRD_STATE_EXT_UP


        case PS2KBRD_STATE_UP:
        {
            //One byte key codes [F0 xx]: KEY UP
            if (ps2key < table_ps2kToVKey_length)
            {
                vkey = table_ps2kToVKey[ps2key];

                switch(vkey)
                {
                    //Unset modifiers
                    case VIRK_LShiftKey:
                    case VIRK_RShiftKey:   modstate &= ~VIRK_ModShift;   break;

                    case VIRK_LControlKey: modstate &= ~VIRK_ModCtrl;    break;
                    case VIRK_LAltKey:     modstate &= ~VIRK_ModAlt;     break;

                    //Toggle locks (by key up)
                    /*case VIRK_CapsLock:  locks ^= (VIRK_LOCK_CAPS_ON);   break;
                    case VIRK_NumLock:   locks ^= (VIRK_LOCK_NUM_ON);    break;
                    case VIRK_Scroll:    locks ^= (VIRK_LOCK_SCROLL_ON); break;*/

                    default: break;
                }

                ps2st->oflags = PS2K_FLAG_KEY_UP;
            }

            rxstate = PS2KBRD_STATE_INITIAL;
            break;
        } //PS2KBRD_STATE_UP


        default:
        {
            //Initial state
            if (rxstate >= PS2KBRD_STATE_PAUSE_DOWN && rxstate <= (PS2KBRD_STATE_PAUSE_DOWN + 7))
            {
                //Receive Pause/Break codes
                if (ps2key == ps2_pause_break_code[(rxstate - PS2KBRD_STATE_PAUSE_DOWN)])
                {
                    rxstate++;

                    if (rxstate == (PS2KBRD_STATE_PAUSE_DOWN + 8))
                    {
                        vkey = VIRK_Pause;
                        rxstate = PS2KBRD_STATE_INITIAL;
                    }
                }
                else
                    rxstate = PS2KBRD_STATE_INITIAL;
            }
            else if (ps2key == 0xE0)
            {
                //Extended code (2 bytes): [E0 xx]
                rxstate = PS2KBRD_STATE_EXT_DOWN;
            }
            else if (ps2key == 0xF0)
            {
                //Key up, wait key code [F0 xx]
                rxstate = PS2KBRD_STATE_UP;
            }
            else if (ps2key == 0xE1)
            {
                //First byte of "Pause/Break" code
                rxstate = (PS2KBRD_STATE_PAUSE_DOWN + 1);
            }
            else if (ps2key < table_ps2kToVKey_length)
            {
                //One byte codes, KEY DOWN, no change state here
                vkey = table_ps2kToVKey[ps2key];

                //Save modifiers
                if (vkey == VIRK_LShiftKey || vkey == VIRK_RShiftKey)
                    modstate |= VIRK_ModShift;
                else if (vkey == VIRK_LControlKey)
                    modstate |= VIRK_ModCtrl;
                else if (vkey == VIRK_LAltKey)
                    modstate |= VIRK_ModAlt;
                else
                {
                    //Toggle locks (by key down)
                    switch(vkey)
                    {
                        case VIRK_CapsLock:  locks ^= (VIRK_LOCK_CAPS_ON);   break;
                        case VIRK_NumLock:   locks ^= (VIRK_LOCK_NUM_ON);    break;
                        case VIRK_Scroll:    locks ^= (VIRK_LOCK_SCROLL_ON); break;
                        default: break; //no actions by default
                    }
                }
            }

            break;
        }
    } //switch

    //Convert numpad key codes (when "Num Lock" is OFF)
    if ((vkey >= VIRK_NumPad0 && vkey <= VIRK_NumPad9))
    {
        if ((locks & VIRK_LOCK_NUM_ON) == 0)
            vkey = table_numpad_convert[(vkey - VIRK_NumPad0)];
    }
    else if (vkey == VIRK_Decimal && (locks & VIRK_LOCK_NUM_ON) == 0)
    {
        //numpad '.' => 'Del'
        vkey = VIRK_Delete;
    }

    //Add modifiers to virtual key
    if (vkey != VIRK_NoKey)
        vkey = modstate | vkey;

    //Update PS/2 state
    ps2st->modifiers = modstate;
    ps2st->state     = rxstate;
    ps2st->locks     = locks;

    //Return virtual key
    return vkey;
}

/*===========================================================================
 * End of file PS2KeyCode.c
 *===========================================================================*/
