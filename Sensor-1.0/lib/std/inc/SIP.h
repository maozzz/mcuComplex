/*===========================================================================
 *
 *  Standard library
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2015-2018
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File SIP.h: SIP packet (RFC 3261) definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __SIP_H
#define __SIP_H

#include "../../bastypes.h"
#include "ParseUtils.h"
#include "AuthDigest.h"
#include "IPv4.h"
#include "DNS.h"

/*===========================================================================
 * SIP packet definitions
 *===========================================================================*/

//Default SIP port
#define SIP_DEFAULT_PORT                        5060

//SIP protocol version
#define SIP_PROTOVER                            "SIP/2.0\0"
#define SIP_PROTOVER_BCD                        20

//SIP requests names
//Basic requests defined at RFC 3261
#define SIP_REQ_INVITE                          "INVITE\0"
#define SIP_REQ_ACK                             "ACK\0"
#define SIP_REQ_BYE                             "BYE\0"
#define SIP_REQ_CANCEL                          "CANCEL\0"
#define SIP_REQ_OPTIONS                         "OPTIONS\0"
#define SIP_REQ_REGISTER                        "REGISTER\0"
//Additional requests
#define SIP_REQ_INFO							"INFO\0"      //RFC 2976
#define SIP_REQ_PRACK							"PRACK\0"     //RFC 3262
#define SIP_REQ_SUBSCRIBE                       "SUBSCRIBE\0" //RFC 3265
#define SIP_REQ_NOTIFY							"NOTIFY\0"    //RFC 3265
#define SIP_REQ_UPDATE							"UPDATE\0"    //RFC 3311
#define SIP_REQ_MESSAGE                         "MESSAGE\0"   //RFC 3428
#define SIP_REQ_REFER							"REFER\0"     //RFC 3515
#define SIP_REQ_PUBLISH                         "PUBLISH\0"   //RFC 3903

//Requests indexes at sipRequests[] array
#define IND_INVITE								0   
#define IND_ACK									1
#define IND_BYE									2
#define IND_CANCEL								3
#define IND_OPTIONS								4
#define IND_REGISTER							5
#define IND_INFO								6
#define IND_PRACK								7
#define IND_SUBSCRIBE							8
#define IND_NOTIFY								9
#define IND_UPDATE								10
#define IND_MESSAGE								11
#define IND_REFER								12
#define IND_PUBLISH								13

//Request (method) names array
extern cpcstr_t sipRequests[];

