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
 *  File RingBuffer.c. Ring buffer implementation.
 *
 *===========================================================================*/

#include "../../inc/RingBuffer.h"
#include <string.h>

void RBInit(RingBuffer *rb, u8 *buffer, u32 length)
{
        if (!rb || !buffer)
                return;

        rb->buffer = buffer;
        rb->mask = length - 1;
        memset((void*)rb->buffer, (int)0, (size_t)length);

        RBReset(rb);
}



void RBReset(RingBuffer *rb)
{
        rb->ofsRead = rb->ofsWrite = 0;
}



s32 RBWrite(RingBuffer *rb, u8 *data, s32 length)
{
        s32 i = 0;

        while(i < length && !IsRBFull(rb))
                rb->buffer[rb->ofsWrite++ & rb->mask] = data[i++];

        return i;
}



s32 RBRead(RingBuffer *rb, u8 *data, s32 length)
{
        s32 i = 0;

        while(i < length && !IsRBEmpty(rb))
            data[i++] = rb->buffer[rb->ofsRead++ & rb->mask];

		if (IsRBEmpty(rb))
			rb->ofsRead = rb->ofsWrite = 0;

        return i;
}



s32 RBRemove(RingBuffer *rb, s32 length)
{
        /*s32 i = 0;
        u8 ch;
        while(i < length && !IsRBEmpty(rb))
            ch = rb->buffer[rb->ofsRead++ & rb->mask];
        return i;*/

        u32 count = RBCount(rb);

        if ((u32)length < count)
        {
			rb->ofsRead += (u32)length;
            return length;
        }
        else
        {
            rb->ofsRead += count;
            return count;
        }
}



u8 *RBHead(RingBuffer *rb)
{
        if (!IsRBEmpty(rb))
                return &rb->buffer[rb->ofsRead & rb->mask];
        else
                return 0;
}

/*===========================================================================
 * End of file RingBuffer.c
 *===========================================================================*/

