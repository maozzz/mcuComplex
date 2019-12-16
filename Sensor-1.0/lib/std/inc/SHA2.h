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
 *  File SHA2.h: SHA-2 hash algorithm definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __SHA2_H
#define __SHA2_H

#include "../../bastypes.h"

//SHA-2 algorithm names (224/256 bit)
#define SHA2_224_ALGNAME			"SHA-224\0"
#define SHA2_256_ALGNAME			"SHA-256\0"

//SHA-2 algorithm init vector size (256/224 bit hash), bits
#define SHA2_IV_BITSIZE				256

//SHA-2 algorithm block size:
//bits
#define SHA2_BLOCK_BITSIZE			512
//8-bit words
#define SHA2_BLOCK_U8SIZE			(SHA2_BLOCK_BITSIZE >> 3)
//32-bit words
#define SHA2_BLOCK_U32SIZE			(SHA2_BLOCK_BITSIZE >> 5)

typedef struct tag_SHA2Context
{
	u32 h0;
	u32 h1;
	u32 h2;
	u32 h3;
	u32 h4;
	u32 h5;
	u32 h6;
	u32 h7;
	u32 hashSize;
	u64 length;
} SHA2Context;

/* 	Compute SHA-2 (224,256)-bit hash for specified byte array.
	@iv      : initialization vector (0 - use default)
	@data    : pointer to data array;	
	@length  : data length to compute hash;
	@hash    : pointer to buffer of hash;
	@hashSize: computed hash size at bits: [224, 256]
	returns  : pointer to computed hash. */
u8 *SHA2ComputeHash(const u8 *iv, 
					const u8 *data, s32 length, u8 *hash, u32 hashSize);

/* Compute SHA-2 (224,256)-bit hash for specified file */
u8 *SHA2ComputeFileHash(const char *path, u8 *hash, u32 hashSize);

/* Initialize SHA-2 algorithm */
void SHA2Initialize(SHA2Context *ct, const u8 *iv, u32 hashSize);

/* Performs the SHA-2 transformation of the current data block */
void SHA2TransformBlock(SHA2Context *ct, 
					    const u8 *block, s32 length);

/* Performs the SHA-2 transformation of the final data block */
u8 *SHA2TransformFinalBlock(SHA2Context *ct, 
						    const u8 *block, s32 length, u8 *hash);

#endif /* __SHA2_H */

/*===========================================================================
 * End of file SHA2.h
 *===========================================================================*/