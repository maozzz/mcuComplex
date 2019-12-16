/*===========================================================================
 *
 *  MCU library
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2014-2019
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File 1986BE9x_SysTimer.c. System timer functions.
 *
 *===========================================================================*/

#include "../../inc/SysTimer.h"
#include <string.h>

//System timer configuration
static SysTimerConfig cfgSysTimer1;


//Initialize system timer
MCRESULT InitializeSysTimer(const SysTimerConfig *cfg)
{
    u32 clk, strvr;

    if (!cfg)
        return MCR_INVALID_CFG;	

	//Copy configuration
    memcpy(&cfgSysTimer1, cfg, sizeof(SysTimerConfig));

    //Check speed
    if (!cfgSysTimer1.speed)
         cfgSysTimer1.speed = 1000;

    //TICKINT[1]=0: disable interrupt
    //ENABLE[0]=0:  turn off timer
    SYSTICK->CTRL &= ~(PN1|PN0);

    //Reset current value
    SYSTICK->VAL = 0;

    //Select timer clock source
    switch(cfg->clkSrc)
    {
        case CLKSRC_LSI:
        {
            TurnClkSrc(CLKSRC_LSI, 1);

            //CLKSOURCE[2]=0: LSI as clock source
            SYSTICK->CTRL &= ~PN2;

            clk = FREQ_LSI;
            break;
        }

        //case CLKSRC_HCLK:
        default:
        {
            //CLKSOURCE[2]=1: HCLK as clock source
            SYSTICK->CTRL |= PN2;

            clk = GetHCLK();
            break;
        }
    }

    //Solve start timer value
    strvr = (u32)(clk / cfgSysTimer1.speed) - 1U;

    //Check start value [1..(2^24-1)]
    if (strvr == 0)
        strvr = 1;
    else if (strvr > 0x00FFFFFF)
        strvr = 0x00FFFFFF;

    //Set start value [23..0]
    SYSTICK->LOAD = strvr;

    //TICKINT[1] = 1/0 - enable/disable interrupt
    if (cfg->timerEvent)
		SYSTICK->CTRL |= PN1;
    else
		SYSTICK->CTRL &= ~PN1;

    return MCR_OK;
}



/* Turn ON/OFF system timer */
void TurnSysTimer(SYSTIMBLOCK tim, u8 on)
{
    //ENABLE[0]: 1/0 - ON/OFF timer
    if (on)
	    SYSTICK->CTRL |= PN0;
    else
	    SYSTICK->CTRL &= ~PN0;
}



/* */
u8 IsSysTimerCount(SYSTIMBLOCK tim)
{
    //COUNTFLAG[16]=1: zero event
	return (SYSTICK->CTRL & PN16) ? 1 : 0;
}



/* */
void WaitSysTimer(SYSTIMBLOCK tim, u32 clkcnt)
{
    //ENABLE[0]=0: turn OFF timer
    SYSTICK->CTRL &= ~PN0;

    //Reset counter
    SYSTICK->VAL  = 0;

	//Set start value [23..0]
	SYSTICK->LOAD = (clkcnt & 0x00FFFFFF);

    //ENABLE[0]=1: turn ON timer
	SYSTICK->CTRL |= PN0;

    //COUNTFLAG[16]=0 - counter in progress
    while((SYSTICK->CTRL & PN16) == 0) __NOP();    
}



/* System timer interrupt handler */
void SysTick_Handler(void)
{
    //Call user defined handler
    if (cfgSysTimer1.timerEvent)
        cfgSysTimer1.timerEvent();
}

/*===========================================================================
 * End of file 1986BE9x_SysTimer.c
 *===========================================================================*/
