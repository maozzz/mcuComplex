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
 *  File targetmcu.h: MCU 1986BE91T evaluation board definitions.
 *
 *===========================================================================*/

#ifndef __TARGETMCU_H
#define __TARGETMCU_H

//EVALUATION BOARD MCU (1986BE91T, DON'T CHANGE!):
#define TARGET_MCU     	MCU_1986BE9x
#define TARGET_MCUM     MCUM_1986BE91T

//EVALUATION BOARD EXTERNAL OSCILLATORS (DON'T CHANGE!), Hz:
#ifndef FREQ_HSE
#define FREQ_HSE       8000000 //HSE: 8MHz
#endif

#ifndef FREQ_LSE
#define FREQ_LSE       32768   //LSE: 32,768 KHz
#endif

#endif

/*===========================================================================
 * End of file targetmcu.h
 *===========================================================================*/

