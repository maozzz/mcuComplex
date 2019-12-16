/*===========================================================================
 *
 *  Network library
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2014-2018
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File TCP.h: Transmition Control Protocol (RFC 793) definitions
 *		        and function prototypes.
 *
 *===========================================================================*/

#ifndef __TCP_H
#define __TCP_H

#include "../../bastypes.h"
#include "IPv4.h"

//TCP header minimum length (without options), bytes
#define TCP_HEADER_LENGTH               20

//TCP header maximum length, bytes
//(dataOffset field 4bits: 15 32-bit words = 60 bytes)
#define TCP_HEADER_MAXLENGTH            60

//TCP pseudo header length (checksum calculation only), bytes
#define TCP_PSHEADER_LENGTH             12

//TCP header flags (TCPHdr::flags)
#define TCP_HFLAG_FIN                   0x0001
#define TCP_HFLAG_SYN                   0x0002
#define TCP_HFLAG_RST                   0x0004
#define TCP_HFLAG_PSH                   0x0008
#define TCP_HFLAG_ACK                   0x0010
#define TCP_HFLAG_URG                   0x0020
#define TCP_HFLAG_ECN                   0x0040
#define TCP_HFLAG_CWR                   0x0080

//TCP header flags (TCPHdr::flags2)
#define TCP_HFLAG_NONCE                 0x0001

/* Represents TCP header */    
typedef struct tag_TCPHdr
{
	//Source port
	u16 srcPort;

	//Destination port
	u16 destPort;

	//Sequence Number
	u32 seqNumber;

	//Acknowledgment Number
	u32 ackNumber;


    //Flags (see TCP_HFLAG_NONCE)
    u8 flags2:1;

    //Reserved
    u8 reserved:3;

    //Data offset (header length), 32-bit words
    u8 dataOffset:4;


    //Flags (see TCP_HFLAG_x)
    u8 flags;


	//Window, bytes
	u16 window;

	//Checksum
	u16 checkSum;

	//Urgent pointer
	u16 urgPtr;
} TCPHdr;



/* Calculate TCP segment checksum */
u16 TCPCheckSum(u32 srcIP, u32 destIP,
                const u8 *tcpHdr,
                const u8 *tcpPld, s32 length);

#endif /* __TCP_H */

/*===========================================================================
 * End of file TCP.h
 *===========================================================================*/
