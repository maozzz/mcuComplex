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

#include "../../inc/wavfile.h"

/* Fill wav file header with specified parameters
   smpRate: sample rate, Hz
   smpWidth: sample width, bits
   chCount: count of channels */
void FillWavHeader(WavHeader *wh, u32 smpRate, u16 smpWidth, u16 chCount)
{	
	//'RIFF' (hex values at little-endian format)
	wh->chain1  = WAVF_STR_RIFF;

	//sizeof(chain21,chain22,length2): 12 bytes
	//sizeof(chain3,length3): 8 bytes
	//length1 must be incremented by data length (length3)
	wh->length1 = 12 + sizeof(WavFormat) + 8;

	//'WAVE', 'fmt ', WaveFormat size
	wh->chain21 = WAVF_STR_WAVE;
	wh->chain22 = WAVF_STR_FMT;
	wh->length2 = sizeof(WavFormat);

	//Fill WaveFormat.
	//Type = 1: PCM uncompressed
	wh->format.type      = 1;
	wh->format.chCount   = chCount;
	wh->format.smpRate   = smpRate;
	wh->format.smpWidth  = smpWidth;
	wh->format.align     = chCount * (smpWidth >> 3);
	wh->format.byteSpeed = smpRate * wh->format.align;

	//'data'
	wh->chain3  = WAVF_STR_DATA;
	//length3 must set by user
	wh->length3 = 0;
}

/*===========================================================================
 * End of file wavfile.h
 *===========================================================================*/
