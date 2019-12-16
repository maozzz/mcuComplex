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
 *  File stdmath.h: Standard math functions.
 *
 *===========================================================================*/

#ifndef __STDMATH_H
#define __STDMATH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../bastypes.h"

//Use sinus table instead internal function
#ifndef USE_SIN_TABLE
#define USE_SIN_TABLE			1
#endif /* USE_SIN_TABLE */

//Math constants
#define MATH_PId4               0.7853981633974480F     //PI/4
#define MATH_PId2               1.5707963267949000F     //PI/2
#define MATH_PI                 3.1415926535897900F     //PI
#define MATH_PI3d2              4.7123889803846900F     //3*PI/2
#define MATH_PI2                6.2831853071795900F     //2*PI

#ifdef USE_SIN_TABLE

float mcsin(float a);
float mccos(float a);

/* USE_SIN_TABLE */
#else

#include <math.h>

#define mcsin(a)        ((float)sin((float)(a)))
#define mccos(a)        ((float)cos((float)(a)))

#endif /* !USE_SIN_TABLE */

/* Returns absolute value */
#define mcabs(v)		((v < 0) ? -v : v)

/* Default square root function */
#define mcsqrt(x)		mcsqrt_nrf(x)

/* Returns square root of 32-bit integer
   (Newthon's algorithm, fastest, optimal divider initial value,
	round to next integer) */
u32 mcsqrt_nrf(s32 x);

/* Returns square root of 32-bit integer
   (Newthon's algorithm, fast, round to next integer) */
u32 mcsqrt_nr(s32 x);

/* Get square root of 32-bit integer
   (Newthon's algorithm, fast, no round) */
u32 mcsqrt_n(s32 x);

/* Returns square root of 32-bit integer
   (simple algorithm, slow) */
u32 mcsqrt_s(s32 x);

#ifdef __cplusplus
}
#endif

#endif /* !__STDMATH_H */

/*===========================================================================
 * End of file stdmath.h
 *===========================================================================*/
