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
 *  File 1986BE1x_ARINC429.c: ARINC429 (GOST 18977-79) functions.
 *
 *===========================================================================*/

#include "../../inc/ARINC429.h"
#include <consp.h>

//Ouputs
#define ARINC_PORT_OUT_OPTS    (PN_PWR_FASTEST|PN_NOINFILTER|PN_NOPULLDOWN|\
                                PN_NOPULLUP|PN_NOSHMIT|PN_CTRLDRIVER)

//Inputs
#define ARINC_PORT_IN_OPTS     (PN_INPUT|PN_NOINFILTER|PN_NOPULLDOWN|\
                                PN_NOPULLUP|PN_NOSHMIT|PN_CTRLDRIVER)


/* Tx/Rx control structure (runtime) */
typedef struct tag_ARINCRtmConfig
{
    //Event handlers
    void (*evhandler)();
} ARINCRtmConfig;


//Runtime control strucures
#if (TARGET_MCU == MCU_1986BE1x)
static ARINCRtmConfig arincTxRtmCfg[ARINCT_COUNT] = { {0},{0},{0},{0} };
static ARINCRtmConfig arincRxRtmCfg[ARINCR_COUNT] = { {0},{0},{0},{0}, {0},{0},{0},{0} };

#else
#error "Current MCU doesn't have ARINCR/ARINCT controller. Check TARGET_MCU at file 'targetmcu.h'"

#endif


/* Tune ARINC429 clock parameters */
static MCRESULT TuneARINCClkParams(u32 *pdiv)
{
    //DIV: div = HCLK / 1MHz, div=[1..127]
    *pdiv = (u32)(GetHCLK() / 1000000);
    return MCR_OK;
}



/* Map tx pins (ARINC transmitter) */
static MCRESULT ARINCTMapPortPins(const ARINC429Config *cfg)
{
    #if (TARGET_MCU == MCU_1986BE1x)
    //MCU 1918BE1x
    switch(cfg->arinc)
    {
        case ARINC_N1: //Tx channel #1
        {
            if (cfg->mapRxTx == ARINCT1_MAP_TX_D13D14) //[OUT1+,OUT1-] => [D13,D14]
                InitializePORTEx(PORT_D, (PN13|PN14), 0, PN_ALTER|ARINC_PORT_OUT_OPTS);
            else
                return MCR_INVALID_MAPPING;
            break;
        }

        case ARINC_N2: //Tx channel #2
        {
            if (cfg->mapRxTx == ARINCT2_MAP_TX_F11F12)      //[OUT2+,OUT2-] => [F11,F12]
                InitializePORTEx(PORT_F, (PN11|PN12), 0, PN_REMAP|ARINC_PORT_OUT_OPTS);

            else if (cfg->mapRxTx == ARINCT2_MAP_TX_F13F14) //[OUT2+,OUT2-] => [F13,F14]
                InitializePORTEx(PORT_F, (PN13|PN14), 0, PN_BASIC|ARINC_PORT_OUT_OPTS);
            else
                return MCR_INVALID_MAPPING;
            break;
        }

        case ARINC_N3: //Tx channel #3
        {
            if (cfg->mapRxTx == ARINCT3_MAP_TX_D15E1)      //[OUT3+,OUT3-] => [D15,E1]
            {
                InitializePORTEx(PORT_D, (PN15), 0, PN_BASIC|ARINC_PORT_OUT_OPTS);
                InitializePORTEx(PORT_E,  (PN1), 0, PN_BASIC|ARINC_PORT_OUT_OPTS);
            }
            else if (cfg->mapRxTx == ARINCT3_MAP_TX_F9F10) //[OUT3+,OUT3-] => [F9,F10]
                InitializePORTEx(PORT_F, (PN9|PN10), 0, PN_REMAP|ARINC_PORT_OUT_OPTS);
            else
                return MCR_INVALID_MAPPING;
            break;
        }

        case ARINC_N4: //Tx channel #4
        {
            if (cfg->mapRxTx == ARINCT4_MAP_TX_E0E2)      //[OUT4+,OUT4-] => [E0,E2]
                InitializePORTEx(PORT_E, (PN0|PN2), 0, PN_BASIC|ARINC_PORT_OUT_OPTS);

            else if (cfg->mapRxTx == ARINCT4_MAP_TX_F7F8) //[OUT4+,OUT4-] => [F7,F8]
                InitializePORTEx(PORT_F, (PN7|PN8), 0, PN_REMAP|ARINC_PORT_OUT_OPTS);
            else
                return MCR_INVALID_MAPPING;
            break;
        }

        default: return MCR_INVALID_MAPPING;
    }

    #else
    //Other MCUs
    return MCR_INVALID_MAPPING;

    #endif

    return MCR_OK;
}



