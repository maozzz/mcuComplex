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
 *  File GOST34112012.h: GOST-34.11-2012 hash algorithm definitions and 
 *                       functions prototypes.
 *
 *===========================================================================*/

#ifndef __GOST34112012_H
#define __GOST34112012_H

#include "../../bastypes.h"

//GOST-34.11-2012 algorithm names
#define GOST34112012_256_ALGNAME		"GOST-34.11-2012-256\0"
#define GOST34112012_512_ALGNAME		"GOST-34.11-2012-512\0"

//GOST-34.11-2012 algorithm init vector size (all hash sizes)
//bits
#define GOST34112012_IV_BITSIZE			512
//8-bit words
#define GOST34112012_IV_U8SIZE			(GOST34112012_IV_BITSIZE >> 3)
//32-bit words
#define GOST34112012_IV_U32SIZE			(GOST34112012_IV_BITSIZE >> 5)

//GOST-34.11-2012 algorithm block size:
//bits
#define GOST34112012_BLOCK_BITSIZE		512
//8-bit words
#define GOST34112012_BLOCK_U8SIZE		(GOST34112012_BLOCK_BITSIZE >> 3)
//32-bit words
#define GOST34112012_BLOCK_U32SIZE		(GOST34112012_BLOCK_BITSIZE >> 5)

typedef struct tag_GOST34112012Context
{
	u32 N[GOST34112012_BLOCK_U32SIZE];
    u32 E[GOST34112012_BLOCK_U32SIZE];
    u32 h[GOST34112012_BLOCK_U32SIZE];

	u32 hashSize;
} GOST34112012Context;


/* 	Compute GOST-34.11-2012 256/512-bit hash for specified byte array.
	@iv      : initialization vector (0 - use default)
	@data    : pointer to data array;	
	@length  : data length to compute hash;
	@hash    : pointer to buffer of hash;
	@hashSize: computed hash size at bits: [256, 512]
	returns  : pointer to computed hash. */
u8 *GOST34112012ComputeHash(const u8 *iv, 
							const u8 *data, s32 length, u8 *hash, u32 hashSize);

/* Compute GOST-34.11-2012 256/512-bit hash for specified file */
u8 *GOST34112012ComputeFileHash(const char *path, u8 *hash, u32 hashSize);

/* Initialize GOST-34.11-2012 algorithm */
void GOST34112012Initialize(GOST34112012Context *ct, const u8 *iv, u32 hashSize);

/* Performs the GOST-34.11-2012 transformation of the current data block */
void GOST34112012TransformBlock(GOST34112012Context *ct, 
								const u8 *block, s32 length);

/* Performs the GOST-34.11-2012 transformation of the final data block */
u8 *GOST34112012TransformFinalBlock(GOST34112012Context *ct, 
									const u8 *block, s32 length, u8 *hash);

#endif /* __GOST34112012_H */

/*===========================================================================
 * End of file GOST34112012.h
 *===========================================================================*/