//SIP response codes (according RFC 3261, pp 21.1 - 21.6).
//Provisional responses 1xx:
#define SIP_RSP_100_TRYING                      100
#define SIP_RSP_180_RINGING                     180
#define SIP_RSP_181_CALL_IS_FORWARDED           181
#define SIP_RSP_182_QUEUED                      182
#define SIP_RSP_183_SESSION_PROGRESS            183
//Successful responses 2xx:
#define SIP_RSP_200_OK                          200
#define SIP_RSP_202_ACCEPTED_RESPONSE           202
//Redirection responses 3xx:
#define SIP_RSP_300_MULTIPLE_CHOICES            300
#define SIP_RSP_301_MOVED_PERMANENTLY           301
#define SIP_RSP_302_MOVED_TEMPORARILY           302
#define SIP_RSP_305_USE_PROXY                   305
#define SIP_RSP_380_ALTERNATIVE_SERVICE         380
//Request failure responses 4xx:
#define SIP_RSP_400_BAD_REQUEST                 400
#define SIP_RSP_401_UNAUTHORIZED                401
#define SIP_RSP_402_PAYMENT_REQUIRED            402
#define SIP_RSP_403_FORBIDDEN                   403
#define SIP_RSP_404_NOT_FOUND                   404
#define SIP_RSP_405_METHOD_NOT_ALLOWED          405
#define SIP_RSP_406_NOT_ACCEPTABLE              406
#define SIP_RSP_407_PROXY_AUTH_REQUIRED         407
#define SIP_RSP_408_REQUEST_TIMEOUT             408
#define SIP_RSP_410_GONE                        410
#define SIP_RSP_413_REQUEST_ENTITY_TOO_LARGE    413
#define SIP_RSP_414_REQUEST_URI_TOO_LONG        414
#define SIP_RSP_415_UNSUPPORTED_MEDIA_TYPE      415
#define SIP_RSP_416_UNSUPPORTED_URI_SCHEME      416
#define SIP_RSP_420_BAD_EXTENSION               420
#define SIP_RSP_421_EXTENSION_REQUIRED          421
#define SIP_RSP_423_INTERVAL_TOO_BRIEF          423
#define SIP_RSP_480_TEMP_UNAVAILABLE            480
#define SIP_RSP_481_CALL_DOES_NOT_EXIST         481
#define SIP_RSP_482_LOOP_DETECTED               482
#define SIP_RSP_483_TOO_MANY_HOPS               483
#define SIP_RSP_484_ADDRESS_INCOMPLETE          484
#define SIP_RSP_485_AMBIGUOUS                   485
#define SIP_RSP_486_BUSY_HERE                   486
#define SIP_RSP_487_REQUEST_TERMINATED          487
#define SIP_RSP_488_NOT_ACCEPTABLE_HERE         488
#define SIP_RSP_489_EVENT_PACK_NOT_SUPPORTED	489
#define SIP_RSP_491_REQUEST_PENDING             491
#define SIP_RSP_493_UNDECIPHERABLE              493
//Server failure responses 5xx:
#define SIP_RSP_500_SERVER_INTERNAL_ERROR       500
#define SIP_RSP_501_NOT_IMPLEMENTED             501
#define SIP_RSP_502_BAD_GATEWAY                 502
#define SIP_RSP_503_SERVICE_UNAVAILABLE         503
#define SIP_RSP_504_SERVER_TIMEOUT              504
#define SIP_RSP_505_VERSION_NOT_SUPPORTED       505
#define SIP_RSP_513_MESSAGE_TOO_LARGE           513
//Global failures responses 6xx:
#define SIP_RSP_600_BUSY_EVERYWHERE             600
#define SIP_RSP_603_DECLINE                     603
#define SIP_RSP_604_DOES_NOT_EXIST_ANYWHERE     604
#define SIP_RSP_606_NOT_ACCEPTABLE              606


//SIP packet headers
#define SIP_HDR_VIA                             "Via\0"
#define SIP_HDR_FROM                            "From\0"
#define SIP_HDR_TO                              "To\0"
#define SIP_HDR_CALLID                          "Call-ID\0"
#define SIP_HDR_CSEQ                            "CSeq\0"
#define SIP_HDR_CONTTYPE                        "Content-Type\0"
#define SIP_HDR_CONTLENGTH                      "Content-Length\0"
#define SIP_HDR_MAXFORWARDS                     "Max-Forwards\0"
#define SIP_HDR_CONTACT                         "Contact\0"
#define SIP_HDR_ALLOW							"Allow\0"
#define SIP_HDR_USERAGENT						"User-Agent\0"
#define SIP_HDR_ACCEPT							"Accept\0"
#define SIP_HDR_SUPPORTED						"Supported\0"
#define SIP_HDR_EXPIRES							"Expires\0"
#define SIP_HDR_TIMESTAMP						"Timestamp\0"
#define SIP_HDR_RETRYAFTER						"Retry-After\0"
//Authorization headers at 407 response
#define SIP_HDR_PROXY_AUTHENTICATE              "Proxy-Authenticate\0"  //Request
#define SIP_HDR_PROXY_AUTHORIZATION             "Proxy-Authorization\0" //Answer 

//SIP transport protocol names
#define SIP_TRANPROTO_UDP                       "UDP\0"
#define SIP_TRANPROTO_TCP                       "TCP\0"
#define SIP_TRANPROTO_TLS                       "TLS\0"

#define SIP_TRANPROTO_UDPl                      "udp\0"
#define SIP_TRANPROTO_TCPl                      "tcp\0"
#define SIP_TRANPROTO_TLSl                      "tls\0"

//SIP URI schemes
extern cpcstr_t SIP_URISCHEME_SIP ;
extern cpcstr_t SIP_URISCHEME_SIPS;
extern cpcstr_t SIP_URISCHEME_TEL ;

