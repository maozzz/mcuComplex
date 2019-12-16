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
 *  File swmodel_CPU.c: CPU functions.
 *
 *===========================================================================*/

#include "../../inc/CPU.h"
#include <string.h>
 
static _rst_clk    STRUCT_RST_CLK;

static _port	    STRUCT_PORTA;
static _port	    STRUCT_PORTB;
static _port	    STRUCT_PORTC;
static _port	    STRUCT_PORTD;
static _port	    STRUCT_PORTE;
static _port	    STRUCT_PORTF;

static _ssp	    STRUCT_SSP1;
static _ssp	    STRUCT_SSP2;
static _ssp	    STRUCT_SSP3;
static _ssp	    STRUCT_SSP4;

static _uart	    STRUCT_UART1;
static _uart	    STRUCT_UART2;
static _uart	    STRUCT_UART3;

static _dsp_irq    STRUCT_DSP_IRQ;
static _dsp_clk    STRUCT_DSP_CLK;
static _dsp_acodec STRUCT_DSP_ACODEC;

static _bkp STRUCT_BKP;

_bkp *BKP = &STRUCT_BKP;

//Pointers to MCU "peripheral registers"
_rst_clk    *RST_CLK    = &STRUCT_RST_CLK;

_port	    *PORTA      = &STRUCT_PORTA;
_port	    *PORTB      = &STRUCT_PORTB;
_port	    *PORTC      = &STRUCT_PORTC;
_port	    *PORTD      = &STRUCT_PORTD;
_port	    *PORTE      = &STRUCT_PORTE;
_port	    *PORTF      = &STRUCT_PORTF;

_ssp	    *SSP1       = &STRUCT_SSP1;
_ssp	    *SSP2       = &STRUCT_SSP2;
_ssp	    *SSP3       = &STRUCT_SSP3;
_ssp	    *SSP4       = &STRUCT_SSP4;

_uart	    *UART1	= &STRUCT_UART1;
_uart	    *UART2	= &STRUCT_UART2;
_uart	    *UART3	= &STRUCT_UART3;

_dsp_irq    *DSP_IRQ    = &STRUCT_DSP_IRQ;
_dsp_clk    *DSP_CLK    = &STRUCT_DSP_CLK;
_dsp_acodec *DSP_ACODEC = &STRUCT_DSP_ACODEC;


//Current CPU "clock"
static u32 currHCLK = 0;


/* Clear registers of software model */
void ClearSwModelRegisters()
{
	memset(RST_CLK,    0, sizeof(_rst_clk));

	memset(PORTA,      0, sizeof(_port));
	memset(PORTB,      0, sizeof(_port));
	memset(PORTC,      0, sizeof(_port));
	memset(PORTD,      0, sizeof(_port));
	memset(PORTE,      0, sizeof(_port));
	memset(PORTF,      0, sizeof(_port));

	memset(SSP1,       0, sizeof(_ssp));
	memset(SSP2,       0, sizeof(_ssp));
	memset(SSP3,       0, sizeof(_ssp));
	memset(SSP4,       0, sizeof(_ssp));

	memset(UART1,       0, sizeof(_uart));
	memset(UART2,       0, sizeof(_uart));
	memset(UART3,       0, sizeof(_uart));

	memset(DSP_IRQ,    0, sizeof(_dsp_irq));
	memset(DSP_CLK,    0, sizeof(_dsp_clk));
	memset(DSP_ACODEC, 0, sizeof(_dsp_acodec));

	//Operation on "ARM-DSP bridge" is completed
	RST_CLK->DSP_CONTROL_STATUS |= PN14;
}



/* */
MCRESULT SetHCLK(u32 hclk, u32 clkSrc)
{    
    u32 freqSrc;

	ClearSwModelRegisters();    

    if (!(hclk > 0 && hclk <= MAX_VALID_HCLK))
    {
        currHCLK = 0;
        return MCR_SETUPCLK_FAIL;
    }

    //Clock sources
    switch(clkSrc)
    {
        case CLKSRC_HSI       : freqSrc = FREQ_HSI; break;
        case CLKSRC_HSE       : freqSrc = FREQ_HSE; break;
        case CLKSRC_LSE       : freqSrc = FREQ_LSE; break;
        case CLKSRC_LSI       : freqSrc = FREQ_LSI; break;
        case CLKSRC_HSI_DIRECT: { currHCLK = FREQ_HSI; return MCR_OK; }
        default: return MCR_SETUPCLK_FAIL;
    }

    currHCLK = ((u32)(hclk / freqSrc)) * freqSrc;
    return MCR_OK;
}



/* */
u32 GetHCLK(void)
{
	return currHCLK;
}



/* Gets CPU identifier */
u32 GetCPUID()
{
	return (u32)(MCU_SWMODEL);
}



/* */
u8 TurnClkSrc(u32 clkSrc, u8 turn)
{
    return turn;
}



/* */
s32 TuneClkSrcHSIInc(s32 trimInc)
{
    return 0;
}



/* */
s32 TuneClkSrcHSIVal(s32 trimVal)
{
    return trimVal;
}



/* */
MCRESULT InitializePORTEx(PORTBLOCK port, u32 pins, u32 pinValues, u32 opts)
{
	return MCR_OK;
}



/* Turn port */
void TurnPORT(PORTBLOCK port, u8 turn)
{

}



/* */
MCRESULT InitializeEBus(EBusConfig *cfg)
{
	return MCR_OK;
}



/* */
void EBusSetClock(u32 ws)
{

}



/* */
void EBusSetClockEx(u32 addr, u32 wsSetup, u32 wsActive, u32 wsHold)
{

}



/* */
void EBusSetTime(u32 ws_ns)
{
		u32 hclkMHz = (u32)(GetHCLK() / 1000000);
		EBusSetClock(CalcClocks(ws_ns, hclkMHz));
}



/* */
void EBusSetTimeEx(u32 addr, u32 wsSetup_ns, u32 wsActive_ns, u32 wsHold_ns)
{
}



/* */
void EBusSelectMemory(u32 selmem)
{
}



/* */
void MCUSoftReset()
{

}



/* */
void __NOP()
{

}



/* */
s32 PORTSetPull(PORTBLOCK port, PINMASK pins, u8 up, u8 down)
{
    return 0;
}

/*===========================================================================
 * End of file swmodel_CPU.c
 *===========================================================================*/