/* Map rx pins (ARINC receiver) */
static MCRESULT ARINCRMapPortPins(const ARINC429Config *cfg)
{
    #if (TARGET_MCU == MCU_1986BE1x)
    //MCU 1918BE1x
    //All rx channels ##1,2-8: [B0,B1],[B2,B3] - [B14,B15]
    if (cfg->arinc <= ARINC_N8)
    {
        u32 rxpins = ((PN0|PN1) << (2 * (cfg->arinc)));
        InitializePORTEx(PORT_B, rxpins, 0, PN_ALTER|ARINC_PORT_IN_OPTS);
        //ConPrintf("rxpins: %08X\n", rxpins);
        return MCR_OK;
    }
    else
        return MCR_INVALID_MAPPING;

    #else
    //Other MCUs
    return MCR_INVALID_MAPPING;

    #endif
}



/* Initialize ARINC429 transmitter */
MCRESULT InitializeARINC429T(const ARINC429Config *cfg)
{
    u32 control, div, tmp;
    u32 INTE_TXRx, ODDx, EN_PARx, CLKx;

    //Map Tx pins
    if (ARINCTMapPortPins(cfg) != MCR_OK)
        return MCR_INVALID_MAPPING;

    //Tune clock divider
    TuneARINCClkParams(&div);

    //Save pointer to event handler
    arincTxRtmCfg[cfg->arinc].evhandler = cfg->evhandler;

    //Enable clock (one ARINC429T block, 4 channels)
    ClkEnable(CLK_ARINCT);    

    //Turn OFF selected tx channel
    TurnARINC429T(cfg->arinc, 0);


    //CONTROL2 register (channels #3,#4):
    //Enable interrupts (1/0 - on/off)
    //Channel #4
    //INTE_HFT4[20]: tx FIFO half full
    //INTE_TXR4[19]: tx FIFO empty
    //INTE_FFT4[18]: tx FIFO full
    //Channel #3
    //INTE_HFT3[17]: tx FIFO half full
    //INTE_TXR3[16]: tx FIFO empty
    //INTE_FFT3[15]: tx FIFO full
    //Channel #4
    //ODD4[7]   : 1/0 - odd/even parity
    //EN_PAR4[6]: 1/0 - enable/disable parity bit (32 bit at data word)
    //CLK4[5]   : 1/0 - 12.5/100kbs speed (when CONTROL1.DIV[14..8] is not 0)
    //CH_EN4[4] : 1/0 - work/reset mode
    //Channel #3
    //ODD3[3]   : 1/0 - odd/even parity
    //EN_PAR3[2]: 1/0 - enable/disable parity bit (32 bit at data word)
    //CLK3[1]   : 1/0 - 12.5/100kbs speed (when CONTROL1.DIV[14..8] is not 0)
    //CH_EN3[0] : 1/0 - work/reset mode

    //CONTROL1 register (channels #1,#2):
    //Enable interrupts (1/0 - on/off)
    //Channel #2
    //INTE_HFT2[20]: tx FIFO half full
    //INTE_TXR2[19]: tx FIFO empty
    //INTE_FFT2[18]: tx FIFO full
    //Channel #1
    //INTE_HFT1[17]: tx FIFO half full
    //INTE_TXR1[16]: tx FIFO empty
    //INTE_FFT1[15]: tx FIFO full
    //HCLK divider
    //DIV[14..8]: div = HCLK / 1MHz, div at [1..127]
    //Channel #2
    //ODD2[7]   : 1/0 - odd/even parity
    //EN_PAR2[6]: 1/0 - enable/disable parity bit (32 bit at data word)
    //CLK2[5]   : 1/0 - 12.5/100kbs speed (when DIV[14..8] is not 0)
    //CH_EN2[4] : 1/0 - work/reset mode
    //Channel #1
    //ODD1[3]   : 1/0 - odd/even parity
    //EN_PAR1[2]: 1/0 - enable/disable parity bit (32 bit at data word)
    //CLK1[1]   : 1/0 - 12.5/100kbs speed (when DIV[14..8] is not 0)
    //CH_EN1[0] : 1/0 - work/reset mode

    //---------------------------------------------------
    //Set DIV[14..8]: div = HCLK / 1MHz, div at [1..127]
    control = ARINCT->CONTROL1;
    control = (control & ~(0x7F << 8)) | (div << 8);
    ARINCT->CONTROL1 = control;

    //---------------------------------------------------
    //Read register value
    if (cfg->arinc < ARINC_N3)
        control = ARINCT->CONTROL1; //CONTROL1: channels #1,#2
    else
        control = ARINCT->CONTROL2; //CONTROL2: channels #3,#4

    //Select bit masks
    if (cfg->arinc == ARINC_N1 || cfg->arinc == ARINC_N3)
    {
        //Channels #1,#3
        INTE_TXRx = PN16;
        ODDx      = PN3;
        EN_PARx   = PN2;
        CLKx      = PN1;
    }
    else
    {
        //Channels #2,#4
        INTE_TXRx = PN19;
        ODDx      = PN7;
        EN_PARx   = PN6;
        CLKx      = PN5;
    }

    //Set parity control
    tmp = (cfg->opts & ARINC_PARITY_MASK);
    if (tmp == ARINC_PARITY_NONE)      //No parity:   ODDx=0; EN_PARx=0
        control &= ~(ODDx|EN_PARx);
    else if (tmp == ARINC_PARITY_EVEN) //Even parity: ODDx=0; EN_PARx=1
    {
        control &= ~ODDx;
        control |= EN_PARx;
    }
    else //Odd parity (by default): ODDx=1; EN_PARx=1
        control |= (ODDx|EN_PARx);

    //Set tx speed
    tmp = (cfg->opts & ARINC_SPEED_MASK);
    if (tmp == ARINC_SPEED_12500) //CLKx=1: 12.5kbit/s
        control |= CLKx;
    else                          //CLKx=0: 100kbit/s (by default)
        control &= ~CLKx;

    //Enable/disable interrupt (tx FIFO empty)
    if (cfg->evhandler)  //Enable:  INTE_TXRx=1
        control |= INTE_TXRx;
    else                 //Disable: INTE_TXRx=0
        control &= ~INTE_TXRx;

    //Update register value
    if (cfg->arinc < ARINC_N3)
        ARINCT->CONTROL1 = control; //CONTROL1: channels #1,#2
    else
        ARINCT->CONTROL2 = control; //CONTROL2: channels #3,#4

    //---------------------------------------------------
    //CONTROL3 register (divider for each channel)
    //DIV_CH4[31..24]: ch #4 DIV
    //DIV_CH3[23..16]: ch #3 DIV
    //DIV_CH2[15..8] : ch #2 DIV
    //DIV_CH1[7..0]  : ch #1 DIV

    //---------------------------------------------------
    //CONTROL4 register (since revision 4)
    //ENSYNC[3..0]: 1 - OUT_A - data bits, OUT_B - sync signal

    //---------------------------------------------------
    //Each tx channel has separate interrupt line
    if (cfg->evhandler)
    {
        switch(cfg->arinc)
        {
            case ARINC_N1: IRQEnable(IRQ_ARINCT1); break;
            case ARINC_N2: IRQEnable(IRQ_ARINCT2); break;
            case ARINC_N3: IRQEnable(IRQ_ARINCT3); break;
            case ARINC_N4: IRQEnable(IRQ_ARINCT4); break;
        }
    }
    else
    {
        switch(cfg->arinc)
        {
            case ARINC_N1: IRQDisable(IRQ_ARINCT1); break;
            case ARINC_N2: IRQDisable(IRQ_ARINCT2); break;
            case ARINC_N3: IRQDisable(IRQ_ARINCT3); break;
            case ARINC_N4: IRQDisable(IRQ_ARINCT4); break;
        }
    }

    return MCR_OK;
}



