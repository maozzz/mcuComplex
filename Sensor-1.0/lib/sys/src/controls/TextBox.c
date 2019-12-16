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
 *  File TextBox.c: Text box functions.
 *
 *===========================================================================*/

#include "../../inc/controls/TextBox.h"
#include "../../inc/controls/Dialog.h"
#include "controls-pr.h"
#include <string.h>

//Count of chars on tabulation char
//Tabulation with 4 chars (X coord always incremented, see code)
#ifndef TAB_OFFSET
#define TAB_OFFSET       3
#endif

#define CHCODE_NULL       0x00
#define CHCODE_TAB        0x09
#define CHCODE_CR         0x0D
#define CHCODE_LF         0x0A


#if (__TEXTBOX_BUFFER_UNALIGNED_ACCESS_ENABLED == 0)

/* When buffer memory has 16-bit cells: can't use unaligned access */
static void WriteTextBufferChar(char *text, s32 offset, char ch)
{
    u16 *pu16 = (u16*)text;
    u16 tmp;

    pu16 = &pu16[(offset >> 1)];

    tmp = *pu16;

    if (offset & 1) //Odd offset (replace major byte)
        tmp = (tmp & 0x00FF) + (((u16)ch) << 8);
    else            //Even offset (replace minor byte)
        tmp = (tmp & 0xFF00) + ((u16)ch);

    *pu16 = tmp;
}

#else

/* Unaligned access enabled */
__STATIC_INLINE void WriteTextBufferChar(char *text, s32 offset, char ch)
{
    text[offset] = ch;
}

#endif  /* __TEXTBOX_BUFFER_UNALIGNED_ACCESS_ENABLED */


/* */
static void UpdateFirstVisiblePosition(CtrlParams *ctrl)
{
    //Pointer to current control
    TextBox *textBox = (TextBox *)ctrl;
    //Char coordinates at text buffer, char index, string length
    s32 xBuf, yBuf, charIndex, strLength;
    //Top-left corner of text box
    s32 xTBox, yTBox;
    //Indicates the word wrap option is ON
    u32 isWordWrap;
    //Restricted height
    s16 ctrlHeight;
    //Char code that put to screen buffer
    char chcode;

    if (!ctrl)
        return;

    textBox->indexFirstVisible = -1;

    if ((ctrl->opts & CTRL_OPTS_VISIBLE) == 0 || !textBox->text)
        return;

    strLength = strlen(textBox->text);

    if (ctrl->opts & CTRL_OPTS_WORDWRAP)
        textBox->scrollH = 0;

    ctrlHeight = GetRestrictedHeight(ctrl);

    if (ctrlHeight <= 0)
        return;

    if (ctrl->parent)
    {
        //Parent control defined
        xTBox = ctrl->parent->x + ctrl->x;
        yTBox = ctrl->parent->y + ctrl->y;
    }
    else
    {
        //No parent
        xTBox = ctrl->x;
        yTBox = ctrl->y;
    }

    xBuf = 0;
    yBuf = 0;
    charIndex = 0;

    isWordWrap = (ctrl->opts & CTRL_OPTS_WORDWRAP);

    xTBox -= textBox->scrollH;
    yTBox -= textBox->scrollV;

    while(charIndex <= strLength)
    {
        chcode = textBox->text[charIndex];

        //Skip CR
        if (chcode == CHCODE_CR)
        {
            charIndex++;
            continue;
        }

        //Check that char visible on text box and print it
        if (yBuf >= textBox->scrollV && xBuf >= textBox->scrollH)
        {
            //First vis here
            textBox->indexFirstVisible = charIndex;
            textBox->xBufFirstVisible  = xBuf;
            textBox->yBufFirstVisible  = yBuf;
            break;
        }

        //Get coordinates of next char
        charIndex++;
        xBuf++;

        switch(chcode)
        {
            case CHCODE_LF:  { xBuf = 0; yBuf++;   break; }
            case CHCODE_TAB: { xBuf += TAB_OFFSET; break; }
        }

        //Wrap to next line
        if (isWordWrap && xBuf >= ctrl->width)
        {
            xBuf -= ctrl->width;
            yBuf++;
        }

        //Next chars will invisible: break drawing
        if (yBuf >= textBox->scrollV + ctrlHeight)
            break;
    } //while
}


/* */
static void ClearTextBoxWorkVars(TextBox *textBox)
{
    textBox->countOfLines = 0;
    textBox->lineMaxWidth = 0;
    UpdateFirstVisiblePosition(&textBox->control);
}



