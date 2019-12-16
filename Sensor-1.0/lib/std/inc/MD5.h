/*===========================================================================
 *
 *  Standard library
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2015
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File MD5.h: MD-5 hash algorithm definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __MD5_H
#define __MD5_H

#include "../../bastypes.h"

//Uncomment to build compact (slower) code
//#define __MD5_COMPACT_CODE			1

//MD-5 algorithm name
#define MD5_ALGNAME					"MD-5\0"

//MD-5 algorithm init vector size, bits
#define MD5_IV_BITSIZE				128

//MD-5 algorithm block size:
//bits
#define MD5_BLOCK_BITSIZE			512
//8-bit words
#define MD5_BLOCK_U8SIZE			(MD5_BLOCK_BITSIZE >> 3)
//32-bit words
#define MD5_BLOCK_U32SIZE			(MD5_BLOCK_BITSIZE >> 5)

typedef struct tag_MD5Context
{
	u32 h0;
	u32 h1;
	u32 h2;
	u32 h3;	

	u64 length;
} MD5Context;

/* Compute MD-5 128-bit hash for specified byte array.
   @iv    : initialization vector (0 - use default)
   @data  : pointer to data array;	
   @length: data length to compute hash;
   @hash  : pointer to buffer of hash;
   returns: pointer to computed hash. */
u8 *MD5ComputeHash(const u8 *iv, const u8 *data, s32 length, u8 *hash);

/* Compute MD-5 hash for specified file */
u8 *MD5ComputeFileHash(const char *path, u8 *hash);

/* Initialize MD-5 algorithm */
void MD5Initialize(MD5Context *ct, const u8 *iv);

/* Performs the MD-5 transformation of the current data block */
void MD5TransformBlock(MD5Context *ct, 
					   const u8 *block, s32 length);

/* Performs the MD-5 transformation of the final data block */
u8 *MD5TransformFinalBlock(MD5Context *ct, 
						   const u8 *block, s32 length, u8 *hash);

#endif /* __MD5_H */

/*===========================================================================
 * End of file MD5.h
 *===========================================================================*/
