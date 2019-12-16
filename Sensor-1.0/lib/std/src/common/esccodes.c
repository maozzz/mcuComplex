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
 *  File esccodes.c: ANSI escape sequences.
 *
 *===========================================================================*/
#define _CRT_SECURE_NO_WARNINGS

#include "../../inc/esccodes.h"
#include "../../inc/stdcf.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* Escape sequence maximum length */
#define ESC_SEQ_MAXLENGTH	16

/* */
typedef struct tag_ESCState
{
	u32 esci;
	u8 escSeq[ESC_SEQ_MAXLENGTH];
	LpKeyCodeHandler keyPressed;
} ESCState;


/* */
#define SPRINTF(...)  std_sprintf(&buf[strlen(buf)], __VA_ARGS__)

/* Control Sequence Introducer (CSI): 'Esc[' */
const char ESC_CSI[3] = { 0x1B, 0x5B, 0x00 };

/* Graphic attributes:
0 All attributes off
1 Bold
4 Underscore
5 Blink
7 Reverse video
8 Concealed on*/
#define ESC_GRATTRS_COUNT        6
static const u8 escGrAttrs[ESC_GRATTRS_COUNT] =
{ 0, 1, 4, 5, 7, 8 };


/* Clears the screen */
void ESCClearScreen(char *buf, u8 mode)
{
		SPRINTF("%s%uJ", ESC_CSI, mode & 0x03);
}



/* Clears all characters from the cursor position to the end of the line */
void ESCClearLine(char *buf, u8 mode)
{
		SPRINTF("%s%uK", ESC_CSI, mode & 0x03);
}



/* Moves the cursor up by the specified number of lines */
void ESCCurMove(char *buf, u8 curdir, u16 count)
{
		SPRINTF("%s%u%c", ESC_CSI, count, (char)curdir);
}



/* Moves the cursor to the specified position */
void ESCCurPos(char *buf, u16 x, u16 y)
{
		SPRINTF("%s%u;%uH", ESC_CSI, y, x);
}



/* Hide cursor */
void ESCCurHide(char *buf)
{
		SPRINTF("%s?25l", ESC_CSI);
}



/* Show cursor */
void ESCCurShow(char *buf)
{
		SPRINTF("%s?25h", ESC_CSI);
}



/* Saves the current cursor position */
void ESCCurSave(char *buf)
{
		SPRINTF("%ss", ESC_CSI);
}



/* Returns the cursor to the position */
void ESCCurRestore(char *buf)
{
		SPRINTF("%su", ESC_CSI);
}



/* Set graphic mode */
void ESCGraphicMode(char *buf, u16 mode, u8 fColor, u8 bColor)
{
        u32 i, isAttrsExist = 0;

        //Add 'Esc['
        SPRINTF(ESC_CSI);

        //Add graphic attributes
        i = 0;
        while(i < ESC_GRATTRS_COUNT)
        {
                if (mode & (1 << i))
                {
						SPRINTF("%u;", escGrAttrs[i]);
                        isAttrsExist = 1;
                }

                i++;
        }

        if (!isAttrsExist)
        {
				SPRINTF(";");
        }

        //Add fore/back colors.
        //fColor: [30..37]; bColors: [40..47]
		SPRINTF("%u;%um", fColor, bColor + 10);
}



/* Reset graphic mode to default values */
void ESCGraphicReset(char *buf)
{
		SPRINTF("%s0;37;40m", ESC_CSI);
}

/*===========================================================================
 * ESC sequence handler
 *===========================================================================*/

//
#define RiseKeyPressed(es, keyCode) if (es->keyPressed)\
										(es)->keyPressed(keyCode)

//
static ESCState escState;

//F1, Ctrl+F1, Shift+F1, Alt+F1
static const u8 escF1 [] = { 0x70, 0x00, 0x1B, 0x4F, 0x50, 0x00 };		
static const u8 escCF1[] = { 0x70, 0x02, 0x1B, 0x4F, 0x35, 0x50, 0x00 };	
static const u8 escSF1[] = { 0x70, 0x01, 0x1B, 0x4F, 0x32, 0x50, 0x00 };	
static const u8 escAF1[] = { 0x70, 0x04, 0x1B, 0x4F, 0x33, 0x50, 0x00 };
							
