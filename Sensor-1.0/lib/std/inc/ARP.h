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
 *  File ARP.h: Address Resolution Protocol (RFC 826) definitions 
 *		and functions prototypes.
 *
 *===========================================================================*/

#ifndef __ARP_H
#define __ARP_H

#include "../../bastypes.h"
#include "Ethernet.h"

//ARP constants (network order)
#define ARP_HWTYPE_ETHERNET		0x0100

#define ARP_REQUEST			0x0100
#define ARP_ANSWER			0x0200

#define RARP_REQUEST			0x0300
#define RARP_ANSWER			0x0400

#define ARP_LENGTH			28

typedef struct tag_ARPEth
{
	//Hardware type (Ethernet: 0x0001)
	u16 hwType;
	//Protocol type (IPv4: 0x0800)
	u16 protoType;
	//Hardware address length (Ethernet: 6)
	u8 hwLength;
	//Protocol address length (IPv4: 4)
	u8 protoLength;
	//Operation (1 - request, 2 - answer)
	u16 operation;
	//Sender hardware address
	u8 sha[ETH_MACADDR_LENGTH];
	//Sender protocol address
	u32 spa;
	//Target hardware address
	u8 tha[ETH_MACADDR_LENGTH];
	//Target protocol address
	u32 tpa;
} ARPEth;

#endif /* __ARP_H */

/*===========================================================================
 * End of file ARP.h
 *===========================================================================*/