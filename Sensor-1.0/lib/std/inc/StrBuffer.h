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
 *  File StrBuffer.h: String buffer definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __STRBUFFER_H
#define __STRBUFFER_H

#include "../../bastypes.h"
#include "KeyCode.h"

/* Represents string buffer state */
typedef struct tag_StrBufferState
{
	//Position at string buffer
	u32 curPos;	

	//Maximum size of string buffer
	u32 length;	

	//Pointer to string buffer
	char *buffer;
} StrBufferState;


/* */
void ResetStrBuffer(StrBufferState *sbs);

/* */
void ChangeStrBuffer(StrBufferState *sbs, KeyCode key);

#endif /* __STRBUFFER_H */

/*===========================================================================
 * End of file StrBuffer.h
 *===========================================================================*/