//F2, Ctrl+F2, Shift+F2, Alt+F2
static const u8 escF2 [] = { 0x71, 0x00, 0x1B, 0x4F, 0x51, 0x00 };		
static const u8 escCF2[] = { 0x71, 0x02, 0x1B, 0x4F, 0x35, 0x51, 0x00 };	
static const u8 escSF2[] = { 0x71, 0x01, 0x1B, 0x4F, 0x32, 0x51, 0x00 };	
static const u8 escAF2[] = { 0x71, 0x04, 0x1B, 0x4F, 0x33, 0x51, 0x00 };
							
//F3, Ctrl+F3, Shift+F3, Alt+F3
static const u8 escF3 [] = { 0x72, 0x00, 0x1B, 0x4F, 0x52, 0x00 };		
static const u8 escCF3[] = { 0x72, 0x02, 0x1B, 0x4F, 0x35, 0x52, 0x00 };	
static const u8 escSF3[] = { 0x72, 0x01, 0x1B, 0x4F, 0x32, 0x52, 0x00 };	
static const u8 escAF3[] = { 0x72, 0x04, 0x1B, 0x4F, 0x33, 0x52, 0x00 };
							
//F4, Ctrl+F4, Shift+F4, Alt+F4
static const u8 escF4 [] = { 0x73, 0x00, 0x1B, 0x4F, 0x53, 0x00 };		
static const u8 escCF4[] = { 0x73, 0x02, 0x1B, 0x4F, 0x35, 0x53, 0x00 };	
static const u8 escSF4[] = { 0x73, 0x01, 0x1B, 0x4F, 0x32, 0x53, 0x00 };	
static const u8 escAF4[] = { 0x73, 0x04, 0x1B, 0x4F, 0x33, 0x53, 0x00 };

//F5, Ctrl+F5, Shift+F5, Alt+F5
static const u8 escF5 [] = { 0x74, 0x00, 0x1B, 0x5B, 0x31, 0x36, 0x7E, 0x00 };
static const u8 escCF5[] = { 0x74, 0x02, 0x1B, 0x5B, 0x31, 0x35, 0x3B, 0x35, 0x7E, 0x00 };
static const u8 escSF5[] = { 0x74, 0x01, 0x1B, 0x5B, 0x31, 0x35, 0x3B, 0x32, 0x7E, 0x00 };	
static const u8 escAF5[] = { 0x74, 0x04, 0x1B, 0x5B, 0x31, 0x35, 0x3B, 0x33, 0x7E, 0x00 };
							
//F6, Ctrl+F6, Shift+F6, Alt+F6
static const u8 escF6 [] = { 0x75, 0x00, 0x1B, 0x5B, 0x31, 0x37, 0x7E, 0x00 };	
static const u8 escCF6[] = { 0x75, 0x02, 0x1B, 0x5B, 0x31, 0x37, 0x3B, 0x35, 0x7E, 0x00 };	
static const u8 escSF6[] = { 0x75, 0x01, 0x1B, 0x5B, 0x31, 0x37, 0x3B, 0x32, 0x7E, 0x00 };	
static const u8 escAF6[] = { 0x75, 0x04, 0x1B, 0x5B, 0x31, 0x37, 0x3B, 0x33, 0x7E, 0x00 };
							
//F7, Ctrl+F7, Shift+F7, Alt+F7
static const u8 escF7 [] = { 0x76, 0x00, 0x1B, 0x5B, 0x31, 0x38, 0x7E, 0x00 };	
static const u8 escCF7[] = { 0x76, 0x02, 0x1B, 0x5B, 0x31, 0x38, 0x3B, 0x35, 0x7E, 0x00 };	
static const u8 escSF7[] = { 0x76, 0x01, 0x1B, 0x5B, 0x31, 0x38, 0x3B, 0x32, 0x7E, 0x00 };	
static const u8 escAF7[] = { 0x76, 0x04, 0x1B, 0x5B, 0x31, 0x38, 0x3B, 0x33, 0x7E, 0x00 };
							
//F8, Ctrl+F8, Shift+F8, Alt+F8
static const u8 escF8 [] = { 0x77, 0x00, 0x1B, 0x5B, 0x31, 0x39, 0x7E, 0x00 };	
static const u8 escCF8[] = { 0x77, 0x02, 0x1B, 0x5B, 0x31, 0x39, 0x3B, 0x35, 0x7E, 0x00 };	
static const u8 escSF8[] = { 0x77, 0x01, 0x1B, 0x5B, 0x31, 0x39, 0x3B, 0x32, 0x7E, 0x00 };	
static const u8 escAF8[] = { 0x77, 0x04, 0x1B, 0x5B, 0x31, 0x39, 0x3B, 0x33, 0x7E, 0x00 };

