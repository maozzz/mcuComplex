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
 *  File dvi4.c: Function prototypes and type definitions
 *               for 32-kbit DVI4 encoder/decoder.
 *
 *===========================================================================*/

#ifndef __DVI4_H
#define __DVI4_H

#include "../../bastypes.h"

//DVI4 frame header length (according RFC-3551):
//(Word16, UWord8, UWord8) = 4 bytes
#define DVI4_HEADER_LENGTH                      4

//Frame buffer length, samples (default)
//160smp = 8000Hz * 20ms
#define DVI4_SRCFRAME_LENGTH                    160

//Encoded frame length, bytes (default):
//DVI4 header + (80bytes = 160smp * 4bit)
#define DVI4_ENCFRAME_LENGTH                    (DVI4_HEADER_LENGTH + 80)


typedef struct tag_DVI4State
{
        Word16 predict;
        Word16 index;
} DVI4State;


void InitializeDVI4(DVI4State *state);

Word32 EncodeDVI4(DVI4State *enc,
                        const Word16 *smpBuf, Word32 length, UWord8 *encBuf);

Word32 DecodeDVI4(DVI4State *dec,
                        const UWord8 *encBuf, Word32 length, 
						Word16 *smpBuf, Word32 smpLength);


Word32 EncodeVDVI(DVI4State *enc,
                        const Word16 *smpBuf, Word32 length, UWord8 *encBuf);

Word32 DecodeVDVI(DVI4State *dec,
                        const UWord8 *encBuf, Word32 length, 
						Word16 *smpBuf, Word32 smpLength);

#endif /* __DVI4_H */

/*===========================================================================
 * End of file dvi4.h
 *===========================================================================*/

