/*===========================================================================
 *
 *  Audio library
 *
 *  G.722.2 (2003/07) and Cor.1,2,3 | 26173-710 ANSI-C source code
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File g7222.h: Basic function prototypes of coder G.722.2.
 *
 *  Notes:
 *   1. This coder work at 16000Hz sample rate
 *
 *===========================================================================*/

#ifndef __G7222_H
#define __G7222_H

#include "../../bastypes.h"

//Frame buffer length, samples (16bit)
//320smp = 16000Hz * 20ms
#define G7222_SRCFRAME_LENGTH          320

//Encoded frame length, bytes
#define G7222R6_ENCFRAME_LENGTH         17 //at  6.60 kbit/s
#define G7222R8_ENCFRAME_LENGTH         23 //at  8.85 kbit/s
#define G7222R12_ENCFRAME_LENGTH        32 //at 12.65 kbit/s
#define G7222R14_ENCFRAME_LENGTH        36 //at 14.25 kbit/s
#define G7222R15_ENCFRAME_LENGTH        40 //at 15.85 kbit/s
#define G7222R18_ENCFRAME_LENGTH        46 //at 18.25 kbit/s
#define G7222R19_ENCFRAME_LENGTH        50 //at 19.85 kbit/s
#define G7222R23_ENCFRAME_LENGTH        58 //at 23.05 kbit/s
#define G7222R24_ENCFRAME_LENGTH        60 //at 23.85 kbit/s

//Length of SID frame, bytes
#define G7222_SIDFRAME_LENGTH           5

//Maximum encoded frame size
#define G7222_ENCFRAME_MAX_LENGTH       64

//G.722.2 encoder options (G7222EncState::opts)
#define G7222_OPTS_VAD                  0x01 //Enable VAD

//G.722.2 rates (G7222EncState::rate)
#define G7222_RATE_6_60kb               0 //rate  6.60 kbit/s
#define G7222_RATE_8_85kb               1 //rate  8.85 kbit/s
#define G7222_RATE_12_65kb              2 //rate 12.65 kbit/s
#define G7222_RATE_14_25kb              3 //rate 14.25 kbit/s
#define G7222_RATE_15_85kb              4 //rate 15.85 kbit/s
#define G7222_RATE_18_25kb              5 //rate 18.25 kbit/s
#define G7222_RATE_19_85kb              6 //rate 19.85 kbit/s
#define G7222_RATE_23_05kb              7 //rate 23.05 kbit/s
#define G7222_RATE_23_85kb              8 //rate 23.85 kbit/s


//G.722.2 parameters
#define G7222_PIT_MAX                    231 //Maximum pitch lag
#define G7222_L_INTERPOL            (16 + 1) //Length of filter for interpolation
#define G7222_M                           16 //Order of LP filter
#define G7222_L_MEANBUF                    3 //
#define G7222_L_FILT                      12 //Delay of up-sampling filter
#define G7222_L_FILT16k                   15 //Delay of down-sampling filter
#define G7222_M16k                        20 //
#define G7222_DTX_HIST_SIZE                8 //

#define G7222_L_TOTAL                   384 //Total size of speech buffer
#define G7222_L_FRAME                   256 //Frame size
#define G7222_OPL_DECIM                   2 //Decimation in open-loop pitch analysis
#define G7222_COMPLEN                    12 //Number of sub-bands used by VAD
#define G7222_F_5TH_CNT                   5 //Number of 5th order filters
#define G7222_F_3TH_CNT                   6 //Number of 3th order filters

/* VAD state */
typedef struct tag_VadVars
{
    Word16 bckr_est[G7222_COMPLEN];              /* background noise estimate                */
    Word16 ave_level[G7222_COMPLEN];             /* averaged input components for stationary */
                                           /* estimation                               */
    Word16 old_level[G7222_COMPLEN];             /* input levels of the previous frame       */
    Word16 sub_level[G7222_COMPLEN];             /* input levels calculated at the end of a frame (lookahead)  */
    Word16 a_data5[G7222_F_5TH_CNT][2];          /* memory for the filter bank               */
    Word16 a_data3[G7222_F_3TH_CNT];             /* memory for the filter bank               */

    Word16 burst_count;                    /* counts length of a speech burst          */
    Word16 hang_count;                     /* hangover counter                         */
    Word16 stat_count;                     /* stationary counter                       */

    /* Note that each of the following two variables holds 15 flags. Each flag reserves 1 bit of the
     * variable. The newest flag is in the bit 15 (assuming that LSB is bit 1 and MSB is bit 16). */
    Word16 vadreg;                         /* flags for intermediate VAD decisions     */
    Word16 tone_flag;                      /* tone detection flags                     */

    Word16 sp_est_cnt;                     /* counter for speech level estimation      */
    Word16 sp_max;                         /* maximum level                            */
    Word16 sp_max_cnt;                     /* counts frames that contains speech       */
    Word16 speech_level;                   /* estimated speech level                   */
    Word32 prev_pow_sum;                   /* power of previous frame                  */

} VadVars;


