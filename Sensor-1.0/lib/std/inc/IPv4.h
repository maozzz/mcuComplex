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
 *  File IPv4.h: Internet Protocol version 4 (RFC 791),
 *               DSCP, ECN fields (RFCs 2474, 3168)
 *                               definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __IPV4_H
#define __IPV4_H

#include "../../bastypes.h"

//IPv4 header minimum length, bytes
#define IPV4_HEADER_LENGTH              20

//IPv4 header maximum length, bytes
//(IHL field 4bits: 15 32-bit words = 60 bytes)
#define IPV4_HEADER_MAXLENGTH           60

//Current IP protocol version
#define IPV4_VERSION                    4

//Protocol numbers (IPv4Hdr::protocol):
#define IPV4_PROTOCOL_ICMP              1
#define IPV4_PROTOCOL_IGMP              2
#define IPV4_PROTOCOL_TCP               6
#define IPV4_PROTOCOL_UDP               17

/*===========================================================================
 * IPv4 "Type of Service" field (TOS, RFC 791, p 3.1),
 * superceded by DSCP (RFC 2474):
 *   7 6 5 4 3 2 1 0
 *  |PREC |D|T|R|M|x|
 *===========================================================================*/

//TOS[7..5] Precedence field (RFC 791):
//Network Control
#define IPV4_PREC_NETCONTROL            0x07
//Internetwork Control
#define IPV4_PREC_INETCONTROL           0x06
//CRITIC/ECP
#define IPV4_PREC_CRITIC                0x05
//Flash Override
#define IPV4_PREC_FLASHOVERRIDE         0x04
//Flash
#define IPV4_PREC_FLASH                 0x03
//Immediate
#define IPV4_PREC_IMMEDIATE             0x02
//Priority
#define IPV4_PREC_PRIORITY              0x01
//Routine
#define IPV4_PREC_ROUTINE               0x00

//TOS[4] D: 0/1 - normal/low  delay
#define IPV4_LOW_DELAY                  0x10
//TOS[3] T: 0/1 - normal/high throughput
#define IPV4_HIGH_THROUGHPUT            0x08
//TOS[2] R: 0/1 - normal/high relibility
#define IPV4_HIGH_RELIBILITY            0x04
//TOS[1] M: 0/1 - normal/minimize monetary cost.
#define IPV4_MIN_MONETARY				0x02

/*===========================================================================
 * IPv4 Differentiated Services Code Point (DSCP) and
 *  Explicit Congestion Notification fields (ECN) (RFC 2474, 3168),
 *  supercedes TOS (RFC 791):
 *   7 6 5 4 3 2 1 0
 *  |    DSCP   |ECN|
 *===========================================================================*/

//Commonly defined Per-Hop Behaviors (PHB):
//Default Forwarding (DF): best-effort traffic
#define IPV4_PHB_DF                     0x00

//Expedited Forwarding (EF, RFC 3246): low-loss, low-latency traffic
#define IPV4_PHB_EF                     0x2E //101110b - recommended value

//Voice Admit (VA, RFC 5865): identical EF but also admitted using a CAC
#define IPV4_PHB_VA                     0x2C //101100b

//Assured Forwarding (AF, RFCs 2597, 3260): assurance of delivery
//AF class 1 (AFx{1,2,3}: low, medium, high drop precedence)
#define IPV4_PHB_AF11           0x0A //001010b
#define IPV4_PHB_AF12           0x0C //001100b
#define IPV4_PHB_AF13           0x0E //001110b
//AF class 2
#define IPV4_PHB_AF21           0x12 //010010b
#define IPV4_PHB_AF22           0x14 //010100b
#define IPV4_PHB_AF23           0x16 //010110b
//AF class 3
#define IPV4_PHB_AF31           0x1A //011010b
#define IPV4_PHB_AF32           0x1C //011100b
#define IPV4_PHB_AF33           0x1E //011110b
//AF class 4
#define IPV4_PHB_AF41           0x22 //100010b
#define IPV4_PHB_AF42           0x24 //100100b
#define IPV4_PHB_AF43           0x26 //100110b

