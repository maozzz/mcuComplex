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
 *  File SHA1.h: SHA-1 hash algorithm definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __SHA1_H
#define __SHA1_H

#include "../../bastypes.h"

//Uncomment to build compact (slower) code
//#define __SHA1_COMPACT_CODE			1

//SHA-1 algorithm name
#define SHA1_ALGNAME				"SHA-1\0"

//SHA-1 algorithm init vector size, bits
#define SHA1_IV_BITSIZE				160

//SHA-1 algorithm block size:
//bits
#define SHA1_BLOCK_BITSIZE			512
//8-bit words
#define SHA1_BLOCK_U8SIZE			(SHA1_BLOCK_BITSIZE >> 3)
//32-bit words
#define SHA1_BLOCK_U32SIZE			(SHA1_BLOCK_BITSIZE >> 5)

typedef struct tag_SHA1Context
{
	u32 h0;
	u32 h1;
	u32 h2;
	u32 h3;
	u32 h4;

	u64 length;
} SHA1Context;

/* 	Compute SHA-1 160-bit hash for specified byte array.
	@iv    : initialization vector (0 - use default)
	@data  : pointer to data array;	
	@length: data length to compute hash;
	@hash  : pointer to buffer of hash;
	returns: pointer to computed hash. */
u8 *SHA1ComputeHash(const u8 *iv, const u8 *data, s32 length, u8 *hash);

/* Compute SHA-1 hash for specified file */
u8 *SHA1ComputeFileHash(const char *path, u8 *hash);

/* Initialize SHA-1 algorithm */
void SHA1Initialize(SHA1Context *ct, const u8 *iv);

/* Performs the SHA-1 transformation of the current data block */
void SHA1TransformBlock(SHA1Context *ct, 
					    const u8 *block, s32 length);

/* Performs the SHA-1 transformation of the final data block */
u8 *SHA1TransformFinalBlock(SHA1Context *ct, 
						    const u8 *block, s32 length, u8 *hash);

#endif /* __SHA1_H */

/*===========================================================================
 * End of file SHA1.h
 *===========================================================================*/