/* Encoder: DTX state */
typedef struct tag_dtx_encState
{
    Word16 isf_hist[G7222_M * G7222_DTX_HIST_SIZE];
    Word16 log_en_hist[G7222_DTX_HIST_SIZE];
    Word16 hist_ptr;
    Word16 log_en_index;
    Word16 cng_seed;

    /* DTX handler stuff */
    Word16 dtxHangoverCount;
    Word16 decAnaElapsedCount;
    Word32 D[28];
    Word32 sumD[G7222_DTX_HIST_SIZE];
} dtx_encState;


/* Decoder: DTX state */
typedef struct tag_dtx_decState
{
    Word16 since_last_sid;
    Word16 true_sid_period_inv;
    Word16 log_en;
    Word16 old_log_en;
    Word16 level;
    Word16 isf[G7222_M];
    Word16 isf_old[G7222_M];
    Word16 cng_seed;

    Word16 isf_hist[G7222_M * G7222_DTX_HIST_SIZE];
    Word16 log_en_hist[G7222_DTX_HIST_SIZE];
    Word16 hist_ptr;

    Word16 dtxHangoverCount;
    Word16 decAnaElapsedCount;

    Word16 sid_frame;
    Word16 valid_data;
    Word16 dtxHangoverAdded;

    //Contains previous state
    Word16 dtxGlobalState;

    //Updated in main decoder */
    //Marker to know if CNI data is ever renewed
    Word16 data_updated;

    Word16 dither_seed;
    Word16 CN_dith;
    Word16 dtx_vad_hist;

} dtx_decState;



/* G.722.2 encoder state */
typedef struct tag_G7222EncState
{
    Word16 mem_decim[2 * G7222_L_FILT16k];       /* speech decimated filter memory */
    Word16 mem_sig_in[6];                  /* hp50 filter memory */
    Word16 mem_preemph;                    /* speech preemph filter memory */
    Word16 old_speech[G7222_L_TOTAL - G7222_L_FRAME];  /* old speech vector at 12.8kHz */
    Word16 old_wsp[G7222_PIT_MAX / G7222_OPL_DECIM];   /* old decimated weighted speech vector */
    Word16 old_exc[G7222_PIT_MAX + G7222_L_INTERPOL];  /* old excitation vector */
    Word16 mem_levinson[G7222_M + 2];            /* levinson routine memory */
    Word16 ispold[G7222_M];                      /* old isp (immittance spectral pairs) */
    Word16 ispold_q[G7222_M];                    /* quantized old isp */
    Word16 past_isfq[G7222_M];                   /* past isf quantizer */
    Word16 mem_wsp;                        /* wsp vector memory */
    Word16 mem_decim2[3];                  /* wsp decimation filter memory */
    Word16 mem_w0;                         /* target vector memory */
    Word16 mem_syn[G7222_M];                     /* synthesis memory */
    Word16 tilt_code;                      /* tilt of code */
    Word16 old_wsp_max;                    /* old wsp maximum value */
    Word16 old_wsp_shift;                  /* old wsp shift */
    Word16 Q_old;                          /* old scaling factor */
    Word16 Q_max[2];                       /* old maximum scaling factor */
    Word16 gp_clip[2];                     /* gain of pitch clipping memory */
    Word16 qua_gain[4];                    /* gain quantizer memory */

    Word16 old_T0_med;
    Word16 ol_gain;
    Word16 ada_w;
    Word16 ol_wght_flg;
    Word16 old_ol_lag[5];
    Word16 hp_wsp_mem[9];
    Word16 old_hp_wsp[G7222_L_FRAME / G7222_OPL_DECIM + (G7222_PIT_MAX / G7222_OPL_DECIM)];

    Word16 first_frame;

    Word16 isfold[G7222_M];                      /* old isf (frequency domain) */
    Word32 L_gc_thres;                     /* threshold for noise enhancer */
    Word16 mem_syn_hi[G7222_M];                  /* modified synthesis memory (MSB) */
    Word16 mem_syn_lo[G7222_M];                  /* modified synthesis memory (LSB) */
    Word16 mem_deemph;                     /* speech deemph filter memory */
    Word16 mem_sig_out[6];                 /* hp50 filter memory for synthesis */
    Word16 mem_hp400[6];                   /* hp400 filter memory for synthesis */
    Word16 mem_oversamp[2 * G7222_L_FILT];       /* synthesis oversampled filter memory */
    Word16 mem_syn_hf[G7222_M];                  /* HF synthesis memory */
    Word16 mem_hf[2 * G7222_L_FILT16k];          /* HF band-pass filter memory */
    Word16 mem_hf2[2 * G7222_L_FILT16k];         /* HF band-pass filter memory */
    Word16 seed2;                          /* random memory for HF generation */
    Word16 vad_hist;

    Word16 gain_alpha;

    //VAD state
    VadVars vadSt0;

    //DTX encoder state
    dtx_encState dtx_encSt0;

    //Tx serial state
    Word16 sid_update_counter;
    Word16 sid_handover_debt;
    Word16 prev_ft;

    //Encoder options
    //Selected rate (see G7222_RATE_x)
    Word16 rate;

    //Options (see G7222_OPTS_x)
    Word16 opts;
} G7222EncState;



