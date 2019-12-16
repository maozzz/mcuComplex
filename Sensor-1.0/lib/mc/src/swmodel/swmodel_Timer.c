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
 *  File swmodel_Timer.c: Timer functions.
 *
 *===========================================================================*/

#include "../../inc/Timer.h"
#include <string.h>
 
static TimerConfig cfgTIMER1;

/* */
MCRESULT InitializeTimerX(const TimerConfig *cfg)
{
	memcpy(&cfgTIMER1, cfg, sizeof(TimerConfig));
	return MCR_OK;
}



/* */
void TurnTimerX(TIMBLOCK tim, u8 on)
{

}



/* */
void Timer1_IRQHandler()
{
		//Call event handler
		if (cfgTIMER1.zeroEvent)
			cfgTIMER1.zeroEvent();
}

/*===========================================================================
 * End of file swmodel_Timer.c
 *===========================================================================*/