/* Draw textbox */
void DrawTextBox(ScreenParams *scp, CtrlParams *ctrl)
{        
    //Pointer to current control
    TextBox *textBox = (TextBox *)ctrl;
    //Colors
    color_t fColor, bColor;
    //Char coordinates at text buffer, char index, string length
    s32 xBuf, yBuf, charIndex, strLength;
    //Screen coordinates
    s32 xScreen, yScreen;
    //Top-left corner of text box
    s32 xTBox, yTBox;
    //Flags: cursor visible, password textbox
    s32 isCurVisible, isPwdTextbox;
    //Selection start, end
    s32 selStart, selEnd;
    //Indicates the word wrap option is ON
    u32 isWordWrap;
    //Restricted width and height
    s16 ctrlWidth, ctrlHeight;
    //Char code that put to screen buffer
    char chcode;

    if (!ctrl || (ctrl->opts & CTRL_OPTS_VISIBLE) == 0)
        return;
    else if (!textBox->text)
        return;    

    strLength = strlen(textBox->text);

    if (ctrl->opts & CTRL_OPTS_WORDWRAP)
        textBox->scrollH = 0;

    isCurVisible = 0;

    ctrlWidth  = GetRestrictedWidth(ctrl);
    ctrlHeight = GetRestrictedHeight(ctrl);

    if (ctrlWidth <= 0 || ctrlHeight <= 0)
        return;

    if (ctrl->parent)
    {
        //Parent control defined
        xTBox = ctrl->parent->x + ctrl->x;
        yTBox = ctrl->parent->y + ctrl->y;

        if ((ctrl->parent->opts & CTRL_OPTS_ENABLED) &&
            (ctrl->opts & CTRL_OPTS_ENABLED))
        {
            fColor = ctrl->foreColor;
            bColor = ctrl->backColor;

            if ((ctrl->parent->opts & CTRL_OPTS_FOCUSED) &&
                (ctrl->opts & CTRL_OPTS_FOCUSED))
                isCurVisible = 1;
        }
        else
        {
            //Textbox disabled or parent disabled
            fColor = ctrl->disForeColor;
            bColor = ctrl->parent->backColor;
        }
    }
    else
    {
        //No parent
        xTBox = ctrl->x;
        yTBox = ctrl->y;

        if (ctrl->opts & CTRL_OPTS_ENABLED)
        {
            fColor = ctrl->foreColor;
            bColor = ctrl->backColor;

            if (ctrl->opts & CTRL_OPTS_FOCUSED)
                isCurVisible = 1;
        }
        else
        {
            //Textbox disabled
            fColor = ctrl->disForeColor;
            bColor = ctrl->backColor;
        }
    }

    //Hide cursor
    if (ctrl->opts & CTRL_OPTS_NOCURSOR)
        isCurVisible = 0;

    if (ctrl->opts & CTRL_OPTS_PWDBOX)
        isPwdTextbox = 1;
    else
        isPwdTextbox = 0;

    //VARIANT 1.
    //Set textbox region fore colors
    //SetForeColors(scp, fColor, xTBox, yTBox, ctrlWidth, ctrlHeight);
    //Fill textbox region with spaces
    //PrintFillRectangle(scp, xTBox, yTBox, ctrlWidth, ctrlHeight, bColor, 0x20);

    //VARIANT 2.
    //Set textbox region fore/back colors
    SetForeBackColors(scp, fColor, bColor, xTBox, yTBox, ctrlWidth, ctrlHeight);

    //Draw border
    if ((ctrl->opts & CTRL_OPTS_BORDER_MASK) != CTRL_OPTS_BORDER_NONE)
        PrintBorderRectangle(scp, xTBox, yTBox, ctrlWidth, ctrlHeight);

    //Check cursor position
    if (textBox->curPos < strLength &&
        textBox->text[(textBox->curPos)] == CHCODE_CR)
    {
        textBox->curPos++;
    }
    else if (textBox->curPos > strLength)
    {
        textBox->curPos = strLength;
        textBox->selStart = 0;
        textBox->selEnd = 0;
    }

    //Selection start/end
    if (textBox->selStart > textBox->selEnd)
    {
        selStart = textBox->selEnd;
        selEnd   = textBox->selStart;
    }
    else
    {
        selStart = textBox->selStart;
        selEnd   = textBox->selEnd;
    }


    isWordWrap = (ctrl->opts & CTRL_OPTS_WORDWRAP);

    xTBox -= textBox->scrollH;
    yTBox -= textBox->scrollV;

    //Experimental: start draw from specified index
    if (textBox->indexFirstVisible > 0)
    {
        xBuf = textBox->xBufFirstVisible;
        yBuf = textBox->yBufFirstVisible;
        charIndex = textBox->indexFirstVisible;
    }
    else
    {
        xBuf = 0;
        yBuf = 0;
        charIndex = 0;
    }    

    while(charIndex <= strLength)
    {            
            chcode = textBox->text[charIndex];           

            //Skip CR
            if (chcode == CHCODE_CR)
            {
                charIndex++;
                continue;
            }

            //Check that char visible on text box and print it
            if (yBuf >= textBox->scrollV && xBuf >= textBox->scrollH)
            {                
                if (xBuf < textBox->scrollH + ctrlWidth)
                {
                    //TextBox coords => Screen coords
                    xScreen = xTBox + xBuf;
                    yScreen = yTBox + yBuf;

                    //Check that char visible at screen
                    if (xScreen >= 0 && yScreen >= 0 &&
                        xScreen < scp->width && yScreen < scp->height)
                    {
                        //Draw cursor (invert fore/back colors)
                        if (isCurVisible && charIndex == (s32)textBox->curPos)
                        {
                            SetForeBackColor(scp,
                                             ctrl->backColor, ctrl->foreColor,
                                             xScreen, yScreen);
                        }

                        //Highlight selected text
                        if (charIndex >= selStart && charIndex < selEnd)
                        {
                            SetForeBackColor(scp,
                                             ctrl->selForeColor, ctrl->selBackColor,
                                             xScreen, yScreen);
                        }

                        if (!(chcode == CHCODE_LF || chcode == CHCODE_TAB))
                        {
                            u8 bch = (isPwdTextbox) ? TEXTBOX_CHCODE_PASSWORDCHAR : chcode;

                            scp->buffer[(yScreen * scp->width + xScreen)] = bch;
                        }
                    }
                }
            }

            //Get coordinates of next char
            charIndex++;
            xBuf++;

            switch(chcode)
            {
                case CHCODE_LF:  { xBuf = 0; yBuf++;   break; }
                case CHCODE_TAB: { xBuf += TAB_OFFSET; break; }
            }

            //Wrap to next line
            if (isWordWrap && xBuf >= ctrl->width)
            {                
                xBuf -= ctrl->width;
                yBuf++;               
            }

            //Next chars will invisible: break drawing
            if (yBuf >= textBox->scrollV + ctrlHeight)
                break;
    } //while        

    #if (CTRLMOD_EN_SCROLLBAR == 1)
    //Draw vertical scroll bar
    if (ctrl->opts & CTRL_OPTS_SCBARV)
    {
        //VScroll bar is attached to right side of textbox
        CtrlParams *sbvCtrl = &textBox->sbv.control;
        sbvCtrl->x      = ctrl->x + ctrl->width;
        sbvCtrl->y      = ctrl->y;
        sbvCtrl->height = ctrl->height;
        sbvCtrl->width  = 1;
        DrawScrollBar(scp, sbvCtrl);
    }

    //Draw horizontal scroll bar
    if (ctrl->opts & CTRL_OPTS_SCBARH)
    {
        //HScroll bar is attached to bottom side of textbox
        CtrlParams *sbhCtrl = &textBox->sbh.control;
        sbhCtrl->x      = ctrl->x;
        sbhCtrl->y      = ctrl->y + ctrl->height;
        sbhCtrl->width  = ctrl->width;
        sbhCtrl->height = 1;
        DrawScrollBar(scp, sbhCtrl);
    }
    #endif /* CTRLMOD_EN_SCROLLBAR */
}



