/*===========================================================================
 *
 *  TCP/IP protocols library
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
 *  File network.h. Network protocol stack definitions and
 *                  functions prototypes.
 *
 *===========================================================================*/

#ifndef __NETWORK_H
#define __NETWORK_H

#include <globaldef.h>
#include "../../bastypes.h"
#include "../../mc/inc/ETHIO.h"
#include "../../std/inc/Ethernet.h"
#include "../../std/inc/IPv4.h"
#include "../../std/inc/DNS.h"

/*===========================================================================
 * Network interface definitions
 *===========================================================================*/

//Maximum count of network interfaces including localhost
//(i.e. minimum value should 2: localhost and one real interface)
#ifndef NET_IFC_MAXCOUNT
#define NET_IFC_MAXCOUNT                        2
#endif /* !NET_IFC_MAXCOUNT */

//Local interface index (should be 0)
#define NET_IFC_LOCAL                           0

//All interfaces
#define NET_IFC_ALL								255

//Network interface flags (NetIfcConfig::flags)
//Interface disabled: can't receive/send any data
//Set this flag to disable interface
#define NET_IFC_FLAGS_DISABLED                  1

//IP probing in progress. 
//Don't set/reset this flag manually.
#define NET_IFC_FLAGS_IPPROBE                   2

//IP conflict detect on interface. 
//Don't set/reset this flag manually.
#define NET_IFC_FLAGS_IPCONFLICT                4

//Default interface flags
#define NET_IFC_FLAGS_DEFAULT                   (0)

/* Indicates that specified interface is ready to receive/transmit data */
#define IsIfcReady(ifc) (((ifc)->flags & \
                             (NET_IFC_FLAGS_DISABLED\
                              |NET_IFC_FLAGS_IPPROBE\
                              |NET_IFC_FLAGS_IPCONFLICT)) == 0)

/* Represents network interface configuration.
   Address and mask at network order: 127.0.0.1 -> 0x0100007F. */
typedef struct tag_NetIfcConfig
{
		//Interface address
        u32 ip;

        //Subnet mask
        u32 mask;

        //Gateway address
        u32 gate;

        //Interface MAC address at canonical format
        u8  mac[ETH_MACADDR_LENGTH];

        //Interface index
        u16 index;

        //MTU, bytes
        u16 mtu;

        //Interface flags (see NET_IFC_FLAGS_x)
        u16 flags;

        //Attached device
        const ETHConfig *dev;
} NetIfcConfig;

/*===========================================================================
 * ARP definitions
 *===========================================================================*/

//ARP table update interval (SHOULD 1000 msec), msec
#define NET_ARP_TABLE_UPDINTERVAL		1000

//Check ethernet link interval, msec
#ifndef NET_ARP_CHECK_LINK_INTERVAL
#define NET_ARP_CHECK_LINK_INTERVAL		250
#endif /* !NET_ARP_CHECK_LINK_INTERVAL */

//Probe handler interval, msec
#ifndef NET_ARP_PROBE_HANDLER_INTERVAL
#define NET_ARP_PROBE_HANDLER_INTERVAL	5
#endif /* !NET_ARP_PROBE_HANDLER_INTERVAL */

//Maximum size of ARP table
#ifndef NET_ARP_TABLE_SIZE
#define NET_ARP_TABLE_SIZE				25
#endif /* !NET_ARP_TABLE_SIZE */

//ARP entry timeout 1 (last usage), sec
#ifndef NET_ARP_TIMEOUT1
#define NET_ARP_TIMEOUT1				120
#endif /* NET_ARP_TIMEOUT1 */

//ARP entry timeout 2 (ttl), sec
#ifndef NET_ARP_TIMEOUT2
#define NET_ARP_TIMEOUT2				600
#endif /* !NET_ARP_TIMEOUT2 */

//Count of retransmissions of ARP request
#ifndef NET_ARP_RETRMCOUNT
#define NET_ARP_RETRMCOUNT				3
#endif /* !NET_ARP_RETRMCOUNT */

//ARP entry flags (ARPEntry::flags)
//Indicates that ARP entry is static
#define NET_ARP_FLAG_STATIC             0x01

/* Represents entry of ARP table */
typedef struct tag_ARPEntry
{
        //IP address
        u32 ip;

        //ARP entry timeout (last usage), sec
        u16 timeout1;

        //ARP entry timeout (time to live), sec
        u16 timeout2;

        //ARP entry flags (see NET_ARP_FLAG_x)
        u16 flags;

        //MAC address
        u8 mac[ETH_MACADDR_LENGTH];
} ARPEntry;

/*===========================================================================
 * IPv4 definitions
 *===========================================================================*/

//Default TTL value of IPv4 packet
#ifndef NET_IPV4_TTL
#define NET_IPV4_TTL						64
#endif /* !NET_IPV4_TTL */

//Maximum size of IPv4 route table
#ifndef NET_ROUTE_TABLE_SIZE
#define NET_ROUTE_TABLE_SIZE				12
#endif /* !NET_ROUTE_TABLE_SIZE */

//Build or not with route module (route incoming packets)
#ifndef NET_ROUTE_MODULE_ENABLE
#define NET_ROUTE_MODULE_ENABLE             1
#endif /* NET_ROUTE_MODULE_ENABLE */

