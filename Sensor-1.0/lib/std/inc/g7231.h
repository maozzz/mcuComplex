/*===========================================================================
 *
 *  Audio library
 *
 *  ITU-T G.723.1 Software Package Release 2 (June 2006)
 *
 *  ITU-T G.723.1 Speech Coder   ANSI-C Source Code     Version 5.00
 *  Copyright (c) 1995, AudioCodes, DSP Group, France Telecom,
 *  Universite de Sherbrooke.  All rights reserved.
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File g7231.h. Basic function prototypes of coder G.723.1.
 *
 *===========================================================================*/

#ifndef __G7231_H
#define __G7231_H

#include "../../bastypes.h"

//Frame buffer length for both all modes, 16-bit samples
//240smp = 8000Hz * 30ms
#define G7231_SRCFRAME_LENGTH           240

//Encoded frame length at 6.4 kbit/s, bytes
#define G7231R6_ENCFRAME_LENGTH         24

//Encoded frame length at 5.3 kbit/s, bytes:
#define G7231R5_ENCFRAME_LENGTH         20

//SID frame length (32 bits), bytes:
#define G7231_SIDFRAME_LENGTH           4


//G.723.1 options (G7231State::opts)
#define G7231_OPTS_VAD                  0x01 //Enable VAD
#define G7231_OPTS_PREPROCESS           0x02 //Enable speech pre process
#define G7231_OPTS_POSTPROCESS          0x04 //Enable speech post process

//G.723.1 rates (G7231State::rate)
#define G7231_RATE_6_40kb               6400 //6.40 kbit/s
#define G7231_RATE_5_33kb               5300 //5.33 kbit/s


//G.723.1 specified definitions
#define G7231_Frame                     240
#define G7231_SubFrames                 4
#define G7231_LpcOrder                  10
#define G7231_PitchMin                  18
#define G7231_PitchMax                  (G7231_PitchMin + 127)
#define G7231_LpcFrame                  180
#define G7231_SubFrLen                  (G7231_Frame/G7231_SubFrames)
#define G7231_SizErr                    5
#define G7231_NbAvAcf                   3 //Nb of frames for Acf average
#define G7231_NbAvGain                  3 //Nb of frames for gain average
#define G7231_LpcOrderP1                (G7231_LpcOrder + 1)
#define G7231_SizAcf                    ((G7231_NbAvAcf + 1)* G7231_LpcOrderP1) //Size of array Acf


/* G.723.1 CNG encoder */
typedef struct tag_G7231CNGEncState
{
    Word16 CurGain;
    Word16 PrevFrameType;
    Word16 Acf[G7231_SizAcf];
    Word16 ShAcf[G7231_NbAvAcf+1];
    Word16 LspSid[G7231_LpcOrder];
    Word16 SidLpc[G7231_LpcOrder];
    Word16 RC[G7231_LpcOrderP1];
    Word16 ShRC;
    Word16 Ener[G7231_NbAvGain];
    Word16 NbEner;
    Word16 IRef;
    Word16 SidGain;
    Word16 RandSeed;
} G7231CNGEncState;


/* G.723.1 CNG decoder */
typedef struct tag_G7231CNGDecState
{
    Word16 CurGain;
    Word16 PastFtyp;
    Word16 LspSid[G7231_LpcOrder];
    Word16 SidGain;
    Word16 RandSeed;
} G7231CNGDecState;


/* G.723.1 VAD state */
typedef struct tag_G7231VADState
{
    Word16 Hcnt;
    Word16 Vcnt;
    Word32 Penr;
    Word32 Nlev;
    Word16 Aen;
    Word16 Polp[4];
    Word16 NLpc[G7231_LpcOrder];
} G7231VADState;


/* G.723.1 encoder state */
typedef struct tag_G7231EncState
{
   //Extra
   Word16 extra;

   //High pass variables
   Word16 HpfZdl;
   Word32 HpfPdl;

   //Sine wave detector
   Word16 SinDet;

   //Lsp previous vector
   Word16 PrevLsp[G7231_LpcOrder];

   //All pitch operation buffers
   Word16 PrevWgt[G7231_PitchMax];
   Word16 PrevErr[G7231_PitchMax];
   Word16 PrevExc[G7231_PitchMax];

   //Required memory for the delay
   Word16 PrevDat[G7231_LpcFrame - G7231_SubFrLen];

   //Used delay lines
   Word16 WghtFirDl[G7231_LpcOrder];
   Word16 WghtIirDl[G7231_LpcOrder];
   Word16 RingFirDl[G7231_LpcOrder];
   Word16 RingIirDl[G7231_LpcOrder];

   //Taming procedure errors
   Word32 Err[G7231_SizErr];

   //CNG encoder state
   G7231CNGEncState CNGEnc;

   //VAD state
   G7231VADState VADState;

   //Encoder rate (see G7231_RATE_x)
   Word16 rate;

   //Encoder options (see G7231_OPTS_x)
   Word16 opts;

} G7231EncState;


/* G.723.1 decoder state */
typedef struct tag_G7231DecState
{
   Word16 Ecount;
   Word16 InterGain;
   Word16 InterIndx;
   Word16 Rseed;
   Word16 Park;
   Word16 Gain;

   //Lsp previous vector
   Word16 PrevLsp[G7231_LpcOrder];

   //All pitch operation buffers
   Word16 PrevExc[G7231_PitchMax];

   //Used delay lines
   Word16 SyntIirDl[G7231_LpcOrder];
   Word16 PostFirDl[G7231_LpcOrder];
   Word16 PostIirDl[G7231_LpcOrder];

   //CNG decoder state
   G7231CNGDecState CNGDec;

   //Decoder options (see G7231_OPTS_x)
   Word16 opts;

} G7231DecState;




/* Initialize encoder */
void InitializeG7231Encoder(G7231EncState *enc);

/* Initialize decoder */
void InitializeG7231Decoder(G7231DecState *dec);

/* Encode frame */
Word32 EncodeG7231(G7231EncState *enc,
                   const Word16 *smpBuf, Word32 length,
                   UWord8 *encBuf);

/* Decode frame */
Word32 DecodeG7231(G7231DecState *dec,
                   const UWord8 *encBuf, Word32 length,
                   Word16 *smpBuf, Word32 smpLength);

/* Get size of encoded frame */
Word32 GetG7231FrameSize(UWord8 *encBuf);

#endif /* !__G7231_H */

/*===========================================================================
 * End of file g7231.h
 *===========================================================================*/