/* Convert coordinates of specified char to position */
static s32 CharCoordToPos(TextBox *textBox, s32 *xChar, s32 *yChar)
{
    //Pointer to current control
    CtrlParams *ctrl;
    //Char coordinates at text buffer, char index, string length
    s32 xBuf, yBuf, xCurr, charIndex, strLength;
    //Requested coordinates
    s32 xReq, yReq;
    //Indicates the word wrap option is ON
    u32 isWordWrap;
    //Char code that put to screen buffer
    char chcode;

    ctrl = &textBox->control;

    strLength = strlen(textBox->text);

    xReq = *xChar;
    yReq = *yChar;

    if (xReq < 0)
        xReq = 0;

    if (yReq < 0)
        yReq = 0;

    xCurr = 0;
    xBuf = 0;
    yBuf = 0;
    charIndex = 0;    

    isWordWrap = (ctrl->opts & CTRL_OPTS_WORDWRAP);

    while(charIndex < strLength)
    {        
        chcode = textBox->text[charIndex];

        //Skip CR
        if (chcode == CHCODE_CR)
        {
            charIndex++;
            continue;
        }

        xCurr = xBuf;

        if (yBuf == yReq && xBuf == xReq)
        {
            *xChar = xCurr;
            *yChar = yBuf;
            return charIndex;
        }

        //Get coordinates of next char
        charIndex++;
        xBuf++;

        switch(chcode)
        {
            case CHCODE_LF:
            {
                if (yBuf == yReq)
                {
                    *xChar = xCurr;
                    *yChar = yBuf;
                    return (charIndex - 1);
                }

                xBuf = 0;
                yBuf++;
                break;
            }

            case CHCODE_TAB:
            {                
                xBuf += TAB_OFFSET;
                break;
            }
        }

        //Wrap to next line
        if (isWordWrap && xBuf >= ctrl->width)
        {            
            if (yBuf == yReq)
            {
                *xChar = xCurr;
                *yChar = yBuf;
                return (charIndex - 1);
            }

            xBuf -= ctrl->width;
            yBuf++;
        }
    } //while

    *xChar = xBuf;
    *yChar = yBuf;
    return strLength;
}



/* Convert char position to coordinates */
s32 CharPosToCoord(TextBox *textBox, s32 pos, s32 *xChar, s32 *yChar)
{
    //Pointer to current control
    CtrlParams *ctrl;
    //Char coordinates at text buffer, char index, string length
    s32 xBuf, yBuf, charIndex, strLength;
    //Indicates the word wrap option is ON
    u32 isWordWrap;
    //Char code that put to screen buffer
    char chcode;

    *xChar = 0;
    *yChar = 0;

    if (!textBox || !textBox->text)
        return 0;

    ctrl = &textBox->control;

    strLength = strlen(textBox->text);

    xBuf = 0;
    yBuf = 0;
    charIndex = 0;

    isWordWrap = (ctrl->opts & CTRL_OPTS_WORDWRAP);

    while(charIndex <= strLength)
    {        
        chcode = textBox->text[charIndex];

        //Skip CR
        if (chcode == CHCODE_CR)
        {
            charIndex++;
            continue;
        }

        //Found coordinates
        if (charIndex == pos)
        {
            *xChar = xBuf;
            *yChar = yBuf;
            return 1;
        }

        //Get coordinates of next char
        charIndex++;
        xBuf++;

        switch(chcode)
        {
            case CHCODE_LF:  { xBuf = 0; yBuf++; break;   }
            case CHCODE_TAB: { xBuf += TAB_OFFSET; break; }
        }

        //Wrap to next line
        if (isWordWrap && xBuf >= ctrl->width)
        {
            xBuf -= ctrl->width;
            yBuf++;
        }
    } //while

    return 0;
}



