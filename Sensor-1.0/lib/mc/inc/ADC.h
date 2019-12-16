/*===========================================================================
 *
 *  MCU library
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
 *  File ADC.h: ADC definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __ADC_H
#define __ADC_H

#include "types.h"
#include "CPU.h"

//ADC block number
#define ADC_N1                  0 //ADC #1
#define ADC_N2                  1 //ADC #2

//Vref sources
#define ADC_VREFINT             0 //Internal Vref
#define ADC_VREFEXT             1 //External Vref

typedef u8                      ADCBLOCK;

/* Represents ADC result */
typedef struct tag_ADCResult
{
		//Sample value
        u32 value;

		//Channel number
        u32 channel;
} ADCResult;


/* Represents ADC configuration */
typedef struct tag_ADCConfig
{
        //ADC block (see ADC_Nx)
        ADCBLOCK adc;

        //Sync work ADCs: 1/0
        u8 sync;

        //Reference voltage source (see ADC_VREFx)
        u16 vRefSrc;

        //Delay between ADCs calcs
        u16 delayADC;

        //Delay between two calcs
        u16 delayGo;

        //Channel mask
        u32 chMask;

        //Requested ADC Clock, Hz [ADC_MIN_CLOCK .. ADC_MAX_CLOCK]
        u32 adcClk;

        //Clock source (see CLKSRC_x)
        u32 clkSrc;

        //ADC result handler.
        //Whether defined (i.e result != 0) ADC interrupt will be used
        //Otherwise use ADCRead, ADCReadCh functions to get result
        void (*result)(ADCResult *r);
} ADCConfig;



/* Initialize ADC */
MCRESULT InitializeADC(const ADCConfig *cfg);

/* Turn ADC */
void TurnADC(ADCBLOCK adc, u8 turn);

/* Synchronously read ADC sample from any channel */
u32 ADCReadAny(ADCBLOCK adc, u32 *chNumber);

/* Synchronously read ADC sample from specified channel */
u32 ADCReadSync(ADCBLOCK adc, u32 chNumber);

/* Read ADC sample from specified channel */
u32 ADCRead(ADCBLOCK adc, u32 chNumber);

/* Convert ADC sample to voltage */
float ADCToVoltage(u32 sample);

/* Convert voltage to ADC sample */
u32 ADCToSample(float voltage);

#endif /* __ADC_H */

/*===========================================================================
 * End of file ADC.h
 *===========================================================================*/