//Route entry flags (RouteEntry::flags)
#define NET_ROUTE_FLAG_UP					0x01
#define NET_ROUTE_FLAG_GATE					0x02
#define NET_ROUTE_FLAG_HOST					0x04
#define NET_ROUTE_FLAG_DIRECTED				0x08
#define NET_ROUTE_FLAG_MODIFIED				0x10
#define NET_ROUTE_FLAG_STATIC				0x20

/* Represents entry at IPv4 route table */
typedef struct tag_RouteEntry
{
        //Destination IP or subnet address
        u32 destIP;

        //Mask
        u32 mask;

        //Gateway IP address
        u32 gate;

        //Network interface index
        u8 ifc;

        //Route entry flags (see NET_ROUTE_FLAG_x)
        u8 flags;

        //Route metric
        u16 metric;
} RouteEntry;

/*===========================================================================
 * IGMP definitions
 *===========================================================================*/

//IGMP state update interval, msec
#ifndef NET_IGMP_STATE_UPDINTERVAL
#define NET_IGMP_STATE_UPDINTERVAL			100
#endif /* !NET_IGMP_STATE_UPDINTERVAL */

//Maximum size of interface reception state (for all interfaces)
#ifndef NET_IGMP_STATE_SIZE
#define NET_IGMP_STATE_SIZE					7
#endif /* !NET_IGMP_STATE_SIZE */

//Enable/disable source list support
#ifndef NET_IGMP_EN_SRCLIST
#define NET_IGMP_EN_SRCLIST					0
#endif /* !NET_IGMP_EN_SRCLIST */

#if (NET_IGMP_EN_SRCLIST == 1)
//Maximum size of source list 
#ifndef NET_IGMP_SRCLIST_SIZE
#define NET_IGMP_SRCLIST_SIZE				8
#endif /* !NET_IGMP_SRCLIST_SIZE */
#endif /* NET_IGMP_EN_SRCLIST == 1 */

//IGMPv3 default values of timers and counters (RFC 3376, p 8)
//Robustness Variable. MUST NOT be 0, and SHOULD NOT be 1. Default: 2
#ifndef NET_IGMP_ROBUST_VAR
#define NET_IGMP_ROBUST_VAR				    2
#endif /* !NET_IGMP_ROBUST_VAR */

//Unsolicited Report Interval. The time (msec) between repetitions of
//a host's initial report membership in a group. Default: 1 second.
#ifndef NET_IGMP_REPORT_INTERVAL
#define NET_IGMP_REPORT_INTERVAL			1000
#endif /* !NET_IGMP_REPORT_INTERVAL */

//Query Response Interval. The Max Response Time used to calculate 
//the Max Resp Code inserted into General Queries. Default: 10 seconds.
#ifndef NET_IGMP_QRESPONSE_INTERVAL
#define NET_IGMP_QRESPONSE_INTERVAL			10000
#endif /* !NET_IGMP_QRESPONSE_INTERVAL */

//Last Member Query Interval. The Max Response Time used to calculate 
//the Max Resp Code inserted into Group-Specific and
//Group-and-Source-Specific Queries. Default: 1 second.
#ifndef NET_IGMP_LASTMEMBER_INTERVAL
#define NET_IGMP_LASTMEMBER_INTERVAL		1000
#endif /* !NET_IGMP_LASTMEMBER_INTERVAL */

//Reception state entry flags (RStateEntry::flags)
//Indicates that response to Group-Specific Query or 
//Group-and-Source-Specific Query in pending state
#define NET_IGMP_RST_FLAGS_GRPQRSP			0x01

/* Represents interface reception state entry */
typedef struct tag_RStateEntry
{
		//Multicast address (224.0.0.1 => 0x010000E0)
        u32 maddr;

		//Interface index: [1..(registered ifc count-1)]
        u8 ifc;

		//Filter mode (see IGMPV3_RTYPE_IS_x at "IGMP.h")
		u8 filterMode;
		
		//Flags (see NET_IGMP_RST_FLAGS_x)
		u8 flags;

		//Report retransmission count
		u8 retrmCount;

		//Report retransmission timer, msec
        u32 retrmTimer;

		//Group/Group-Source-Specific Query response timer, msec
        u32 grpQTimer;

		#if (NET_IGMP_EN_SRCLIST == 1)
		//Source list
		u32 slist[NET_IGMP_SRCLIST_SIZE];

		//Queried source list
		u32 qlist[NET_IGMP_SRCLIST_SIZE];

		//Count of sources at list
		u16 scount;

		//Count of queried sources at list
		u16 qcount;
		#endif /* NET_IGMP_EN_SRCLIST == 1 */

} RStateEntry;

/*===========================================================================
 * TCP definitions
 *===========================================================================*/

/* Indicates that TCP module enabled
   (disabled by default: EEPROM space economy, enable it at "globaldef.h") */
#ifndef NET_TCP_MODULE_ENABLE
#define NET_TCP_MODULE_ENABLE           0
#endif /* !NET_TCP_MODULE_ENABLE */

