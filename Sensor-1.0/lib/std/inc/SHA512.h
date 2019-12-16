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
 *  File SHA512.h: SHA-512 hash algorithm definitions and 
 *		           functions prototypes.
 *
 *===========================================================================*/

#ifndef __SHA512_H
#define __SHA512_H

#include "../../bastypes.h"

//SHA-512 algorithm names
#define SHA512_224_ALGNAME			"SHA-512/224\0"
#define SHA512_256_ALGNAME			"SHA-512/256\0"
#define SHA512_384_ALGNAME			"SHA-384\0"
#define SHA512_512_ALGNAME			"SHA-512\0"

//SHA-512 algorithm init vector size (all hash sizes), bits
#define SHA512_IV_BITSIZE			512

//SHA-512 algorithm block size:
//bits
#define SHA512_BLOCK_BITSIZE		1024
//8-bit words
#define SHA512_BLOCK_U8SIZE			(SHA512_BLOCK_BITSIZE >> 3)
//64-bit words
#define SHA512_BLOCK_U64SIZE		(SHA512_BLOCK_BITSIZE >> 6)


typedef struct tag_SHA512Context
{
	u64 h0;
	u64 h1;
	u64 h2;
	u64 h3;
	u64 h4;
	u64 h5;
	u64 h6;
	u64 h7;	
	u64 length;
	u32 hashSize;
} SHA512Context;

/* 	Compute SHA-512 (224,256,384,512)-bit hash for specified byte array.
	@iv      : initialization vector (0 - use default)
	@data    : pointer to data array;	
	@length  : data length to compute hash;
	@hash    : pointer to buffer of hash;
	@hashSize: computed hash size at bits: [224,256,384,512]
	returns  : pointer to computed hash. */
u8 *SHA512ComputeHash(const u8 *iv, 
					  const u8 *data, s32 length, u8 *hash, u32 hashSize);

/* Compute SHA-512 (224,256,384,512)-bit hash for specified file */
u8 *SHA512ComputeFileHash(const char *path, u8 *hash, u32 hashSize);

/* Initialize SHA-512 algorithm */
void SHA512Initialize(SHA512Context *ct, const u8 *iv, u32 hashSize);

/* Performs the SHA-512 transformation of the current data block */
void SHA512TransformBlock(SHA512Context *ct, 
					      const u8 *block, s32 length);

/* Performs the SHA-512 transformation of the final data block */
u8 *SHA512TransformFinalBlock(SHA512Context *ct, 
						      const u8 *block, s32 length, u8 *hash);

#endif /* __SHA512_H */

/*===========================================================================
 * End of file SHA512.h
 *===========================================================================*/