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
 *  File Timer.h. Timer defenitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __TIMER_H
#define __TIMER_H

#include "types.h"
#include "CPU.h"

//Timers
#define TIM_N1					0
#define TIM_N2					1
#define TIM_N3					2
#define TIM_N4					3

#define TIMER_OID_TURN			0x01
#define TIMER_OID_SPEED			0x02

typedef u8              TIMBLOCK;

/* Represents timer configuration */
typedef struct tag_TimerConfig
{
        //Timer number (see TIM_Nx)
        TIMBLOCK tim;

        //Timer speed, Hz
        u32 speed;

        //Zero event handler
        void (*zeroEvent)(void);
} TimerConfig;

/* Initialize timer */
MCRESULT InitializeTimerX(const TimerConfig *cfg);

/* Sets option using specified option id and value */
s32 TimerSetOption(TIMBLOCK tim, u32 oid, const void *ival);

/* Gets option using specified option identifier */
s32 TimerGetOption(TIMBLOCK tim, u32 oid, void *oval);

/* Turn ON/OFF selected timer */
void TurnTimerX(TIMBLOCK tim, u8 on);

/* Set speed for specified timer.
   Timer must be initialized. */
MCRESULT SetTimerXSpeed(TIMBLOCK tim, u32 speed);

#endif /* __TIMER_H */

/*===========================================================================
 * End of file Timer.h
 *===========================================================================*/

