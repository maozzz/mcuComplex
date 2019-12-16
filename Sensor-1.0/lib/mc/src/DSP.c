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
 *  File DSP.c: implementation selection.
 *
 *===========================================================================*/

#include "../inc/types.h"

#if (TARGET_MCU == MCU_1901BC1x)

#include "./1901BC1x/1901BC1x_DSP.c"

#elif (TARGET_MCU == MCU_SWMODEL)

#include "./swmodel/swmodel_DSP.c"

#else

#error "Current MCU doesn't have DSP core"

#endif

/*===========================================================================
 * End of file ACodec.c
 *===========================================================================*/
