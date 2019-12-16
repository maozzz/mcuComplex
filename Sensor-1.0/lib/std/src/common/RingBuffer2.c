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
 *  File RingBuffer2.c. Ring buffer implementation.
 *
 *===========================================================================*/

#include "../../inc/RingBuffer2.h"
#include <string.h>

/* Initialize ring buffer using specified pointer to buffer and length */
void InitializeRBuf(RingBuffer *rb, u8 *buffer, s32 length)
{
    //Source data buffer
   rb->head   = buffer;
   rb->tail   = buffer;
   rb->pstart = buffer;
   rb->pend   = &buffer[length];
   rb->length = length;
}



/* Get count of elements at ring buffer */
s32 RBufCount(RingBuffer *rb)
{
    s32 count;

    count = rb->head - rb->tail;
    if (count < 0)
       count += rb->length;

    return count;
}



/* Get count of free elements at ring buffer */
s32 RBufSpace(RingBuffer *rb)
{
    s32 count;

    count = rb->head - rb->tail;
    if (count < 0)
       count += rb->length;

    count = rb->length - count - 1;

    return count;
}



/* Write to ring buffer */
s32 RBufWrite(RingBuffer *rb, const void *src, s32 n)
{
    s32 i = 0, count;
    u8 *psrc = (u8*)src;

    count = rb->head - rb->tail;
    if (count < 0)
       count += rb->length;

    //Free space
    count = rb->length - count - 1;

    if (count > n)
        count = n;

    while(i < count)
    {
        *rb->head++ = *psrc++; //write to "head"

        if (rb->head == rb->pend)
            rb->head =  rb->pstart;

        i++;
    }

    return i;
}



/* Read from ring buffer */
s32 RBufRead(RingBuffer *rb, void *dest, s32 n)
{
    s32 i = 0, count;
    u8 *pdest = (u8*)dest;

    count = rb->head - rb->tail;
    if (count < 0)
       count += rb->length;

    if (count > n)
        count = n;

    while(i < count)
    {
        *pdest++ = *rb->tail++; //read from "tail"

        if (rb->tail == rb->pend)
            rb->tail =  rb->pstart;

        i++;
    }

    return i;
}

/*===========================================================================
 * End of file RingBuffer2.c
 *===========================================================================*/
