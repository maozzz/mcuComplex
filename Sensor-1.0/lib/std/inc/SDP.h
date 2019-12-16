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
 *  File SDP.h: SDP packet (RFC 4566) definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __SDP_H
#define __SDP_H

#include "../../bastypes.h"
#include "ParseUtils.h"
#include "IPv4.h"

#include <stdlib.h>  
#include <stdio.h>   
#include <string.h>  

/*===========================================================================
 * SDP packet definitions
 *===========================================================================*/

//SDP headers
#define SDP_HDR_V                               "v\0"
#define SDP_HDR_O                               "o\0"
#define SDP_HDR_S                               "s\0"
#define SDP_HDR_C                               "c\0"
#define SDP_HDR_T                               "t\0"
#define SDP_HDR_M                               "m\0"
#define SDP_HDR_A                               "a\0"

//List of sttributes names (a=<attrname>)
#define SDP_ATTRNAME_RTPMAP                     "rtpmap\0"
#define SDP_ATTRNAME_FMTP                       "fmtp\0"
#define SDP_ATTRNAME_SSRC                       "ssrc\0"
#define SDP_ATTRNAME_PTIME                      "ptime\0"
#define SDP_ATTRNAME_MAXPTIME                   "maxptime\0"

//Session media stream direction
#define SDP_ATTRNAME_INACTIVE                   "inactive\0"
#define SDP_ATTRNAME_RECVONLY                   "recvonly\0"
#define SDP_ATTRNAME_SENDONLY                   "sendonly\0"
#define SDP_ATTRNAME_SENDRECV                   "sendrecv\0"

//SDPRequest::streamdir values
#define SDP_STREAMDIR_INACTIVE					0
#define SDP_STREAMDIR_RECVONLY					1
#define SDP_STREAMDIR_SENDONLY					2
#define SDP_STREAMDIR_SENDRECV					3


//Maximum length of string buffer (common text value)
#define SDP_STR_MAXLENGTH                       96

//m=<media type> <port> <proto> <pt numbers>+
//Maximum count of 'm' headers at SDPPacket structure
#define SDP_HDR_M_COUNT				     		1

//Media type name maximum length
#define SDP_MEDIA_NAME_MAXLENGTH                16

//Media type names
#define SDP_MEDIA_AUDIO							"audio\0"
#define SDP_MEDIA_VIDEO							"video\0"

//Format of media packet
#define SDP_PROTONAME_MAXLENGTH					16
#define SDP_PROTONAME_RTPAVP                    "RTP/AVP\0"

//Maximum count of pt numbers
#define SDP_MEDIA_PTNUMS_MAXLENGTH	     		12


//a=rtpmap:<pt number> <codec name>/<rate>
//Maximum count of 'a=rtpmap' attributes at SDPPacket structure
#define SDP_RTPMAPS_MAXLENGTH					(SDP_MEDIA_PTNUMS_MAXLENGTH)

//Maximum length of encoding name
#define SDP_RTPMAP_ENCNAME_MAXLENGTH            20

//Encoding names (see pt numbers at "RTP.h")
#define SDP_ENCNAME_G711U						"PCMU"
#define SDP_ENCNAME_GSM							"GSM"
#define SDP_ENCNAME_G7231						"G723"
#define SDP_ENCNAME_G711A						"PCMA"
#define SDP_ENCNAME_DVI4						"DVI4"
#define SDP_ENCNAME_G7222						"G722"
#define SDP_ENCNAME_G729A						"G729a"
#define SDP_ENCNAME_G726r40						"G726-40"
#define SDP_ENCNAME_G726r32						"G726-32"
#define SDP_ENCNAME_G726r24						"G726-24"
#define SDP_ENCNAME_G726r16						"G726-16"
#define SDP_ENCNAME_TELEVENT					"telephone-event"
#define SDP_ENCNAME_VDVI						"VDVI"
#define SDP_ENCNAME_ILBC						"iLBC"
#define SDP_ENCNAME_SPEEX						"speex"


