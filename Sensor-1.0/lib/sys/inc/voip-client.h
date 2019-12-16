/*===========================================================================
 *
 *  VoIP client library
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2015
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File voip-client.h: VoIP client definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __VOIP_CLIENT_H
#define __VOIP_CLIENT_H

#include <globaldef.h>
#include "../../bastypes.h"
#include "../../std/inc/MSession.h"
#include "../../std/inc/DNS.h"

/*===========================================================================
 * Call state
 *===========================================================================*/

//Call state flags (CallState::flags)
//Indicate that state represents incoming call
#define CLST_FL_INCOMING			0x01 
//Indicate that outgoing call was redirected
#define CLST_FL_REDIRECT			0x02
//Rise response timeout event 
#define CLST_FL_RISETIMEOUT			0x04
//Transport error
#define CLST_FL_TRANSPORTERROR		0x08

//Call states (CallState::state, CallState::state0)
//Initial states (CallState::state0)
#define CLST_REGISTER		0x0001 
#define CLST_OPTIONS		0x0002 
#define CLST_MESSAGE		0x0003 
#define CLST_INVITE			0x0004 
//Progress states
#define CLST_PROGRESS		0x0005
#define CLST_REDIRECT		0x0006
#define CLST_CANCEL			0x0007
#define CLST_CANCELED		0x0008
#define CLST_SUCCESS		0x0009
//End states
#define CLST_BYE			0x000A
#define CLST_FAILURE		0x000B
//Default state
#define CLST_DEFAULT		0x0080

#define CLST_CONTBUF_MAXLENGTH		(sizeof(MSession))

//Maximum count of redirections
#define CLST_MAX_REDIECT_COUNT		8

/* Represents in/out call state */
typedef struct tag_CallState
{
		//Flags (CLST_FL_x)
        u32 flags;

		//Pointer to media session parameters
        MSession *ses;

		//Content type
		const char *contentType;

		//Content length
		s32 contentLength;

		//Content buffer
		u32 content[(CLST_CONTBUF_MAXLENGTH >> 2) + 1];		

		//Retransmission interval, msec
		u32 retrmInterval;		      		

		//Index at call state array
        u16 index;

		//Redirect counter
		u16 redirCnt;

		//Retransmission index
		u8 retrmIndex;

		//Count of retransmissions
		u8 retrmCount;

		//Initial state (see CLST_x)
		u8 state0; 

        //Current state (see CLST_x)
        u8 state;                

		//Dialog parameters
		//Local CSeq header
		CSeq cSeq;

		//Last response code
		u16 respCode;

		//Destination port number (network order)
		u16 destPort;

		//Remote target
		TargetUri target;

		//Mandatory headers of SIP packet
		SIPMHeaders mhdr;	

		//Destination IP address or hostname
		char destHost[DNS_MAX_HOSTNAME_LENGTH];		
} CallState;

/*===========================================================================
 * VoIP client configuration
 *===========================================================================*/

//VoIP client options (VoIPClientConfig::opts)

//Basic options
//Disable all incoming calls
#define VOIPCLIENT_OPTS_IN_CALLS_DISABLE		0x0001
//Use subscriber name at "From" header
#define VOIPCLIENT_OPTS_USE_SUBSNAME			0x0002
//Answer on incoming calls automatically
#define VOIPCLIENT_OPTS_AUTO_ANSWER				0x0004

//Registration options
//Enable registration at server
#define VOIPCLIENT_OPTS_REGONSERVER				0x0008
//Enable outgoing calls without registration
#define VOIPCLIENT_OPTS_OUT_CALLS_WOREG			0x0010
//Enable incoming calls without registration
#define VOIPCLIENT_OPTS_IN_CALLS_WOREG			0x0020

//NAT Support options
//Handle "Via" "received"
#define VOIPCLIENT_OPTS_HANDLE_VIA_RECEIVED		0x0040
//Insert "Via" "received"
#define VOIPCLIENT_OPTS_INSERT_VIA_RECEIVED		0x0080
//Handle "Via" "rport"
#define VOIPCLIENT_OPTS_HANDLE_VIA_RPORT		0x0100
//Insert "Via" "rport"
#define VOIPCLIENT_OPTS_INSERT_VIA_RPORT		0x0200

