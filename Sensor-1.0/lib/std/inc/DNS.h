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
 *  File DNS.h: Domain Name System (DNS, RFC 1035),
 *		        Link-Local Multicast Name Resolution (LLMNR, RFC 4795)
 *              definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __DNS_H
#define __DNS_H

#include "../../bastypes.h"

//DNS port number: 53, host order (RFC 1035, p 4.2.1)
#define DNS_PORT					53

//DNS port number: 53, network order
#define DNS_NPORT					0x3500

//LLMNR port number: 5355, host order (RFC 4795, p 2)
#define DNS_LLMNR_PORT				5355

//LLMNR port number: 5355, network order
#define DNS_LLMNR_NPORT				0xEB14

//LLMNR request destination address: "224.0.0.252" (RFC 4795, p 2)
#define DNS_LLMNR_MADDR				((u32)0xFC0000E0)

//According RFC 1035, p 2.3.4. "Size limits":
//Maximum label length
#define DNS_MAX_LABEL_LENGTH		63

//Maximum domain name length 
//(stricted length, RFC 1035 defines maximum length 255 chars)
#define DNS_MAX_HOSTNAME_LENGTH		64

//Maximum DNS message length
#define DNS_MAX_MESSAGE_LENGTH		512

//Special domain to support gateway location and
//Internet address to host mapping.
#define INADDR_ARPA_HOSTNAME		"in-addr.arpa\0"

//According RFC 1035, p 3.2.2 "TYPE values":
#define DNS_TYPE_A					1   //Host IPv4 address
#define DNS_TYPE_NS					2   //Authoritative name server
#define DNS_TYPE_CNAME				5   //Canonical name
#define DNS_TYPE_SOA				6   //Start of authority
#define DNS_TYPE_WKS				11  //Well known service description
#define DNS_TYPE_PTR				12  //Domain name pointer
#define DNS_TYPE_HINFO				13  //Host information
#define DNS_TYPE_MINFO				14  //Mailbox or mail list information
#define DNS_TYPE_MX					15  //Mail exchanger
#define DNS_TYPE_TXT				16  //Text string
#define DNS_TYPE_AXFR				252 //Transfer of an entire zone
#define DNS_TYPE_ALL				255 //All cached records

//According RFC 3596:
#define DNS_TYPE_AAAA				28  //Host IPv6 address

//According RFC 1035, p 3.2.4 "CLASS values":
#define DNS_CLASS_IN				1   //Internet
#define DNS_CLASS_CH				3   //CHAOS class
#define DNS_CLASS_HS				4   //Hesiod [Dyer 87]
#define DNS_CLASS_ANY				255 //Any class

//Operation code (DNSHeader::OPCode).
//Codes 3-15 reserved for future use
//(According RFC 1035, p 4.1.1 "Header section format")
#define DNS_OPCODE_QUERY			0 //Standard query (QUERY)
#define DNS_OPCODE_IQUERY			1 //Inverse query (IQUERY)
#define DNS_OPCODE_STATUS			2 //Server status request (STATUS)

//Response code (DNSHeader::RCode)
//Codes 6-15 reserved for future use
//(According RFC 1035, p 4.1.1 "Header section format")
#define DNS_RCODE_OK				0 //No error condition
#define DNS_RCODE_FORMAT_ERROR		1 //Unable to interpret the query
#define DNS_RCODE_SERVER_FAILURE	2 //Problem with the name server
#define DNS_RCODE_NAME_ERROR		3 //Domain name does not exist
#define DNS_RCODE_NOT_IMPLEMENTED	4 //Kind of query does not supported
#define DNS_RCODE_REFUSED			5 //Refuses to perform operation


/* Represents header section at DNS message. 
   All fields at network bytes order.
   (According RFC 1035, p 4.1.1 "Header section format")*/
