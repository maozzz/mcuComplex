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
 *  File Timer.c: implementation selection.
 *
 *===========================================================================*/

#include "../inc/types.h"

#if (TARGET_MCU == MCU_1986BE9x)

#include "./1986BE9x/1986BE9x_Timer.c"

#elif (TARGET_MCU == MCU_1986BE1x)

#include "./1986BE1x/1986BE1x_Timer.c"

#elif (TARGET_MCU == MCU_1901BC1x)

#include "./1901BC1x/1901BC1x_Timer.c"

#elif (TARGET_MCU == MCU_SWMODEL)

#include "./swmodel/swmodel_Timer.c"

#else

#error "Current MCU doesn't have timer"

#endif

/*===========================================================================
 * End of file Timer.c
 *===========================================================================*/
