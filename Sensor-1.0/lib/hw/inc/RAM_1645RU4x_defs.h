/*===========================================================================
 *
 *  Hardware library
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
 *  File RAM_1645RU4x_defs.h: RAM 1645RU4x definitions.
 *
 *===========================================================================*/

#ifndef __RAM_1645RU4x_DEFS_H
#define __RAM_1645RU4x_DEFS_H

//Size, bytes (1Mx16)
#define RAM_1645RUx_SIZE            (1 << 21)

//Cell size, bits
#define RAM_1645RUx_CELLSIZE        16

//Count of sectors: 1
#define FM_1645RUx_SECCNT           1

//Time intervals, ns

#if defined(TARGET_RAMM) && (TARGET_RAMM == RAMM_1645RU4AU)

#define RAM_1645RUx_tphzoe_ns          15
#define RAM_1645RUx_tphzce_ns          15
#define RAM_1645RUx_taa_ns             30

#define RAM_1645RUx_tcyr_ns            30
#define RAM_1645RUx_tcyw_ns            30

#define RAM_1645RUx_tas_ns             5
#define RAM_1645RUx_tah_ns             5
#define RAM_1645RUx_twweh_ns           20

#else /* TARGET_RAMM == RAMM_1645RU4AU */

#define RAM_1645RUx_tphzoe_ns          20
#define RAM_1645RUx_tphzce_ns          20
#define RAM_1645RUx_taa_ns             35

#define RAM_1645RUx_tcyr_ns            35
#define RAM_1645RUx_tcyw_ns            35

#define RAM_1645RUx_tas_ns             5
#define RAM_1645RUx_tah_ns             5
#define RAM_1645RUx_twweh_ns           25

#endif /* TARGET_RAMM != RAMM_1645RU4AU */

#endif /* !__RAM_1645RU4x_DEFS_H */

/*===========================================================================
 * End of file RAM_1645RU4x_defs.h
 *===========================================================================*/