/* Initialize ARINC429 receiver */
MCRESULT InitializeARINC429R(const ARINC429Config *cfg)
{
    u32 control, div, tmp;
    ARINCBLOCK arn;

    //Map Rx pins
    if (ARINCRMapPortPins(cfg) != MCR_OK)
        return MCR_INVALID_MAPPING;

    //Tune clock divider
    TuneARINCClkParams(&div);

    //Save pointer to event handler
    arincRxRtmCfg[cfg->arinc].evhandler = cfg->evhandler;

    //Enable clock (one ARINC429R, 8 channels)
    ClkEnable(CLK_ARINCR);

    //Turn OFF selected rx channel
    TurnARINC429R(cfg->arinc, 0);

    //Number of receiver channel
    arn = cfg->arinc;

    //---------------------------------------------
    //CONTROL3 register
    //Enable interrupts for all channels (1/0 - on/off)
    //INTEHF[31]: rx FIFO half full
    //INTEFF[30]: rx FIFO full
    //INTERR[29]: rx error or 4T pause between words
    //INTEDR[28]: rx FIFO not empty
    //SDI2_8-SDI2_1[21..14]: SDI2 (expected bit 9 of rx word)
    //SDI1_8-SDI1_1[7..0]:   SDI1 (expected bit 8 of rx word)
    control = ARINCR->CONTROL3;

    if (cfg->evhandler)
        control |= (PN31|PN30|PN29|PN28);
    else
        control &= ~(PN31|PN30|PN29|PN28);

    //opts[2]=1: enable SDI2,SDI1 detection
    if (cfg->opts & PN2)
    {
        //Set SDI2x,SDI1x bits
        control &= ~((PN14|PN0) << arn);

        if (cfg->opts & PN1) //SDI2x=1
            control |= (PN14 << arn);

        if (cfg->opts & PN0) //SDI1x=1
            control |= (PN0  << arn);
    }

    ARINCR->CONTROL3 = control;

    //---------------------------------------------
    //CONTROL2 register
    //DA[31]               : 1 - direct access to rx FIFOs
    //SD_EN8-SD_EN1[24..17]: 1/0 - on/off SDI bits detection
    //LB_EN8-LB_EN1[10..3] : 1/0 - on/off LABEL detection
    //DIV[2..0]            : high 3 bits of divider
    control = ARINCR->CONTROL2;    

    //Set DA[31]=0   : no direct access
    //Set DIV[2..0]=0: reset high 3 bits of divider
    //Set LB_ENx=0   : no labels detection, use "ARINC429SetLabels" function
    control &= ~(PN31 | 0x07 | (PN3 << arn));

    //Set DIV[2..0]: high 3 bits of divider
    control |= ((div >> 4) & 0x07);

    //opts[2]=1: enable SDI2,SDI1 detection
    if (cfg->opts & PN2) //SD_ENx=1
        control |= (PN17 << arn);
    else                 //SD_ENx=0
        control &= ~(PN17 << arn);

    ARINCR->CONTROL2 = control;

    //---------------------------------------------
    //CONTROL1 register
    //DIV[31..28]        : low 4 bits of divider
    //CLK8-CLK1[21..14]  : 1/0 - 12.5/100 kbit/s
    //CH_EN8-CH_EN1[7..0]: 1/0 - work/reset mode
    control = ARINCR->CONTROL1;

    //Set DIV[31..28]: low 4 bits
    control = (control & ~(0x0F << 28)) | ((div & 0x0F) << 28);

    //Set CLKx
    if ((cfg->opts & ARINC_SPEED_MASK) == ARINC_SPEED_12500)
        control |= (PN14 << arn);  //12.5kbit/s
    else
        control &= ~(PN14 << arn); //100kbit/s

    //Don't set CH_ENx=1 here, use "TurnARINC429R" function

    ARINCR->CONTROL1 = control;

    //---------------------------------------------
    //CONTROL8 register (since revision 4)
    //ODD[27..14] : 1/0 - odd/even parity
    //ENPAR[13..0]: 1/0 - enable/disable parity bit (data bit instead parity bit)
    control = ARINCR->CONTROL8;

    tmp = (cfg->opts & ARINC_PARITY_MASK);
    if (tmp == ARINC_PARITY_NONE)      //No parity check (32-bit is data bit)
        control &= ~(PN0 << arn);
    else if (tmp == ARINC_PARITY_EVEN) //Even parity
    {
        control &= ~(PN14 << arn);
        control |=  (PN0  << arn);
    }
    else //Odd parity (by default)
    {
        control |= ((PN14|PN0) << arn);
    }

    ARINCR->CONTROL8 = control;

    //---------------------------------------------
    //CONTROL4, CONTROL5 registers (since revision 2)
    //Dividers for channels #1-#4 and #5-#8 respectively
    //DIV_CH4[31..24], DIV_CH3[23..16], DIV_CH2[15..8], DIV_CH1[7..0]
    //DIV_CH8[31..24], DIV_CH7[23..16], DIV_CH6[15..8], DIV_CH5[7..0]

    //CONTROL9 register (since revision 4)
    //ENSYNC[7..0]: 1 - IN_A data bits, IN_B sync signal

    //INTMASK register (since revision 3)
    //Enable interrupts for each channel
    //Channel #8: IEDR8[31], IERR8[30], IEFF8[29], IEHF8[28]
    //Channel #7: IEDR7[27], IERR7[26], IEFF7[25], IEHF7[24]
    //Channel #6: IEDR6[23], IERR6[22], IEFF6[21], IEHF6[20]
    //Channel #5: IEDR5[19], IERR5[18], IEFF5[17], IEHF5[16]
    //Channel #4: IEDR4[15], IERR4[14], IEFF4[13], IEHF4[12]
    //Channel #3: IEDR3[11], IERR3[10], IEFF3[9],  IEHF3[8]
    //Channel #2: IEDR2[7],  IERR2[6],  IEFF2[5],  IEHF2[4]
    //Channel #1
    //IEDR1[3]: rx FIFO not empty
    //IERR1[2]: rx error or 4T pause between words
    //IEFF1[1]: rx FIFO full
    //IEHF1[0]: rx FIFO half full

    //---------------------------------------------
    //All rx channels has one interrupt line
    if (cfg->evhandler)
    {
        IRQEnable(IRQ_ARINCR);
    }
    else
    {
        IRQDisable(IRQ_ARINCR);
    }

	return MCR_OK;
}



