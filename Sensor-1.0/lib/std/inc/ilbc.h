/*===========================================================================
 *
 *  Audio library
 *
 *  iLBC Speech Coder ANSI-C Source Code
 *
 *  Copyright (C) The Internet Society (2004).
 *  All Rights Reserved.
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File ilbc.h: Top level functions prototypes of iLBC encoder/decoder.
 *
 *===========================================================================*/

#ifndef __ILBC_H
#define __ILBC_H

#include "../../bastypes.h"

//Frame buffer length at 13.33 kbit/s, samples
//240smp = 8000Hz * 30ms
#define ILBCR13_SRCFRAME_LENGTH			240

//Frame buffer length at 15.20 kbit/s, samples
//160smp = 8000Hz * 20ms
#define ILBCR15_SRCFRAME_LENGTH			160

//Encoded frame length at 13.33 kbit/s, bytes:
//50B = 400bit / 8
#define ILBCR13_ENCFRAME_LENGTH			50

//Encoded frame length at 15.20 kbit/s, bytes:
//38B = 304bit / 8
#define ILBCR15_ENCFRAME_LENGTH			38


//ILBC rates (ILBCEncState::rate, ILBCDecState::rate)
#define ILBC_RATE_13_33kb				30 //13.33 kbit/s (30ms frame)
#define ILBC_RATE_15_20kb				20 //15.20 kbit/s (20ms frame)

//ILBC options (ILBCDecState::opts)
#define ILBC_OPTS_POSTPROCESS           0x04 //Enable speech post process


//General codec settings
#define ILBC_ULP_CLASSES                3
#define ILBC_CB_NSTAGES                 3
#define ILBC_NSUB_MAX                   6

#define ILBC_LPC_FILTERORDER            10
#define ILBC_LPC_LOOKBACK               60
#define ILBC_BLOCKL_MAX                 240
#define ILBC_SUBL                        40

//Enhancer
#define ILBC_ENH_BLOCKL                 80  /* Block length */
#define ILBC_ENH_NBLOCKS_TOT            8   /* ILBC_ENH_NBLOCKS + ILBC_ENH_NBLOCKS_EXTRA */
#define ILBC_ENH_BUFL                   ((ILBC_ENH_NBLOCKS_TOT)*ILBC_ENH_BLOCKL)


/* ULP state */
typedef struct tag_ILBCULPState
{
    int lsf_bits[6][ILBC_ULP_CLASSES+2];

    int start_bits[ILBC_ULP_CLASSES+2];
    int startfirst_bits[ILBC_ULP_CLASSES+2];

    int scale_bits[ILBC_ULP_CLASSES+2];
    int state_bits[ILBC_ULP_CLASSES+2];

    int extra_cb_index[ILBC_CB_NSTAGES][ILBC_ULP_CLASSES+2];
    int extra_cb_gain[ILBC_CB_NSTAGES][ILBC_ULP_CLASSES+2];

    int cb_index[ILBC_NSUB_MAX][ILBC_CB_NSTAGES][ILBC_ULP_CLASSES+2];
    int cb_gain[ILBC_NSUB_MAX][ILBC_CB_NSTAGES][ILBC_ULP_CLASSES+2];
} ILBCULPState;



/* ILBC encoder state */
typedef struct tag_ILBCEncState
{
    //Selected rate (see ILBC_RATE_x)
    int rate;

    //Basic parameters for different frame sizes
    int blockl;
    int nsub;
    int nasub;
    int no_of_bytes;
    int no_of_words;
    int lpc_n;
    int state_short_len;
    const ILBCULPState *ULP_inst;

    //Analysis filter state
    float anaMem[ILBC_LPC_FILTERORDER];

    //Old lsf parameters for interpolation
    float lsfold[ILBC_LPC_FILTERORDER];
    float lsfdeqold[ILBC_LPC_FILTERORDER];

    //Signal buffer for LP analysis
    float lpc_buffer[ILBC_LPC_LOOKBACK + ILBC_BLOCKL_MAX];

    //State of input HP filter
    float hpimem[4];

} ILBCEncState;



/* ILBC decoder state */
typedef struct tag_ILBCDecState
{
    //Selected rate (see ILBC_RATE_x)
    int rate;

    //Options (see ILBC_OPTS_x)
    int opts;

    //Basic parameters for different frame sizes
    int blockl;
    int nsub;
    int nasub;
    int no_of_bytes;
    int no_of_words;
    int lpc_n;
    int state_short_len;
    const ILBCULPState *ULP_inst;

    //Synthesis filter state
    float syntMem[ILBC_LPC_FILTERORDER];

    //Old LSF for interpolation
    float lsfdeqold[ILBC_LPC_FILTERORDER];

    //Pitch lag estimated in enhancer and used in PLC
    int last_lag;

    //PLC state information
    int prevLag;
    int consPLICount;
    int prevPLI;
    int prev_enh_pl;
    float prevLpc[ILBC_LPC_FILTERORDER + 1];
    float prevResidual[ILBC_NSUB_MAX * ILBC_SUBL];
    float per;
    unsigned long seed;

    //Previous synthesis filter parameters
    float old_syntdenum[(ILBC_LPC_FILTERORDER + 1)*ILBC_NSUB_MAX];

    //State of output HP filter
    float hpomem[4];

    //Enhancer state information    
    float enh_buf[ILBC_ENH_BUFL];
    float enh_period[ILBC_ENH_NBLOCKS_TOT];

} ILBCDecState;


/* Encoder initialization */
void InitializeILBCEncoder(ILBCEncState *enc);

/* Decoder initialization */
void InitializeILBCDecoder(ILBCDecState *dec);

/* Encode frame 20/30ms (160/240 samples) to 38/50 bytes */
Word32 EncodeILBC(ILBCEncState *enc,
                  const Word16 *smpBuf, Word32 length, UWord8 *encBuf);

/* Decode 38/50 bytes to 20/30ms (160/240 samples) frame */
Word32 DecodeILBC(ILBCDecState *dec,
                  const UWord8 *encBuf, Word32 length,
                  Word16 *smpBuf, Word32 smpLength);

#endif /* __ILBC_H */

/*===========================================================================  
 * End of file ilbc.h
 *===========================================================================*/
