/*===========================================================================
 *
 *  Math library
 *
 *  Internet Society, 2014-2016
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File random.h: Random generators definitons and functions prototypes.
 *
 *===========================================================================*/

#ifndef __RANDOM_H
#define __RANDOM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../bastypes.h"

/*===========================================================================
 * Common definitions
 *===========================================================================*/

/* Gets random 32-bit unsigned integer */
#define getRandU32(rnd) ( ( (u32)(((float)0xFFFF) * (rnd)) << 16) |\
                          ( (u32)(((float)0xFFFF) * (rnd))      ) )


//Pointers to selected random functions
//{srandEc|srandPM|srandQ}
extern void  (*srandDf)(long seed);
//{randEc|randPM|randPMSh|randQ}
extern float (*randDf)();

/*===========================================================================
 * L'Ecuyer algorithm
 *===========================================================================*/

/* Initial seed of L'Ecuyer random generator. */
void srandEc(long seed);

/* Random generator with L'Ecuyer algorithm, period 10^18.
   returns random uniformly distributed between 0 and 1. */
float randEc(void);

/* Random generator with L'Ecuyer algorithm, period 10^18.
   returns random uniformly distributed between 0 and 2^32-1. */
u32 randEc32(void);

/*===========================================================================
 * Park-Miller algorithm (+ Bays-Durham shuffle)
 *===========================================================================*/

/* Initial seed of Park-Miller random generator. */
void srandPM(long seed);

/* Random generator with Park-Miller algorithm, period 10^8.
   returns random uniformly distributed between 0 and 1. */
float randPM(void);

/* Random generator with Park-Miller algorithm, period 10^8.
   returns random uniformly distributed between 0 and 2^32-1. */
u32 randPM32(void);

/* Random generator with Park-Miller algorithm and Bays-Durham shuffle.
   returns random uniformly distributed between 0 and 1. */
float randPMSh(void);

/* Random generator with Park-Miller algorithm and Bays-Durham shuffle.
   returns random uniformly distributed between 0 and 2^32-1. */
u32 randPMSh32(void);

/*===========================================================================
 * Quick random algorithm
 *===========================================================================*/

/* Initial seed of quick random generator. */
void srandQ(long seed);

/* Quick random generator
   returns random uniformly distributed between 0 and 1. */
float randQ(void);

/* Quick random generator
   returns random uniformly distributed between 0 and 2^32-1. */
u32 randQ32(void);

/*===========================================================================
 * Common functions
 *===========================================================================*/

/* Generate random string using current random function (randDf):
   hex chars */
char *StrRandomHex(char *dest, s32 length);

/* Generate random string using current random function (randDf):
   latin alphabet chars and digits */
char *StrRandomAbc(char *dest, s32 length);

#ifdef __cplusplus
}
#endif

#endif /* __RANDOM_H */

/*===========================================================================
 * End of file random.h
 *===========================================================================*/
