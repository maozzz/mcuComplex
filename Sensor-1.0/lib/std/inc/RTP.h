/*===========================================================================
 *
 *  Standard library
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
 *  File RTP.h: RTP packet definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __RTP_H
#define __RTP_H

#include "../../bastypes.h"
#include "bit-op.h"

//Structure definitions
//RTP packet header length, bytes
#define RTP_HEADER_LENGTH       12
//RTP.CC is 4bits length =  15 CSRCs, 32bit words
#define RTP_CSRC_MAXLENGTH      15
//Extension header length, 32bit words (extID:2 + extLength:2)
#define RTP_EXTHEADER_LENGTH    1
//Extension data maximum length, 32bit words
#define RTP_EXT_MAXLENGTH       4

//Maximum RTP header length: header + CSRCs + extID-length + extdata + payload
//Note: RTP_CSRC_MAXLENGTH, RTP_EXT_MAXLENGTH at 32 bit words
#define RTP_HEADER_MAXLENGTH (RTP_HEADER_LENGTH +\
                             (RTP_CSRC_MAXLENGTH << 2) +\
                            ((RTP_EXTHEADER_LENGTH + RTP_EXT_MAXLENGTH) << 2))

//Payload types (PT) for audio codecs
//according RFC 3551 "RTP A/V Profile"
#define RTP_PT_G711U                    0  //G.711 u-Law (PCMU)
#define RTP_PT_G726r32                  2  //G.726 32 kbit/s
#define RTP_PT_GSM                      3  //GSM 06.10 (ETS 300 961)
#define RTP_PT_G7231                    4  //G.723.1
#define RTP_PT_DVI4_8KHz                5  //DVI4 8000Hz
#define RTP_PT_DVI4_16KHz               6  //DVI4 16000Hz
#define RTP_PT_G711A                    8  //G.711 a-Law (PCMA)
#define RTP_PT_G7222                    9  //G.722.2
#define RTP_PT_DVI4_11KHz               16 //DVI4 11025Hz
#define RTP_PT_DVI4_22KHz               17 //DVI4 22050Hz
#define RTP_PT_G729A                    18 //G.729a

//Next payloads has dynamic PT (dynamic PT at [95..127]).
//Dynamic payload start with this number
#define RTP_PT_DYNSTART                 95
//Count of dynamic payload numbers
#define RTP_PT_DYNCOUNT                (128 - RTP_PT_DYNSTART)

//Define here default dynamic values
#define RTP_PT_G726r40                  96 //G.726 40 kbit/s
#define RTP_PT_G726r24                  97 //G.726 24 kbit/s
#define RTP_PT_G726r16                  98 //G.726 16 kbit/s
#define RTP_PT_G729B                    99 //G.729b
#define RTP_PT_TELEVENT                101 //Dual-Tone Multi-Frequency
#define RTP_PT_VDVI                    103 //VDVI
#define RTP_PT_ILBC                    104 //iLBC
#define RTP_PT_SPEEX                   105 //speex


//Standard RTP header (First 12 bytes of RTP packet)
typedef struct tag_RTPHeader
{
                       //Header bytes:
                       //[0]:
        u16     CC:4;  //CSRC count, 32bit words
        u16     X:1;   //Extension flag (X)
        u16     P:1;   //Padding flag   (P)
        u16     V:2;   //Version        (V=2)
                       //[1]:
        u16     PT:7;  //Payload type   (PT)
        u16     M:1;   //Marker bit     (M)

        u16     SN;    //[2..3] : Sequense number (SN)
        u32     TM;    //[4..7] : Timestamp (TM)
        u32     SSRC;  //[8..11]: Synchronyzation source (SSRC)
} RTPHeader;



//RTP header with extension
typedef struct tag_RTPPacket
{
        RTPHeader hdr;

        u32     CSRC[RTP_CSRC_MAXLENGTH]; //Contrib.sources (CSRC)
                                          //CSRC length at RtpPacket::CC

        u32     ext[RTP_EXT_MAXLENGTH];   //Extension data
        u16     extID;                    //Extension id (defined by profile)
        u16     extLength;                //Extension length, 32bit words
} RTPPacket;

/* Initialize specified RTP header with default values */
RTPHeader *InitRTPHeader(RTPHeader *hdr, u8 pt);

/* Initialize specified RTP packet with default values */
RTPPacket *InitRTPPacket(RTPPacket *rtp, u8 pt);

/* Converts data array to rtp packet structure.
   data: pointer to buffer that contains rtp packet;
   length: buffer length;
   eturn: pointer to structure or 0. */
RTPPacket *DataToRTPPacket(const u8 *data, s32 length,
                           RTPPacket *rtp, s32 *pldOffset);

/* Converts rtp packet structure to byte array.
   rtp: pointer to rtp structure;
   return: count of writed bytes. */
u32 RTPPacketToData(RTPPacket *rtp, u8 *data);

/* Converts rtp header structure to byte array.
   header: pointer to rtp header;
   return: count of writed bytes. */
u32 RTPHeaderToData(RTPHeader *header, u8 *data);

#endif /* __RTP_H */

/*===========================================================================
 * End of file RTP.h
 *===========================================================================*/