//Class Selector (CS): compatibility with the IP Precedence field.
#define IPV4_PHB_CS7            0x07
#define IPV4_PHB_CS6            0x06
#define IPV4_PHB_CS5            0x05
#define IPV4_PHB_CS4            0x04
#define IPV4_PHB_CS3            0x03
#define IPV4_PHB_CS2            0x02
#define IPV4_PHB_CS1            0x01
#define IPV4_PHB_CS0            0x00

//Not-ECT: disable ECN
#define IPV4_ECN_NOTECT         0x00
//ECT(1): enable ECN, set by sender
#define IPV4_ECN_ECT1           0x01
//ECT(0): enable ECN, set by sender
#define IPV4_ECN_ECT0           0x02
//CE: congestion exist, set by router
#define IPV4_ECN_CE             0x03

/*===========================================================================
 * IPv4 packet options
 *  7 6 5 4 3 2 1 0
 * |C|Cls| Option  |
 *===========================================================================*/

//Options
//Copy flag
#define IPV4_OPTS_COPY_FLAG			0x80

//Control options (class 0)
//End of options list (RFC 791)
#define IPV4_OPTS_END_LIST			0
//NOP (RFC 791)
#define IPV4_OPTS_NOP				1
//Security (RFCs 791, 1108)
#define IPV4_OPTS_SECURITY			130
//Loose Source Route (RFC 791)
#define IPV4_OPTS_LOOSE_SRC_ROUTE	131
//Extended Security (RFC 1108)
#define IPV4_OPTS_EXT_SECURITY		133
//Commercial Security
#define IPV4_OPTS_COM_SECURITY		134
//Record Route (RFC 791)
#define IPV4_OPTS_RECORD_ROUTE		7
//Stream Identifier (RFCs 791, 1122)
#define IPV4_OPTS_STREAM_ID			136
//Strict Source Route (RFC 791)
#define IPV4_OPTS_STRICT_SRC_ROUTE	137
//Encode
#define IPV4_OPTS_ENCODE			15
//IMI Traffic Descriptor
#define IPV4_OPTS_IMI_TRAFFIC		144
//Extended Internet Protocol (RFC 1385)
#define IPV4_OPTS_EXT_IP			145
//Address Extension (RFC 1475)
#define IPV4_OPTS_EXT_ADDRESS		147
//Router Alert (RFC 2113)
#define IPV4_OPTS_ROUTER_ALERT		148
//Selective Directed Broadcast Mode (RFC 1770)
#define IPV4_OPTS_DIR_BCAST_MODE	149
//Dynamic Packet State
#define IPV4_OPTS_DYN_STATE			151
//Upstream Multicast Packet
#define IPV4_OPTS_UPS_MCAST			152
//Quick-Start (QS, RFC 4782)
#define IPV4_OPTS_QUICK_START		25

//Debugging and measurement options (class 2)
//Timestamp (RFCs 781, 791)
#define IPV4_OPTS_TIMESTAMP			68
//Traceroute (RFC 1393)
#define IPV4_OPTS_TRACEROUTE		82

/* Router Alert (RFC 2113) */
extern const u8 ipv4Opts_RouterAlert[];


/* Represents IPv4 header */
typedef struct tag_IPv4Hdr
{
        //Internet Header Length, 32-bit words
        u8 ihl:4;

        //Version: 4 for IPv4
        u8 version:4;

        //ECN
        u8 ecn:2;

        //DSCP (TOS)
        u8 dscp:6;

        //Total length (header + payload), bytes
        u16 length;

        //Fragment ID
        u16 fragId;

        //Fragment offset (high 5 bits)
        u8 fragOffsetH:5;

        //More fragments
        u8 MF:1;

        //Don't fragment
        u8 DF:1;

        //Reserved bit
        u8 R:1;

        //Fragment offset (low 8 bits)
        u8 fragOffsetL;

        //Time to live
        u8 ttl;

        //Encapsulated protocol
        u8 protocol;

        //Header checksum
        u16 checksum;

        //Source IP address
        u32 srcIP;

        //Destination IP address
        u32 destIP;

		//Options
		u8  opts[40];		
} IPv4Hdr;

