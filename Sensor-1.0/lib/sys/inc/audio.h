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
 *  File audio.c: Audio library definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __AUDIO_H
#define __AUDIO_H

#include <globaldef.h>
#include "../../std/inc/dtmf.h"
#include "../../std/inc/RTP.h"
#include "../../std/inc/MSession.h"

/*===========================================================================
 * Audio configuration (1/0 - enabled/disabled).
 *===========================================================================*/

//Enable encoding/decoding
#ifndef AUD_EN_MOD_AUDIO
#define AUD_EN_MOD_AUDIO        1
#endif /* !AUD_EN_MOD_AUDIO */

#ifndef AUD_EN_MOD_GENERATOR
//Add source: "lib/std/src/math/stdmath.c"
#define AUD_EN_MOD_GENERATOR    1
#endif /* !AUD_EN_MOD_GENERATOR */

#ifndef AUD_EN_MOD_OSC
//Add source: "lib/mc/src/UART.c"
#define AUD_EN_MOD_OSC          0
#endif /* !AUD_EN_MOD_OSC */


#if (AUD_EN_MOD_AUDIO == 1)
#ifndef AUD_EN_CODEC_G711
//Add source: "lib/std/src/codec/g711/g711.c"
#define AUD_EN_CODEC_G711       1
#endif /* !AUD_EN_CODEC_G711 */

#ifndef AUD_EN_CODEC_G726
//Add source: "lib/std/src/codec/g726/g726.c"
#define AUD_EN_CODEC_G726       1
#endif /* !AUD_EN_CODEC_G726 */

#ifndef AUD_EN_CODEC_GSM
//Add source: "lib/std/src/codec/gsm0610/gsm0610.c"
#define AUD_EN_CODEC_GSM        1
#endif /* !AUD_EN_CODEC_GSM */

#ifndef AUD_EN_CODEC_DVI
//Add source: "lib/std/src/codec/dvi4/dvi4c"
#define AUD_EN_CODEC_DVI        1
#endif /* !AUD_EN_CODEC_DVI */

#ifndef AUD_EN_CODEC_G7231
//Add source: "lib/std/src/codec/g723.1/g7231.c"
#define AUD_EN_CODEC_G7231      0
#endif /* !AUD_EN_CODEC_G7231 */

#ifndef AUD_EN_CODEC_DTMF
//Add source: "lib/std/src/codec/dtmf/dtmf.c"
#define AUD_EN_CODEC_DTMF       1
#endif /* !AUD_EN_CODEC_DTMF */
#endif /* AUD_EN_MOD_AUDIO == 1 */

/*===========================================================================
 * Signal generator definitions
 *===========================================================================*/

//GenConfig::opts
//Signal type
#define GEN_TYPE_SIN                    0x01 //Sinus
#define GEN_TYPE_RECT                   0x02 //Rectangle
#define GEN_TYPE_SAW                    0x03 //Saw

//Signal type mask
#define GEN_TYPE_MASK                   0x0F

/* Represents generator configuration */
typedef struct tag_GenConfig
{
        //Frequency, Hz
        float F;

        //Phase, sec
        float P;

        //Amplitude
        s16 A;

        //Constant
        s16 H;

        //Options (see GEN_TYPE_x)
        u32 opts;
} GenConfig;


/* Represents generator state */
typedef struct tag_GenState
{
        //Buffer generated samples
        s16 *smpBuffer;

        //Samples buffer length
        u32 length;

        //Current time, samples
        //(should set to 0)
        u32 tm;

        //Time while signal is turned ON, samples
        //(should more than 0)
        u32 tmON;

        //Time while signal is turned OFF, samples
        u32 tmOFF;

        //Value while signal is turned OFF
        s16 valOFF;
} GenState;


#if (AUD_EN_MOD_GENERATOR == 1)
/** Generate samples */
void GenerateSamples(GenConfig *gcfg, s32 count, u32 smpRate,
                     s16 *smpBuffer, s32 length);

/** Get sample */
s16 GetGenSample(GenState *genSt);
/* AUD_EN_MOD_GENERATOR == 1 */
#else

#define GenerateSamples(a,b,c,d,e)
#define GetGenSample(x)         0
#endif /* AUD_EN_MOD_GENERATOR != 1 */

/*===========================================================================
 * Audio definitions
 *===========================================================================*/

//Input/output sample rate, Hz
//It's recommended set to 8000
#define AUD_SAMPLE_RATE                 8000

//Count of samples per millisecond
#define AUD_SAMPLE_MSEC                 ((u32)(AUD_SAMPLE_RATE / 1000))

//Count of millisecond per sample
#define AUD_MSEC_SAMPLE                 (1000.0F / (float)AUD_SAMPLE_RATE)

//Minimum frame duration, ms
#define AUD_FRAME_MINDURATION   20

//Maximum frame duration, ms
#define AUD_FRAME_MAXDURATION   100

//Minimum frame buffer length, samples.
//Default value, in some cases must defined by current codec.
#define AUD_FRAME_MINLENGTH  ((u32)(AUD_SAMPLE_RATE * AUD_FRAME_MINDURATION / 1000))

//Maximum frame buffer length, samples.
#define AUD_FRAME_MAXLENGTH  ((u32)(AUD_SAMPLE_RATE * AUD_FRAME_MAXDURATION / 1000))

//Maximum RTP in/out encoded payload length, bytes
//(G.711x encodes 16 bit samples to 8 bit codes)
#define AUD_RTP_PLD_MAXLENGTH    AUD_FRAME_MAXLENGTH

//Out RTP packet length, bytes: header + payload
#define AUD_TRANRTP_LENGTH  (RTP_HEADER_LENGTH + AUD_RTP_PLD_MAXLENGTH)