/* Maximum size of TCB table (maximum count of connections) */
#ifndef NET_TCP_TCBTABLE_SIZE
#define NET_TCP_TCBTABLE_SIZE			12
#endif /* !NET_TCP_TCBTABLE_SIZE */

/* Maximum size of TCP listeners table (maximum count of opened ports) */
#ifndef NET_TCP_TCPLTABLE_SIZE
#define NET_TCP_TCPLTABLE_SIZE			12
#endif /* !NET_TCP_TCPLTABLE_SIZE */

/* TCP state update interval, msec */
#ifndef NET_TCP_STATE_UPDINTERVAL
#define NET_TCP_STATE_UPDINTERVAL		1
#endif /* !NET_TCP_STATE_UPDINTERVAL */


/* The time of connection inactivity after which the
   first keep-alive request is sent, msec */
#ifndef NET_TCP_KEEPALIVE_TIME
#define NET_TCP_KEEPALIVE_TIME          (30000)
#endif /* !NET_TCP_KEEPALIVE_TIME */

/* The duration between two successive keepalive retransmissions,
   if acknowledgement to the previous keepalive transmission
   is not received, msec */
#ifndef NET_TCP_KEEPALIVE_INTERVAL
#define NET_TCP_KEEPALIVE_INTERVAL      (10000)
#endif /* !NET_TCP_KEEPALIVE_INTERVAL */

/* The number of retransmissions to be carried out before declaring
   that remote end is not available */
#ifndef NET_TCP_KEEPALIVE_PROBES
#define NET_TCP_KEEPALIVE_PROBES        (9)
#endif /* !NET_TCP_KEEPALIVE_PROBES */


/* Time to pass to be sure the remote TCP received the
   acknowledgment of its connection termination request, msec */
#ifndef NET_TCP_TIMEWAIT_INTERVAL
#define NET_TCP_TIMEWAIT_INTERVAL       (2000)   //max 2min
#endif /* !NET_TCP_TIMEWAIT_INTERVAL */

/* All timers: maximum retransmition interval, msec */
#ifndef NET_TCP_TIMALL_MAX_INTERVAL
#define NET_TCP_TIMALL_MAX_INTERVAL     (120000) //max 2min
#endif /* !NET_TCP_TIMALL_MAX_INTERVAL */


/* Retransmition timers (by default): */
/* SYN/SYN,ACK segments: initial interval, msec
   Must not less than "NET_TCP_STATE_UPDINTERVAL" */
#define NET_TCP_TIMSYN_INITIAL          (1000)
/* Count SYN of retransmitions */
#define NET_TCP_TIMSYN_COUNT            (7) //max 9

/* FIN/FIN,ACK/DATA segments: initial interval, msec
   Must not less than "NET_TCP_STATE_UPDINTERVAL" */
#define NET_TCP_TIMDATA_INITIAL         (200)
/* Count DATA of retransmitions */
#define NET_TCP_TIMDATA_COUNT           (7) //max 16


/* TCP connection states. DO NOT CHANGE ORDER OF STATES */
/* Represents no connection state at all. */
#define NET_TCP_STATE_CLOSED			0

/* Represents waiting for a matching connection request after having
   sent a connection request. */
#define NET_TCP_STATE_SYNSENT			1

/* Represents waiting for a confirming connection request acknowledgment 
   after having both received and sent a connection request. */
#define NET_TCP_STATE_SYNRECEIVED		2

/* Represents an open connection, data received can be delivered
   to the user. The normal state for the data transfer phase of
   the connection. */
#define NET_TCP_STATE_ESTABLISHED		3

/* Represents waiting for a connection termination request from
   the remote TCP, or an acknowledgment of the connection termination
   request previously sent. */
#define NET_TCP_STATE_FINWAIT1			4

/* Represents waiting for a connection termination request from
   the remote TCP. */
#define NET_TCP_STATE_FINWAIT2			5

/* Represents waiting for a connection termination request from
   the local user */
#define NET_TCP_STATE_CLOSEWAIT         6

/* Represents waiting for an acknowledgment of the connection
   termination request previously sent to the remote TCP
   (which includes an acknowledgment of its connection termination
   request). */
#define NET_TCP_STATE_LASTACK			7

/* Represents waiting for a connection termination request
   acknowledgment from the remote TCP. */
#define NET_TCP_STATE_CLOSING			8

/* Represents waiting for enough time to pass to be sure the remote
   TCP received the acknowledgment of its connection termination request. */
#define NET_TCP_STATE_TIMEWAIT			9


/* TCB options (TCBRecord::opts) */
/* Indicates that connection is outgoing */
#define NET_TCP_OPTS_OUTGOING           0x0001

/* TCP keep-alive procedure enabled */
#define NET_TCP_OPTS_KEEPALIVE          0x0002

/* For upper level protocol (HTTP):
   wait closing request from remote side */
#define NET_TCP_OPTS_UPROTO_KEEPALIVE   0x0004


/* Default TCB record options: NET_TCP_OPTS_KEEPALIVE or 0 */
#define NET_TCP_OPTS_DEFAULT            (NET_TCP_OPTS_KEEPALIVE)