//F9, Ctrl+F9, Shift+F9, Alt+F9
static const u8 escF9 [] = { 0x78, 0x00, 0x1B, 0x5B, 0x32, 0x30, 0x7E, 0x00 };	
static const u8 escCF9[] = { 0x78, 0x02, 0x1B, 0x5B, 0x32, 0x30, 0x3B, 0x35, 0x7E, 0x00 };	
static const u8 escSF9[] = { 0x78, 0x01, 0x1B, 0x5B, 0x32, 0x30, 0x3B, 0x32, 0x7E, 0x00 };	
static const u8 escAF9[] = { 0x78, 0x04, 0x1B, 0x5B, 0x32, 0x30, 0x3B, 0x33, 0x7E, 0x00 };
							
//F10, Ctrl+F10, Shift+F10, Alt+F10
static const u8 escF10 [] = { 0x79, 0x00, 0x1B, 0x5B, 0x32, 0x31, 0x7E, 0x00 };	
static const u8 escCF10[] = { 0x79, 0x02, 0x1B, 0x5B, 0x32, 0x31, 0x3B, 0x35, 0x7E, 0x00 };	
static const u8 escSF10[] = { 0x79, 0x01, 0x1B, 0x5B, 0x32, 0x31, 0x3B, 0x32, 0x7E, 0x00 };	
static const u8 escAF10[] = { 0x79, 0x04, 0x1B, 0x5B, 0x32, 0x31, 0x3B, 0x33, 0x7E, 0x00 };
							
//F11, Ctrl+F11, Shift+F11, Alt+F11
static const u8 escF11 [] = { 0x7A, 0x00, 0x1B, 0x5B, 0x32, 0x33, 0x7E, 0x00 };	
static const u8 escCF11[] = { 0x7A, 0x02, 0x1B, 0x5B, 0x32, 0x33, 0x3B, 0x35, 0x7E, 0x00 };	
static const u8 escSF11[] = { 0x7A, 0x01, 0x1B, 0x5B, 0x32, 0x33, 0x3B, 0x32, 0x7E, 0x00 };	
static const u8 escAF11[] = { 0x7A, 0x04, 0x1B, 0x5B, 0x32, 0x33, 0x3B, 0x33, 0x7E, 0x00 };
							
//F12, Ctrl+F12, Shift+F12, Alt+F12
static const u8 escF12 [] = { 0x7B, 0x00, 0x1B, 0x5B, 0x32, 0x34, 0x7E, 0x00 };	
static const u8 escCF12[] = { 0x7B, 0x02, 0x1B, 0x5B, 0x32, 0x34, 0x3B, 0x35, 0x7E, 0x00 };	
static const u8 escSF12[] = { 0x7B, 0x01, 0x1B, 0x5B, 0x32, 0x34, 0x3B, 0x32, 0x7E, 0x00 };	
static const u8 escAF12[] = { 0x7B, 0x04, 0x1B, 0x5B, 0x32, 0x34, 0x3B, 0x33, 0x7E, 0x00 };

//Home, Ctrl+Home, Shift+Home, Alt+Home
static const u8 escHome [] = { 0x24, 0x00, 0x1B, 0x5B, 0x31, 0x7E, 0x00 };	
static const u8 escCHome[] = { 0x24, 0x02, 0x1B, 0x5B, 0x31, 0x3B, 0x35, 0x48, 0x00 };	
static const u8 escSHome[] = { 0x24, 0x01, 0x1B, 0x5B, 0x31, 0x3B, 0x32, 0x48, 0x00 };
static const u8 escAHome[] = { 0x24, 0x04, 0x1B, 0x5B, 0x31, 0x3B, 0x33, 0x48, 0x00 };
							
//Insert, Ctrl+Insert
static const u8 escIns []  = { 0x2D, 0x00, 0x1B, 0x5B, 0x32, 0x7E, 0x00 };	
static const u8 escCIns[]  = { 0x2D, 0x02, 0x1B, 0x5B, 0x32, 0x3B, 0x35, 0x7E, 0x00 };
							
