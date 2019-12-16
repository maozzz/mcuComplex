/*===========================================================================
 *
 *  Math library
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
 *  File FFT.h: Fast Fourier Transformation (FFT) definitions
 *              and functions prototypes.
 *
 *===========================================================================*/

#ifndef __FFT_H
#define __FFT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../bastypes.h"

#define FFT_DIRECT              0
#define FFT_INVERSE             1

/* Solve Fast Furieux Transformation (FFT).
   @dataRe: Real part of I/O Data
   @dataIm: Imaginary part of I/O Data
   @N:      N = log2(<length dataRe>), N=[1..14];
   @direction: direct or inverse transform (FFT_DIRECT, FFT_INVERSE)
   return: 0 on success, otherwise 1. */
s32 FFT(float *dataRe, float *dataIm, u32 N, u8 direction);

#ifdef __cplusplus
}
#endif

#endif /* __FFT_H */

/*===========================================================================
 * End of file FFT.h
 *===========================================================================*/

