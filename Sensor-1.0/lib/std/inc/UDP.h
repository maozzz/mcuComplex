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
 *  File UDP.h: User Datagram Protocol (RFC 768) definitions
 *              and functions prototypes.
 *
 *===========================================================================*/

#ifndef __UDP_H
#define __UDP_H

#include "../../bastypes.h"
#include "IPv4.h"

//UDP datagram header length
#define UDP_HEADER_LENGTH               8

//UDP pseudo header length (checksum calculation only)
#define UDP_PSHEADER_LENGTH             12


typedef struct tag_UDPHdr
{
        //Source port
        u16 srcPort;

        //Destination port
        u16 destPort;

        //Datagram length (header + payload), bytes
        u16 length;

        //Checksum
        u16 checkSum;

} UDPHdr;



/* Calculate UDP datagram checksum */
u16 UDPCheckSum(u32 srcIP, u32 destIP,
                const u8 *udpHdr,
                const u8 *udpPld, s32 length);

#endif /* __UDP_H */

/*===========================================================================
 * End of file UDP.h
 *===========================================================================*/

