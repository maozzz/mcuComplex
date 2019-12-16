/*===========================================================================
 *
 *  Audio library
 *
 * This source code is a product of Sun Microsystems, Inc. and is provided
 * for unrestricted use.  Users may copy or modify this source code without
 * charge.
 *
 * SUN SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND INCLUDING
 * THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR TRADE PRACTICE.
 *
 * Sun source code is provided with no support and without any obligation on
 * the part of Sun Microsystems, Inc. to assist in its use, correction,
 * modification or enhancement.
 *
 * SUN MICROSYSTEMS, INC. SHALL HAVE NO LIABILITY WITH RESPECT TO THE
 * INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY THIS SOFTWARE
 * OR ANY PART THEREOF.
 *
 * In no event will Sun Microsystems, Inc. be liable for any lost revenue
 * or profits or other special, indirect and consequential damages, even if
 * Sun has been advised of the possibility of such damages.
 *
 * Sun Microsystems, Inc.
 * 2550 Garcia Avenue
 * Mountain View, California  94043
 *
 *---------------------------------------------------------------------------
 *
 * Atmel Corporation
 *
 * File              : g726.h
 *
 * Description       : Header file for the ADPCM Encoder.
 *
 *                     Portions of software based on code released to
 *                     public domain by Sun Microsystem, Inc.(see notes below).
 *
 *===========================================================================*/

#ifndef __G726_H
#define __G726_H

#include "../../bastypes.h"

/*
 * The following is the definition of the state structure
 * used by the G.726 encoder and decoder to preserve their internal
 * state between successive calls.  The meanings of the majority
 * of the state structure fields are explained in detail in the
 * ITU-T Recommendation G.726.  The field names are essentially indentical
 * to variable names in the bit level description of the coding algorithm
 * included in this Recommendation. */
typedef struct tag_G726State
{
        Long   yl;    /* Locked or steady state step size multiplier. */
        Word16 yu;    /* Unlocked or non-steady state step size multiplier. */
        Word16 dms;   /* Short term energy estimate. */
        Word16 dml;   /* Long term energy estimate. */
        Word16 ap;    /* Linear weighting coefficient of 'yl' and 'yu'. */

        Word16 a[2];  /* Coefficients of pole portion of prediction filter. */
        Word16 b[6];  /* Coefficients of zero portion of prediction filter. */
        Word16 pk[2]; /* Signs of previous two samples of a partially
                       * reconstructed signal.*/

        Word16 dq[6]; /* Previous 6 samples of the quantized difference
                       * signal represented in an internal floating point format */

        Word16 sr[2]; /* Previous 2 samples of the quantized difference
                       * signal represented in an internal floating point format */
} G726State;


//Frame buffer length, samples (default)
//160smp = 8000Hz * 20ms
#define G726_SRCFRAME_LENGTH                    160

//Encoded frame length (default), bytes:
//40B = 16kbit/s * 20ms
#define G726R16_ENCFRAME_LENGTH                 40

//60B = 24kbit/s * 20ms
#define G726R24_ENCFRAME_LENGTH                 60

//80B = 32kbit/s * 20ms
#define G726R32_ENCFRAME_LENGTH                 80

//100B = 40kbit/s * 20ms
#define G726R40_ENCFRAME_LENGTH                 100


void InitializeG726(G726State *state);

Word32 EncodeG726r16(G726State *enc,
                        const Word16 *smpBuf, Word32 length, UWord8 *encBuf);

Word32 DecodeG726r16(G726State *dec,
                        const UWord8 *encBuf, Word32 length, 
						Word16 *smpBuf, Word32 smpLength);


Word32 EncodeG726r24(G726State *enc,
                        const Word16 *smpBuf, Word32 length, UWord8 *encBuf);

Word32 DecodeG726r24(G726State *dec,
                        const UWord8 *encBuf, Word32 length, 
						Word16 *smpBuf, Word32 smpLength);


Word32 EncodeG726r32(G726State *enc,
                        const Word16 *smpBuf, Word32 length, UWord8 *encBuf);

Word32 DecodeG726r32(G726State *dec,
                        const UWord8 *encBuf, Word32 length, 
						Word16 *smpBuf, Word32 smpLength);


Word32 EncodeG726r40(G726State *enc,
                        const Word16 *smpBuf, Word32 length, UWord8 *encBuf);

Word32 DecodeG726r40(G726State *dec,
                        const UWord8 *encBuf, Word32 length, 
						Word16 *smpBuf, Word32 smpLength);

#endif /* __G726_H */

/*===========================================================================
 * End of file g726.h
 *===========================================================================*/