/* Represents Transmission Control Block (TCB) record: RFC 793, p 3.2 */
typedef struct tag_TCBRecord
{
        //-------------------------------		
        //Remote/local end points
        //Remote address
        u32 raddr;

        //Local address
        u32 laddr;

        //Remote port (network order)
        u16 rport;

        //Local port (network order)
        u16 lport;

        //-------------------------------
        //Connection state (see NET_TCP_STATE_x)
        u16 state;

        //Connection options (see NET_TCP_OPTS_x)
        u16 opts;

        //Pointer to user-defined data
        void *tag;

        //-------------------------------
        //Pointers to start of data
        const u8 *pTxBeg;

        //Pointer to transmitted and acknowledgement data
        const u8 *pTxAck;

        //Pointer to transmitted data
        const u8 *pTxCurr;

        //Pointer to end of data
        const u8 *pTxEnd;

        //Tx segment length
        u32 txLength;

        //-------------------------------
        //Configured timers
        //SYN / SYN,ACK segments: initial retrm interval, ms
        u32 timSynInitial;

        //FIN / FIN,ACK and DATA segments: initial retrm interval, ms
        u32 timDataInitial;

        //SYN / SYN,ACK segments: count of retransmissions
        u8 timSynCount;

        //FIN / FIN,ACK and DATA segments: count of retransmissions
        u8 timDataCount;

        //-------------------------------
        //Current time counters
        //Retransmission counter
        u8 retrmCounter;

        //Count of retransmissions
        u8 retrmCount;

        //Retransmission timer, ms
        u32 retrmTimer;

        //Retransmission interval, ms
        u32 retrmInterval;        

        //Keep-alive timer, ms
        u32 keepAliveTimer;

        //Keep-alive interval, ms
        u32 keepAliveInterval;

        //Keep-alive counter, ms
        u32 keepAliveCounter;

        //Incoming data timer, ms
        u32 recvTimer;

        //Incoming data timeout, ms (0 - infinity)
        u32 recvTimeout;
        //-------------------------------
        //Send Sequence Variables
		//Send unacknowledged
        //u32 sndUna;

        //Send next (SEQ number)
		u32 sndNxt;

        //Send window
		u32 sndWnd;

        //Send urgent pointer
        //u32 sndUP;

        //Segment sequence number used for last window update
        //u32 sndWL1;

        //Segment acknowledgment number used for last window update
        //u32 sndWL2;

        //Initial send sequence number
		u32 ISS;
    
        //Maximum segment size (MSS)
        u32 sndMSS;

        //-------------------------------
        //Receive Sequence Variables
        //Receive next (ACK number)
		u32 rcvNxt;

        //Receive window
		u32 rcvWnd; 

        //Receive urgent pointer
        //u32 rcvUP;

        //Initial receive sequence number
		u32 IRS;		        

        //Maximum segment size (MSS)
        u32 rcvMSS;
} TCBRecord;


/* Represents TCP listener control structure */
typedef struct tag_TCPListener
{
    //Local address (network order)
    u32 laddr;

    //Local port (network order)
    u16 port;

    //Maximum number of connections
    u16 maxcon;

    //Count of connections
    u16 concount;

    //Reserved
    u16 reserved;
} TCPListener;

/*===========================================================================
 * DNS definitions
 *===========================================================================*/

//DNS cashe update interval (SHOULD 1000 msec), msec
#define NET_DNS_CACHE_UPDINTERVAL			1000

//Maximum length of IPv4 route table
#ifndef NET_DNS_CACHE_SIZE
#define NET_DNS_CACHE_SIZE					4
#endif /* !NET_DNS_CACHE_SIZE */

//Maximum count of DNS servers
#ifndef NET_DNS_NSERVER_COUNT
#define NET_DNS_NSERVER_COUNT				2
#endif /* !NET_DNS_NSERVER_COUNT */

//DNS request retransmission interval (2..5 sec), sec
#define NET_DNS_REQUEST_RETRMINTERVAL		2

//DNS request retransmission count
#define NET_DNS_REQUEST_RETRMCOUNT			12

//DNS LLMNR request retransmission interval, sec
#define NET_DNS_LLMNR_RETRMINTERVAL			1

//DNS LLMNR request retransmission count
#define NET_DNS_LLMNR_RETRMCOUNT			2

//Maximum TTL value, sec
#define NET_DNS_ENTRY_TTL_MAX	  		   (4 * 24 * 3600)

//Default value for TTL2 (last usage), sec
#define NET_DNS_ENTRY_TTL2					60000

//Number of stored IP addresses
#ifndef NET_DNS_ENTRY_IPCOUNT
#define NET_DNS_ENTRY_IPCOUNT				4
#endif /* !NET_DNS_ENTRY_IPCOUNT */

//TTL value in the record returned at LLMNR response.
//According RFC 4795, p 2.8 "RR TTL": 
//"A default value of 30 seconds is RECOMMENDED."
#define NET_DNS_LOCAL_ENTRY_TTL				30

//"localhost" string
#define NET_DNS_LOCALHOST					"localhost\0"

//DNS request flags (DNSEntry::flags)
//Search records only at cache, don't send request to resolve name/address
#define NET_DNS_FLAGS_CACHEONLY				0x01

