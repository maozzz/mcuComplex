/*===========================================================================
 *
 *  Standard library
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2014-2016
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File wavfile.h: Wav file definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __WAVFILE_H
#define __WAVFILE_H

#include "../../bastypes.h"

#define WAVF_STR_RIFF		0x46464952U //'RIFF'
#define WAVF_STR_WAVE		0x45564157U //'WAVE'
#define WAVF_STR_FMT		0x20746D66U //'fmt '
#define WAVF_STR_DATA		0x61746164U //'data'

/* */
typedef struct tag_WavFormat
{
	//Wave type (PCM uncompressed = 1)
	u16 type;

	//Channel count
	u16 chCount;

	//Sample rate, Hz
	u32 smpRate;

	//Speed, bytes
	u32 byteSpeed;

	//Sample align, bytes
	u16 align;

	//Sample width, bits
	u16 smpWidth;
} WavFormat;


/* */
typedef struct tag_WavHeader
{
	//'RIFF': [0x52, 0x49, 0x46, 0x46]
	u32 chain1;

	//length1 = 12 + length2 + 8 + length3
	u32 length1;

	//'WAVE': [0x57, 0x41, 0x56, 0x45]
	u32 chain21;

	//'fmt ': [0x66, 0x6D, 0x74, 0x20]
	u32 chain22;

	//Size of WaveFormat structure
	u32 length2;

	//Wave format
	WavFormat format;

	//'data': [0x64, 0x61, 0x74, 0x61]
	u32 chain3;

	//Data length
	u32 length3;
} WavHeader;



/* Fill wav file header with specified parameters
   smpRate: sample rate, Hz
   smpWidth: sample width, bits
   chCount: count of channels */
void FillWavHeader(WavHeader *wh, u32 smpRate, u16 smpWidth, u16 chCount);

#endif /* !__WAVFILE_H */

/*===========================================================================
 * End of file wavfile.h
 *===========================================================================*/