//In RTP packet length, bytes: header + CSRCs + extension + payload
#define AUD_RECVRTP_LENGTH  (RTP_HEADER_MAXLENGTH + AUD_RTP_PLD_MAXLENGTH)

//Out RPT packet with DTMF payload length, bytes: header + DTMF frame
#define AUD_TRANDTMF_LENGTH (RTP_HEADER_LENGTH + DTMF_ENCFRAME_LENGTH)

//Audio sources count
#ifndef AUD_ASRC_COUNT
#define AUD_ASRC_COUNT                  3
#endif /* !AUD_ASRC_COUNT */

//Ring buffer length, samples.
//AUD_ASRC_BUF_LENGTH = 2^N, N > 1
//AUD_ASRC_BUF_LENGTH > AUD_DECODED_MAXLENGTH
#define AUD_ASRC_BUF_LENGTH             1024

//Timeout to wait audio from remote source, samples (~250ms)
#define AUD_ASRC_TIMEOUT                2000

//Decode audio error codes
#define AUD_ERR_NONE                    0
#define AUD_ERR_INVALID_RTP             1
#define AUD_ERR_INVALID_LENGTH          2
#define AUD_ERR_NO_ASRC                 3

//AudioParams::input,output valid values:
#define AUD_IN_NONE                    0xFF //No input
#define AUD_IN_ADC                     0    //Input: ADC
#define AUD_IN_GENERATOR               1    //Input: generator,
											//AudioParams::genIn must be defined

//AudioParams::output valid values:
#define AUD_OUT_NONE                   0xFF //No output
#define AUD_OUT_DAC_N1                 0    //Output: DAC #1
#define AUD_OUT_DAC_N2                 1    //Output: DAC #2

//AudioParams::volInput,volInput fields:
//Minimum volume
#define AUD_VOL_MIN                     0
//Minimum volume
#define AUD_VOL_MAX                     15
//Middle volume
#define AUD_VOL_MIDDLE                  8
//Default volume:
#define AUD_VOL_DEFAULT                 AUD_VOL_MIDDLE

/* Audio parameters */
typedef struct tag_AudioParams
{
        //Audio input (see AUD_IO_x)
		u8 input;

        //Audio output (see AUD_IO_x)
		u8 output;

        //Input volume [0..15] => [0..100]%
		u8 volInput;

        //Output volume [0..15] => [0..100]%
		u8 volOutput;

        //Output codec (see RTP_PT_x at "RTP.h")
        u8 codec;

		//Reserved
		u8 reserved;

        //Frame duration, ms [20..100]
		u16 frameDuration;

		//Pointer to input generator
        GenState *genIn;

		//Pointer to output generator
        GenState *genOut;
} AudioParams;


//Last decode audio error code (see AUD_ERR_x)
extern u8 audErrorCode;

//Current DTMF state
extern DTMFState dtmfEncState;

//Pointer to current audio parameters
extern AudioParams *audioParams;

//Pointer to RTP header at outgoing packets
extern RTPHeader *tranRTPHdr;

//Last input sample
extern u32 inputSample;

//
#define UpdateSamplerInput(inVal) inputSample = inVal

/* Initialize audio module */
int InitializeAudio(void);

/* Reset parameters of audio module */
void ResetAudioParams();

/* Reset RTP parameters */
void ResetRTP();

/* Reset audio module */
void ResetAudio(void);

/* Capture and play samples.
  Call this function with freq "AUD_SAMPLE_RATE" Hz */
void SamplerHandler();

/* Gets current event code */
s16 GetEventCode();

/* Indicates that event is end */
u8 IsEndEvent();

/* Encode captured audio and put to RTP packet */
s32 EncodeAudio(MSession *ses, u8 *destbuf, s32 length);

/* Parse RTP packet, decode and play audio */
s32 DecodeAudio(MSession *ses, const u8 *srcbuf, s32 length);

/* Encode specified DTMF event */
s32 EncodeEvent(MSession *ses, s16 code, u16 duration, u8 *destbuf, s32 length);

/* Read captured audio to specified buffer */
s32 ReadAudioBuffer(u8 *destbuf, s32 length);

/* Write specified buffer to audio buffer */
s32 WriteAudioBuffer(u32 ssrc, const u8 *srcbuf, s32 length);

/*===========================================================================
 * Osciloscope
 *===========================================================================*/

#if (AUD_EN_MOD_OSC == 1)

/* Transmit oscilloscope data within UART.
   Initialize and turn UART before using this function. */
void TransmitOscData(u32 uart);

/* AUD_EN_MOD_OSC == 1 */
#else

#define TransmitOscData(x)

#endif /* AUD_EN_MOD_OSC != 1 */

/*===========================================================================
 * Auxiliary functions and variables
 *===========================================================================*/

/* Codec information */
typedef struct tag_CodecInfo
{
        cpcstr_t name;
        const s16 pt;
} CodecInfo;

//List of available audio codecs
extern CodecInfo audioCodecList[];

/* Get count of available audio codecs */
s32 GetAudioCodecCount();

/* Get codec info index using specified payload type number */
s32 IndexOfAudioCodecByPT(s16 pt);

/* Get codec info index using specified codec name */
s32 IndexOfAudioCodecByName(const char *codecName);

/* Get codec info using specified payload type number */
CodecInfo *GetAudioCodecByPT(s16 pt);

/* Get codec info using specified codec name */
CodecInfo *GetAudioCodecByName(const char *codecName);

/* Get frame length (samples) using specified duration (ms) */
u16 GetAudioFrameLength(u8 pt, u8 durationms);

#endif /* __AUDIO_H */

/*===========================================================================
 * End of file audio.h
 *===========================================================================*/