//Branch "magic" sequence: "z9hG4bK"
#define SIP_BRANCH_MAGIC                        "z9hG4bK\0"

//Content types
extern cpcstr_t SIP_CONTTYPE_SDP      ;
extern cpcstr_t SIP_CONTTYPE_TEXTPLAIN;

//Default abonent shown name
#define SIP_ABNAME_DEFAULT                      "noname\0"

//Length restrictions
#define SIP_MAX_TRANPROTO_LENGTH                4

#define SIP_MAX_BRANCH_LENGTH                   48
#define SIP_MAX_RECEIVED_LENGTH                 16

#define SIP_MAX_REQUEST_NAME_LENGTH             12
#define SIP_MAX_PROTONAME_LENGTH                12

//Max 'Via' headers count
#define SIP_MAX_HOPS                            4

#define SIP_MAX_CALLID_RANDOM_LENGTH			36
#define SIP_MAX_CALLID_LENGTH                   (SIP_MAX_CALLID_RANDOM_LENGTH +\
												 DNS_MAX_HOSTNAME_LENGTH)

#define SIP_MAX_ABNAME_LENGTH                   20
#define SIP_MAX_SIPID_LENGTH                    20
#define SIP_MAX_SIPDOMAIN_LENGTH                DNS_MAX_HOSTNAME_LENGTH

#define SIP_MAX_AOR_LENGTH                      (SIP_MAX_SIPID_LENGTH +\
                                                 SIP_MAX_SIPDOMAIN_LENGTH + 2)

#define SIP_MAX_URI_LENGTH                      SIP_MAX_AOR_LENGTH

#define SIP_MAX_URISCHEME_LENGTH                8
#define SIP_MAX_TAG_LENGTH                      36

#define SIP_MAX_CONTTYPE_LENGTH                 32

//Packet flags (SIPPacket::flags)
//Indicate that parsed packet is SIP request
#define SIP_FL_ISREQUEST						0x0001
//Warning flags
#define SIP_FL_REQUEST_URI_TOO_LONG				0x0002
#define SIP_FL_BAD_CONTENT_LENGTH				0x0004
#define SIP_FL_VERSION_NOT_SUPPORTED			0x0008

/*===========================================================================
 * SIP timer values
 *===========================================================================*/

//RTT Estimate (default value), ms 
#define SIP_TIM_T1								500

//The maximum retransmit interval for non-INVITE requests 
//and INVITE responses, ms
#define SIP_TIM_T2								4000 

//Maximum duration a message will remain in the network, ms
#define SIP_TIM_T4								5000 

//INVITE request retransmit interval, for UDP only (initially T1), ms
#define SIP_TIM_A								SIP_TIM_T1 

//INVITE transaction timeout timer, ms
#define SIP_TIM_B(t1)							(64*(t1))

//Proxy INVITE transaction timeout (> 3min), ms
#define SIP_TIM_C								190000 

//Wait time for response retransmits (> 32s for UDP, 0s for TCP/SCTP), ms
#define SIP_TIM_D								35000  
 
//Non-INVITE request retransmit interval, UDP only (initially T1), ms
#define SIP_TIM_E								SIP_TIM_T1

//Non-INVITE transaction timeout timer, ms
#define SIP_TIM_F(t1)							(64*(t1))  

//INVITE response retransmit interval (initially T1), ms
#define SIP_TIM_G								SIP_TIM_T1

//Wait time for ACK receipt, ms
#define SIP_TIM_H(t1)							(64*(t1))  

//Wait time for ACK retransmits (T4 for UDP, 0s for TCP/SCTP), ms
#define SIP_TIM_I(t4)							(t4)

//Wait time for for non-INVITE request retransmits 
//(64*T1 for UDP, 0s for TCP/SCTP), ms
#define SIP_TIM_J(t1)							(64*(t1))

//Wait time for response retransmits (T4 for UDP, 0s for TCP/SCTP), ms
#define SIP_TIM_K(t4)							(t4)


/*===========================================================================
 * SIP structures
 *===========================================================================*/

//Flags of "Via" header
//Indicate that "Via" header has "rport" parameter
#define SIP_FL_VIA_RPORT				1
//Indicate that "Via" header has "received" parameter
#define SIP_FL_VIA_RECEIVED				2