/*===========================================================================
 * IP address classes:
 * cl     dec       hex      mask
 * A: [0  ..127] [00..7F] 255.0.0.0
 * B: [128..191] [80..BF] 255.255.0.0
 * C: [192..223] [C0..DF] 255.255.255.0
 * D: [224..239] [E0..EF] multicast
 * E: [240..255] [F0..FF] reserved
 *
 * Multicast addresses assignments appear as follows [IANA] (RFC 3171):
 * 224.0.0.0   - 224.0.0.255 (224.0.0/24) Local Network Control Block
 * 224.0.1.0   - 224.0.1.255 (224.0.1/24) Internetwork Control Block
 * 224.0.2.0   - 224.0.255.0 AD-HOC Block
 * 224.1.0.0   - 224.1.255.255 (224.1/16) ST Multicast Groups
 * 224.2.0.0   - 224.2.255.255 (224.2/16) SDP/SAP Block
 * 224.252.0.0 - 224.255.255.255 DIS Transient Block
 * 225.0.0.0   - 231.255.255.255 RESERVED
 * 232.0.0.0   - 232.255.255.255 (232/8) Source Specific Multicast Block
 * 233.0.0.0   - 233.255.255.255 (233/8) GLOP Block
 * 234.0.0.0   - 238.255.255.255 RESERVED
 * 239.0.0.0   - 239.255.255.255 (239/8) Administratively Scoped Block
 *===========================================================================*/

#ifndef AF_INET
//Address family: Internet Protocol
#define AF_INET                 2
#endif /* !AF_INET */

#ifndef INADDR_ANY
//Any address: 0.0.0.0
#define INADDR_ANY              (u32)0
#endif /* !INADDR_ANY */

#ifndef INADDR_BROADCAST
//Broadcast address: 255.255.255.255
#define INADDR_BROADCAST		(u32)0xFFFFFFFF
#endif /* !INADDR_BROADCAST */

//Loopback address: 127.0.0.1
#define INADDR_LOOPBACKN		(u32)0x0100007F

//Loopback network mask: 8 bit
#define MASK_LOOPBACKN			(u32)0x000000FF

//Start address of multicast block
#define INADDR_MULTICASTN       (u32)0x000000E0

//Multicast mask
#define MASK_MULTICASTN         (u32)0x000000F0

/* Represents network end point.
   Address and port number must be at network byte order */
typedef struct tag_NetEndPoint
{
        //IPv4 address
        u32 addr;

        //Port number
        u16 port;

        //Address family: AF_INET
        u16 afam;
} NetEndPoint;

/* Indicates that ip address is multicast */
#define IsIPMCast(ip)				(((ip) & MASK_MULTICASTN) == INADDR_MULTICASTN)

/* Indicates that ip address is broadcast */
#define IsIPBCast(ip, mask)			(((ip) | (mask)) == (u32)0xFFFFFFFF)
//#define IsIPBCast(ip, mask)     (((ip) & ~(mask)) == ~(mask))

/* Indicates that ip1 and ip2 address at same subnet */
#define IsIPSubnet(ip1, ip2, mask)	(((ip1) & (mask)) == ((ip2) & (mask)))

/* Solve internet checksum */
u16 InetCheckSum(const u8 *buf, s32 length);

/* Solve checksum for IPv4 header */
u16 IPV4CheckSum(const u8 *hdrBuf, s32 length);

/* Solve checksum for IGMP message */
u16 IGMPCheckSum(const u8 *igmpBuf, s32 length);

/* Solve checksum for ICMP message */
u16 ICMPCheckSum(const u8 *icmpBuf, s32 length);

/* Convert IPv4 address to it string representation */
char *IPToStr(const u32 ip, char *str);

/* Convert IPv4 string at "a.b.c.d" format to integer value
   with network byte order */
u32 StrToIP(const char *str);

/* Indicating that specified string contains IPv4 address */
u32 IsIPAddress(const char *str);

/* Indicating that specified value contains valid mask
   (continuous binary 1s) */
u32 IsIPMaskValid(u32 mask);

#endif /* __IPV4_H */

/*===========================================================================
 * End of file IPv4.h
 *===========================================================================*/

