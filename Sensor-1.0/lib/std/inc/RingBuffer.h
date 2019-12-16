/*===========================================================================
 *
 *  Standard library.
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
 *  File RingBuffer.h: Ring buffer definitions and function prototypes.
 *
 *===========================================================================*/

#ifndef __RINGBUFFER_H
#define __RINGBUFFER_H

#include "../../bastypes.h"

typedef struct tag_RingBuffer
{
        u8 *buffer;

	//mask = 2^N - 1, N > 1
	u32 mask;
        u32 ofsWrite;
        u32 ofsRead;
} RingBuffer;

#define IsRBFull(rb)  ((((rb)->ofsWrite -  (rb)->ofsRead) & ~((rb)->mask)) != 0)
#define IsRBEmpty(rb)   ((rb)->ofsWrite == (rb)->ofsRead)
#define RBCount(rb)		((rb)->ofsWrite -  (rb)->ofsRead)
#define RBFreeSpace(rb) (((rb)->mask) + 1 - RBCount(rb))

void RBInit(RingBuffer *rb, u8 *buffer, u32 length);
void RBReset(RingBuffer *rb);
s32  RBWrite(RingBuffer *rb, u8 *data, s32 length);
s32  RBRead(RingBuffer *rb, u8 *data, s32 length);
s32  RBRemove(RingBuffer *rb, s32 length);
u8   *RBHead(RingBuffer *rb);

#endif

/*===========================================================================
 * End of file RingBuffer.h
 *===========================================================================*/