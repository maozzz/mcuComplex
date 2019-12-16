/*===========================================================================
 *
 *  Audio library
 *
 *  GSM 06.10 (ETS 300 961) 13.2 kbit/s encoder/decoder.
 *
 *  Copyright 1992 by Jutta Degener and Carsten Bormann, Technische
 *  Universitaet Berlin.
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File gsm0610.h: Top level functions prototypes of GSM 06.10 13.2 kbit/s
 *                  encoder/decoder.
 *
 *===========================================================================*/

#ifndef __GSM0610_H
#define __GSM0610_H

#include "../../bastypes.h"

/* */
typedef struct tag_GSM0610State
{
        Word16 dp0[280];
        Word16 e[50];

        Word16 z1;          /* Offset_com. */
        Word32 L_z2;        /* Offset_com. */
        Word32 mp;          /* Preemphasis  */

        Word16 u[8];        /*  */
        Word16 LARpp[2][8]; /*  */
        Word16 j;           /*  */

        Word16 ltp_cut;     /* LTP crosscorr (Should define LTP_CUT) */
        Word16 nrp;         /* Synthesis */
        Word16 v[9];        /* Synthesis */
        Word16 msr;         /* Postprocessing */

        UWord8 fast;        /* (Should define FAST and USE_FLOAT_MUL) */
} GSM0610State;


//Frame buffer length, samples
//160smp = 8000Hz * 20ms
#define GSM0610_SRCFRAME_LENGTH           160

//Encoded frame length, bytes:
//33B = 264bit / 8
#define GSM0610_ENCFRAME_LENGTH           33


void InitializeGSM0610(GSM0610State *state);

Word32 EncodeGSM0610(GSM0610State *enc,
                     const Word16 *smpBuf, Word32 length, UWord8 *encBuf);

Word32 DecodeGSM0610(GSM0610State *dec,
                     const UWord8 *encBuf, Word32 length, 
					 Word16 *smpBuf, Word32 smpLength);

#endif /* __GSM0610_H */

/*===========================================================================
 * End of file gsm0610.h
 *===========================================================================*/

