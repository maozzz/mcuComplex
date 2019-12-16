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
 *  File StrBuffer.c: String buffer functions.
 *
 *===========================================================================*/
#define _CRT_SECURE_NO_WARNINGS

#include "../../inc/StrBuffer.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* */
void ResetStrBuffer(StrBufferState *sbs)
{
	sbs->curPos = 0;	

	if (sbs->buffer)
		memset(sbs->buffer, 0, sbs->length);
}



/* */
void AddToStrBuffer(StrBufferState *sbs, u8 b)
{
	u32 len = strlen(sbs->buffer);

	sbs->buffer[len] = b;
	sbs->buffer[len + 1] = 0;
	
	sbs->curPos = len + 1;

	//Check overflow
	if (len == sbs->length - 2)
	{
		printf("Command full '%s'\r\n", sbs->buffer);
	
		ResetStrBuffer(sbs);
	}
}



/* */
void RemoveFromStrBuffer(StrBufferState *sbs)
{
	u32 i, len;

	if (sbs->curPos == 0)
		return;
			
	i = sbs->curPos - 1;
	len = strlen(sbs->buffer);

	while(i < len)
	{
		sbs->buffer[i] = sbs->buffer[i + 1];
		i++;
	}

	sbs->curPos--;
}



/* */
void AddTabToStrBuffer(StrBufferState *sbs)
{
	if (sbs->curPos < strlen(sbs->buffer))
		sbs->curPos++;
}


/* */
void ChangeStrBuffer(StrBufferState *sbs, KeyCode key)
{
	switch(key)
	{		
		//NULL, DEL: no actions
        case VIRK_NoKey: break;

        case VIRK_Backspace: RemoveFromStrBuffer(sbs);
        case VIRK_Tabulation: break;
				
		default:
		{								
			AddToStrBuffer(sbs, (u8)key);
			break;
		}
	}
}

/*===========================================================================
 * End of file StrBuffer.c
 *===========================================================================*/