/* Get count of lines at text buffer */
s32 GetLinesCount(TextBox *textBox)
{
    //String length
    s32 strLength;
    //Char coordinates at text buffer
    s32 xBuf, yBuf, charIndex, cwidth;
    //Word wrap on
    u32 isWordWrap;
    //Char
    char chcode;

    if (!textBox || !textBox->text)
            return 0;
    else if (textBox->countOfLines != 0)
        return textBox->countOfLines;

    strLength  = strlen(textBox->text);
    cwidth     = textBox->control.width;
    isWordWrap = (textBox->control.opts & CTRL_OPTS_WORDWRAP);

    //At least one line at text even the text has zero length
    yBuf = 1;
    xBuf = 0;
    charIndex = 0;

    while(charIndex < strLength)
    {
            chcode = textBox->text[charIndex];

            //Skip CR
            if (chcode == CHCODE_CR)
            {
                charIndex++;
                continue;
            }

            charIndex++;
            xBuf++;

            switch(chcode)
            {
                case CHCODE_LF:
                {
                    xBuf = 0;
                    yBuf++;
                    break;
                }

                case CHCODE_TAB:
                {
                    xBuf += TAB_OFFSET;
                    break;
                }
            }

            //Check X coordinate
            if (isWordWrap && xBuf >= cwidth)
            {
                xBuf -= cwidth;
                yBuf++;
            }
    } //while

    textBox->countOfLines = yBuf;
    return yBuf;
}



/* Get maximum line length at text buffer */
s32 GetLineMaxWidth(TextBox *textBox)
{
    //String length
    s32 strLength;
    //Char coordinates at text buffer
    s32 xBuf = 0, charIndex = 0, maxWidth = 0;
    //Char code
    char chcode;

    if (!textBox || !textBox->text)    
            return 0;
    else if (textBox->control.opts & CTRL_OPTS_WORDWRAP)
    {
        textBox->lineMaxWidth = textBox->control.width;
        return textBox->lineMaxWidth;
    }
    else if (textBox->lineMaxWidth != 0)
        return textBox->lineMaxWidth;

    strLength = strlen(textBox->text);
    xBuf = 0;
    charIndex = 0;
    maxWidth = -1;

    while(charIndex <= strLength)
    {
        chcode = textBox->text[charIndex];

        //Skip CR
        if (chcode == CHCODE_CR)
        {
            charIndex++;
            continue;
        }

        charIndex++;
        xBuf++;

        switch(chcode)
        {
            case CHCODE_LF:            
            {
                if (maxWidth < xBuf)
                    maxWidth = xBuf;

                xBuf = 0;
                break;
            }

            case CHCODE_TAB:
            {
                xBuf += TAB_OFFSET;
                break;
            }
        }
    } //while

    if (maxWidth < xBuf)
        maxWidth = xBuf;

    textBox->lineMaxWidth = maxWidth;
    return maxWidth;
}



/* Delete selected text */
static s32 DeleteSelectedText(TextBox *textBox, u32 enableEvents)
{
    s32 sstart, send, slen;
    s32 strLength, i, delOffset;
    char *text;

    //Read-only text box: can't delete text
    if (textBox->selStart == textBox->selEnd ||
        (textBox->control.opts & CTRL_OPTS_RDONLY))
        return 0;

    if (textBox->selStart > textBox->selEnd)
    {
        sstart = textBox->selEnd;
        send   = textBox->selStart;
    }
    else
    {
        sstart = textBox->selStart;
        send   = textBox->selEnd;
    }

    slen = send - sstart;

    text      = textBox->text;
    strLength = strlen(text);

    //Shift chars
    while(send < strLength)
    {        
        u8 srcch = text[send];
        WriteTextBufferChar(text, sstart, srcch);
        sstart++;
        send++;
    }

    //Fill tail chars with zeros
    //memset(&text[(strLength - slen)], 0, slen);
    delOffset = strLength - slen;
    for(i = 0; i < slen; i++)
    {
        WriteTextBufferChar(text, delOffset, 0);
        delOffset++;
    }

    //Update cursor position
    if (textBox->curPos >= textBox->selStart)
    {
        if (textBox->curPos >= slen)
            textBox->curPos -= slen;

        if (textBox->prevCurX >= slen)
            textBox->prevCurX -= slen;
    }

    //Clear selection
    textBox->selStart = textBox->selEnd = 0;

    ClearTextBoxWorkVars(textBox); //on delete selected

    //Call event handler
    if (enableEvents && textBox->text_changed_event)
        textBox->text_changed_event(textBox, 0); //delete selected

    return 1;
}



