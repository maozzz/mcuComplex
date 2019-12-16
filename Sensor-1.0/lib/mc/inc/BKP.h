/*===========================================================================
 *
 *  MCU library
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
 *  File BKP.h: Backup module definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __BKP_H
#define __BKP_H

#include "../../bastypes.h"

/* Initialize BKP */
MCRESULT InitializeBKP(const void *cfg);

/* Gets size of BKP, bytes */
s32 GetBKPSize();

/* Clear BKP */
void BKPClear();

/* Write BKP */
s32 BKPWrite(u32 offset, const void *buffer, s32 count);

/* Read BKP */
s32 BKPRead(u32 offset, void *buffer, s32 count);

#endif /* !__BKP_H */

/*===========================================================================
 * End of file BKP.h
 *===========================================================================*/

