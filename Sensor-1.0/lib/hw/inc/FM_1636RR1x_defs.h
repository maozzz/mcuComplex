/*===========================================================================
 *
 *  Hardware library
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2016
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File FM_1636RR1x_defs.h: Flash memory 1636RR1x (512Kx8) definitions.
 *
 *===========================================================================*/

#ifndef __FM_1636RR1x_DEFS_H
#define __FM_1636RR1x_DEFS_H

//Size, bytes (512Kx8):
#define FM_1636RR1x_SIZE            (1 << 19)

//Cell size, bits
#define FM_1636RR1x_CELLSIZE        8

//Count of sectors: 8x64K
#define FM_1636RR1x_SECCNT          8

//Time intervals, ns
#define FM_1636RRx_tphzoe_ns       (16 + 10)

#if defined(TARGET_FMM) && (TARGET_FMM == FMM_1636RR1AU)

#define FM_1636RRx_tcyw_ns         (65 + FM_1636RRx_tphzoe_ns)
#define FM_1636RRx_tcyr_ns         (65 + FM_1636RRx_tphzoe_ns)
#define FM_1636RRx_twweh_ns        35

#else /* TARGET_FMM == FMM_1636RR1AU */

#define FM_1636RRx_tcyw_ns         (70 + FM_1636RRx_tphzoe_ns)
#define FM_1636RRx_tcyr_ns         (70 + FM_1636RRx_tphzoe_ns)
#define FM_1636RRx_twweh_ns        40

#endif /* TARGET_FMM != FMM_1636RR1AU */

#define FM_1636RRx_tas_ns ((FM_1636RRx_tcyw_ns - FM_1636RRx_twweh_ns) >> 1)
#define FM_1636RRx_tah_ns ((FM_1636RRx_tcyw_ns - FM_1636RRx_twweh_ns) >> 1)

#endif

/*===========================================================================
 * End of file FM_1636RR1x_defs.h
 *===========================================================================*/

