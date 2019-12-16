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
 *  File BSP.h: BSP controller definitions and function prototypes.
 *
 *===========================================================================*/

#ifndef __BSP_H
#ifndef __BSP_H

#include "types.h"

//BSP number
#define BSP_N1	0
#define BSP_N2	1
#define BSP_N3	2

typedef u8 		BSPBLOCK;

/* Represents BSP configuration */
typedef struct tag_BSPConfig
{
	BSPBLOCK bsp;
} BSPConfig;


/* Initialize BSP controller */
MCRESULT InitializeBSP(BSPConfig *cfg);

#endif /* !__BSP_H */

/*===========================================================================
 * End of file BSP.h
 *===========================================================================*/