/* Turn ARINC429 transmitter */
void TurnARINC429T(ARINCBLOCK arn, u8 on)
{
    u32 control, mask;

    if (arn < ARINC_N3)
    {
        //CONTROL1 register: 1/0 - work/reset mode
        //CH_EN1[0], CH_EN2[4]
        mask = (arn == ARINC_N1) ? PN0 : PN4;

        control = ARINCT->CONTROL1;

        if (on) //1 - work mode
            control |= mask;
        else    //0 - reset mode
            control &= ~mask;

        ARINCT->CONTROL1 = control;
    }
    else
    {
        //CONTROL2 register: 1/0 - work/reset mode
        //CH_EN3[0], CH_EN4[4]
        mask = (arn == ARINC_N3) ? PN0 : PN4;

        control = ARINCT->CONTROL2;

        if (on) //1 - work mode
            control |= mask;
        else    //0 - reset mode
            control &= ~mask;

        ARINCT->CONTROL2 = control;
    }
}



/* Turn ARINC429 receiver */
void TurnARINC429R(ARINCBLOCK arn, u8 on)
{
    u32 control;

    //CONTROL1 register (receiver):
    //CH_EN8-CH_EN1[7..0]: 1/0 - work/reset mode
    control = ARINCR->CONTROL1;

    if (on) //1 - work mode
        control |= (1 << arn);
    else    //0 - reset mode
        control &= ~(1 << arn);

    ARINCR->CONTROL1 = control;
}



