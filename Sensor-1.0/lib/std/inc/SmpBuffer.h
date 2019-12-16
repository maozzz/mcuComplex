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
 *  File SmpBuffer.h: Sample ring buffer definitions and function prototypes.
 *
 *===========================================================================*/

#ifndef __SMPBUFFER_H
#define __SMPBUFFER_H

#include "../../bastypes.h"

typedef s16               smp_t;

/* */
typedef struct tag_SmpBuffer
{
		//Pointer to buffer
        smp_t *buffer;

        //mask = 2^N - 1, N > 1
        u32 mask;

		//Write position
		volatile u32 posWrite;

		//Read position
		volatile u32 posRead;
} SmpBuffer;

/* */
#define IsSBFull(sb)  ((((sb)->posWrite -  (sb)->posRead) & (~(sb)->mask)) != 0)

/* */
#define IsSBEmpty(sb)   ((sb)->posWrite == (sb)->posRead)

/* */
#define SBCount(sb)     ((sb)->posWrite -  (sb)->posRead)

/* */
#define SBFreeSpace(sb) (((sb)->mask) + 1 - SBCount(sb))

/* */
void SBInit(SmpBuffer *sb, smp_t *buffer, u32 length);

/* */
void SBReset(SmpBuffer *sb);

/* */
s32  SBWrite(SmpBuffer *sb, const smp_t *data, s32 length);

/* */
s32  SBRead(SmpBuffer *sb, smp_t *data, s32 length);

/* */
s32  SBRemove(SmpBuffer *sb, s32 length);

/* */
smp_t *SBHead(SmpBuffer *sb);

/* */
u8   SBReadItem(SmpBuffer *sb, smp_t *data);

/* */
u8   SBWriteItem(SmpBuffer *sb, smp_t data);

/* */
s32 SBWriteSync(SmpBuffer *sb, const smp_t *data, s32 length);

/* */
s32 SBReadSync(SmpBuffer *sb, smp_t *data, s32 length);

#endif /* !__SMPBUFFER_H */

/*===========================================================================
 * End of file SmpBuffer.h
 *===========================================================================*/
