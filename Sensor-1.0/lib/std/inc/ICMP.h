/*===========================================================================
 *
 *  Network library
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
 *  File ICMP.h: Internet Control Message Protocol definitions
 *               and function prototypes.
 *
 *===========================================================================*/

#ifndef __ICMP_H
#define __ICMP_H

#include "../../bastypes.h"
#include "IPv4.h"

/* Common ICMP definitions */
//ICMP packet minimum length, bytes
#define ICMP_PACKET_LENGTH			8 

//ICMP header data length, bytes
#define ICMP_HDRDATA_LENGTH			4 

//ICMP payload maximum length, bytes
#define ICMP_PAYLOAD_MAXLENGTH		(IPV4_HEADER_MAXLENGTH + 8)

//ICMP packet maximum length
#define ICMP_PACKET_MAXLENGTH		(ICMP_PACKET_LENGTH + ICMP_PAYLOAD_MAXLENGTH)


/* ICMP types */
#define ICMP_TYPE_ECHO_REPLY		0  //Echo reply (RFC 792)
/* [1..2] - unassigned */
#define ICMP_TYPE_DEST_UNREACH		3  //Destination Unreachable (RFC 792)
/* [4] - Source Quench (RFC 792, deprecated by RFC 6633) */
#define ICMP_TYPE_REDIRECT			5  //Redirect (RFC 792)
/* [6] - Alternate Host Address (deprecated by RFC 6918) */
/* [7] - unassigned */
#define ICMP_TYPE_ECHO_REQUEST		8  //Echo request (RFC 792)
#define ICMP_TYPE_ROUTER_ADVERT		9  //Router Advertisement (RFC 1256)
#define ICMP_TYPE_ROUTER_SOLICIT	10 //Router Solicitation (RFC 1256)
#define ICMP_TYPE_TIME_EXCEED		11 //Time Exceeded (RFC 792)
#define ICMP_TYPE_PARAM_PROBLEM		12 //Parameter Problem (RFC 792)
#define ICMP_TYPE_TIMESTAMP			13 //Timestamp (RFC 792)
#define ICMP_TYPE_TIMESTAMP_REPLY	14 //Timestamp Reply (RFC 792)
/* [15] - Information Request  (RFC 792, deprecated by RFC 6918) */
/* [16] - Information Reply    (RFC 792, deprecated by RFC 6918) */
/* [17] - Address Mask Request (RFC 950, deprecated by RFC 6918) */
/* [18] - Address Mask Reply   (RFC 950, deprecated by RFC 6918) */
/* [19] - reserved for Security [Solo] */
/* [20..29] - reserved (for Robustness Experiment) */
/* [30] - Traceroute (RFC 1393, deprecated by RFC 6918) */
/* [31] - Datagram Conversion Error (RFC 1475, deprecated by RFC 6918) */
/* [32] - Mobile Host Redirect (deprecated by RFC 6918) */
/* [33] - IPv6 Where-Are-You (deprecated by RFC 6918) */
/* [34] - IPv6 I-Am-Here     (deprecated by RFC 6918) */
/* [35] - Mobile Registration Request (deprecated by RFC 6918) */
/* [36] - Mobile Registration Reply   (deprecated by RFC 6918) */
/* [37] - Domain Name Request (RFC 1788, deprecated by RFC 6918) */
/* [38] - Domain Name Reply   (RFC 1788, deprecated by RFC 6918) */
/* [39] - SKIP (deprecated by RFC 6918) */
#define ICMP_TYPE_SECURITY_FAILURE		40 //Security failure (RFC 2521)
/* [41] - utilized by experimental mobility protocols (RFC 4065) */
/* [42 ..252] - unassigned */
/* [253..254] - RFC3692-style Experiment 1,2 (RFC4727) */
/* [255] - reserved (JBP) */

