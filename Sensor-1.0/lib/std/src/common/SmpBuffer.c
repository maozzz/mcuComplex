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
 *  File SmpBuffer.c. Sample ring buffer implementation.
 *
 *===========================================================================*/

#include "../../inc/SmpBuffer.h"
#include <string.h>

/* */
void SBInit(SmpBuffer *sb, smp_t *buffer, u32 length)
{
	if (!sb || !buffer)
		return;

    sb->buffer = buffer;
    sb->mask = length - 1;
    memset((void*)sb->buffer, (int)0, (size_t)length);

    SBReset(sb);
}



/* */
void SBReset(SmpBuffer *sb)
{
	sb->posRead = sb->posWrite = 0;
}



/* */
s32 SBWrite(SmpBuffer *sb, const smp_t *data, s32 length)
{
        s32 i = 0;

        while(i < length && !IsSBFull(sb))
				sb->buffer[sb->posWrite++ & sb->mask] = data[i++];

        return i;
}



/* */
s32 SBRead(SmpBuffer *sb, smp_t *data, s32 length)
{
        s32 i = 0;

        while(i < length && !IsSBEmpty(sb))
				data[i++] = sb->buffer[sb->posRead++ & sb->mask];

        return i;
}



/* */
s32 SBWriteSync(SmpBuffer *sb, const smp_t *data, s32 length)
{
		s32 i = 0;

		while(i < length)
		{
				while(IsSBFull(sb));

				sb->buffer[sb->posWrite++ & sb->mask] = data[i++];
		}

		return i;
}



/* */
s32 SBReadSync(SmpBuffer *sb, smp_t *data, s32 length)
{
		s32 i = 0;

		while(IsSBEmpty(sb));

		while(i < length && !IsSBEmpty(sb))
				data[i++] = sb->buffer[sb->posRead++ & sb->mask];

		return i;
}



/* */
s32 SBRemove(SmpBuffer *sb, s32 length)
{
        s32 i = 0;
        u8 ch;

        while(i < length && !IsSBEmpty(sb))
			ch = sb->buffer[sb->posRead++ & sb->mask];

        return i;

		/*u32 count = SBCount(sb);

		  if ((u32)length < count)
		  {
				sb->posRead += (u32)length;
				return length;
		  }
		  else
		  {
				sb->posRead += count;
				return count;
		  } */
}



/* */
smp_t *SBHead(SmpBuffer *sb)
{
        if (!IsSBEmpty(sb))
				return &sb->buffer[sb->posRead & sb->mask];
        else
                return 0;
}



/* */
u8 SBReadItem(SmpBuffer *sb, smp_t *data)
{
        if (IsSBEmpty(sb))
        {
                *data = 0;
                return 0;
        }
        else
        {
				*data = sb->buffer[sb->posRead++ & sb->mask];
                return 1;
        }
}



/* */
u8 SBWriteItem(SmpBuffer *sb, smp_t data)
{
    if (IsSBFull(sb))
        return 0;
        else
        {
				sb->buffer[sb->posWrite++ & sb->mask] = data;
                return 1;
        }
}

/*===========================================================================
 * End of file SmpBuffer.c
 *===========================================================================*/