/* Edit text buffer */
static u32 EditTextBoxBuffer(TextBox *textBox, u8 chcode)
{
    s32 i, chshift, textlen;
    s32 cpos;
    char *text;

    if (chcode == 0 ||
        (textBox->control.opts & CTRL_OPTS_RDONLY))
        return 0;

    textlen = (s32)strlen(textBox->text);
    text    = textBox->text;

    if (chcode == 0x08) //Backspace
    {
        //Delete char before cursor
        if (textlen > 0 && textBox->curPos > 0)
        {
            cpos = textBox->curPos;

            if (text[cpos] == CHCODE_LF &&
                text[cpos - 1] == CHCODE_CR)
            {
                if (cpos > 2 && text[cpos - 3] == CHCODE_CR)
                {
                    cpos -= 3;  //CR LF CR2 LF2 => CR2 LF2
                    chshift = 2;
                }
                else if (cpos > 1)
                {
                    cpos -= 2; //K CR LF => CR LF
                    chshift = 1;
                }
                else //CR LF => no change, return
                    return 1;
            }
            else
            {
                if (cpos > 1 && text[cpos - 2] == CHCODE_CR)
                {
                    //CR LF K => K
                    cpos -= 2;
                    chshift = 2;
                }
                else
                {
                    //K1 K2 => K2
                    cpos -= 1;
                    chshift = 1;
                }
            }

            //Shift chars left
            for(i = cpos; i < textlen; i++)
            {                
                //text[i] = text[(i + chshift)];
                u8 srcch = text[(i + chshift)];
                WriteTextBufferChar(text, i, srcch);
            }

            if (chshift == 2)
            {
                textBox->curPos -= 2;

                //text[textlen - 2] = 0;
                //text[textlen - 1] = 0;
                WriteTextBufferChar(text, textlen - 2, 0);
                WriteTextBufferChar(text, textlen - 1, 0);
            }
            else
            {
                textBox->curPos--;

                //text[textlen - 1] = 0;
                WriteTextBufferChar(text, textlen - 1, 0);

                //Remove CR
                if (textBox->curPos == (textlen - 1) &&
                    textBox->curPos > 0 &&
                    text[(textBox->curPos - 1)] == CHCODE_CR)
                {
                    textBox->curPos--;

                    //text[textBox->curPos] = 0;
                    WriteTextBufferChar(text, textBox->curPos, 0);
                }
            }

            ClearTextBoxWorkVars(textBox); //on backspace

            if (textBox->text_changed_event)
                textBox->text_changed_event(textBox, 0); //backspace

            return 1;
        }
    }
    else if (chcode == 0x7F) //Delete
    {
        //Delete char after cursor                
        if (textlen > 0 && textBox->curPos < textlen)
        {
            cpos = textBox->curPos;

            if (cpos > 0 &&
                text[cpos] == CHCODE_LF &&
                text[cpos - 1] == CHCODE_CR)
            {
                cpos = textBox->curPos - 1; //CR LF K => K
                chshift = 2;
            }
            else
            {
                cpos = textBox->curPos; //K1 K2 => K2
                chshift = 1;
            }

            //Shift chars left
            for(i = cpos; i < textlen; i++)
            {
                //text[i] = text[i + chshift];
                u8 srcch = text[i + chshift];
                WriteTextBufferChar(text, i, srcch);
            }

            if (chshift == 2)
            {
                textBox->curPos--;
                //text[textlen - 2] = 0;
                //text[textlen - 1] = 0;
                WriteTextBufferChar(text, textlen - 2, 0);
                WriteTextBufferChar(text, textlen - 1, 0);
            }
            else
            {
                //text[textlen - 1] = 0;
                WriteTextBufferChar(text, textlen - 1, 0);
            }

            //Skip CR
            if (text[(textBox->curPos)] == CHCODE_CR)
                textBox->curPos++;

            ClearTextBoxWorkVars(textBox); //on delete

            if (textBox->text_changed_event)
                textBox->text_changed_event(textBox, 0); //delete char

            return 1;
        }
    }    
    else if ((textlen + 1) < textBox->textMaxLength)
    {
        //Insert char to string       
        if ((chcode == 0x09 || chcode == 0x0A) && textBox->control.height == 1)
        {
            //Single line text box: return
            return 0;
        }

        //First step: delete selected text
        if (DeleteSelectedText(textBox, 0))
            textlen = (s32)strlen(text);

        //Second step: insert char
        //Note: insert always 0x0A (not 0x0D 0x0A)

        //Shift chars right
        for(i = textlen; i > textBox->curPos; i--)
        {            
            u8 srcch = text[i - 1];
            WriteTextBufferChar(text, i, srcch);
        }

        WriteTextBufferChar(text, textBox->curPos, chcode);
        textBox->curPos++;
        WriteTextBufferChar(text, textlen + 1, 0);

        ClearTextBoxWorkVars(textBox); //on insert char

        if (textBox->text_changed_event)
            textBox->text_changed_event(textBox, 0); //insert char

        return 1;
    }

    return 0;
}







/* */
static void SaveTextBoxSelStart(TextBox *textBox, KeyCode modifier)
{
    if (modifier == VIRK_ModShift &&
        textBox->selStart == textBox->selEnd)
    {
        textBox->selStart = textBox->curPos;
        textBox->selEnd   = textBox->curPos;
    }
}



/* */
static void SaveTextBoxSelEnd(TextBox *textBox, KeyCode modifier)
{
    if (modifier == VIRK_ModShift)
    {
        textBox->selEnd = textBox->curPos;
    }
    else
    {
        textBox->selStart = 0;
        textBox->selEnd   = 0;
    }
}