/* Represents "Via" header parameters */
typedef struct tag_Via
{
		//Header flags (see SIP_FL_VIA_x)
		u32 flags;

		//Parameter "received" (source IP address, network bytes order)
        u32 received;

		//Parameter "rport" (host byte order)
		u16 rport;

        //Port number (host byte order)
        u16 port;

        //Transport protocol name (without sip version)
        //["UDP", "TCP", "TLS"]
        char tranProto[SIP_MAX_TRANPROTO_LENGTH];

        //DNS name or IP address of SIP node
        char host[DNS_MAX_HOSTNAME_LENGTH];

        //Parameter "branch"
        char branch[SIP_MAX_BRANCH_LENGTH];		     
} Via;



/* Represents "From", "To", "Contact" header parameters */
typedef struct tag_Abonent
{
        //Shown abonent name
        char name[SIP_MAX_ABNAME_LENGTH];

		//URI scheme (see SIP_URISCHEME_x)
        char uriScheme[SIP_MAX_URISCHEME_LENGTH];

        //URI: id@domain
        char uri[SIP_MAX_URI_LENGTH];        

        //Tag value
        char tag[SIP_MAX_TAG_LENGTH];

        //Used at "Contact" header (see SIP_TRANPROTO_x)
        char tranProto[SIP_MAX_TRANPROTO_LENGTH];

        //Port number
        u16 port;
} Abonent;



/* Represents "CSeq" header parameters */
typedef struct tag_CSeq
{
        //Sequence number
        u32 number;

        //Request name (see SIP_REQ_x)
        char request[SIP_MAX_REQUEST_NAME_LENGTH];
} CSeq;



/* Represents "Content-Type" header parameters */
typedef char      ContentType;



/* Represents "Call-ID" header parameters */
typedef char      CallID;



/* Represents target uri (request only) */
typedef struct tag_TargetUri
{
		//URI scheme: ["sip", "sips", "tel"]
		char    uriScheme[SIP_MAX_URISCHEME_LENGTH];

		//URI string: name@domain
		char    uri[SIP_MAX_URI_LENGTH];

		//Port number
		u16     port;
} TargetUri;



/* Represents mandatory headers of SIP packet */
typedef struct tag_SIPMHeaders
{	
		//Count of "Via" headers
		s32     viaCount;

		//"Via" headers
		Via     via[SIP_MAX_HOPS];	

		//"From" header
		Abonent from;

		//"To" header
		Abonent to;

		//"CSeq" header
		CSeq    cSeq;    	

		//"Call-ID" header
		CallID  callID[SIP_MAX_CALLID_LENGTH];		
} SIPMHeaders;



typedef struct tag_SIPPacket	SIPPacket;

typedef void (*SIPAUTHHANDLER)(SIPPacket *sip, 
								const char *authScheme, 
								void *authParams, 
								u32 isAuthRequest);

typedef SIPAUTHHANDLER			LPSIPAUTHHANDLER;


/* Represents SIP packet */
typedef struct tag_SIPPacket
{		
        //Uri line, first line at packet
		TargetUri target;        

		//Mandatory headers
		SIPMHeaders mhdr;
		       
		//Additional headers
        //"Contact" header
        Abonent     contact;

        //"Content-Type" header ("application/sdp")
        ContentType contentType[SIP_MAX_CONTTYPE_LENGTH];		

		//Request name (request only, see SIP_REQ_x)
        char		request[SIP_MAX_REQUEST_NAME_LENGTH];

		//Response code (response only, see SIP_RSP_x)
        u16			respCode;

		//SIP version at bcd format: "SIP/2.0" => 20
        u16			version;

		//"Content-Length" header
        u32         contentLength;

		//"Max-Forwards" header
        u32         maxForwards;

		//"Expires" header
		u32			expires;

		//"Retry-After" header
		u32			retryAfter;

		//"Timestamp" header
		u32			timestamp;					

		//Parse flags (see SIP_FL_x)
		u32			flags;

		//Handler that called when authentication header detected
        LPSIPAUTHHANDLER authHandler;
} SIPPacket;


/*===========================================================================
 * Parse functions
 *===========================================================================*/