//Create inverse request
#define NET_DNS_FLAGS_INVERSE				0x02

//Create non-recursive request
#define NET_DNS_FLAGS_NONRECURSIVE			0x04

//Indication flags. Don't set/reset this flags manually
//Indicates that LLMNR request will send
#define NET_DNS_FLAGS_LLMNR					0x08

//Create advisory request (with C = 1)
//Has meaning only when NET_DNS_FLAGS_LLMNR is set
#define NET_DNS_FLAGS_ADVISORY				0x10

//Indicates that name verification in progress
#define NET_DNS_FLAGS_NAMEVERIFYING			0x20

//Indicates that name conflict detected
#define NET_DNS_FLAGS_NAMECONFLICT			0x40

/* Represents Internet host address information */
typedef struct tag_IPHostEntry
{
	//List of IP addresses that are associated with a host
	u32 ip[NET_DNS_ENTRY_IPCOUNT];

	//Count of addresses
	s32 count;
		
	//DNS name of the host
	char dname[DNS_MAX_HOSTNAME_LENGTH];
} IPHostEntry;



/* Represents entry of DNS cache */
typedef struct tag_DNSEntry
{
		//Internet host address information
		IPHostEntry data;

		//Name server addresses
		u32 nsaddr[NET_DNS_NSERVER_COUNT];

		//Current server index (DNS query sequentual)
		u32 nsIndex;

		//TTL1 (time to live), sec
		u32 ttl1;

		//TTL2 (last usage), sec
		u16 ttl2;		

		//Type of entry (see DNS_TYPE_x at "DNS.h")
		u16 type;

		//Transaction ID
		u16 ID;	

		//Retransmission counter
		u8 retrmCount;

		//Entry flags (see NET_DNS_FLAGS_x)
		u8 flags;		
} DNSEntry;

/*===========================================================================
 * Common definitions
 *===========================================================================*/

//Network protocols options (NetProtoConfig::opts)
//Receive frames with any multicast MAC addresses
#define NET_OPTS_ETH_RECVANYMCAST		0x00000001

//Add new ARP entry when ARP request is received
#define NET_OPTS_ARP_ADDENTBYREQUEST	0x00000002

//Enable ARP broadcast replies
#define NET_OPTS_ARP_BCASTREPLIES		0x00000004

//Don't fragment IP packets (DF is set at outgoing packets)
#define NET_OPTS_IPV4_DONTFRAG			0x00000008

//Receive own multicast packets
#define NET_OPTS_IPV4_RECVOWNMCAST		0x00000010

//Enable IP packet routing
#define NET_OPTS_IPV4_ENROUTE			0x00000020

//Enable loose/strict source route
#define NET_OPTS_IPV4_ENSRCROUTE		0x00000040

//Enable ICMP protocol
#define NET_OPTS_ICMP_ENABLE			0x00000080

//Ignore UDP packet checksum errors
#define NET_OPTS_UDP_IGNCSUMERR			0x00000100

//Put checksum to UDP packet
#define NET_OPTS_UDP_SETCSUM			0x00000200

//DNS query mode parallel (otherwise sequential)
#define NET_OPTS_DNS_QUERYPARALLEL		0x00000400

//Send non-recursive DNS queries
#define NET_OPTS_DNS_NONRECURSIVE		0x00000800

//Enable Link Local Multicast Name Resolution (LLMNR)
#define NET_OPTS_DNS_LLMNR_ENABLE		0x00001000

//Refresh DNS cache entries automatically (when TTL = 0)
#define NET_OPTS_DNS_AUTOREFRESH		0x00002000


//Default network protocols options
#define NET_OPTS_DEFAULT       ( NET_OPTS_IPV4_DONTFRAG \
							   | NET_OPTS_UDP_SETCSUM \
                               | NET_OPTS_ICMP_ENABLE \
							   | NET_OPTS_DNS_LLMNR_ENABLE)
							  /* | NET_OPTS_ETH_RECVANYMCAST \ */
							  /* | NET_OPTS_ARP_ADDENTBYREQUEST \ */
							  /* | NET_OPTS_ARP_BCASTREPLIES \ */
							  /* | NET_OPTS_IPV4_RECVOWNMCAST */                              
                              /* | NET_OPTS_IPV4_ENROUTE \ */
                              /* | NET_OPTS_IPV4_ENSRCROUTE \ */
                              /* | NET_OPTS_UDP_IGNCSUMERR \ */                              
							  /* | NET_OPTS_DNS_QUERYPARALLEL \ */
							  /* | NET_OPTS_DNS_AUTOREFRESH */
							  /* | NET_OPTS_DNS_NONRECURSIVE */

/* Represents IP conflict information */
typedef struct tag_IPConflictInfo
{
        NetIfcConfig *ifc;
} IPConflictInfo;


//TCP handlers
//TCP handler
typedef s32 (*TCPHandler)(TCBRecord *tcb);
typedef TCPHandler          LPTCPHandler;

//TCP error handler
typedef s32 (*TCPErrorHandler)(TCBRecord *tcb, s32 errcode);
typedef TCPErrorHandler     LPTCPErrorHandler;

