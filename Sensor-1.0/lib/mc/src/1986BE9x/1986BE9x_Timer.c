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
 *  File 1986BE9x_Timer.c: Timer functions.
 *
 *===========================================================================*/

#include "../../inc/Timer.h"
#include <string.h>

//Timer configurations
static TimerConfig cfgTIMER1;
static TimerConfig cfgTIMER2;
static TimerConfig cfgTIMER3;

#if (TARGET_MCU == MCU_1986BE1x)
static TimerConfig cfgTIMER4;

static _timer *regTIMERx[4]      = { TIMER1, TIMER2, TIMER3, TIMER4 };
static TimerConfig *cfgTIMERx[4] = { &cfgTIMER1, &cfgTIMER2, &cfgTIMER3, &cfgTIMER4 };
static const u8 timerClkNums[4]  = { CLK_TIMER1, CLK_TIMER2, CLK_TIMER3, CLK_TIMER4 };

#else
static _timer *regTIMERx[3]      = { TIMER1, TIMER2, TIMER3 };
static TimerConfig *cfgTIMERx[3] = { &cfgTIMER1, &cfgTIMER2, &cfgTIMER3 };
static const u8 timerClkNums[3]  = { CLK_TIMER1, CLK_TIMER2, CLK_TIMER3 };

#endif

#define get_timer(x)		(regTIMERx[(x) & 3])
#define get_timer_cfg(x)	(cfgTIMERx[(x) & 3])
#define get_timer_clknum(x)	(timerClkNums[(x) & 3])

/* */
typedef struct tag_TimerClkParams
{
        //Input parameters
        u32 speed;

        //Output parameters
        u32 arr;
        u32 psg;
        u8 brg;
} TimerClkParams;


/* */
static MCRESULT TuneTimerClkParams(TimerClkParams *p)
{
        u32 hclk;

        if (!p)
                return MCR_SETUPCLK_FAIL;

        hclk = GetHCLK();

        //PSG, ARR: 16bit
        //BRG: 7bit and values at [0..7]
        p->psg = 0;
        while(p->psg < 0x00010000)
        {
                p->brg = 0;
                while(p->brg < 8)
                {
                        p->arr = (u32)(hclk / ((1 << p->brg) * (p->psg + 1) * p->speed) - 1);

                        if (p->arr > 0 && p->arr < 0x0000FFFF)
                                return MCR_OK;

                        p->brg++;
                }

                p->psg++;
        }

        return MCR_SETUPCLK_FAIL;
}



/* */
MCRESULT InitializeTimerX(const TimerConfig *cfg)
{
	_timer *reg;
        TimerClkParams p;

	//Check arguments
        if (!cfg || cfg->tim >= TIM_COUNT)
                return MCR_INVALID_CFG;

	//Tune clock parameters
        p.speed = cfg->speed;
        if (TuneTimerClkParams(&p) != MCR_OK)
                return MCR_SETUPCLK_FAIL;		

        //Copy configuration
	memcpy(get_timer_cfg(cfg->tim), cfg, sizeof(TimerConfig));

        //Enable timer clock
	ClkEnable(get_timer_clknum(cfg->tim));

        //Turn timer OFF
        TurnTimerX(cfg->tim, 0);

        //Timer[1..3] at IRQ14..16 (check it at file "startup_1986BExx.mca")
        //Timer4 at IRQ13  (check it at file "startup_1986BE1x.mca")
        //Enable/disable timer interrupt
        if (cfg->tim == TIM_N4)
        {
		#if (TARGET_MCU == MCU_1986BE1x)
                if (cfg->zeroEvent)
                        IRQEnable(IRQ_TIMER4);
                else
                        IRQDisable(IRQ_TIMER4);

                //At UART_CLOCK register:
                //TIM4_BRG [23..16]
                RST_CLK->UART_CLOCK &= ~(0xFF << 16);
                RST_CLK->UART_CLOCK |= (u8)p.brg << 16;

                //TIM4_CLK_EN [26] = 1
                RST_CLK->UART_CLOCK |= PN26;
		#endif /* TARGET_MCU == MCU_1986BE1x */
        }
        else
        {               
                if (cfg->zeroEvent)
                        IRQEnable(IRQ_TIMER1 + cfg->tim);
                else
                        IRQDisable(IRQ_TIMER1 + cfg->tim);

                //TIMx_BRG bits: [0..7], [8..15], [16..23] (value at [0..7])
                //TIMx_CLOCK = HCLK / (1 << TIMx_BRG)
                RST_CLK->TIM_CLOCK &= ~(0xFF << (8 * cfg->tim));
                RST_CLK->TIM_CLOCK |= (u8)p.brg << (8 * cfg->tim);

                //TIMx_CLK_EN [24, 25, 26] = 1
                RST_CLK->TIM_CLOCK |= PN24 << cfg->tim;
        }

        //Get registers
	reg = get_timer(cfg->tim);

        //Reset timer control
        reg->CNTRL = 0;

        //CLK = TIMx_CLOCK / (PSG + 1)
        reg->PSG = (u16)p.psg;

        //CNT_ZERO_EVENT_IE[0] = 1/0 - enable/disable interrupt
        if (cfg->zeroEvent)
                reg->IE |= PN0;
        else
                reg->IE &= ~PN0;

        //DIR[3]: 1/0 - count down/up
        //CNT_MODE[6..7]: 1 - auto change direction
        //tim->CNTRL |= PN3;

        //Set begin value CNT[15..0]
        reg->CNT = 0;

        //Set restart value ARR[15..0]
        reg->ARR = (u16)p.arr;

        return MCR_OK;
}