//Delete, Ctrl+Delete, Shift+Delete
static const u8 escDel []  = { 0x2E, 0x00, 0x1B, 0x5B, 0x33, 0x7E, 0x00 };	
static const u8 escCDel[]  = { 0x2E, 0x02, 0x1B, 0x5B, 0x33, 0x3B, 0x35, 0x7E, 0x00 };	
static const u8 escSDel[]  = { 0x2E, 0x01, 0x1B, 0x5B, 0x33, 0x3B, 0x32, 0x7E, 0x00 };	
							
//Page Up, Ctrl+Page Up, Alt+Page Up
static const u8 escPgUp [] = { 0x21, 0x00, 0x1B, 0x5B, 0x35, 0x7E,	0x00 };
static const u8 escCPgUp[] = { 0x21, 0x02, 0x1B, 0x5B, 0x35, 0x3B, 0x35, 0x7E, 0x00 };
static const u8 escAPgUp[] = { 0x21, 0x04, 0x1B, 0x5B, 0x35, 0x3B, 0x33, 0x7E, 0x00 };
							
//Page Down, Ctrl+Page Down, Alt+Page Down
static const u8 escPgDown [] = { 0x22, 0x00, 0x1B, 0x5B, 0x36, 0x7E, 0x00 };
static const u8 escCPgDown[] = { 0x22, 0x02, 0x1B, 0x5B, 0x36, 0x3B, 0x35, 0x7E, 0x00 };
static const u8 escAPgDown[] = { 0x22, 0x04, 0x1B, 0x5B, 0x36, 0x3B, 0x33, 0x7E, 0x00 };
							
//End, Ctrl+, Shift+, Alt+
static const u8 escEnd []   = { 0x23, 0x00, 0x1B, 0x4F, 0x46, 0x00 };	
static const u8 escCEnd[]   = { 0x23, 0x02, 0x1B, 0x5B, 0x31, 0x3B, 0x35, 0x46, 0x00 };	
static const u8 escSEnd[]   = { 0x23, 0x01, 0x1B, 0x5B, 0x31, 0x3B, 0x32, 0x46, 0x00 };
static const u8 escAEnd[]   = { 0x23, 0x04, 0x1B, 0x5B, 0x31, 0x3B, 0x33, 0x46, 0x00 };

//Arrow Up, Ctrl+Arrow Up
static const u8 escUp []    = { 0x26, 0x00, 0x1B, 0x5B, 0x41, 0x00 };
static const u8 escCUp[]    = { 0x26, 0x02, 0x1B, 0x5B, 0x31, 0x3B, 0x35, 0x41, 0x00 };

//Arrow Down, Ctrl+Arrow Down
static const u8 escDown []  = { 0x28, 0x00, 0x1B, 0x5B, 0x42, 0x00 };		
static const u8 escCDown[]  = { 0x28, 0x02, 0x1B, 0x5B, 0x31, 0x3B, 0x35, 0x42, 0x00 };

//Arrow Right, Ctrl+Arrow Right
static const u8 escRight [] = { 0x27, 0x00, 0x1B, 0x5B, 0x43, 0x00 };		
static const u8 escCRight[] = { 0x27, 0x02, 0x1B, 0x5B, 0x31, 0x3B, 0x35, 0x43, 0x00 };
							
//Arrow Left, Ctrl+Arrow Left
static const u8 escLeft []  = { 0x25, 0x00, 0x1B, 0x5B, 0x44, 0x00 };
static const u8 escCLeft[]  = { 0x25, 0x02, 0x1B, 0x5B, 0x31, 0x3B, 0x35, 0x44, 0x00 };
							
//Shift+Enter, Alt+Enter
static const u8 escSEnter[] = { 0x0D, 0x01, 0x1B, 0x4F, 0x4D, 0x00 };			
static const u8 escAEnter[] = { 0x0D, 0x04, 0x1B, 0x0D, 0x00 };	

//Shift+Tab	
static const u8 escSTab[] = { 0x09, 0x01, 0x1B, 0x5B, 0x5A, 0x00 };

