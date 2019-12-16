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
 *  File 1986BE9x_DAC.c: DAC functions.
 *
 *===========================================================================*/

#include "../../inc/DAC.h"

#define DAC_REGDATA_OFFSET      0x00000004
#define BASE_DAC1_DATA          (BASE_DAC + DAC_REGDATA_OFFSET)
#define BASE_DAC2_DATA          (BASE_DAC + (DAC_REGDATA_OFFSET << 1))

#define DACx_PORT_CFG   (PN_NOPULLUP | PN_NOPULLDOWN |\
			 PN_NOSHMIT  | PN_CTRLDRIVER | PN_NOINFILTER)

#define PINSELECT(pno,pto,pnv,ptv)  pnOut = pno;  ptOut = pto;\
                                    pnVref = pnv; ptVref = ptv;

/* Convert signed 16-bit sample to DAC N-bit unsigned vaue */
#define SmpToValue(smp)		((u32)((s16)DAC_HALFVALUE + (smp >> (16 - DAC_VALSIZE))))

/* */
static MCRESULT DACMapPortPins(const DACConfig *cfg)
{
        PINMASK pnOut;         //DAC output pin
        PINMASK pnVref;        //DAC Vref input pin
        PORTBLOCK ptOut;      //DAC output port
        PORTBLOCK ptVref;     //DAC Vref port

        switch(cfg->dac)
        {
		//--- Select mapping 1986BE91T ---
		#if (TARGET_MCU == MCU_1986BE9x)
		case DAC_N1: //1986BE91T DAC1
                {
                        PINSELECT(PN9, PORT_E, PN10, PORT_E);
                        break;
                }		

		case DAC_N2: //1986BE91T DAC2
                {
                        PINSELECT(PN0, PORT_E, PN1, PORT_E);
                        break;
                }
		//--- End mapping 1986BE91T ---

		//--- Select mapping 1986BE1T ---
		#elif (TARGET_MCU == MCU_1986BE1x)
		case DAC_N1: //1986BE1T DAC1
                {
                        PINSELECT(PN1, PORT_E, PN15, PORT_D);
                        break;
                }

		case DAC_N2: //1986BE1T DAC2
                {
                        PINSELECT(PN2, PORT_E, PN0, PORT_E);
                        break;
                }
		//--- End mapping 1986BE1T ---

		//--- Select mapping 1901BC1T ---
		#elif (TARGET_MCU == MCU_1901BC1x)
		case DAC_N1: //1901BC1T DAC1
		{
			PINSELECT(PN0, PORT_E, PN1, PORT_E);
			break;
		}
				
		case DAC_N2: //1901BC1T DAC2
		{
			PINSELECT(PN9, PORT_E, PN10, PORT_E);
			break;
		}
		//--- End mapping 1901BC1T ---
				
                #endif

                default:
                        return MCR_INVALID_PORT;
        }

        //DAC output pin
	InitializePORTEx(ptOut, pnOut, 0, PN_ANALOG | PN_PWR_FASTEST | DACx_PORT_CFG);

        //DAC external Vref input.
        if (cfg->vRefSrc == DAC_VREFEXT)
		InitializePORTEx(ptVref, pnVref, 0, PN_ANALOG | PN_INPUT | DACx_PORT_CFG);
	
	return MCR_OK;
}



/* */
MCRESULT InitializeDAC(const DACConfig *cfg)
{
        PINMASK pinVrefEn;

        if (!cfg)
                return MCR_INVALID_CFG;

        switch(cfg->dac)
        {
                //DAC_CFG: Cfg_M_REF0[0] - DAC1 ext Vref enable
		case DAC_N1: pinVrefEn = PN0; break;

                //DAC_CFG: Cfg_M_REF1[1] - DAC2 ext Vref enable
		case DAC_N2: pinVrefEn = PN1; break;

		default: return MCR_INVALID_PORT;
        }

         //Map port pins
        if (DACMapPortPins(cfg) != MCR_OK)
                return MCR_INVALID_MAPPING;

        //Enable DAC clock
        ClkEnable(CLK_DAC);

        //External Vref. Output: 0..AVcc
        //Internal Vref. Output: 0..Vref
        if (cfg->vRefSrc == DAC_VREFEXT)
                DAC->CFG |= pinVrefEn;
        else
                DAC->CFG &= ~pinVrefEn;

        //Select work mode: sync, async
        //Cfg_SYNC_A[4]: 1/0 - sync/async
        if (cfg->sync == DAC_SYNC)
                DAC->CFG |= PN4;
        else
                DAC->CFG &= ~PN4;

        return MCR_OK;
}



/* */
u8 IsTurnDAC(DACBLOCK dac)
{
        //Cfg_ON_DAC0[2], Cfg_ON_DAC1[3]
        return (DAC->CFG & (PN2 << (dac & 1))) ? 1 : 0;
}



/* */
void TurnDAC(DACBLOCK dac, u8 turn)
{
        //Set/reset Cfg_ON_DAC0[2], Cfg_ON_DAC1[3]
        if (turn)
                DAC->CFG |= PN2 << (dac & 1);
        else
                DAC->CFG &= ~(PN2 << (dac & 1));
}



/* Write value */
void DACWrite(DACBLOCK dac, u32 value)
{
	//*(__io_reg*)(BASE_DAC1_DATA + DAC_REGDATA_OFFSET * (dac & 1)) = value & DAC_VALMASK;
	*(__io_reg*)(BASE_DAC1_DATA + (dac << 2)) = (value & DAC_VALMASK);
}



/* Write values synchronously */
void DACWriteSync(u32 *values)
{
         //DAC1 value [27..16], DAC0 value [11..0]
	 DAC->DAC1_DATA =
	 ((values[1] & DAC_VALMASK) << 16) | (values[0] & DAC_VALMASK);
}



/* Write sample */
void DACWriteSmp(DACBLOCK dac, s16 sample)
{
	*(__io_reg*)(BASE_DAC1_DATA + (dac << 2)) = SmpToValue(sample);
}



/* Write samples synchronously */
void DACWriteSyncSmp(DACBLOCK dac, s16 *samples)
{
	*(__io_reg*)(BASE_DAC1_DATA + (dac << 2)) =
		(SmpToValue(samples[0]) << 16) | SmpToValue(samples[1]);
}

/*===========================================================================
 * End of file 1986BE9x_DAC.c
 *===========================================================================*/

