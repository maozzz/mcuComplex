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
 *  File SHA3.h: SHA-3 (Keccak) hash algorithm definitions and 
 *		         functions prototypes.
 *
 *===========================================================================*/

#ifndef __SHA3_H
#define __SHA3_H

#include "../../bastypes.h"

//SHA-3 algorithm names
#define SHA3_224_ALGNAME			"SHA3-224\0"
#define SHA3_256_ALGNAME			"SHA3-256\0"
#define SHA3_384_ALGNAME			"SHA3-384\0"
#define SHA3_512_ALGNAME			"SHA3-512\0"

//Keccak algorithm names
#define KECCAK_224_ALGNAME			"Keccak-224\0"
#define KECCAK_256_ALGNAME			"Keccak-256\0"
#define KECCAK_384_ALGNAME			"Keccak-384\0"
#define KECCAK_512_ALGNAME			"Keccak-512\0"

//SHA-3/Keccak state size, bits 
#define SHA3_IV_BITSIZE				1600

//SHA-3 algorithm block sizes:
//bits
#define SHA3_224_BLOCK_BITSIZE		1152
#define SHA3_256_BLOCK_BITSIZE		1088
#define SHA3_384_BLOCK_BITSIZE		832
#define SHA3_512_BLOCK_BITSIZE		576
//8-bit words
#define SHA3_224_BLOCK_U8SIZE		(SHA3_224_BLOCK_BITSIZE >> 3)
#define SHA3_256_BLOCK_U8SIZE		(SHA3_256_BLOCK_BITSIZE >> 3)
#define SHA3_384_BLOCK_U8SIZE		(SHA3_384_BLOCK_BITSIZE >> 3)
#define SHA3_512_BLOCK_U8SIZE		(SHA3_512_BLOCK_BITSIZE >> 3)
//32-bit words
#define SHA3_224_BLOCK_U32SIZE		(SHA3_224_BLOCK_BITSIZE >> 5)
#define SHA3_256_BLOCK_U32SIZE		(SHA3_256_BLOCK_BITSIZE >> 5)
#define SHA3_384_BLOCK_U32SIZE		(SHA3_384_BLOCK_BITSIZE >> 5)
#define SHA3_512_BLOCK_U32SIZE		(SHA3_512_BLOCK_BITSIZE >> 5)


typedef struct tag_SHA3Context
{
	u32 s[50];
	u32 hashSize;	
	u32 padding;
} SHA3Context;

/* 	Compute SHA-3 (224,256,384,512)-bit hash for specified byte array.
	@iv      : initialization vector (0 - use default)
	@data    : pointer to data array;	
	@length  : data length to compute hash;
	@hash    : pointer to buffer of hash;
	@hashSize: computed hash size at bits: [224,256,384,512]
	returns  : pointer to computed hash. */
u8 *SHA3ComputeHash(const u8 *iv, 
					const u8 *data, s32 length, u8 *hash, u32 hashSize);

/* Compute SHA-3 (224,256,384,512)-bit hash for specified file */
u8 *SHA3ComputeFileHash(const char *path, u8 *hash, u32 hashSize);

/* 	Compute Keccak (224,256,384,512)-bit hash for specified byte array.
	@iv      : initialization vector (0 - use default)
	@data    : pointer to data array;	
	@length  : data length to compute hash;
	@hash    : pointer to buffer of hash;
	@hashSize: computed hash size at bits: [224,256,384,512]
	returns  : pointer to computed hash. */
u8 *KeccakComputeHash(const u8 *iv, 
					  const u8 *data, s32 length, u8 *hash, u32 hashSize);

/* Compute Keccak (224,256,384,512)-bit hash for specified file */
u8 *KeccakComputeFileHash(const char *path, u8 *hash, u32 hashSize);


/* Initialize SHA-3 algorithm */
void SHA3Initialize(SHA3Context *ct, const u8 *iv, u32 hashSize);

/* Initialize Keccak algorithm */
void KeccakInitialize(SHA3Context *ct, const u8 *iv, u32 hashSize);

/* Performs the SHA-3/Keccak transformation of the current data block */
void SHA3TransformBlock(SHA3Context *ct, 
					    const u8 *block, s32 length);

/* Performs the SHA-3/Keccak transformation of the final data block */
u8 *SHA3TransformFinalBlock(SHA3Context *ct, 
						    const u8 *block, s32 length, u8 *hash);

#endif /* __SHA3_H */

/*===========================================================================
 * End of file SHA3.h
 *===========================================================================*/