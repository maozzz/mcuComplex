/*===========================================================================
 *
 *  Audio library
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2014
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File dtmf.h: DTMF (RFC 4733) functions prototypes.
 *
 *===========================================================================*/

#ifndef __DTMF_H
#define __DTMF_H

#include "../../bastypes.h"

//Frame buffer default length (default), samples
//160smp = 8000Hz * 20ms
#define DTMF_SRCFRAME_LENGTH           160

//Encoded frame length, bytes:
#define DTMF_ENCFRAME_LENGTH           4

//No DTMF event
#define DTMF_CODE_NONE                 -1

//Uncomment to ignore volume field at DTMF frame
#define DTMF_IGNORE_VOLUME              1

/* */
typedef struct tag_DTMFState
{
        s16 code;
        u16 duration;
        u8 isEnd;
        u8 volume;
        u16 reserved;
} DTMFState;

/* Initialize DTMF encoder/decoder */
void InitializeDTMF(DTMFState *state);

/* Fill DTMF frame using specified state */
Word32 FillDTMF(DTMFState *enc, UWord8 *encBuf);

/* Encode DTMF frame using specifed samples buffer */
Word32 EncodeDTMF(DTMFState *enc,
                  const Word16 *smpBuf, Word32 length, UWord8 *encBuf);

/* Decode DTMF frame to samples array */
Word32 DecodeDTMF(DTMFState *dec,
                  const UWord8 *encBuf, Word32 length,
                  Word16 *smpBuf, Word32 smpLength);

#endif /* __DTMF_H */

/*===========================================================================
 * End of file dtmf.h
 *===========================================================================*/