//a=fmtp:{pt number} {format string}
//Maximum count of 'a=fmtp' attributes at SDPPacket structure
#define SDP_FMTPS_MAXLENGTH						(SDP_RTPMAPS_MAXLENGTH)
//Maximum length of param string
#define SDP_FMTP_PARSTR_MAXLENGTH				28

//o={creator} {ses-id} {ses-version} {nettype} {addrtype} {unicast-address}
//c={nettype} {addrtype} {connection-address}
#define SDP_CREATOR_MAXLENGTH                   32
#define SDP_SESID_MAXLENGTH                     16
#define SDP_SESVER_MAXLENGTH                    16

#define SDP_NETTYPE_MAXLENGTH                   8
#define SDP_NETADDRTYPE_MAXLENGTH               8
#define SDP_NETADDR_MAXLENGTH					44

//SDP protocol version 0
#define SDP_PROTOVER                            0       
//Network type: internet
#define SDP_NETTYPE_IN                          "IN\0"  
//Address: IPv4
#define SDP_ADDRTYPE_IP4                        "IP4\0" 
//Address: IPv6
#define SDP_ADDRTYPE_IP6                        "IP6\0" 

/*===========================================================================
 * String templates
 *===========================================================================*/

//Header "v": "v={sdp version}CRLF"
extern cpcstr_t SDP_TEMPL_V;

//Header "o": "a={creator} {id} {version} {net.type} {addr.type} {host}CRLF"
extern cpcstr_t SDP_TEMPL_O;

//Header "s": "s={description string}CRLF"
extern cpcstr_t SDP_TEMPL_S;

//Header "c": "c={net.type} {addr.type} {address}CRLF"
extern cpcstr_t SDP_TEMPL_C;

//Header "t": "t={start time} {stop time}CRLF"
extern cpcstr_t SDP_TEMPL_T;

//Header "m": "m={media type} {port number} {protocol} "
extern cpcstr_t SDP_TEMPL_M;

//Header "a": "a={string value}CRLF"
extern cpcstr_t SDP_TEMPL_A;

//Header "a=rtpmap": "a=rtpmap:{pt number} {codec name}/{rate}CRLF"
extern cpcstr_t SDP_TEMPL_A_RTPMAP;

//Header "a=fmtp": "a=fmtp:{pt number} {format string}CRLF"
extern cpcstr_t SDP_TEMPL_A_FMTP;

//Header "a=ssrc": "a=ssrc:{ssrc value}CRLF"
extern cpcstr_t SDP_TEMPL_A_SSRC;

//Header "a=ptime": "a=ptime:{ms value}CRLF"
extern cpcstr_t SDP_TEMPL_A_PTIME;

//Header "a=maxptime": "a=maxptime:{ms value}CRLF"
extern cpcstr_t SDP_TEMPL_A_MAXPTIME;

//Header "a={stream direction}": "a={inactive|sendonly|recvonly|sendrecv}CRLF"
extern cpcstr_t SDP_TEMPL_A_STREAMDIR;

/*===========================================================================
 * SDP packet field types
 *===========================================================================*/

//"v" header: protocol version
typedef u8     version_t;


//"o" header: session originator
typedef struct tag_origin_t
{
		//Creator of session (username)
        char creator[SDP_CREATOR_MAXLENGTH];    

		//Network type (IN)
        char nettype[SDP_NETTYPE_MAXLENGTH];      

		//Address type ("IP4", "IP6")
        char addrtype[SDP_NETADDRTYPE_MAXLENGTH]; 

		//IPv4/IPv6 address or DNS name
        char addr[SDP_NETADDR_MAXLENGTH];

		//Session ID
        u64 sesId;          

		//Session version
        u64 sesVer;        		            

} origin_t;


//"s" header: session name
typedef struct tag_sesname_t
{
        char name[SDP_STR_MAXLENGTH];
} sesname_t;


//"i" header: session information
typedef struct tag_sesinfo_t
{
        char name[SDP_STR_MAXLENGTH];
} sesinfo_t;


