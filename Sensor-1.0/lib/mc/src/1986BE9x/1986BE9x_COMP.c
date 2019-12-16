/*===========================================================================
 *
 *  MCU library.
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2014
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File 1986BE9x_COMP.c. Comparator functions.
 *
 *===========================================================================*/

#include "../../inc/COMP.h"
#include <string.h>

//#define GetCOMP(__x)

static COMPConfig cfgCOMP1;

#define COMP_PORTINPUTS_CONFIG  (PN_NOPULLUP   | PN_NOPULLDOWN | PN_NOSHMIT |\
				 PN_CTRLDRIVER | PN_NOINFILTER)

MCRESULT InitializeCOMP(const COMPConfig *cfg)
{
        PINMASK pinOut;
        u8 funcOut;

        if (!cfg)
                return MCR_INVALID_CFG;

        switch(cfg->comp)
        {
                case COMP_N1: break;
                default:
                        return MCR_INVALID_CFG;
        }

        //Enable COMP bit [19]
        RST_CLK->PER_CLOCK |= PN19;

        //Configure COMP inputs:
        //[In1..In3] -> [E2, E3, E8]
	InitializePORTEx(PORT_E, PN2|PN3|PN8, 0,
			 PN_ANALOG | PN_INPUT | COMP_PORTINPUTS_CONFIG);

        switch(cfg->vRefSrc)
        {
                case COMP_VREFINT:
                case COMP_VREFEXT:
                {
                        //External V: [Vref+, Vref-] -> [E4, E5]
			InitializePORTEx(PORT_E, PN4|PN5, 0,
					 PN_ANALOG | PN_INPUT | COMP_PORTINPUTS_CONFIG);

                        //Set CVRR[1] = vRefi[4]
                        COMP->CFG &= ~PN1;
                        COMP->CFG |= (cfg->vRefi >> 3) & 2;

                        //Set CVR[7..4] = vRefi[3..0]
                        COMP->CFG &= ~(0x0F << 4);
                        COMP->CFG |= cfg->vRefi & 0x0F;

                        //Set CVRSS[2]: 1 - Vref+; 0 - AUcc
                        if (cfg->vRefSrc == COMP_VREFEXT)
                                COMP->CFG |= PN2;
                        else
                                COMP->CFG &= ~PN2;

                        //Set CVREN[3], CREF[8]
                        COMP->CFG |= PN3|PN8;
                        break;
                }

                case COMP_VREFNONE:
                default:
                {
                        //Reset CVREN[3], CREF[8]
                        COMP->CFG &= ~(PN3|PN8);
                        break;
                }
        }


        //Configure output
        switch(cfg->mapOut)
        {
                case COMP_MAP_OUT_B8:
                {
                        pinOut = PN8;
                        funcOut = PN_ALTER;
                        break;
                }

                case COMP_MAP_OUT_B11:
                {
                        pinOut = PN11;
                        funcOut = PN_REMAP;
                        break;
                }

                case COMP_MAP_OUT_NONE:
                default:                pinOut = 0;     break;
        }

        //Comparator OUT always at PORT_B
        if (pinOut)
		InitializePORTEx(PORT_B, pinOut, 0,
				 funcOut | PN_PWR_FAST | COMP_PORTINPUTS_CONFIG);

        cfgCOMP1 = *cfg;
	memcpy(cfgCOMP1, cfg, sizeof(COMPConfig));

        //CMPIE[13] - enable/disable interrupt
        //COMP interrupt at IRQ19.
        //Check it at file "startup_1986BE9x.mca"
        if (cfg->result)
        {
                COMP->CFG |= PN13;
                SYS->ISER00 |= PN19;
        }
        else
        {
                COMP->CFG &= ~PN13;
                SYS->ISER00 &= ~PN19;
        }

        return MCR_OK;
}


void TurnCOMP(COMPBLOCK comp, u8 turn)
{
        //ON[0]
        if (turn)
        {
                COMP->CFG |= PN0;

                //wait Ready[12] = 1 - COMP ready
                while(!(COMP->CFG & PN12));
        }
        else
                COMP->CFG &= ~PN0;
}


/*
Select input "+"
*/
void COMPSelInP(COMPBLOCK comp, u8 inP)
{
        //CREF[8]:
        //0 - IN1; 1 - CVREF;
        COMP->CFG = (COMP->CFG & ~PN8) | ((inP & 1) << 8);
}

/*
Select input "-"
*/
void COMPSelInM(COMPBLOCK comp, u8 inM)
{
        //CCH[10..9]:
        //0 - IN2; 1 - IN1; 2 - IN3; 3 - IVREF;
        COMP->CFG = (COMP->CFG & ~(0x03 << 9)) | ((inM & 0x03) << 9);
}


void COMPSelInPM(COMPBLOCK comp, u8 inP, u8 inM)
{
        COMPSelInP(comp, inP);
        COMPSelInM(comp, inM);
}


void COMPARATOR_IRQHandler(void)
{
        u32 v = (u32)(COMP->RESULT_LATCH & 1);

        if (cfgCOMP1.result)
            cfgCOMP1.result(&v);
}


COMPVALUE COMPRead(COMPBLOCK comp)
{
        //Result:
        //Rslt_Sy[0]: sync with HCLK
        return (u32)(COMP->RESULT & 1);

        //Rslt_As[1]: from COMP
        //return (u32)((COMP->RESULT >> 1) & 1);

        //Rslt_lch[2]:
        //return (u32)((COMP->RESULT >> 2) & 1);
        //return (u32)(COMP->RESULT_LATCH & 1);
}

/*===========================================================================
 * End of file 1986BE9x_COMP.c
 *===========================================================================*/