/* Key handler: move cursor, edit text buffer */
u32 TextBoxKeyHandler(TextBox *textBox, KeyCode key, u32 locks)
{
    //Pointer to current control
    CtrlParams *ctrl;    
    s32 xCur, yCur;
    u32 isMultiline;
    KeyCode modifier;

    if (!textBox || !textBox->text)
        return 0;    

    if (textBox->key_pressed_event)
    {
        if (textBox->key_pressed_event(textBox, key, locks))
            return 1;
    }

    ctrl = &textBox->control;

    isMultiline = (ctrl->height > 1) ? 1 : 0;

    modifier = key & VIRK_MODIFIER_MASK;
    key      = key & VIRK_KEYCODE_MASK;

    //Key code handler
    switch(key)
    {
        case VIRK_Left:
        {
            //Move cursor left
            if (textBox->curPos > 0)
            {
                //Selection start
                SaveTextBoxSelStart(textBox, modifier);

                //Decrement position
                textBox->curPos--;

                //Skip CR
                if (textBox->text[(textBox->curPos)] == CHCODE_CR)
                {
                   if (textBox->curPos > 0)
                       textBox->curPos--;
                   else
                       textBox->curPos++;
                }

                //Selection end
                SaveTextBoxSelEnd(textBox, modifier);

                CharPosToCoord(textBox, textBox->curPos, &xCur, &yCur);

                ScrollTextBoxToCoord(textBox, xCur, yCur);

                textBox->prevCurX = xCur;
                textBox->curPosX  = xCur;
                textBox->curPosY  = yCur;
            }

            return 1;
        }


        case VIRK_Right:
        {
            //Move cursor right        
            s32 strLength = (s32)strlen(textBox->text);

            if (textBox->curPos < strLength)
            {
                //Selection start
                SaveTextBoxSelStart(textBox, modifier);

                //Increment position
                textBox->curPos++;

                //Skip CR
                if (textBox->text[(textBox->curPos)] == CHCODE_CR &&
                   (textBox->curPos < strLength))
                    textBox->curPos++;

                //Selection end
                SaveTextBoxSelEnd(textBox, modifier);

                CharPosToCoord(textBox, textBox->curPos, &xCur, &yCur);

                ScrollTextBoxToCoord(textBox, xCur, yCur);

                textBox->prevCurX = xCur;
                textBox->curPosX  = xCur;
                textBox->curPosY  = yCur;
            }

            return 1;
        }


        case VIRK_Home:
        {
            if (modifier == VIRK_ModCtrl)
            {
                //Ctrl+Home: go to begin of text
                textBox->curPos = (textBox->text[0] == CHCODE_CR) ? 1 : 0;
                RefreshTextBoxScroll(textBox); //Ctrl+Home
            }
            else
            {
                //Selection start
                SaveTextBoxSelStart(textBox, modifier);

                //Move cursor to begin of current line
                CharPosToCoord(textBox, textBox->curPos, &xCur, &yCur);

                xCur = 0;

                textBox->curPos = CharCoordToPos(textBox, &xCur, &yCur);

                //Selection end
                SaveTextBoxSelEnd(textBox, modifier);

                if (textBox->scrollH > 0)
                    ScrollTextBoxH(textBox, -textBox->scrollH);

                textBox->prevCurX = 0;
                textBox->curPosX  = xCur;
                textBox->curPosY  = yCur;
            }

            return 1;
        }


        case VIRK_End:
        {
            if (modifier == VIRK_ModCtrl)
            {
                //Ctrl+End: go to end of text
                textBox->curPos = strlen(textBox->text);
                RefreshTextBoxScroll(textBox); //Ctrl+End
            }
            else
            {
                //Selection start
                SaveTextBoxSelStart(textBox, modifier);

                //Move cursor to end of current line
                CharPosToCoord(textBox, textBox->curPos, &xCur, &yCur);

                xCur = 1000000;

                textBox->curPos = CharCoordToPos(textBox, &xCur, &yCur);

                //Selection end
                SaveTextBoxSelEnd(textBox, modifier);

                ScrollTextBoxToCoord(textBox, xCur, yCur);

                textBox->prevCurX = xCur;
                textBox->curPosX  = xCur;
                textBox->curPosY  = yCur;
            }

            return 1;
        }


        case VIRK_Up:
        case VIRK_Down:
        {
            //Move cursor up/down to one line
            if (isMultiline)
            {                
                //Selection start
                SaveTextBoxSelStart(textBox, modifier);

                CharPosToCoord(textBox, textBox->curPos, &xCur, &yCur);

                //Saved cursor X-position
                if (textBox->prevCurX > 0)
                    xCur = textBox->prevCurX;

                yCur = (key == VIRK_Up) ? (yCur - 1) : (yCur + 1);

                textBox->curPos = CharCoordToPos(textBox, &xCur, &yCur);                

                //Selection end
                SaveTextBoxSelEnd(textBox, modifier);

                ScrollTextBoxToCoord(textBox, xCur, yCur);

                textBox->curPosX  = xCur;
                textBox->curPosY  = yCur;
                return 1;
            }
            else
                return 0;
        }


        case VIRK_PageUp:
        case VIRK_PageDown:
        {
            //Scroll up/down to textbox height
            s32 val;

            if (isMultiline)
            {
                //Selection start
                SaveTextBoxSelStart(textBox, modifier);

                CharPosToCoord(textBox, textBox->curPos, &xCur, &yCur);

                val = (key == VIRK_PageUp) ? -ctrl->height : ctrl->height;

                yCur = yCur + val;

                textBox->curPos = CharCoordToPos(textBox, &xCur, &yCur);

                //Selection end
                SaveTextBoxSelEnd(textBox, modifier);

                ScrollTextBoxV(textBox, val);

                textBox->curPosX  = xCur;
                textBox->curPosY  = yCur;
                return 1;
            }
            else
                return 0;
        }


        default:
        {
            //Edit text buffer (one or multiline)
            key = modifier | key;

            if (key == (VIRK_ModCtrl|VIRK_A))
            {
                //Ctrl+A: select all text
                textBox->selStart = 0;
                textBox->selEnd   = strlen(textBox->text);
                textBox->curPos   = textBox->selEnd;
                RefreshTextBoxScroll(textBox); //Ctrl+A
                return 1;
            }
            else if (key == (VIRK_ModCtrl|VIRK_X) ||
                     key == (VIRK_ModShift|VIRK_Delete))
            {
                //Ctrl+X, Shift+Delete: cut selected text to clipboard
                GUICopyFromControl(&textBox->control);
                DeleteSelectedText(textBox, 1);
                RefreshTextBoxScroll(textBox); //Ctrl+X
                return 1;
            }
            else if ((key == VIRK_Backspace || key == VIRK_Delete) &&
                (textBox->selStart != textBox->selEnd))
            {
                //Delete selected text, no copy to clipboard
                DeleteSelectedText(textBox, 1);
                RefreshTextBoxScroll(textBox); //Backspace,Delete
                return 1;
            }
            else if ((key == VIRK_Enter || key == VIRK_Separator))
            {
                if (isMultiline)
                {                    
                    EditTextBoxBuffer(textBox, VirtualKeyToCharCode(key, locks));
                    RefreshTextBoxScroll(textBox); //New line insert
                    return 1;
                }
                else
                    return 0;
            }
            else if (EditTextBoxBuffer(textBox, VirtualKeyToCharCode(modifier|key, locks)))
            {
                RefreshTextBoxScroll(textBox); //After text edit
                return 1;
            }
            else
                return 0;
        }
    } //switch
}