//"c" header: connection data
typedef struct tag_conndata_t
{
		//Network type (IN)
        char nettype[SDP_NETTYPE_MAXLENGTH];      

		//Address type (IPv4, IPv6)
        char addrtype[SDP_NETADDRTYPE_MAXLENGTH];    

		//IPv4/IPv6 unicast or multicast address
        char addr[SDP_NETADDR_MAXLENGTH];            

		//Multicast TTL
        u8 ttl;               

		//Number of addressess
        u8 nofaddr;  

		//Reserved
        u16 reserved;
} conndata_t;


//"t" header: timing
//start, stop fields is
//decimal representation of NTP time values in seconds since 1900.
//To convert these values to UNIX time, subtract decimal 2208988800.
typedef struct tag_timing_t
{
		//Start time
        u64 start;                      

		//Stop time
        u64 stop;                       
} timing_t;


//"m" header: media description
typedef struct tag_media_t
{
		//Type of media (see SDP_MEDIA_x)
        char mtype[SDP_MEDIA_NAME_MAXLENGTH];

		//Media packet format (see SDP_PROTONAME_x)
        char proto[SDP_PROTONAME_MAXLENGTH];

		//PT numbers array
        u8 ptnums[SDP_MEDIA_PTNUMS_MAXLENGTH];

		//Count of parsed PT numbers
		u16 ptnumCount;

		//Port number
        u16 port;		

} media_t;


//"a=rtpmap" attribute
typedef struct tag_sdpattr_rtpmap_t
{
		//Encoding name (see SDP_ENCNAME_x)
        char encName[SDP_RTPMAP_ENCNAME_MAXLENGTH];

		//RTP payload type number 
		//([0..127], see RTP_PT_x at "RTP.h")
        u16 ptnum;                		         

		//Clock rate, Hz (8000/16000)
        u16 rate;                 
} sdpattr_rtpmap_t;


//"a=fmtp" attribute
typedef struct tag_sdpattr_fmtp_t
{
		//RTP payload type number 
		//([0..127], see RTP_PT_x at "RTP.h")
        u8 ptnum;           

		//Parameters string
        char parstr[SDP_FMTP_PARSTR_MAXLENGTH];   
} sdpattr_fmtp_t;


//"a=ssrc" attribute
typedef u32		sdpattr_ssrc_t;


//"a=ptime" attribute
typedef u16		sdpattr_ptime_t;


//"a=maxptime" attribute
typedef u16		sdpattr_maxptime_t;

/*===========================================================================
 * SDP packet structure
 *===========================================================================*/

//Represents SDP packet
typedef struct tag_SDPPacket
{
		//"v": protocol version (must v=0)
        version_t               v; 
		
		//"o": session originator
        origin_t                o; 
		
		//"s": session name (description)
        sesname_t               s; 
		
		//"i": session information
        //sesinfo_t             i; 
		
		//"c": connection data
        conndata_t              c; 
		
		//"t": timing
        timing_t                t; 
		
		//"m": media description array
        media_t                 m[SDP_HDR_M_COUNT];         
		
		//"a=rtpmap": payload number mapping array
		sdpattr_rtpmap_t        rtpmaps[SDP_RTPMAPS_MAXLENGTH];
		
		//"a=fmtp": payload format array
		sdpattr_fmtp_t			fmtps[SDP_FMTPS_MAXLENGTH];
		
		//"a=ssrc": synchronization source number
		sdpattr_ssrc_t			ssrc;

		//"a=ptime": packet time, ms
		sdpattr_ptime_t			ptime;
		
		//"a=maxptime": maximum packet time, ms
		sdpattr_maxptime_t		maxptime;

		//Media stream direction 
		//(sendrecv, sendonly, recvonly, inactive: see SDP_STREAMDIR_x)
		u8						streamdir;

		//Count of parsed "m" headers
        u8                      mCount;

		//Count of parsed "a=rtpmap" attributes
        u8                      rtpmapCount;

		//Count of parsed "a=fmtp" attributes
        u8                      fmtpCount;
} SDPPacket;


/*===========================================================================
 * Parse functions
 *===========================================================================*/

