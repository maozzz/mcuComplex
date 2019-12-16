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
 *  File types.h: MCU types definitions.
 *
 *===========================================================================*/

#ifndef __TYPES_H
#define __TYPES_H

#include "../../bastypes.h"

//Available MCU series:
#define MCU_SWMODEL		0xFFFFFFFF //Software MCU model
#define MCU_1901BC1x		190110     //ARM Cortex-M3 + DSP, up to 100MHz
#define MCU_1986BE9x		198690     //ARM Cortex-M3, up to 80MHz
#define MCU_1986BE1x		198610     //ARM Cortex-M1, up to 144MHz

//Available MCU models:
#define MCUM_SWMODEL		0xFFFFFFFF //Software MCU model

#define MCUM_1901BC1x		190110
#define MCUM_1901BC1T		MCUM_1901BC1x //ceramic, 132pn, [-60..+85C]
#define MCUM_K1901BC1T		MCUM_1901BC1x //ceramic, 132pn, [-60..+85C]
#define MCUM_K1901BC1TK		MCUM_1901BC1x //ceramic, 132pn, [0..+70С]
#define MCUM_K1901BC1H4		MCUM_1901BC1x //no corp

#define MCUM_1986BE91x		198691
#define MCUM_1986BE91T		MCUM_1986BE91x //ceramic, 132pn, [-60..+125C]
#define MCUM_1986BE92U		MCUM_1986BE91x //ceramic, 64pn,  [-60..+125C]
#define MCUM_1986BE93U		MCUM_1986BE91x //ceramic, 48pn,  [-60..+125C]
#define MCUM_K1986BE9x		MCUM_1986BE91x //ceramic, 132pn, [-60..+125C]
#define MCUM_K1986BE92QC	MCUM_1986BE91x //plastic, 64pn, [0..+70C]
#define MCUM_K1986BE92QI	MCUM_1986BE91x //plastic, 64pn, [-40..+85C]
#define MCUM_K1986BE91H4	MCUM_1986BE91x //no corp

#define MCUM_1986BE94x		198694
#define MCUM_1986BE94T		MCUM_1986BE94x //ceramic, 132pn, [-60..+125C]

#define MCUM_1986BE1x		198610
#define MCUM_1986BE1T		MCUM_1986BE1x //ceramic, 132pn, [-60..+125C]
#define MCUM_K1986BE1T		MCUM_1986BE1x //ceramic, 132pn, [-60..+125C]
#define MCUM_K1986BE1TK		MCUM_1986BE1x //ceramic, 132pn, [0..+70C]
#define MCUM_K1986BE1QI		MCUM_1986BE1x //plastic, 144pn, [-40..+85C]
#define MCUM_K1986BE1H4		MCUM_1986BE1x //no corp

//Return type of initialization functions
typedef u32                     MCRESULT;

//MCRESULT values
#define MCR_OK                  0   //No errors, success
#define MCR_SETUPCLK_DIFREQ     1   //Warning: setup success but
									//current clock != requested clock
#define MCR_SETUPCLK_FAIL       20  //Error: setting requested clock is fail
#define MCR_SETUPCLK_BADSRC     21  //Error: invalid clock source selected
#define MCR_INVALID_PORT        22  //Error: invalid port is selected
#define MCR_INVALID_CFG         23  //Error: invalid configuration
#define MCR_SETUPSPEED_FAIL     24  //Error: can't set speed
#define MCR_INVALID_MAPPING     25  //Error: can't map block i/o pins tp port
#define MCR_FAIL                255 //Common error

#include <targetmcu.h>

//Check definitions
#ifndef FREQ_HSE
#error "'FREQ_HSE' (HSE frequency) must be defined at 'targetmcu.h'."
#endif /* !FREQ_HSE */

#ifndef FREQ_LSE
#error "'FREQ_LSE' (LSE frequency) must be defined at 'targetmcu.h'."
#endif /* !FREQ_LSE */

//Include specified mcu definitions
#if (!defined(TARGET_MCU))
#error "'TARGET_MCU' must be defined at 'targetmcu.h'."

#elif (TARGET_MCU == MCU_1986BE9x)
#include "./1986BE9x/1986BE9x_defs.h"

#elif (TARGET_MCU == MCU_1986BE1x)
#include "./1986BE1x/1986BE1x_defs.h"

#elif (TARGET_MCU == MCU_1901BC1x)
#include "./1901BC1x/1901BC1x_defs.h"

#elif (TARGET_MCU == MCU_SWMODEL)
#include "./swmodel/swmodel_defs.h"

#else
#error "Unknown MCU type. Check 'TARGET_MCU' at 'targetmcu.h'"

#endif /* TARGET_MCU */

#endif /* !__TYPES_H */

/*===========================================================================
 * End of file types.h
 *===========================================================================*/
