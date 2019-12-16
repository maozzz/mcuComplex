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
 *  File MILSTD1553.c: implementation selection (GOST R 52070-2003).
 *
 *===========================================================================*/

#include "../inc/types.h"

#if (TARGET_MCU == MCU_1986BE1x)

#include "./1986BE1x/1986BE1x_MILSTD1553.c"

#elif (TARGET_MCU == MCU_SWMODEL)

#include "./swmodel/swmodel_MILSTD1553.c"

#else

#error "Current MCU doesn't have MILSTD1553 interface"

#endif

/*===========================================================================
 * End of file MILSTD1553.c
 *===========================================================================*/