/* Parse SDP packet */
PARSLT SDPParse(const char *buffer, u32 length, 
				u32 *parseOffset, SDPPacket *sdp);

/* Fill SDP packet fields with default values. */
void ClearSDPPacket(SDPPacket *sdp);

/*===========================================================================
 * Construct functions
 *===========================================================================*/

/* Fill "v" header with specified version number */
void SDPHdr_V_Fill(char *strBuf, u8 version);

/* Convert version structure to "v" header string representation */
void SDPHdr_V_ToString(char *strBuf, version_t *v);

/* Fill "o" header with specified parameters */
void SDPHdr_O_Fill(char *strBuf, const char *creator,
				   u64 sesId, u64 sesVer,
				   const char *netType, const char *addrType,
				   const char *host);

/* Convert origin structure to "o" header string representation */
void SDPHdr_O_ToString(char *strBuf, origin_t *o);

/* Fill "s" header with specified session name */
void SDPHdr_S_Fill(char *strBuf, const char *sesname);

/* Convert sesname structure to "s" header string representation */
void SDPHdr_S_ToString(char *strBuf, sesname_t *s);

/* Fill "c" header with specified parameters */
void SDPHdr_C_Fill(char *strBuf, 
				   const char *nettype, 
				   const char *addrtype, 
				   const char *addr);

/* Convert connection structure to "c" header string representation */
void SDPHdr_C_ToString(char *strBuf, conndata_t *c);

/* Fill "t" header with specified parameters */
void SDPHdr_T_Fill(char *strBuf, u64 start, u64 stop);

/* Convert timing structure to "t" header string representation */
void SDPHdr_T_ToString(char *strBuf, timing_t *t);

/* Fill "m" header with specified parameters */
void SDPHdr_M_Fill(char *strBuf, 
				   const char *mtype, u16 port, const char *proto, 
				   const u8 *ptnums, u16 ptnumCount);

/* Convert media info structure to "m" header string representation */
void SDPHdr_M_ToString(char *strBuf, media_t *m);

/* Fill "a=rtpmap" attribute with specified parameters */
void SDPHdr_A_rtpmap_Fill(char *strBuf, 
						  u8 ptnum, const char *encName, u16 rate);

/* Convert array of "a=rtpmap" attributes to string representation */
void SDPHdr_A_rtpmap_ToString(char *strBuf, 
							  sdpattr_rtpmap_t *rtpmap, s32 count);

/* Fill "a=fmtp" attribute with specified parameters */
void SDPHdr_A_fmtp_Fill(char *strBuf, 
						u8 ptnum, const char *parstr);

/* Convert array of "a=fmtp" attributes to string representation */
void SDPHdr_A_fmtp_ToString(char *strBuf, 
							sdpattr_fmtp_t *fmtp, s32 count);

/* Fill "a=ssrc" attribute with specified parameters */
void SDPHdr_A_ssrc_Fill(char *strBuf, u32 ssrc);

/* Convert "a=ssrc" to string representation */
void SDPHdr_A_ssrc_ToString(char *strBuf, sdpattr_ssrc_t *ssrc);

/* Fill "a=ptime" attribute with specified parameters */
void SDPHdr_A_ptime_Fill(char *strBuf, u16 timems);

/* Convert "a=ptime" to string representation */
void SDPHdr_A_ptime_ToString(char *strBuf, sdpattr_ptime_t *ptime);

/* Fill "a=maxptime" attribute with specified parameters */
void SDPHdr_A_maxptime_Fill(char *strBuf, u16 timems);

/* Convert "a=maxptime" to string representation */
void SDPHdr_A_maxptime_ToString(char *strBuf, sdpattr_maxptime_t *maxptime);

/* Fill "a={stream direction}" attribute with specified parameters */
void SDPHdr_A_streamdir_Fill(char *strBuf, const char *strDir);

/* Convert stream direction from number to string representation */
void SDPHdr_A_streamdir_ToString(char *strBuf, u32 streamdir);

#endif /* __SDP_H */

/*===========================================================================
 * End of file SDP.h
 *===========================================================================*/