//TCP data handler
typedef s32 (*TCPDataHandler)(TCBRecord *tcb, u8 *buf, s32 length);
typedef TCPDataHandler      LPTCPDataHandler;


//UDP datagram handler
typedef s32 (*UDPHandler)(NetEndPoint *locEp, NetEndPoint *remEp, u8 *buf, s32 length);
typedef UDPHandler          LPUDPHandler;

//ICMP message handler
typedef void (*ICMPHandler)(NetIfcConfig *ifc, IPv4Hdr *ipHdr, u8 *buf, s32 length);
typedef ICMPHandler         LPICMPHandler;

//IP conflict handler
typedef void (*IPConflictHandler)(IPConflictInfo *ipci);
typedef IPConflictHandler   LPIPConflictHandler;

//Interface ready handler
typedef void (*IfcReadyHandler)(NetIfcConfig *ifc);
typedef IfcReadyHandler     LPIfcReadyHandler;


/* Represents configuration of network protocols */
typedef struct tag_NetProtoConfig
{
        //DNS servers addresses (127.0.0.1 -> 0x0100007F)
        u32 dns[NET_DNS_NSERVER_COUNT];		        

        //Network proto options (see NET_OPTS_x)
        u32 opts;        

        //ARP entry timeout1, sec
        u16 arpTimeout1;

        //ARP entry timeout2, sec
        u16 arpTimeout2;

        //ARP request repeat count
        u16 arpRepeatCount;

        //IPv4 TTL default value
        u8 ipv4TTL;

        //IGMP robustness variable
        u8 igmpRobustVar;

        //Time to pass to be sure the remote TCP received the
        //ACK of its connection termination request, msec
        u32 tcpTimewait;

        //The time of connection inactivity after which the
        //first keep-alive request is sent, msec
        u32 tcpKeepAliveTime;

        //The duration between two successive keepalive retransmissions,
        //if acknowledgement to the previous keepalive transmission
        //is not received, msec
        u32 tcpKeepAliveIntvl;

        //The number of retransmissions to be carried out before declaring
        //that remote end is not available
        u32 tcpKeepAliveProbes;

        //Event handlers
        //Requested incoming TCP connection
        LPTCPHandler            TCPConnRequested;

        //TCP connection established
        LPTCPHandler            TCPConnEstablished;

        //Connection termination request received
        LPTCPHandler            TCPConnCloseRequest;

        //TCP connection closed
        LPTCPHandler            TCPConnClosed;

        //TCP data transmission completed
        LPTCPHandler            TCPTransmitted;

        //TCP connection error
        LPTCPErrorHandler       TCPConnError;

        //TCP data received (flow control)
        LPTCPDataHandler        TCPFCReceived;

        //TCP data received
        LPTCPDataHandler        TCPReceived;


		//Occurs when the UDP datagram received
        LPUDPHandler            UDPReceived;

		//Occurs when the ICMP message received
        LPICMPHandler           ICMPReceived;

		//Occurs when the IP conflict detected
        LPIPConflictHandler     IPConflictDetected;

		//Occurs when interface ready state changed
        LPIfcReadyHandler       IfcReady;

        //Domain name
        char domName[DNS_MAX_HOSTNAME_LENGTH];

} NetProtoConfig;

/*===========================================================================
 * Common functions
 *===========================================================================*/

/* Protocol stack configuration */
extern NetProtoConfig *netProtoConfig;

/* Time counters, msec */
extern u32 networkTimerMs;
//extern u16 ethStateTimeCounter;
extern u16 arpStateTimeCounter;
//extern u16 ipv4StateTimeCounter;
extern u16 igmpStateTimeCounter;
//extern u16 icmpStateTimeCounter;
//extern u16 udpStateTimeCounter;
extern u16 tcpStateTimeCounter;
extern u16 dnsCacheTimeCounter;

/* Update network timers.
   Call this function with frequency 1000Hz */
__STATIC_INLINE void UpdateNetworkTimers()
{
    networkTimerMs++;

    //ethStateTimeCounter++;
    arpStateTimeCounter++;
    //ipv4StateTimeCounter++;
    igmpStateTimeCounter++;
    //icmpStateTimeCounter++;
    //udpStateTimeCounter++;
    tcpStateTimeCounter++;
    dnsCacheTimeCounter++;
}


/* Indicates that routing enable */
#define IsRoutingEnable()       (netProtoConfig->opts & NET_OPTS_IPV4_ENROUTE)

/* Indicates that ICMP send/receive enable */
#define IsICMPEnable()          (netProtoConfig->opts & NET_OPTS_ICMP_ENABLE)

/* Gets time at milliseconds */
#define GetNetworkTimerMs() networkTimerMs


/* Initialize network protocols stack.
 Call once at initialization. */
void InitializeNetProtoStack();

/*  Reset protocols configuration
    to it default values. */
void ResetNetProtoConfig();

/* Applies immediately network protocols configuration changes */
void ApplyNetProtoConfig();

/* Update network state.
   Call once at main cycle */
void UpdateNetworkState();

/*===========================================================================
 * Network interface control functions
 *===========================================================================*/

/* Gets count of currently registered interfaces */
s32 GetNetIfcCount();