/* Set transmitter speed */
s32 ARINC429TSetSpeed(ARINCBLOCK arn, u32 speed)
{
    u32 control, CLKx, CH_ENx;

    //CONTROL2 register (channels #3,#4):
    //CLK4[5]   : 1/0 - 12.5/100kbs
    //CH_EN4[4] : 1/0 - work/reset mode
    //CLK3[1]   : 1/0 - 12.5/100kbs
    //CH_EN3[0] : 1/0 - work/reset mode

    //CONTROL1 register (channels #1,#2):
    //CLK2[5]   : 1/0 - 12.5/100kbs
    //CH_EN2[4] : 1/0 - work/reset mode
    //CLK1[1]   : 1/0 - 12.5/100kbs
    //CH_EN1[0] : 1/0 - work/reset mode

    //Select bit mask
    if (arn == ARINC_N1 || arn == ARINC_N3)
    {
        CLKx   = PN1;
        CH_ENx = PN0;
    }
    else
    {
        CLKx   = PN5;
        CH_ENx = PN4;
    }

    //Read register value and
    //turn OFF specified channel before speed change
    //(CONTROL1: ch #1,#2; CONTROL2: ch #3,#4)
    if (arn < ARINC_N3)
    {
        control = ARINCT->CONTROL1;

        if (control & CH_ENx)
            ARINCT->CONTROL1 = (control & ~CH_ENx);
    }
    else
    {
        control = ARINCT->CONTROL2;

        if (control & CH_ENx)
            ARINCT->CONTROL2 = (control & ~CH_ENx);
    }

    //Set tx speed
    if (speed == ARINC_SPEED_12500 || speed == 12500)
        control |= CLKx;  //CLKx=1: 12.5kbit/s
    else
        control &= ~CLKx; //CLKx=0: 100kbit/s (by default)

    //Update register value
    if (arn < ARINC_N3)
        ARINCT->CONTROL1 = control; //CONTROL1: ch #1,#2
    else
        ARINCT->CONTROL2 = control; //CONTROL2: ch #3,#4

    return 0;
}



