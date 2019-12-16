/*===========================================================================
 *
 *  MCU library
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2014-2018
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File ARINC429.c: implementation selection (GOST 18977-79 interface).
 *
 *===========================================================================*/

#include "../inc/types.h"

#if (TARGET_MCU == MCU_1986BE1x)

#include "./1986BE1x/1986BE1x_ARINC429.c"

#elif (TARGET_MCU == MCU_SWMODEL)

#include "./swmodel/swmodel_ARINC429.c"

#else

#error "Current MCU doesn't have ARINC429 interface"

#endif

/*===========================================================================
 * End of file ARINC429.c
 *===========================================================================*/