//All known escape sequences
static const u8 * const escSeqs[] =
{
	escF1,  escCF1,  escSF1,  escAF1,
	escF2,  escCF2,  escSF2,  escAF2,
	escF3,  escCF3,  escSF3,  escAF3,
	escF4,  escCF4,  escSF4,  escAF4,
	escF5,  escCF5,  escSF5,  escAF5,
	escF6,  escCF6,  escSF6,  escAF6,
	escF7,  escCF7,  escSF7,  escAF7,
	escF8,  escCF8,  escSF8,  escAF8,
	escF9,  escCF9,  escSF9,  escAF9,
	escF10, escCF10, escSF10, escAF10,
	escF11, escCF11, escSF11, escAF11,
	escF12, escCF12, escSF12, escAF12,

	escHome,   escCHome,   escSHome,  escAHome,
	escIns,    escCIns,
	escDel,    escCDel,    escSDel, 
	escPgUp,   escCPgUp,   escAPgUp,
	escPgDown, escCPgDown, escAPgDown,
	escEnd,    escCEnd,    escSEnd,    escAEnd,
	escUp,     escCUp,
	escDown,   escCDown,
	escRight,  escCRight,
	escLeft,   escCLeft,
	escSEnter, escAEnter,
	escSTab,
	0 //0: end of sequence list
};

//Sequence valid, but not full
#define CHSEQ_VALID		-1
//Invalid sequence
#define CHSEQ_FAIL		-2

/* */
void ResetESCState(ESCState *es);

/* */
s32 CheckSequence(ESCState *es)
{
	//Pointer to sequence
	const u8 *seq;
	//Current sequence byte
	u8 seqCh;
	//Counters
	u32 seqIndex = 0, chIndex;
	
	while((seq = escSeqs[seqIndex]) != 0)
	{
		chIndex = 0;
		while(chIndex <= es->esci)
		{
			seqCh = seq[chIndex + 2];

			if (chIndex == es->esci)
			{							
				return seqCh ? CHSEQ_VALID : seqIndex;				
			}
			else if (es->escSeq[chIndex] != seqCh)			
				break;			

			chIndex++;
		}

		seqIndex++;
	}

	return CHSEQ_FAIL;
}



/* Test only */
s32 CheckSequenceT(u8 *seq, u32 count)
{
	memcpy(escState.escSeq, seq, count);
	escState.esci = count;

	return CheckSequence(&escState);
}



/* */
KeyCode GetKeyCode(u8 b)
{
	return NoKey;
}



/* */
KeyCode GetKeyCodeSeq(s32 seqi)
{
	const u8 *seq;

	if (seqi < 0)
		return NoKey;
	else
	{		
		seq = escSeqs[seqi];
		return (KeyCode)((seq[1] << 16) | seq[0]);
	}
}



/* Escape sequence handler */
void InitializeESCHandler()
{		
	ResetESCState(&escState);		
}



/* */
void ResetESCState(ESCState *es)
{		
	memset(es, 0, sizeof(ESCState));
}



/* */
void SetESCKeyHandler(LpKeyCodeHandler handler)
{
	escState.keyPressed = handler;
}



/* */
void ESCHandlerCh0(ESCState *es, u8 b)
{
	switch(b)
	{
		//NULL, DEL: no actions
		case 0x00: 
		case 0x7F: break;

		//ESC
		case 0x1B: 
		{
			if (es->esci + 1 < ESC_SEQ_MAXLENGTH)
			{
				//printf("es->esci++\r\n");
				es->escSeq[es->esci++] = b;	
			}

			break;
		}		

		//Default: rise key press event
		default:
		{					
			RiseKeyPressed(es, b);			
			break;
		}
	}				
}



/* */
void ESCHandlerChN(ESCState *es, u8 b)
{
	s32 r;

	es->escSeq[es->esci++] = b;	

	r = CheckSequence(es);

	if (r >= 0)
	{	
		es->esci = 0;

		//printf("r>=0\r\n");

		RiseKeyPressed(es, GetKeyCode(r));				
	}
	else if (r == CHSEQ_FAIL)	
	{
		es->esci = 0;

		//printf("r==FAIL\r\n");

		RiseKeyPressed(es, (KeyCode)b);		
	}	
}



/* */
void ESCHandler(const u8 *buffer, s32 count)
{
	s32 i;
	ESCState *es;
	u8 b;

	if (!count)
		return;

	es = &escState;

	i = 0;
	while(i < count)
	{
		b = buffer[i++];

		//printf("%02X", b);	

		if (es->esci)
			ESCHandlerChN(es, b);
		else
			ESCHandlerCh0(es, b);		
	} //while

	printf(" ");	
}

#undef SPRINTF

/*===========================================================================
 * End of file esccodes.c
 *===========================================================================*/