/* Set receiver speed */
s32 ARINC429RSetSpeed(ARINCBLOCK arn, u32 speed)
{
    u32 control;

    //CONTROL1 register
    //CLK8-CLK1[21..14]  : 1/0 - 12.5/100 kbit/s
    //CH_EN8-CH_EN1[7..0]: 1/0 - work/reset mode
    control = ARINCR->CONTROL1;

    //Turn OFF specified channel before speed change
    if (control & (1 << arn))
        ARINCR->CONTROL1 = (control & ~(1 << arn));

    //Set CLKx
    if (speed == ARINC_SPEED_12500 || speed == 12500)
        control |=  (PN14 << arn); //12.5kbit/s
    else
        control &= ~(PN14 << arn); //100kbit/s

    //Update register value
    ARINCR->CONTROL1 = control;

    return 0;
}



/* Indicating that input fifo of specified channel is empty */
u8 IsARINC429InFifoEmpty(ARINCBLOCK arn)
{
    //STATUS1 register: 0 - rx FIFO empty
    //DR8-DR1[7..0]
    if (ARINCR->STATUS1 & (1 << arn))
        return 0;
    else
        return 1;
}



/* Indicating that input fifo of specified channel is full */
u8 IsARINC429InFifoFull(ARINCBLOCK arn)
{
    //STATUS2 register: 1 - rx FIFO half full
    //HF8-HF1[21..14]
    if (ARINCR->STATUS2 & (PN14 << arn))
        return 1;
    else
        return 0;
}



/* Indicating that output fifo of specified channel is empty */
u8 IsARINC429OutFifoEmpty(ARINCBLOCK arn)
{
    u32 TX_Rx;

    //STATUS register: 1 - tx FIFO empty
    //TX_R4[11], TX_R3[8], TX_R2[3], TX_R1[0]

    if (arn < ARINC_N3)
        TX_Rx = 1 << (3 * arn);
    else
        TX_Rx = 1 << (3 * arn + 2);

    if (ARINCT->STATUS & TX_Rx)
        return 1;
    else
        return 0;
}



/* Indicating that output fifo of specified channel is full */
u8 IsARINC429OutFifoFull(ARINCBLOCK arn)
{
    u32 HFTx;

    //STATUS register: 1 - tx FIFO half full
    //HFT4[13], HFT3[10], HFT2[5], HFT1[2]

    if (arn < ARINC_N3)
        HFTx = 1 << (3 * arn + 2);
    else
        HFTx = 1 << (3 * arn + 4);

    if (ARINCT->STATUS & HFTx)
        return 1;
    else
        return 0;
}



/* Set accepted SDI2,SDI1 (receiver only) */
s32 ARINC429SetSDI(ARINCBLOCK arn, u32 sdi)
{
    //CONTROL3 register
    //SDI2_8-SDI2_1[21..14]: SDI2
    //SDI1_8-SDI1_1[7..0]:   SDI1

    //sdi[2]=1: enable SDI2,SDI1 detection
    if (sdi & PN2)
    {
        u32 control = ARINCR->CONTROL3;

        //Set SDI2x,SDI1x bits
        control &= ~((PN14|PN0) << arn);

        if (sdi & PN1) //SDI2x=1
            control |= (PN14 << arn);

        if (sdi & PN0) //SDI1x=1
            control |= (PN0  << arn);

        ARINCR->CONTROL3 = control;
    }

    //CONTROL2 register
    //SD_EN8-SD_EN1[24..17]: 1/0 - on/off SDI bits detection

    //sdi[2]=1: enable SDI2,SDI1 detection
    if (sdi & PN2) //SD_ENx=1
        ARINCR->CONTROL2 |= (PN17 << arn);
    else           //SD_ENx=0
        ARINCR->CONTROL2 &= ~(PN17 << arn);

    return 0;
}



