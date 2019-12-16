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
 *  File g711.h: G.711 a-Law, u-Law (PCMA, PCMU) functions prototypes.
 *
 *===========================================================================*/

#ifndef __G711_H
#define __G711_H

#include "../../bastypes.h"

#define cBias           0x84
#define cClip           32635

//Frame buffer default length (default), samples
//160smp = 8000Hz * 20ms
#define G711_SRCFRAME_LENGTH           160

//Encoded frame length (default), bytes:
//160B = 64kbit/s * 20ms = 8000B/s * 20ms
#define G711_ENCFRAME_LENGTH           160

Word32 EncodeG711U(const Word16 *smpBuf, Word32 length, UWord8 *encBuf);

Word32 DecodeG711U(const UWord8 *encBuf, Word32 length, 
				   Word16 *smpBuf, Word32 smpLength);

Word32 EncodeG711A(const Word16 *smpBuf, Word32 length, UWord8 *encBuf);

Word32 DecodeG711A(const UWord8 *encBuf, Word32 length, 
				   Word16 *smpBuf, Word32 smpLength);

UWord8 ConvertG711U2A(UWord8 ucode);

UWord8 ConvertG711A2U(UWord8 acode);

#endif /* __G711_H */

/*===========================================================================
 * End of file g711.h
 *===========================================================================*/