typedef struct tag_DNSHeader
{
	//Transaction ID
	u16 ID;
	
	//Recursion Desired
	u16 RD:1;
	
	//TrunCation (message was truncated)
	u16 TC:1;
	
	//Authoritative Answer
	u16 AA:1;
	
	//Kind of query (see DNS_OPCODE_x)
	u16 OPCode:4; 
	
	//Message is query/response (0/1)
	u16 QR:1;     

	//Response code (see DNS_RCODE_x)
	u16 RCode:4;
	
	//Reserved. Must be zero
	u16 Z:3;
	
	//Recursion Available
	u16 RA:1;

	//Number of entries in the question section
	u16 QDCount;

	//Number of RRs in the answer section
	u16 ANCount;
	
	//Number of name server RRs in the authority records section
	u16 NSCount;

	//Number of RRs in the additional records section
	u16 ARCount;

} DNSHeader;


/* Represents header section at LLMNR message. 
   All fields at network bytes order.
   (According RFC 4795, 2.1.1 "LLMNR Header Format")*/
typedef struct tag_LLMNRHeader
{
	//Transaction ID
	u16 ID;
	
	//Tentative
	u16 T:1;
	
	//TrunCation (message was truncated)
	u16 TC:1;
	
	//Conflict
	u16 C:1;
	
	//Kind of query (see DNS_OPCODE_x)
	u16 OPCode:4; 
	
	//Message is query/response (0/1)
	u16 QR:1;     

	//Response code (see DNS_RCODE_x)
	u16 RCode:4;
	
	//Reserved. Must be zero
	u16 Z:4;	

	//Number of entries in the question section
	u16 QDCount;

	//Number of RRs in the answer section
	u16 ANCount;
	
	//Number of name server RRs in the authority records section
	u16 NSCount;

	//Number of RRs in the additional records section
	u16 ARCount;

} LLMNRHeader;



/* Represents DNS resource record (RR) */
typedef struct tag_DNSResourceRecord
{
	//Name of the node to which this RR pertains
	char name[DNS_MAX_HOSTNAME_LENGTH];

	//Offset of start of name, bytes
	u16 namePtr;

	//Type code (see DNS_TYPE_x)
	u16 rtype;

	//Class code (see DNS_CLASS_x)
	u16 rclass;
	
	//Time interval that the resource record may be cached, sec
	u32 ttl;

	//Length of the RDATA field, bytes
	u16 rdLength;	

} DNSResourceRecord;



/* Gets host name part from domain name */
char *GetHostFromDName(const char *dname, char *host);

/* Gets domain part from domain name */
char *GetDomainFromDName(const char *dname, char *domain);

/* Add host name to domain and place result to output buffer */
char *ConstructDName(const char *host, const char *domain, char *dname);

/* Compare host name parts of two domain names */
s32 CompareHosts(const char *dname1, const char *dname2);

/* Compare domain parts of two domain names */
s32 CompareDomains(const char *dname1, const char *dname2);

/* Indicates that domain part exist at specified domain name */
u32 IsDomainExist(const char *dname);

/* Convert domain name to list of labels: [[length][label]]* */
s32 DNameToLabels(const char *dname, u8 *labels);

/* Convert list of labels to domain name string
   @buffer: pointer to buffer that contains DNS message;
   @offset: offset of label sequence at buffer, bytes;
   @length: length of DNS message, bytes */
char *LabelsToDName(const u8 *buffer, s32 *offset, s32 length, 
					char *dname);

/* Get label sequence length */
s32 GetLabelsLength(const char *dname);

/* Parse DNS question
   @buffer: pointer to buffer that contains DNS message;
   @offset: question offset at buffer, bytes;
   @length: length of DNS message, bytes;
   @q:      pointer to structure that contains parsed data */
s32 DNSQParse(u8 *buffer, s32 *offset, s32 length, 
			  DNSResourceRecord *q);

/* Parse DNS resource record
   @buffer: pointer to buffer that contains DNS message;
   @offset: resource record offset at buffer, bytes;
   @length: length of DNS message, bytes;
   @rr:     pointer to structure that contains parsed data */
s32 DNSRRParse(u8 *buffer, s32 *offset, s32 length, 			
			   DNSResourceRecord *rr);

/* Fill DNS resource record buffer
   @buffer: pointer to buffer to filled with specified data;
   @offset: offset at buffer, bytes;
   @length: length of DNS message, bytes;
   @rr:     pointer to structure that contains data */
s32 DNSRRFill(u8 *buffer, s32 *offset, s32 length, DNSResourceRecord *rr);

#endif /* __DNS_H */

/*===========================================================================
 * End of file DNS.h
 *===========================================================================*/