/* Update textbox scroll state and cursor position */
void RefreshTextBoxScroll(TextBox *textBox)
{
    s32 xCur, yCur;

    ScrollTextBoxV(textBox, 0);
    ScrollTextBoxH(textBox, 0);

    CharPosToCoord(textBox, textBox->curPos, &xCur, &yCur);

    ScrollTextBoxToCoord(textBox, xCur, yCur);

    textBox->prevCurX = xCur;
    textBox->curPosX  = xCur;
    textBox->curPosY  = yCur;
}



/* Scroll text box by vertical */
void ScrollTextBoxV(TextBox *textBox, s32 value)
{
    s32 tmp, linesCount, prevScroll;
    s32 height;

    prevScroll = textBox->scrollV;

    tmp = textBox->scrollV + value;

    linesCount = GetLinesCount(textBox);

    if (tmp < 0)
        tmp = 0;
    else
    {
        height = textBox->control.height;

        if (linesCount <= height)
                tmp = 0;
        else if (tmp > linesCount - height)
                tmp = linesCount - height;
    }

    textBox->scrollV = tmp;

    if (prevScroll != textBox->scrollV)
        UpdateFirstVisiblePosition(&textBox->control);

    #if (CTRLMOD_EN_SCROLLBAR == 1)
    SetScrollValue(&textBox->sbv,
                   tmp,
                   textBox->control.height,
                   linesCount);
    #endif /* CTRLMOD_EN_SCROLLBAR */
}



/* Scroll text box by horizontal */
void ScrollTextBoxH(TextBox *textBox, s32 value)
{
    s32 tmp, maxWidth, prevScroll;

    prevScroll = textBox->scrollH;

    tmp = textBox->scrollH + value;

    maxWidth = GetLineMaxWidth(textBox);    

    if (tmp < 0)
        tmp = 0;
    else
    {
        s16 width = textBox->control.width;

        if (maxWidth <= width)
                tmp = 0;
        else if (tmp > maxWidth - width)
                tmp = maxWidth - width;
    }

    textBox->scrollH = tmp;

    if (prevScroll != textBox->scrollH)
        UpdateFirstVisiblePosition(&textBox->control);

    #if (CTRLMOD_EN_SCROLLBAR == 1)
    SetScrollValue(&textBox->sbh,
                   tmp,
                   textBox->control.width,
                   maxWidth);
    #endif /* CTRLMOD_EN_SCROLLBAR */
}



/* Scroll textbox content to specified coordinates */
void ScrollTextBoxToCoord(TextBox *textBox, s32 xCur, s32 yCur)
{
    CtrlParams *ctrl = &textBox->control;

    //Vertical scroll
    if (yCur < textBox->scrollV)
        ScrollTextBoxV(textBox, yCur - textBox->scrollV);

    else if (yCur >= textBox->scrollV + ctrl->height)
        ScrollTextBoxV(textBox, yCur - (textBox->scrollV + ctrl->height) + 1);


    //Horizontal scroll
    if (xCur < textBox->scrollH)
        ScrollTextBoxH(textBox, xCur - textBox->scrollH);

    else if (xCur >= (textBox->scrollH + ctrl->width))
        ScrollTextBoxH(textBox, xCur - (textBox->scrollH + ctrl->width) + 1);
}



/* Line wrap ON/OFF */
void SetTextBoxLineWrap(TextBox *textBox, u32 on)
{
    if (on)
        textBox->control.opts |= CTRL_OPTS_WORDWRAP;
    else
        textBox->control.opts &= ~CTRL_OPTS_WORDWRAP;

    ClearTextBoxWorkVars(textBox); //on line wrap on/off

    //Update scroll bars
    ScrollTextBoxV(textBox, 0);
    ScrollTextBoxH(textBox, 0);    
}




/* Update textbox.
   Call this function when size of textbox changed */