/* Indicates that any interface is ready to receive/transmit data */
u32 IsAnyIfcReady();

/* Indicates that all interfaces is ready to receive/transmit data */
u32 IsAllIfcReady();

/* Copy network interface configuration */
NetIfcConfig *CopyNetIfc(NetIfcConfig *destIfc, NetIfcConfig *srcIfc);

/* Get network interface with specified index */
NetIfcConfig *GetNetIfcConfig(s32 index);

/* Get network interface with specified IP address */
NetIfcConfig *GetNetIfcConfigByIP(u32 ifcIP);

/* Get network interface with specified MAC address */
NetIfcConfig *GetNetIfcConfigByMAC(u8 *ifcMAC);

/* Register network interface */
NetIfcConfig *RegisterNetIfc(NetIfcConfig *ifc);

/* Set MAC address at specified interface */
void SetMACAddress(NetIfcConfig *ifc, u8 *mac);

/* Set IPv4 address at specified interface */
void SetIPv4Address(NetIfcConfig *ifc, u32 ip);

/* Set IPv4 gateway address at specified interface */
void SetIPv4Gateway(NetIfcConfig *ifc, u32 gateip);

/* Set subnet mask at specified interface */
void SetIPv4SubnetMask(NetIfcConfig *ifc, u32 mask);

/*===========================================================================
 * Ethernet functions
 *===========================================================================*/

/*===========================================================================
 * ARP functions
 *===========================================================================*/

/* Get maximum size of ARP table  */
#define GetARPTableSize()	(NET_ARP_TABLE_SIZE)

/* Get ARP table entry by index */
ARPEntry *GetARPEntry(s32 index);

/* Start send ARP probe and announce packets */
void StartProbeAnnounce(NetIfcConfig *ifc);

/* Remove all entries from ARP table */
void ResetARPTable(u8 incStatic);

/* Add ARP record with specified parameters */
s32 AddARPEntry(u32 ip, u8 *mac, u16 flags);

/* Remove ARP record with specified IP address */
s32 RemoveARPEntry(u32 ip);

/*===========================================================================
 * IPv4 functions
 *===========================================================================*/

/* Get maximum size of route table  */
#define GetRouteTableSize()	(NET_ROUTE_TABLE_SIZE)

/* Get route table entry by index */
RouteEntry *GetRouteEntry(s32 index);

/* Gets output IP address interface for specified destination host */
u32 GetOutIfcIP(const char *destHost);

/* Indicates that output interface is ready to
   send data to specified remote address */
u32 IsOutIfcReady(u32 destIP);

/* Find route entry using specified interface and destination addresses */
RouteEntry *RouteFind(u32 ifcIP, u32 destIP);

/* Add route with specified parameters */
u8 AddRouteEntry(u32 destIP, u32 mask, u32 gate,
                        u16 ifc, u16 metric, u8 flags);

/* Remove specified route */
u8 RemoveRouteEntry(u32 destIP, u32 mask, u32 gate, u16 ifc);

/* Update parameters of the specified route */
u8 UpdateRouteEntry(RouteEntry *p,
                        u32 destIP, u32 mask, u32 gate,
                        u16 ifc, u16 metric, u8 flags);

/* Reset all entries at IPv4 route table */
void ResetRouteTable(u8 incStatic);

/* Add default routes to Ipv4 route table */
void AddDefaultRoutes();

/* Update default routes using specified interface configuration */
void UpdateDefaultRoutes(NetIfcConfig *ifc, NetIfcConfig *newIfc);

/* Send IPv4 packet with selected parameters
   @locIP       : local address (may be IPADDR_ANY);
   @remIP       : remote address;
   @encProtocol : transport protocol (see IPV4_PROTOCOL_x)
   @ttl         : time to live (0 to use default);
   @opts        : pointer to IP packet options buffer;
   @encHdr      : pointer to transport protocol header;
   @encHdrLength: transport protocol header length, bytes;
   @pld         : pointer to payload buffer;
   @pldLength   : payload length, bytes. */
s32 IPV4SendPacket(u32 locIP, u32 remIP,
                   u8 encProtocol,
                   u8 ttl,
				   const u8 *opts,
                   u8 *encHdr, s32 encHdrLength,
                   const u8 *pld, s32 pldLength);

/*===========================================================================
 * IGMP functions
 *===========================================================================*/

/* Get maximum size of reception state  */
#define GetRStateTableSize()	(NET_IGMP_STATE_SIZE)

/* Get reception state entry by index */
RStateEntry *GetRStateEntry(s32 index);

/* Gets reception state entry using specified
   interface index and multicast address */
RStateEntry *GetRStateEntryE(u8 ifc, u32 maddr);

/* Adds a interface to a multicast group
   @ifc  : interface index;
   @maddr: the multicast address of the group to join. */
s32 JoinMulticastGroup(u8 ifc, u32 maddr);

/* Leaves a multicast group
   @ifc  : interface index;
   @maddr: the multicast address of the group to leave. */
s32 LeaveMulticastGroup(u8 ifc, u32 maddr);

/* Indicates that specified interface is joined to multicast group */
u8 IsIfcMulticastGroup(u8 ifc, u32 maddr, u32 saddr);

