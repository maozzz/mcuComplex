/*===========================================================================
 *
 *  Standard library
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2014-2016
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File RingBuffer2.h: Ring buffer definitions and function prototypes.
 *
 *===========================================================================*/

#ifndef __RINGBUFFER2_H
#define __RINGBUFFER2_H

#include "../../bastypes.h"

//Ring buffer control structure
//(read from "tail", write  to "head")
typedef struct tag_RingBuffer
{    
    //Pointer to free item at buffer
    volatile u8 *head;

    //Pointer to exist item at buffer
    volatile u8 *tail;

    //Pointer to start data buffer
    u8 *pstart;

    //Pointer to end data buffer
    u8 *pend;

    //Buffer length, bytes
    s32 length;
} RingBuffer;


/* Initialize ring buffer using specified pointer to buffer and length */
void InitializeRBuf(RingBuffer *rb, u8 *buffer, s32 length);

/* Get count of elements at ring buffer */
s32 RBufCount(RingBuffer *rb);

/* Get count of free elements at ring buffer */
s32 RBufSpace(RingBuffer *rb);

/* Write to ring buffer */
s32 RBufWrite(RingBuffer *rb, const void *src, s32 n);

/* Read from ring buffer */
s32 RBufRead(RingBuffer *rb, void *dest, s32 n);

#endif /* !__RINGBUFFER2_H */

/*===========================================================================
 * End of file RingBuffer2.h
 *===========================================================================*/
