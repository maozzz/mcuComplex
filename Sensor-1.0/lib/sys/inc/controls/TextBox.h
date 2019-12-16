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
 *  File TextBox.h: Text box definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __TEXTBOX_H
#define __TEXTBOX_H

#include "../../../bastypes.h"
#include "../../std/inc/KeyCode.h"
#include "../screen.h"
#include "ctrltypes.h"
#include "ScrollBar.h"

#ifndef TEXTBOX_CHCODE_PASSWORDCHAR
#define TEXTBOX_CHCODE_PASSWORDCHAR     '*'
#endif

typedef struct tag_TextBox TextBox;

/* Text box */
typedef struct tag_TextBox
{
    //Textbox parameters
    CtrlParams control;

    //Pointer to text buffer
    char *text;

    //Maximum length of text buffer, chars
    //(including zero terminate)
    s32 textMaxLength;


    //Scroll by vertical, count of lines
    s32 scrollV;

    //Scroll by horizontal, count of chars
    s32 scrollH;


    //Current cursor position (char index)
    s32 curPos;

    //Current cursor X-position (column index)
    s32 curPosX;

    //Current cursor Y-position (line index)
    s32 curPosY;

    //Saved cursor X-position
    s32 prevCurX;


    //Selection start
    s32 selStart;

    //Selection end
    s32 selEnd;


    //Work variables
    //Count of lines at text (updated with call of GetLinesCount)
    s32 countOfLines;

    //Maximum width of line, chars (updated with call of GetLineMaxWidth)
    s32 lineMaxWidth;

    //Index of first visible char
    s32 indexFirstVisible;
    s32 xBufFirstVisible;
    s32 yBufFirstVisible;

    //Scrollbar vertical
    ScrollBar sbv;

    //Scrollbar horizontal
    ScrollBar sbh;

    //Key handler
    u32 (*key_pressed_event)(TextBox *textBox, KeyCode key, u32 locks);

    //Pointetr to event handler (text changed)
    void (*text_changed_event)(TextBox *textBox, void *args);
} TextBox;


/* Draw text box */
void DrawTextBox(ScreenParams *scp, CtrlParams *ctrl);

/* Key handler: move cursor, edit text buffer */
u32 TextBoxKeyHandler(TextBox *textBox, KeyCode key, u32 locks);

/* Scroll text box by vertical */
void ScrollTextBoxV(TextBox *textBox, s32 value);

/* Scroll text box by horizontal */
void ScrollTextBoxH(TextBox *textBox, s32 value);

/* Scroll textbox content to specified coordinates */
void ScrollTextBoxToCoord(TextBox *textBox, s32 xCur, s32 yCur);

/* Get count of lines at text buffer */
s32 GetLinesCount(TextBox *textBox);

/* Get maximum line length at text buffer */
s32 GetLineMaxWidth(TextBox *textBox);

/* Convert char position to coordinates */
s32 CharPosToCoord(TextBox *textBox, s32 pos, s32 *xChar, s32 *yChar);

/* Line wrap ON/OFF */
void SetTextBoxLineWrap(TextBox *textBox, u32 on);

/* Set textbox text.
   Not need call "RefreshTextBox" after this fucntion */
void SetTextBoxText(TextBox *textBox, const char *src);

/* Update textbox.
   Call this function when size of textbox changed */
void UpdateTextBox(TextBox *textBox);

/* Refresh textbox.
   Call this function when text at buffer is changed */
void RefreshTextBox(TextBox *textBox);

/* Refresh textbox scrollbars state and
   scroll content to cursor position. */
void RefreshTextBoxScroll(TextBox *textBox);

/* Copy selected text from textbox to clipboard */
void CopyFromTextBox(TextBox *textBox);

/* Paste clipboard data to textbox */
void PasteToTextBox(TextBox *textBox);

#endif /* __TEXTBOX_H */

/*===========================================================================
 * End of file TextBox.h
 *===========================================================================*/