/* ICMP codes */
/* Type 3 (Destination unreachable): */
#define ICMP_CODE_NET_UNREACH		0  //Net Unreachable (RFC 792)
#define ICMP_CODE_HOST_UNREACH		1  //Host Unreachable (RFC 792)
#define ICMP_CODE_PROTO_UNREACH		2  //Protocol Unreachable (RFC 792)
#define ICMP_CODE_PORT_UNREACH		3  //Port Unreachable (RFC 792)
#define ICMP_CODE_FRAG_NEED_DF_SET	4  //Fragm. Needed but DF bit set (RFC 792)
#define ICMP_CODE_SRC_ROUTE_FAILED	5  //Source Route Failed (RFC 792)
#define ICMP_CODE_DEST_NET_UNKNOWN	6  //Destination Network Unknown (RFC 1122)
#define ICMP_CODE_DEST_HOST_UNKNOWN	7  //Destination Host Unknown (RFC 1122)
#define ICMP_CODE_SRC_HOST_ISOLATED	8  //Source Host Isolated (RFC 1122)
#define ICMP_CODE_DEST_NET_PROHIB	9  //Dest. Network is Admin. Prohibited (RFC 1122)
#define ICMP_CODE_DEST_HOST_PROHIB	10 //Dest. Host is Admin. Prohibited (RFC 1122)
#define ICMP_CODE_DEST_NET_UNR_TOS	11 //Destination Network Unreachable for TOS (RFC 1122)
#define ICMP_CODE_DEST_HOST_UNR_TOS	12 //Destination Host Unreachable for TOS (RFC 1122)
#define ICMP_CODE_COM_ADM_PROHIB	13 //Commun. Admin. Prohibited (RFC 1812)
#define ICMP_CODE_HOST_PRECED_VIOL	14 //Host Precedence Violation (RFC 1812)
#define ICMP_CODE_PRECED_CUTOFF_EFC	15 //Precedence cutoff in effect

/* Type 5 (Redirect) */
#define ICMP_CODE_DGRAM_NET			0 //Redirect Datagram for the Network (or subnet)	
#define ICMP_CODE_DGRAM_HOST		1 //Redirect Datagram for the Host	
#define ICMP_CODE_DGRAM_TOSNET		2 //Redirect Datagram for the TOS and Network	
#define ICMP_CODE_DGRAM_TOSHOST		3 //Redirect Datagram for the TOS and Host

/* Type 9 (Router Advertisement) */
#define ICMP_CODE_NORMAL			0  //Normal router advertisement (RFC 3344)
#define ICMP_CODE_NOT_ROUTE_TRAF	16 //Does not route common traffic (RFC 3344)

/* Type 11 (Time Exceeded) */
#define ICMP_CODE_EXCEED_TRANSIT	0 //Time to Live exceeded in Transit	
#define ICMP_CODE_FRAG_REASM_EXCEED	1 //Fragment Reassembly Time Exceeded

/* Type 12 (Parameter Problem) */
#define ICMP_CODE_POINTER_ERR		0 // Pointer indicates the error	
#define ICMP_CODE_MISS_OPTION		1 // Missing a Required Option (RFC 1108)
#define ICMP_CODE_BAD_LENGTH		2 // Bad Length

/* Type 40 (Security failure) */
#define ICMP_CODE_BAD_SPI			0 // Bad SPI	
#define ICMP_CODE_AUTH_FAIL			1 // Authentication Failed	
#define ICMP_CODE_DECOMP_FAIL		2 // Decompression Failed	
#define ICMP_CODE_DECRYPT_FAIL		3 // Decryption Failed	
#define ICMP_CODE_NEED_AUTHENT		4 // Need Authentication	
#define ICMP_CODE_NEED_AUTHORIZ		5 // Need Authorization

/* Represents ICMP header */
typedef struct tag_ICMPHdr
{
	u8 type;
	u8 code;
	u16 checkSum;
	u8 hdrData[ICMP_HDRDATA_LENGTH];
} ICMPHdr;

#endif /* __ICMP_H */

/*===========================================================================
 * End of file ICMP.h
 *===========================================================================*/