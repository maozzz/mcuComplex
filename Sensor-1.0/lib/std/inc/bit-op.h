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
 *  File bit-op.h: Bit operations definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __BIT_OP_H
#define __BIT_OP_H

#include "../../bastypes.h"

#define ROTL32(a, shift) ( ((a) << (shift)) | ((a) >> (32 - (shift))) )
#define ROTR32(a, shift) ( ((a) >> (shift)) | ((a) << (32 - (shift))) )

#define ROTL64(a, shift) ( ((a) << (shift)) | ((a) >> (64 - (shift))) )
#define ROTR64(a, shift) ( ((a) >> (shift)) | ((a) << (64 - (shift))) )

#define SWAPBYTES16(a)   ( (((a) << 8) | ((a) >> 8)) & 0xFFFF )

#define SWAPBYTES32(a)   ( ((a) << 24) | (((a) << 8) & 0x00FF0000) |\
                         ( ((a) >> 8) & 0x0000FF00) | ((a) >> 24) )

#define SWAPBYTES64(a)   ( (((u64)SWAPBYTES32((u32)(a))) << 32) |\
                             (u64)SWAPBYTES32((u32)((a) >> 32)) )

#define SWAPBYTES64_2(dest, src)\
{       u64 src0 = *((u64*)src);\
        u8 *pdest = (u8*)dest;\
        u8 *psrc = (u8*)&src0;\
        pdest[0] = psrc[7];\
        pdest[1] = psrc[6];\
        pdest[2] = psrc[5];\
        pdest[3] = psrc[4];\
        pdest[4] = psrc[3];\
        pdest[5] = psrc[2];\
        pdest[6] = psrc[1];\
        pdest[7] = psrc[0]; }

/* Rotate left */
u32 RotL32(u32 a, u32 shift);

/* Rotate right */
u32 RotR32(u32 a, u32 shift);

/* Rotate left 64-bit */
u64 RotL64(u64 a, u32 shift);

/* Rotate right 64-bit */
u64 RotR64(u64 a, u32 shift);

/* Swap bytes of 32-bit word */
u32 SwapBytes32(u32 a);

/* Swap bytes of 64-bit word */
u64 SwapBytes64(u64 a);

/* Swap bytes of 64-bit word */
void SwapBytes64_2(u64 *dest, const u64 *src);

/* Convert bytes at network order to 16-bit unsigned short */
u16 GetU16NO(u8 *buf);

/* Convert bytes at host order to 16-bit unsigned short */
u16 GetU16HO(u8 *buf);

/* Convert bytes at network order to 32-bit unsigned integer */
u32 GetU32NO(u8 *buf);

/* Convert bytes at host order to 32-bit unsigned integer */
u32 GetU32HO(u8 *buf);

#endif /* !__BIT_OP_H */

/*===========================================================================
 * End of file bit-op.h
 *===========================================================================*/