void UpdateTextBox(TextBox *textBox)
{
    CtrlParams *txtBoxCtrl, *scrBarCtrl;

    txtBoxCtrl = &textBox->control;

    //One line text box: disable word wrap, no vertical scrollbar
    if (txtBoxCtrl->height == 1)
        txtBoxCtrl->opts &= ~(CTRL_OPTS_WORDWRAP|CTRL_OPTS_SCBARV|CTRL_OPTS_SCBARH);

    //Update vertical scrollbar size and position
    scrBarCtrl    = &textBox->sbv.control;
    scrBarCtrl->x = txtBoxCtrl->x + txtBoxCtrl->width;
    scrBarCtrl->y = txtBoxCtrl->y;
    scrBarCtrl->width  = 1;
    scrBarCtrl->height = txtBoxCtrl->height;
    scrBarCtrl->opts  |= CTRL_OPTS_VISIBLE|CTRL_OPTS_ENABLED|CTRL_OPTS_VERTICAL;
    scrBarCtrl->parent = txtBoxCtrl->parent;

    //Update horizontal scrollbar size and position
    scrBarCtrl    = &textBox->sbh.control;
    scrBarCtrl->x = txtBoxCtrl->x;
    scrBarCtrl->y = txtBoxCtrl->y + txtBoxCtrl->height;
    scrBarCtrl->width  = txtBoxCtrl->width;
    scrBarCtrl->height = 1;
    scrBarCtrl->opts  |= CTRL_OPTS_VISIBLE|CTRL_OPTS_ENABLED;
    scrBarCtrl->parent = txtBoxCtrl->parent;

    //Clear work variables
    ClearTextBoxWorkVars(textBox); //on update textbox

    //Update vertical scroll value
    ScrollTextBoxV(textBox, 0);

    //Update horizontal scroll value
    ScrollTextBoxH(textBox, 0);    
}



/* Refresh textbox. Call this function when text at text buffer is changed */
void RefreshTextBox(TextBox *textBox)
{
    //Reset counters
    textBox->curPos   = 0;
    textBox->curPosX  = 0;
    textBox->curPosY  = 0;
    textBox->prevCurX = 0;
    textBox->scrollH  = 0;
    textBox->scrollV  = 0;
    textBox->selStart = 0;
    textBox->selEnd   = 0;

    textBox->countOfLines = 0;
    textBox->lineMaxWidth = 0;

    //Fields:
    //indexFirstVisible, xBufFirstVisible, yBufFirstVisible
    //updated at
    //UpdateTextBox->ClearTextBoxWorkVars->UpdateFirstVisiblePosition

    UpdateTextBox(textBox);
}



/* Set text box text */
void SetTextBoxText(TextBox *textBox, const char *src)
{
    s32 srcLength0, srcLength, isOddLength;
    u16 *pDestU16, *pSrcU16;

    if (!textBox->text || !src)
        return;

    srcLength0 = strlen(src);

    if (srcLength0 == 0)
    {
        //Zero length text
        WriteTextBufferChar(textBox->text, 0, 0);        
    }
    else
    {
        //None-zero length text
        pDestU16 = (u16*)textBox->text;
        srcLength = srcLength0;

        //Length including zero char
        srcLength++;

        if (srcLength > textBox->textMaxLength)
        {
            srcLength   = (textBox->textMaxLength - 2);
            srcLength0  = srcLength;
            isOddLength = 1;
        }
        else
        {
            isOddLength = (srcLength & 1) ? 1 : 0;
        }

        //Count of 16-bit words
        srcLength >>= 1;
        pSrcU16 = (u16*)src;

        while(srcLength)
        {
            *pDestU16++ = *pSrcU16++;
            srcLength--;
        }

        if (isOddLength)
        {
            //Write zero char
            WriteTextBufferChar(textBox->text, srcLength0, 0);
        }
    }

    RefreshTextBox(textBox);
}



/* Copy selected text from textbox to clipboard */
void CopyFromTextBox(TextBox *textBox)
{
    s32 sstart, slen, d;

    if (!guiGlobalState.pClipboard || !textBox->text)
        return;

    if (textBox->selStart > textBox->selEnd)
    {
        sstart = textBox->selEnd;
        slen   = textBox->selStart - sstart;
    }
    else
    {
        sstart = textBox->selStart;
        slen   = textBox->selEnd - sstart;
    }    

    if (slen == 0)
    {
        //No selected text: return
        return;
    }
    //Check that length of selected text less than clipboard size
    else if (slen > guiGlobalState.clipboarMaxLength)
    {
        //VARIANT 1. size = clipboard size
        //slen = guiGlobalState.clipboarMaxLength;

        //VARIANT 2. don't copy to clipboard, return
        //recommended variant
        return;
    }

    d = (slen & 1) ? 1 : 0;

    //Copy data to clipboard buffer
    memcpy(guiGlobalState.pClipboard, &textBox->text[sstart], slen + d);

    guiGlobalState.clipboarLength   = slen;
    guiGlobalState.clipboarDataType = ClipData_Text;
}



/* Paste clipboard data to textbox */
void PasteToTextBox(TextBox *textBox)
{
    s32 i, k, destlen, isTextChanged;
    char       *dstText = textBox->text;
    const char *srcText = (char*)guiGlobalState.pClipboard;

    if (!srcText || !dstText ||
        (textBox->control.opts & CTRL_OPTS_RDONLY))
        return;

    destlen = strlen(dstText);

    i = 0;
    isTextChanged = 0;

    while(i < guiGlobalState.clipboarLength &&
          (destlen + 1) < textBox->textMaxLength)
    {
        //Shift chars right
        for(k = destlen; k > textBox->curPos; k--)
        {            
            u8 srcch = dstText[k - 1];
            WriteTextBufferChar(dstText, k, srcch);
        }

        //Insert char
        destlen++;

        WriteTextBufferChar(dstText, textBox->curPos, srcText[i]);

        textBox->curPos++;
        i++;

        WriteTextBufferChar(dstText, destlen, 0);

        isTextChanged = 1;
    }

    //Call event handler
    if (isTextChanged)
    {
        ClearTextBoxWorkVars(textBox); //on paste text

        RefreshTextBoxScroll(textBox); //on paste text

        if (textBox->text_changed_event)
            textBox->text_changed_event(textBox, 0); //paste to textbox
    }
}

/*===========================================================================
 * End of file TextBox.c
 *===========================================================================*/