//Default VoIP client options
#define VOIPCLIENT_OPTS_DEFAULT				( VOIPCLIENT_OPTS_OUT_CALLS_WOREG\
											| VOIPCLIENT_OPTS_IN_CALLS_WOREG )

//Other definitions
//Maximum length 
#define AUTH_USERNAME_LENGTH				24
#define AUTH_USERPWD_LENGTH					16

//Minimum expiration interval (registration), s
#define VOIPCLIENT_REGEXPIRES_MIN			5

//Maximum expiration interval (registration), s
#define VOIPCLIENT_REGEXPIRES_MAX			7200

//No answer timeout (default), s
#define VOIPCLIENT_NOANSWER_TIMEOUT			50

/* Represents VoIP client configuration.
   Note: all ip and port values at network byte order. */
typedef struct tag_VoIPClientConfig
{       		
		//Options (see VOIPCLIENT_OPTS_x)
        u32 opts;

		//External IP address
		u32 ipExt;

		//External port
        u16 portExt;

        //Local port
        u16 port;		

        //Server port
        u16 serverPort;
        
		//RTP port minimum
        u16 rtpPortMin;

        //RTP port maximum
        u16 rtpPortMax;		
		
		//External RTP port minimum
		u16 rtpPortMinExt;
                
		//Initial retransmission interval, msec
		u16 retrmInterval;
		
		//Count of retransmissions
		u16 retrmCount;

		//Registration retry interval, s
		u16 regRetry;

		//Registration expires, s
        u16 regExpires;    
		
		//No answer timeout (abonent unavailable), s
		u16 noAnsTimeout;

		//Preferred codec
        u8 codec;
        
		//Default frame duration, ms
        u8 frameDuration;				

		//Restricted Access Domains
        char restrictedDomains[DNS_MAX_HOSTNAME_LENGTH];

        //Server hostname or IP address
        char serverHost[DNS_MAX_HOSTNAME_LENGTH];

        //Subscriber name (shown name)
        char name[SIP_MAX_ABNAME_LENGTH];

        //SIP ID
        char sipId[SIP_MAX_SIPID_LENGTH];

        //User name (user authorization)
        char userName[AUTH_USERNAME_LENGTH];

        //User password (user authorization)
        char userPwd[AUTH_USERPWD_LENGTH];

		//Forward all incoming calls
		char fwdAorOnAll[SIP_MAX_AOR_LENGTH];

		//Forward incoming call when client is busy
		char fwdAorOnBusy[SIP_MAX_AOR_LENGTH];

		//Forward incoming call when client is no answered
		char fwdAorOnNoAnswer[SIP_MAX_AOR_LENGTH];

		//Pointer to User-Agent string
		const char *userAgent;
} VoIPClientConfig;

/*===========================================================================
 * VoIP client runtime state
 *===========================================================================*/

//Maximum incoming/outgoing calls at same time
#ifndef CLST_MAXCOUNT
#define CLST_MAXCOUNT							3
#endif /* !CLST_MAXCOUNT */

//VoIP client runtime options (VoIPClientRuntime::opts)
//Conference mode is set on
#define VOIPCLIENTRTM_OPTS_CONFERENCE			0x0001

//Default VoIP client runtime options
#define VOIPCLIENTRTM_OPTS_DEFAULT				(0)

//Authentication mode: Proxy/WWW (VoIPClientRuntime::authMode)
//(different header names used)
#define VOIPCLIENTRTM_AUTHMODE_PROXY			0
#define VOIPCLIENTRTM_AUTHMODE_WWW				1

//Count of authorizations attempts
#define VOIPCLIENTRTM_AUTH_ATTEMPT_COUNT		2

/* Represents VoIP client runtime state */
typedef struct tag_VoIPClientRuntime
{
		//Currently selected call
        CallState *selCall;

		//Call state array
        CallState callst[CLST_MAXCOUNT];        

		//Digest request parameters
		AuthDigestRequest authDigestRequest;

		//WWW/Proxy authentication mode (see VOIPCLIENTRTM_AUTHMODE_x)
		u8 authMode;

		//Auth attempts counter
		u8 authAttemptCnt;

		//Authorization result (2xx, 4xx codes)
		u16 authResult;		

		//Registration expires timer, msec
		u32 authExpires;        

		//Update state tick counter
		u32 tickCounter;

		//Update state interval, msec
        u16 timerInterval;

        //Call array options (see VOIPCLIENTRTM_OPTS_x)
        u16 opts;			

		//Total count of recieved packets
		u32 packetRxCnt;
		
		//Count of invalid packets
		u32 packetRxErrCnt;
		
		//Count of transmitted packets
		u32 packetTxCnt;			

		//Pointer to current forward aor
		char *fwdAor;

} VoIPClientRuntime;

