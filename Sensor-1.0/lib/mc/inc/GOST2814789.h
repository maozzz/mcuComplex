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
 *  File GOST2814789.h: GOST 28147-89 controller definitions
 *              and function prototypes.
 *
 *===========================================================================*/

#ifndef __GOST2814789_H
#ifndef __GOST2814789_H

#include "types.h"

/* Represents configuration */
typedef struct tag_CryptoConfig
{
	u32 reserved;
} CryptoConfig;


/* Initialize GOST 28147-89 controller */
MCRESULT InitializeGOST2814789(CryptoConfig *cfg);

#endif /* !__GOST2814789_H */

/*===========================================================================
 * End of file GOST2814789.h
 *===========================================================================*/
