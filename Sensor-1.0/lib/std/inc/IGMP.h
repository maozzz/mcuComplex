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
 *  File IGMP.h: Internet Group Managment Protocol versions 1-3 
 *               (RFCs 1112, 2236, 3376) 
 *               definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __IGMP_H
#define __IGMP_H

#include "../../bastypes.h"

//The all-systems multicast address: "224.0.0.1" (RFC 3376, p 5)
#define IGMPV3_ALLSYSTEMS_MADDR		((u32)0x010000E0)

//IGMPv3 general query destination address: "224.0.0.1" (RFC 3376, p 4.1.12)
#define IGMPV3_GENQUERY_MADDR		((u32)0x010000E0)

//IGMPv3 report destination address: "224.0.0.22" (RFC 3376, p 4.2.14)
#define IGMPV3_REPORT_MADDR			((u32)0x160000E0)

//IGMPv3 message TTL: 1 (RFC 3376, p 4)
#define IGMPV3_MESSAGE_TTL			1

//IGMPv3 Query minimum length (12 bytes)
#define IGMPV3_QUERY_MINLENGTH		12

//RFC 3376 defines next IGMP message types:
//IGMPv3 Membership Query (General/Group-Specific/Group-Source-Specific)
#define IGMPV3_TYPE_QUERY		0x11          
//IGMPv3 Membership Report
#define IGMPV3_TYPE_REPORT      0x22          

//IGMPv1 Membership Report (RFC 1112)
#define IGMPV1_TYPE_REPORT		0x12          
//IGMPv2 Membership Report (RFC 2236)
#define IGMPV2_TYPE_REPORT		0x16          
//IGMPv2 Leave Group       (RFC 2236)
#define IGMPV2_TYPE_LEAVE		0x17          


//Group Record Types (RFC 3376, p 4.2.12):
//"Current-State Record" is sent in response to a query
//MODE_IS_INCLUDE: interface has a filter mode of INCLUDE
#define IGMPV3_RTYPE_IS_INCLUDE		1
//MODE_IS_INCLUDE: interface has a filter mode of EXCLUDE
#define IGMPV3_RTYPE_IS_EXCLUDE		2

//"Filter-Mode-Change Record" is sent when filter mode is changed
//CHANGE_TO_INCLUDE_MODE: interface has changed filter mode to INCLUDE
#define IGMPV3_RTYPE_TO_INCLUDE		3
//CHANGE_TO_EXCLUDE_MODE: interface has changed filter mode to EXCLUDE
#define IGMPV3_RTYPE_TO_EXCLUDE		4

//"Source-List-Change Record": is sent when source list changed
//ALLOW_NEW_SOURCES: additional sources that the system wishes to hear
#define IGMPV3_RTYPE_ALLOW_NEW		5
//BLOCK_OLD_SOURCES: sources that the system no longer wishes to hear
#define IGMPV3_RTYPE_BLOCK_OLD		6


/* Represents IGMPv3 Query header
   (General/Group-Specific/Group-Source-Specific queries) */
typedef struct tag_IGMPv3QHdr
{
	//Type (must 0x11)
	u8 type;

	//Maximum response time code, 1/10 sec
	u8 maxRespCode;

	//Checksum
	u16 checksum;

	//Group address
	u32 maddr;

	//Querier's Robustness Variable (QRV)
	u8 QRV:3;

	//Suppress router-side processing
	u8 S:1;

	//Reserved (must 0)
	u8 reserved:4;

	//Querier's Query Interval Code (QQIC), sec
	u8 QQIC;

	//Number of sources (N)
	u16 nofSources;

} IGMPv3QHdr;



/* Represents IGMPv3 Membership Report header */
typedef struct tag_IGMPv3RHdr
{
	//Type (must 0x22)
	u8 type;

	//Reserved (must 0)
	u8 reserved0;

	//Checksum
	u16 checksum;

    //Reserved (must 0)
	u16 reserved1;

	//Number of group records (M)
	u16 nofRecords;

} IGMPv3RHdr;



/* Represents IGMPv3 group record
   (without Source Address List and Aux Data) */
typedef struct tag_IGMPv3Record
{
	//Record type (see IGMPV3_RTYPE_x) 
	u8 rtype;
	
	//Auxiliary data length, 32-bit words
	u8 auxLength;

	//Number of sources (N)     
	u16 nofSources;

    //Multicast address
	u32 maddr;

} IGMPv3Record;

#endif /* __IGMP_H */

/*===========================================================================
 * End of file IGMP.h
 *===========================================================================*/
