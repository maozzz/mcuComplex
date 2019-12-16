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
 *  File DAC.h: DAC definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __DAC_H
#define __DAC_H

#include "types.h"
#include "CPU.h"

//DAC channels
#define DAC_N1                  0 //DAC #1
#define DAC_N2                  1 //DAC #2

//Vref sources
#define DAC_VREFINT             0 //Internal ref voltage.
#define DAC_VREFEXT             1 //External ref voltage.

//Mode
#define DAC_ASYNC               0 //Async work
#define DAC_SYNC                1 //Sync work

typedef u8                      DACBLOCK;

/* Represents DAC configuration */
typedef struct tag_DACConfig
{
        //DAC block (see DAC_Nx)
        DACBLOCK dac;

        //Ref voltage source (see DAC_VREFx)
        u16 vRefSrc;

        //Sync/async mode (DAC_ASYNC, DAC_SYNC)
        u8 sync;
} DACConfig;

/* Initialize DAC */
MCRESULT InitializeDAC(const DACConfig *cfg);

/* Indicates that specified DAC is turned ON */
u8 IsTurnDAC(DACBLOCK dac);

/* Turn ON/OFF selected DAC */
void TurnDAC(DACBLOCK dac, u8 turn);

/* Write value */
void DACWrite(DACBLOCK dac, u32 value);

/* Write values synchronously */
void DACWriteSync(u32 *values);

/* Write sample */
void DACWriteSmp(DACBLOCK dac, s16 value);

/* Write samples synchronously */
void DACWriteSyncSmp(DACBLOCK dac, s16 *samples);

#endif /* __DAC_H */

/*===========================================================================
 * End of file DAC.h
 *===========================================================================*/