/* Clear SIP request structure with default values */
void ClearSIPRequest(SIPPacket *sip);

/* Parse SIP request from specified buffer. */
PARSLT SIPParse(const char *buffer, u32 length, 
				u32 *parseOffset, SIPPacket *sip);

/* Skip all by specified char */
PARSLT SIPParse_SkipByChar(ParseContext *cont, char skch);

/* Get index of specified request */
s32 GetRequestIndex(const char *request);

/*===========================================================================
 * Construct functions
 *===========================================================================*/

/* Gets id part from URI string */
char *GetURIId(char *id, const char *uri);

/* Gets address part from URI string */
char *GetURIAddr(char *addr, const char *uri);

/* Gets port number from URI string */
u16 GetURIPort(const char *uri);

/* Gets full URI from specified SIP ID, domain name and server hostname */
char *GetFullURI(char *dest,
                 const char *uri,
                 const char *domainName,
                 const char *serverHost);


/* Get default reason phrase using specified response code */
const char *GetReasonPhrase(u16 respCode);

/* Fill Request line */
void SIP_RequestLine_Fill(char *strBuf, 
				         const char *request, 
						 const char *uriScheme, 
						 const char *uri, u16 port);

/* Fill Request line using specified request name and target uri */
void SIP_RequestLine_ToString(char *strBuf, 
				              const char *request, 
						      TargetUri *tu);

/* Fill Response line */
void SIP_ResponseLine_Fill(char *strBuf, 
						   u16 respCode, 
						   const char *reasonPhrase);

/* Convert array of Via structures to it string representation */
void SIPHdr_Via_ToString(char *strBuf, Via *via, s32 count);

/* Convert From structure to "From" header string representation */
void SIPHdr_From_ToString(char *strBuf, Abonent *from);

/* Convert To structure to "To" header string representation */
void SIPHdr_To_ToString(char *strBuf, Abonent *to);

/* Convert Contact structure to "Contact" header string representation */
void SIPHdr_Contact_ToString(char *strBuf, Abonent *contact);

/* Convert CallID structure to "Call-ID" header string representation */
void SIPHdr_CallID_ToString(char *strBuf, CallID *callid);

/* Convert CSeq structure to "CSeq" header string representation */
void SIPHdr_CSeq_ToString(char *strBuf, CSeq *cSeq);

/* Fill "CSeq" header with specified sequence number and request name */
void SIPHdr_CSeq_Fill(char *strBuf, u32 seqNumber, const char *request);

/* Fill "Content-Type" header with specified content type */
void SIPHdr_ContentType_Fill(char *strBuf, const char *contType);

/* Fill "Accept" header with specified list of content types */
void SIPHdr_Accept_Fill(char *strBuf, const char *contTypes);

/* Fill "User-Agent" header with specified user-agent name */
void SIPHdr_UserAgent_Fill(char *strBuf, const char *userAgent);

/* Fill "Supported" header with specified list of extensions */
void SIPHdr_Supported_Fill(char *strBuf, const char *extensions);

/* Fill "Allow" header using mask of allowed methods */
void SIPHdr_Allow_Fill(char *strBuf, u32 allowMask);

/* Fill "Content-Legth" header with specified content length */
void SIPHdr_ContentLength_Fill(char *strBuf, u32 length);

/* Fill "Max-Forwards" header with specified number of forwards */
void SIPHdr_MaxForwards_Fill(char *strBuf, u32 maxfwd);

/* Fill "Expires" header with specified value */
void SIPHdr_Expires_Fill(char *strBuf, u32 expires);

/* Fill "Timestamp" header with specified value */
void SIPHdr_Timestamp_Fill(char *strBuf, u32 timestamp);

/* Fill "Retry-After" header with specified value */
void SIPHdr_RetryAfter_Fill(char *strBuf, u32 retryAfter);

/*===========================================================================
 * Random strings generators
 *===========================================================================*/

/* Generate branch sequence */
char *GenerateBranch(char *branchBuf);

/* Generate tag sequence */
char *GenerateTag(char *tagBuf);

#endif /* __SIP_H */

/*===========================================================================
 * End of file SIP.h
 *===========================================================================*/
