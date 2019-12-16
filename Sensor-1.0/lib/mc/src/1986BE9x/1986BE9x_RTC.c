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
 *  File 1986BE9x_RTC.c. Real time clock functions.
 *
 *===========================================================================*/

#include "../../inc/RTC.h"
#include <string.h>

//Wait while WEC[6] = 1
#define WaitRTCRegReady()       while(BKP->RTC_CS & PN6);

//RTC configuration
static RTCConfig cfgRTC1;

/* */
typedef struct tag_RTCClkParams
{
        //Input parameters
        //Requested speed
        u32 speed;
        //Requested clock source
        u32 clkSrc;

        //Output parameters
        u32 c1sel;
        u32 prl;

} RTCClkParams;


/* */
MCRESULT TuneRTCClkParams(RTCClkParams *p)
{
        u32 clk;

        if (!p)
                return MCR_SETUPCLK_FAIL;

        switch(p->clkSrc)
        {
                case CLKSRC_LSI: clk = FREQ_LSI; break;
                case CLKSRC_HSE: clk = FREQ_HSE; break;
                case CLKSRC_HSI: clk = FREQ_HSI; break;

                case CLKSRC_LSE:
                default:         clk = FREQ_LSE; break;
        }

        if (p->clkSrc == CLKSRC_HSE ||
            p->clkSrc == CLKSRC_HSI)
        {
                //speed = clk / ((1 << (c1sel - 7)) * prl),
                //c1sel at [7..15]; prl at [0..2^20-1]
                p->c1sel = 7;
                while(p->c1sel < 16)
                {
                        p->prl = (u32)(clk / ((1 << (p->c1sel - 7)) * p->speed));

                        if (p->prl > 0 && p->prl <= 0x000FFFFF)
                                break;

                        p->c1sel++;
                }
        }
        else
        {
                //speed = clk / prl,
                //prl at [0..2^20-1]
                p->c1sel = 7;
                p->prl = (u32)(clk / p->speed);
        }

        if (p->c1sel < 16 &&
            p->prl > 0 && p->prl <= 0x000FFFFF)
                return MCR_OK;
        else
                return MCR_SETUPCLK_FAIL;
}



/* */
MCRESULT InitializeRTC(const RTCConfig *cfg)
{
	RTCConfig *destCfg;
        RTCClkParams clkParams;

        //Check configuration
        if (!cfg)
                return MCR_INVALID_CFG;

        //Tune clock parameters
	clkParams.speed  = cfg->speed;
        clkParams.clkSrc = cfg->clkSrc;
        if (TuneRTCClkParams(&clkParams) != MCR_OK)
                return MCR_SETUPCLK_FAIL;

	destCfg = &cfgRTC1;

	//Copy configuration
	memcpy(destCfg, cfg, sizeof(RTCConfig));

        //Enable RTC clock
        ClkEnable(CLK_RTC);

        //Turn OFF RTC
	TurnRTC(destCfg->rtc, 0);

        //Setup RTC clock source
        //RTC_SEL[3..2]:
        //0 - LSI; 1 - LSE; 2 - HSI_C1; 3 - HSE_C1
        //Reset: RTC_SEL[3..2], HSE_RTC_EN[8], HSI_RTC_EN[9]
        BKP->REG_0F &= ~(0x03 << 2);
        RST_CLK->RTC_CLOCK &= ~(PN8 | PN9);

	switch(destCfg->clkSrc)
        {
                case CLKSRC_LSI:
                {
                        TurnClkSrc(CLKSRC_LSI, 1);

			//RTC_SEL[3..2]: 0 - select LSI
			BKP->REG_0F &= ~(0x03 << 2);
                        break;
                }


                case CLKSRC_HSE:
                {
                        //HSE_C1 = HSE / (1 << (HSE_SEL - 7)), HSE_SEL at [7..15]
                        //Set HSE_SEL[3..0]
                        RST_CLK->RTC_CLOCK &= ~0x0F;
                        RST_CLK->RTC_CLOCK |= clkParams.c1sel;

                        TurnClkSrc(CLKSRC_HSE, 1);

			//HSE_RTC_EN[8] = 1
			//RTC_SEL[3..2]: 3 - select HSE_C1
			RST_CLK->RTC_CLOCK |= PN8;
			BKP->REG_0F |= (0x03 << 2);
                        break;
                }

                case CLKSRC_HSI:
                {
                        //HSI_C1 = HSI / (1 << (HSI_SEL - 7)), HSI_SEL at [7..15]
                        //Set HSI_SEL[7..4]
                        RST_CLK->RTC_CLOCK &= ~0xF0;
                        RST_CLK->RTC_CLOCK |= clkParams.c1sel << 4;

                        TurnClkSrc(CLKSRC_HSI, 1);

			//HSI_RTC_EN[9] = 1
			//RTC_SEL[3..2]: 2 - select HSI_C1
			RST_CLK->RTC_CLOCK |= PN9;
			BKP->REG_0F |= (0x02 << 2);
                        break;
                }


                case CLKSRC_LSE:
                default:
                {
                        TurnClkSrc(CLKSRC_LSE, 1);

			//RTC_SEL[3..2]: 1 - select LSE
			BKP->REG_0F |= (0x01 << 2);
                        break;
                }
        }


        //Set CAL[12..5] - mask count of ticks from 2^20
        BKP->REG_0F &= ~(0xFF << 5);
	BKP->REG_0F |= ((u8)destCfg->cal) << 5;

        //Set RTC_PRL[19..0]
        //speed = clk / prl, clk = [LSI, LSE, HSI_C1, HSE_C1]
        WaitRTCRegReady();
        BKP->RTC_PRL = clkParams.prl;

        //Set SECF[1] = 1
        WaitRTCRegReady();
        BKP->RTC_CS |= PN1;

        WaitRTCRegReady();

        //Enable/disable interrupt from RTC
	if (destCfg->tick)
        {
                //SECF_IE[4] = 1 - enabled
                BKP->RTC_CS |= PN4;
                IRQEnable(IRQ_RTC);
        }
        else
        {
                //SECF_IE[4] = 0 - disabled
                BKP->RTC_CS &= ~PN4;
                IRQDisable(IRQ_RTC);
        }

        return MCR_OK;
}



void TurnRTC(RTCBLOCK rtc, u8 turn)
{
        //Set/reset RTC_EN[4] - turn ON/OFF RTC
        if (turn)
                BKP->REG_0F |= PN4;
        else
                BKP->REG_0F &= ~PN4;
}



/* */
void RTCWrite(RTCVALUE val)
{
        WaitRTCRegReady();
        BKP->RTC_CNT = val;
}



/* */
RTCVALUE RTCRead()
{
        //Twice reading RTC counter:
        //"1986BE9x and MDR32F9Qx Series Errata Notice" (p 0005)
        RTCVALUE v1, v2;
        v1 = BKP->RTC_CNT;
        v2 = BKP->RTC_CNT;

        if (v1 != v2)
                return BKP->RTC_CNT;
        else
                return v1;
}



/* */
void BACKUP_IRQHandler(void)
{
        //if SECF[1] = 1
        if (BKP->RTC_CS & PN1)
        {
                //Reset SECF[1]
		WaitRTCRegReady();
                BKP->RTC_CS |= PN1;

                if (cfgRTC1.tick)
                        cfgRTC1.tick();
        }
}

/*===========================================================================
 * End of file 1986BE9x_RTC.c
 *===========================================================================*/

