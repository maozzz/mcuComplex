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
 *  File SysTimer.h: System timer definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __SYSTIMER_H
#define __SYSTIMER_H

#include "types.h"
#include "CPU.h"

//System timer number
#define SYSTIM_N1       0

typedef u8              SYSTIMBLOCK;

/* Represents system timer configuration */
typedef struct tag_SysTimerConfig
{
        //Systimer number (see SYSTIM_x)
        SYSTIMBLOCK tim;

        //Speed, Hz
        u32 speed;

        //Clock source (see CLKSRC_x)
        u32 clkSrc;

        //Timer event handler
        void (*timerEvent)(void);
} SysTimerConfig;

/* Initialize system timer */
MCRESULT InitializeSysTimer(const SysTimerConfig *cfg);

/* Turn ON/OFF system timer */
void TurnSysTimer(SYSTIMBLOCK tim, u8 on);

/* Indicates that system timer counting in progress */
u8 IsSysTimerCount(SYSTIMBLOCK tim);

/* Wait specified number of system clocks */
void WaitSysTimer(SYSTIMBLOCK tim, u32 clkcnt);

#endif /* __SYSTIMER_H */

/*===========================================================================
 * End of file SysTimer.h
 *===========================================================================*/