/* G.722.2 decoder state */
typedef struct tag_G7222DecState
{
    Word16 old_exc[G7222_PIT_MAX + G7222_L_INTERPOL];  /* old excitation vector */
    Word16 ispold[G7222_M];                      /* old isp (immittance spectral pairs) */
    Word16 isfold[G7222_M];                      /* old isf (frequency domain) */
    Word16 isf_buf[G7222_L_MEANBUF * G7222_M];   /* isf buffer(frequency domain) */
    Word16 past_isfq[G7222_M];                   /* past isf quantizer */
    Word16 tilt_code;                      /* tilt of code */
    Word16 Q_old;                          /* old scaling factor */
    Word16 Qsubfr[4];                      /* old maximum scaling factor */
    Word32 L_gc_thres;                     /* threshold for noise enhancer */
    Word16 mem_syn_hi[G7222_M];            /* modified synthesis memory (MSB) */
    Word16 mem_syn_lo[G7222_M];            /* modified synthesis memory (LSB) */
    Word16 mem_deemph;                     /* speech deemph filter memory */
    Word16 mem_sig_out[6];                 /* hp50 filter memory for synthesis */
    Word16 mem_oversamp[2 * G7222_L_FILT]; /* synthesis oversampled filter memory */
    Word16 mem_syn_hf[G7222_M16k];         /* HF synthesis memory */
    Word16 mem_hf[2 * G7222_L_FILT16k];    /* HF band-pass filter memory */
    Word16 mem_hf2[2 *G7222_L_FILT16k];    /* HF band-pass filter memory */
    Word16 mem_hf3[2 *G7222_L_FILT16k];    /* HF band-pass filter memory */
    Word16 seed;                           /* random memory for frame erasure */
    Word16 seed2;                          /* random memory for HF generation */
    Word16 old_T0;                         /* old pitch lag */
    Word16 old_T0_frac;                    /* old pitch fraction lag */
    Word16 lag_hist[5];
    Word16 dec_gain[23];                   /* gain decoder memory */
    Word16 seed3;                          /* random memory for lag concealment */
    Word16 disp_mem[8];                    /* phase dispersion memory */
    Word16 mem_hp400[6];                   /* hp400 filter memory for synthesis */

    Word16 prev_bfi;
    Word16 state;
    Word16 first_frame;
    Word16 vad_hist;

    //DTX decoder state
    dtx_decState dtx_decSt0;

    //Rx serial state
    Word16 prev_frame_type;
    Word16 prev_mode;

    //Other variables
    Word16 prev_frame_rate;
    Word16 reset_flag;
    Word16 prev_reset_flag;
} G7222DecState;



/* Encoder initialization */
void InitializeG7222Encoder(G7222EncState *enc);

/* Decoder initialization */
void InitializeG7222Decoder(G7222DecState *dec);

/* Encode frame */
Word32 EncodeG7222(G7222EncState *enc,
                   const Word16 *smpBuf, Word32 length,
                   UWord8 *encBuf);

/* Decode frame */
Word32 DecodeG7222(G7222DecState *dec,
                   const UWord8 *encBuf, Word32 length,
                   Word16 *smpBuf, Word32 smpLength);

/* Get size of packed frame, excluding TOC byte */
Word32 GetG7222FrameSize(UWord8 *encBuf);

#endif /* !__G7222_H */

/*===========================================================================
 * End of file g7222.h
 *===========================================================================*/