/* Add sources to specified reception state */
s32 AddGroupSources(u8 ifc, u32 maddr, u8 filterMode, u32 *slist, u16 scount);

/* Drop specified sources from reception state */
s32 DropGroupSources(u8 ifc, u32 maddr, u32 *slist, u16 scount);

/* Drop all sources from reception state */
s32 DropGroupSourcesAll(u8 ifc, u32 maddr, u8 filterMode);

/*===========================================================================
 * ICMP functions
 *===========================================================================*/

/* Send ICMP Echo Request (Type 8) */
s32 ICMPSendEchoRequest(u32 locIP, u32 remIP,
                        u16 seqId, u16 seqNum, const u8 *data, s32 length);

/*===========================================================================
 * TCP functions
 *===========================================================================*/

#if (NET_TCP_MODULE_ENABLE == 0)
/* Count of TCP connectons  */
#define GetTCBTableSize()           (0)

/* Size of TCP listeners table */
#define GetTCPListenerTableSize()	(0)
#else
/* Count of TCP connectons  */
#define GetTCBTableSize()           (NET_TCP_TCBTABLE_SIZE)

/* Size of TCP listeners table */
#define GetTCPListenerTableSize()	(NET_TCP_TCPLTABLE_SIZE)
#endif

/* Get pointer to TCB record using specified index */
TCBRecord *GetTCBRecord(s32 i);

/* Get pointer to TCP listener using specified index */
TCPListener *GetTCPListener(s32 i);

/* Find listener using specified port (port at network order) */
TCPListener *TCPListenerFind(u16 port);

/* Starts listening for incoming connection requests with a maximum number
   of pending connection (laddr and port at network byte order) */
TCPListener *TCPListenerStart(u32 laddr, u16 port, u16 maxcon);

/* Closes the listener using specified pointer to listener control structure */
s32 TCPListenerStop(TCPListener *lsnr);

/* Closes the listener using specified port */
s32 TCPListenerStopByPort(u16 port);


/* Start TCP connection using specified remote address and port */
TCBRecord *TCPConnect(u32 addr, u16 port);

/* Reset specified TCP connection:
   send RST,ACK segment,
   reset "TCBRecord" fields to defaults,
   rise "TCPConnError", "TCPConnClosed" events */
s32 TCPReset(TCBRecord *tcb, u32 sendRST);

/* Reset all TCP connections
   (send RST,ACK segment and reset structure) */
void TCPResetAllConnections();

/* Set CLOSEWAIT state for specified TCP connection */
s32 TCPSetCloseWait(TCBRecord *tcb);

/* Closes the TCP connection using pointer to specified tcb record */
s32 TCPClose(TCBRecord *tcb);

/* Stop receive data from remote side */
void TCPRxFlowStop(TCBRecord *tcb);

/* Start receive data from remote side */
void TCPRxFlowStart(TCBRecord *tcb);

/* Start send data using TCP connection */
s32 TCPWrite(TCBRecord *tcb, const u8 *buf, s32 length);

/* Indicates that enabled transmit next segment of data */
s32 IsTCPWriteEnabled(TCBRecord *tcb);

/* Set receive timeout (msec) for specified connection */
void TCPSetRecvTimeout(TCBRecord *tcb, u32 timeout);

/*===========================================================================
 * UDP functions
 *===========================================================================*/

/* Send UDP datagram using specified local and remote endpoints */
s32 UDPSend(NetEndPoint *locEp, NetEndPoint *remEp, u8 *buf, s32 length);

/* Send UDP datagram using specified destination address and port */
s32 UDPSendTo(u32 destAddr, u16 destPort, u8 *buf, s32 length);

/*===========================================================================
 * DNS functions
 *===========================================================================*/

/* Get maximum count of configured name servers */
#define GetDNSNServerCount() (NET_DNS_NSERVER_COUNT)

/* Get maximum size of DNS cache  */
#define GetDNSCacheSize()	 (NET_DNS_CACHE_SIZE)

/* Get DNS cache entry by index */
DNSEntry *GetDNSCacheEntry(s32 index);

/* Indicates that any DNS server is configured */
u32 IsDNSServersConfigured();

/* Indicates that hostname in conflict */
u32 IsHostnameConflict();

/* Indicates that hostname verification in progress */
u32 IsHostnameVerifying();

/* Reset all entries at DNS cache */
void ResetDNSCache();

/* Reset specified DNS cache entry to it default values */
void ResetDNSEntry(DNSEntry *p);

/* Resolves a host name or IP address to a pointer to IPHostEntry */
IPHostEntry *GetHostEntry(const char *hostNameOrAddress, s32 *errCode);

/* Resolves a host name or IP address to a pointer to IPHostEntry */
IPHostEntry *GetHostEntryE(const char *hostNameOrAddress, 
						   u32 flags, s32 *errCode);

/* Gets the local host name */
const char *GetHostName();

/* Sets the local host name */
void SetHostName(const char *dname);

/* Starts verification of local host name */
void StartHostNameVerify();

#endif /*__NETWORK_H */

/*===========================================================================
 * End of file network.h
 *===========================================================================*/