/* Set accepted labels (receiver only) */
s32 ARINC429SetLabels(ARINCBLOCK arn, const u8 *labels, s32 count)
{
    s32 i;
    u32 control;

    //Select rx channel
    //CHAN[3..0]: [7..0] => [ch #8..ch #1];
    ARINCR->CHANNEL = (u32)arn;

    //CONTROL2 register
    //LB_EN8-LB_EN1[10..3] : 1/0 - on/off labels detection
    control = ARINCR->CONTROL2;

    if (labels == 0 || count == 0)
    {
        //Turn OFF labels detection
        ARINCR->CONTROL2 = control & ~(PN3 << arn);
        return 0;
    }
    else
    {
        //Turn ON labels detection
        ARINCR->CONTROL2 = control | (PN3 << arn);

        if (count > ARINCRx_FIFO_LABEL_LENGTH)
            count = ARINCRx_FIFO_LABEL_LENGTH;

        //LABEL register: labels FIFO
        i = 0;
        while(i < count)
        {
            ARINCR->LABEL = labels[i++];
        }

        return i;
    }
}



/* Read data */
s32 ARINC429Read(ARINCBLOCK arn, void *dest, s32 count)
{
    u32 datar, DRx;
    s32 i;
    u8 *pdest = (u8*)dest;

    //Select rx channel
    //CHAN[3..0]: [7..0] => [ch #8..ch #1];
    //            [14] => direct access (CONTROL2.DA[31]=1) ch ##1,2 OR
    //            access within DATA_R1..DATA_R8 registers (from revision 3)
    ARINCR->CHANNEL = (u32)arn;

    //When CHANNEL.CHAN[3..0]=14
    //DATA_R1-DATA_R8 (since revision 3)

    //DATA_R register - rx FIFO, sizes:
    //Channels #1,#2: 256x32
    //Channels #3-#6:  64x32
    //Channels #7,#8:  32x32

    //STATUS2 register
    //HF8-HF1[21..14]: 1/0 - FIFO half full/not half full
    //FF8-FF1[7..0]  : 1/0 - FIFO full/not full

    //STATUS1 register
    //ERR8-ERR1[21..14]: 1 - receive error
    if (ARINCR->STATUS1 & (PN14 << arn))
    {
        //Clear error bit
        //CH_EN8-CH_EN1[7..0]: 1/0 - work/reset mode
        ARINCR->CONTROL1 &= ~(1 << arn);
        __NOP();
        ARINCR->CONTROL1 |= (1 << arn);
        return -1;
    }

    //STATUS1 register
    //DR8-DR1[7..0]: 1/0 - FIFO not empty/empty
    DRx = (1 << arn);

    i = 0;
    while((ARINCR->STATUS1 & DRx) && ((i + 4) <= count))
    {
        datar = ARINCR->DATA_R;
        pdest[i    ] = (u8)datar;
        pdest[i + 1] = (u8)(datar >> 8);
        pdest[i + 2] = (u8)(datar >> 16);
        pdest[i + 3] = (u8)(datar >> 24);
        i += 4;
    }

    return i;
}



/* Write data */
s32 ARINC429Write(ARINCBLOCK arn, const void *src, s32 count)
{
    u32 datat, HFTx;
    s32 i, shift;
    const u8 *psrc = (u8*)src;
    volatile u32 *pDATAx_T;

    //STATUS register
    //Channel #4
    //HFT4[13] : 1 - tx FIFO half full
    //FFT4[12] : 1 - tx FIFO full
    //TX_R4[11]: 1 - tx FIFO empty
    //Channel #3
    //HFT3[10]: 1 - tx FIFO half full
    //FFT3[9] : 1 - tx FIFO full
    //TX_R3[8]: 1 - tx FIFO empty
    //Channel #2
    //HFT2[5] : 1 - tx FIFO half full
    //FFT2[4] : 1 - tx FIFO full
    //TX_R2[3]: 1 - tx FIFO empty
    //Channel #1
    //HFT1[2] : 1 - tx FIFO half full
    //FFT1[1] : 1 - tx FIFO full
    //TX_R1[0]: 1 - tx FIFO empty

    //DATA1_T - channel #1 FIFO: 256x32
    //DATA2_T - channe2 #1 FIFO: 64x32
    //DATA3_T - channe3 #1 FIFO: 64x32
    //DATA4_T - channe4 #1 FIFO: 64x32

    //Pointer to FIFO of specified channel
    pDATAx_T = (volatile u32 *)(BASE_ARINCT + 0x000C + (arn << 2));

    //HFTx: 1/0 - FIFO half full/not half full
    if (arn < ARINC_N3)
        HFTx = 1 << (3 * arn + 2);
    else
        HFTx = 1 << (3 * arn + 4);

    i = 0;
    datat = 0;
    shift = 0;
    while(((ARINCT->STATUS & HFTx) == 0) && i < count)
    {        
        //Get 32-bit word using source data
        datat |= ((u32)psrc[i]) << shift;
        shift += 8;
        i++;

        //Write FIFO
        if (shift == 32)
        {
            *pDATAx_T = datat;
            datat = 0;
            shift = 0;
        }
    }

    //Write FIFO (last bytes)
    if (shift)
        *pDATAx_T = datat;

    return i;
}



