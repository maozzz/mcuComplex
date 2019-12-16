/*===========================================================================
 *
 *  Audio library
 *
 *  ITU-T G.729 Software Package Release 2 (November 2006) 
 *
 *  ITU-T G.729A Speech Coder with Annex B    ANSI-C Source Code
 *  Version 1.5    Last modified: October 2006
 *
 *  Copyright (c) 1996,
 *  AT&T, France Telecom, NTT, Universite de Sherbrooke, Lucent Technologies,
 *  Rockwell International
 *  All rights reserved.
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File g729ab.h. Basic function prototypes of coder G.729ab.
 *
 *===========================================================================*/

#ifndef __G729AB_H
#define __G729AB_H

#include "../../bastypes.h"

//Frame buffer length, samples (16bit)
//80smp = 8000Hz * 10ms
#define G729AB_SRCFRAME_LENGTH           80

//Encoded frame length (80 bits), bytes:
#define G729AB_ENCFRAME_LENGTH           10

//SID frame length (16 bits), bytes:
#define G729AB_SIDFRAME_LENGTH           2


//G.729ab options (G729ABState::opts)
#define G729AB_OPTS_VAD                 0x01 //Enable VAD
#define G729AB_OPTS_PREPROCESS          0x02 //Enable speech pre process
#define G729AB_OPTS_POSTPROCESS         0x04 //Enable speech post process


/* G.729ab encoder/decoder state */
typedef struct tag_G729ABState
{
    //Options (see G729AB_OPTS_x)
    UWord32 opts;
} G729ABState;


/* */
void InitializeG729ABEncoder(G729ABState *enc);

/* */
void InitializeG729ABDecoder(G729ABState *dec);

/* */
Word32 EncodeG729AB(G729ABState *enc,
                    const Word16 *smpBuf, Word32 length, UWord8 *encBuf);

/* */
Word32 DecodeG729AB(G729ABState *dec,
                    const UWord8 *encBuf, Word32 length,
                    Word16 *smpBuf, Word32 smpLength);

#endif /* !__G729AB_H */

/*===========================================================================
 * End of file g729ab.h
 *===========================================================================*/