/* */
void TurnTimerX(TIMBLOCK tim, u8 on)
{
		_timer *reg = get_timer(tim);

        //Set/reset CNT_EN[0] - turn ON/OFF counter
        if (on)
                reg->CNTRL |= PN0;
        else
                reg->CNTRL &= ~PN0;
}



/* */
MCRESULT SetTimerXSpeed(TIMBLOCK tim, u32 speed)
{
	_timer *reg;
	TimerConfig *cfg;
	TimerClkParams p;

	//Check arguments
	if (tim >= TIM_COUNT)
			return MCR_INVALID_CFG;

	//Select configuration
	cfg = get_timer_cfg(tim);

	//Current speed == requested speed: return ok
	if (cfg->speed == speed)
		return MCR_OK;

	//Tune clock paramters
	p.speed = speed;
	if (TuneTimerClkParams(&p) != MCR_OK)
			return MCR_SETUPCLK_FAIL;

	//Save speed value
	cfg->speed = speed;

	//Get pointer to timer registers
	reg = get_timer(tim);

	//Reset CNT_EN[0] - turn OFF counter
	reg->CNTRL &= ~PN0;

	//Setup clocks
	if (tim == TIM_N4)
	{
			#if (TARGET_MCU == MCU_1986BE1x)
			//At UART_CLOCK register:
			//TIM4_BRG [23..16]
			RST_CLK->UART_CLOCK &= ~(0xFF << 16);
			RST_CLK->UART_CLOCK |= (u8)p.brg << 16;
			#endif /* TARGET_MCU == MCU_1986BE1x */
	}
	else
	{
			//TIMx_BRG bits: [0..7], [8..15], [16..23] (value at [0..7])
			//TIMx_CLOCK = HCLK / (1 << TIMx_BRG)
			RST_CLK->TIM_CLOCK &= ~(0xFF << (8 * tim));
			RST_CLK->TIM_CLOCK |= (u8)p.brg << (8 * tim);
	}

	//Reset timer control
	reg->CNTRL = 0;

	//CLK = TIMx_CLOCK / (PSG + 1)
	reg->PSG = (u16)p.psg;

	//Set begin value CNT[15..0]
	reg->CNT = 0;

	//Set restart value ARR[15..0]
	reg->ARR = (u16)p.arr;

	//Set CNT_EN[0] - turn ON counter
	reg->CNTRL |= PN0;

	return MCR_OK;
}



/* */
__IRQH_FATTR void TimerX_IRQHandler(_timer *reg, TimerConfig *cfg)
{
        //if CNT_ZERO_EVENT[0] = 1
        if (reg->STATUS & PN0)
        {
                //Bit CNT_ZERO_EVENT[0] must be reset
                reg->STATUS &= ~PN0;

                //Call event handler
                if (cfg->zeroEvent)
                        cfg->zeroEvent();
        }
}



/* */
void Timer1_IRQHandler()
{
        TimerX_IRQHandler(TIMER1, &cfgTIMER1);
}



/* */
void Timer2_IRQHandler()
{
        TimerX_IRQHandler(TIMER2, &cfgTIMER2);
}



/* */
void Timer3_IRQHandler()
{
        TimerX_IRQHandler(TIMER3, &cfgTIMER3);
}


#if (TARGET_MCU == MCU_1986BE1x)
/* */
void Timer4_IRQHandler()
{
        TimerX_IRQHandler(TIMER4, &cfgTIMER4);
}
#endif /* TARGET_MCU == MCU_1986BE1x */


//Indicates that value has zero value
#define IsValOFF(v)		(*((u8*)(v)) == 0x00 || *((u8*)(v)) == 0x30)

/* Sets option using specified option id and value */
s32 TimerSetOption(TIMBLOCK tim, u32 oid, const void *ival)
{
	if (tim >= TIM_COUNT)
		return -1;

	switch(oid)
	{
		case TIMER_OID_TURN:
		{
			TurnTimerX(tim, IsValOFF(ival) ? 0 : 1);
			return 1;
		}

		case TIMER_OID_SPEED:
		{
			if (SetTimerXSpeed(tim, *((u32*)ival)) == MCR_OK)
				return 4;
			else
				return -1;
		}

		default: return -1;
	}
}



/* Gets option using specified option identifier */
s32 TimerGetOption(TIMBLOCK tim, u32 oid, void *oval)
{
	if (tim >= TIM_COUNT)
		return -1;

	switch(oid)
	{
		case TIMER_OID_TURN:
		{
			//CNT_EN[0]: 1/0 - ON/OFF timer
			if (get_timer(tim)->CNTRL & PN0)
				*((u8*)oval) = 1;
			else
				*((u8*)oval) = 0;
			return 1;
		}

		case TIMER_OID_SPEED:
		{
			*((u32*)oval) = get_timer_cfg(tim)->speed;
			return 4;
		}

		default: return -1;
	}
}


/*===========================================================================
 * End of file 1986BE9x_Timer.c
 *===========================================================================*/