/*===========================================================================
 * VoIP client user-defined handlers
 *===========================================================================*/

typedef void (*SIPPACKETHANDLER)(CallState *s, 
								 SIPPacket *sip, 
								 const void *content);

typedef void (*VOIPTRANERRORHANDLER)(CallState *s);

typedef SIPPACKETHANDLER	    LPSIPPACKETHANDLER;

typedef VOIPTRANERRORHANDLER    LPVOIPTRANERRORHANDLER;


/* Represents VoIP client handlers */
typedef struct tag_VoIPClientHandlers
{
		//Request handler
		LPSIPPACKETHANDLER      sipRequestHandler;

		//Response handler
		LPSIPPACKETHANDLER	    sipResponseHandler;		

		//Transmit error handler
		LPVOIPTRANERRORHANDLER  voIPTransmitErrorHandler;
		
} VoIPClientHandlers;

/*===========================================================================
 * Variables
 *===========================================================================*/

//Current VoIP client configuration
extern VoIPClientConfig   *voIPClient;

//Current VoIP client runtime state
extern VoIPClientRuntime  *voIPClientRtm;

//Vurrent VoIP client handlers
extern VoIPClientHandlers *voIPClientHandlers;

/*===========================================================================
 * Call control functions and definitions
 *===========================================================================*/

//Port search type (see GetFreePort function)
//Search free random port
#define FPORT_RANDOM							0
//Search first free port
#define FPORT_LINEAR							1

/* Initialize call state module and reset all states to default values */
void InitializeVoIPClient();

/* Reset call module to default state */
void ResetVoIPClient(void);

/* Reset stored auth parameters */
void ResetVoIPClientAuthParams();

/* Received packet handler. */
void VoIPPacketHandler(NetEndPoint *remEp, const void *buffer, u32 count);

/* Update call state timers.
   Call this function with interval 
   VoIPClientRuntime::timerInterval msec */
void UpdateVoIPClientTimers(void);

/* Update VoIP client state.
   Call once at main cycle */
void UpdateVoIPClientState();

/* Start register at server */
void ClientRegister();

/* Send instant message */
void SendIMessage(const char *uri, const char *message);

/* Get options of remote client */
void GetOptions(const char *uri);

/* Make outgoing call */
void MakeCall(const char *uri);

/* Answer to incoming call */
void AnswerCall(CallState *s);

/* End specified incoming/outgoing call */
void EndCall(CallState *s);

/* Search and end call using data from ICMP message */
void EndCallByICMPMessage(u8 *icmp, s32 length);

/* End all incoming/outgoing calls */
void EndCalls();


/* Find call state using specified remote and local end points */
CallState *FindCallState(NetEndPoint *locEp, NetEndPoint *remEp);

/* Find call state using specified Call-ID */
CallState *FindCallStateByCallID(char *callID);

/* Find call state using specified URI */
CallState *FindCallStateByURI(char *uri);

/* */
u8 IsFreeCallState(CallState *s);

/* */
u8 IsSelectedCallState(CallState *s);

/* Get free port at host byte order. 
   @searchType: port search type (see FPORT_x).
   return: port nuber at host byte order. */
u16 GetFreePort(u32 searchType);

/* Get free call state */
CallState *GetFreeCallState(void);

/* Get specified call state */
CallState *GetCallState(u8 callIndex);

/* Reset PT mapping */
void ResetRTPMapping(u8 *rtpMap);

/* Maps dynamic payload using specified encoder name */
void MapRTPPayload(MSession *ses, const char *encName, u8 dynPT);

/* Reset specified call state to it default values */
void ResetCallState(CallState *s);

/* Reset all call states to its default values */
void ResetCallStates();

/* Returns count of active calls */
u32 GetActiveCallsCount();

/* Select next active call and return pointer to it or
   retutn 0 whether no active calls found  */
CallState *SelectActiveCall(s32 selDir);

#endif /* __VOIP_CLIENT_H */

/*===========================================================================
 * End of file voip-client.h
 *===========================================================================*/
