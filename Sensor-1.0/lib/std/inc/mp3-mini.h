/*============================================================================
 *
 * MPEG Audio Layer III decoder
 * Copyright (c) 2001, 2002 Fabrice Bellard,
 *           (c) 2007 Martin J. Fiedler
 *
 * This file is a stripped-down version of the MPEG Audio decoder from
 * the FFmpeg libavcodec library.
 *
 * FFmpeg and minimp3 are free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg and minimp3 are distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301 USA
 *
 * ---------------------------------------------------------------------------
 *
 *  File mp3-mini.h: MP3 mini decoder definitions and functions prototypes.
 *                   Not MCU implementation.
 *
 *============================================================================*/

#ifndef __MP3_MINI_H
#define __MP3_MINI_H

#include "../../bastypes.h"

typedef u8  uint8_t;
typedef u16 uint16_t;
typedef u32 uint32_t;

typedef s8  int8_t;
typedef s16 int16_t;
typedef s32 int32_t;

#define MP3_MAX_SAMPLES_PER_FRAME		(1152 * 2)

#define BACKSTEP_SIZE				512
#define EXTRABYTES				24
#define MP3_MAX_CHANNELS			2
#define SBLIMIT					32

/* */
typedef struct tag_MP3FrameInfo
{
	int smpRate;
	int chCount;

	//Generated amount of audio per frame
	int decSize;
} MP3FrameInfo;


/* */
typedef struct tag_bitstream_t
{
	const uint8_t *buffer;
	const uint8_t *buffer_end;
	int           index;
	int           size_in_bits;
} bitstream_t;


/* */
typedef struct tag_MP3DecState
{
	uint8_t		last_buf[(2 * BACKSTEP_SIZE + EXTRABYTES)];
	int		last_buf_size;
	int		frame_size;
	uint32_t	free_format_next_header;
	int		error_protection;
	int		sample_rate;
	int		sample_rate_index;
	int		bit_rate;
	bitstream_t	gb;
	bitstream_t	in_gb;
	int		nb_channels;
	int		mode;
	int		mode_ext;
	int		lsf;
	int16_t		synth_buf[MP3_MAX_CHANNELS][512 * 2];
	int		synth_buf_offset[MP3_MAX_CHANNELS];
	int32_t		sb_samples[MP3_MAX_CHANNELS][36][SBLIMIT];
	int32_t		mdct_buf[MP3_MAX_CHANNELS][SBLIMIT * 18];
	int		dither_state;
} MP3DecState;


/* */
void InitializeMP3Decoder(MP3DecState *dec);

/* */
Word32 DecodeMP3(MP3DecState *dec,
		const UWord8 *encBuf, Word32 length,
		Word16 *smpBuf, Word32 smpLength, MP3FrameInfo *info);

#endif /* !__MP3_MINI_H */

/*============================================================================
 * End of file mp3-mini.h
 *============================================================================*/