/* Read data word */
s32 ARINC429ReadWord(ARINCBLOCK arn, u32 *word)
{
    //Select rx channel
    //CHAN[3..0]: [7..0] => [ch #8..ch #1];
    ARINCR->CHANNEL = (u32)arn;

    //STATUS1 register
    //ERR8-ERR1[21..14]: 1 - receive error
    if (ARINCR->STATUS1 & (PN14 << arn))
    {
        //Clear error bit
        //CH_EN8-CH_EN1[7..0]: 1/0 - work/reset mode
        ARINCR->CONTROL1 &= ~(1 << arn);
        __NOP();
        ARINCR->CONTROL1 |= (1 << arn);
        return -1;
    }

    //STATUS1 register
    //DR8-DR1[7..0]: 1/0 - FIFO not empty/empty
    if (ARINCR->STATUS1 & (1 << arn))
    {
        *word = ARINCR->DATA_R;
        return 4;
    }
    else
    {
        return 0;
    }
}



/* Write data word */
s32 ARINC429WriteWord(ARINCBLOCK arn, u32 word)
{
    u32 HFTx;
    volatile u32 *pDATAx_T;

    //STATUS register: 1 - tx FIFO half full
    //HFT4[13], HFT3[10], HFT2[5], HFT1[2]
    if (arn < ARINC_N3)
        HFTx = 1 << (3 * arn + 2);
    else
        HFTx = 1 << (3 * arn + 4);

    if ((ARINCT->STATUS & HFTx) == 0)
    {
        //Pointer to FIFO of specified channel
        pDATAx_T = (volatile u32 *)(BASE_ARINCT + 0x000C + (arn << 2));

        *pDATAx_T = word;
        return 4;
    }
    else
        return 0;
}



/* IRQ handler: ARINC429 receiver (all channels) */
void ARINC429R_IRQHandler()
{
    u32 chnum, status1;

    status1 = ARINCR->STATUS1;    

    //For each rx channel
    for(chnum = 0; chnum < ARINCR_COUNT; chnum++)
    {
        //STATUS1
        //ERR8-ERR1[21..14]: 1 - rx error
        if (status1 & (PN14 << chnum))
        {
            //Reset channel to reset error bit
            //CH_EN8-CH_EN1[7..0]: 1/0 - work/reset mode
            ARINCR->CONTROL1 &= ~(1 << chnum);
            __NOP();
            ARINCR->CONTROL1 |= (1 << chnum);
        }

        //STATUS1
        //DR8-DR1[7..0]: 1 - rx FIFO not empty
        if (status1 & (1 << chnum))
        {
            //Channel #1-#8: call user-defined handler
            if (arincRxRtmCfg[chnum].evhandler)
                arincRxRtmCfg[chnum].evhandler();
        }
    }
}



/* IRQ handler: ARINC429 transmitter, channel #1 */
void ARINC429T1_IRQHandler()
{
    if (arincTxRtmCfg[0].evhandler)
        arincTxRtmCfg[0].evhandler();
}



/* IRQ handler: ARINC429 transmitter, channel #2 */
void ARINC429T2_IRQHandler()
{
    if (arincTxRtmCfg[1].evhandler)
        arincTxRtmCfg[1].evhandler();
}



/* IRQ handler: ARINC429 transmitter, channel #3 */
void ARINC429T3_IRQHandler()
{
    if (arincTxRtmCfg[2].evhandler)
        arincTxRtmCfg[2].evhandler();
}



/* IRQ handler: ARINC429 transmitter, channel #4 */
void ARINC429T4_IRQHandler()
{
    if (arincTxRtmCfg[3].evhandler)
        arincTxRtmCfg[3].evhandler();
}

/*===========================================================================
 * End of file 1986BE1x_ARINC429.c
 *===========================================================================*/
