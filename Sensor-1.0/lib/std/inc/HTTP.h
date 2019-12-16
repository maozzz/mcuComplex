/*===========================================================================
 *
 *  Standard library
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2018
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File HTTP.h: HTTP (RFC 7231) definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __HTTP_H
#define __HTTP_H

#include "../../bastypes.h"
#include "ParseUtils.h"
#include "AuthDigest.h"
#include "IPv4.h"
#include "DNS.h"

/*===========================================================================
 * HTTP packet definitions
 *===========================================================================*/

//Default HTTP port (80)
#define HTTP_DEFAULT_PORT                        80

//HTTP protocol version
#define HTTP_PROTOVER_1p0                        "HTTP/1.0"
#define HTTP_PROTOVER_1p1                        "HTTP/1.1"
#define HTTP_PROTOVER_1p0_BCD                    10
#define HTTP_PROTOVER_1p1_BCD                    11

//HTTP methods (according RFC 7231, p 4.3 unless otherwise specified)
#define HTTP_MET_GET                             "GET"
#define HTTP_MET_POST                            "POST"
#define HTTP_MET_OPTIONS                         "OPTIONS"
#define HTTP_MET_HEAD                            "HEAD"
#define HTTP_MET_PUT                             "PUT"
#define HTTP_MET_DELETE                          "DELETE"
#define HTTP_MET_TRACE                           "TRACE"
#define HTTP_MET_CONNECT                         "CONNECT"
#define HTTP_MET_PATCH                           "PATCH"   //RFC 5789

//Requests indexes at httpRequests[] array
#define HTTP_IND_GET							0
#define HTTP_IND_POST							1
#define HTTP_IND_OPTIONS						2
#define HTTP_IND_HEAD							3
#define HTTP_IND_PUT                            4
#define HTTP_IND_DELETE                         5
#define HTTP_IND_TRACE							6
#define HTTP_IND_CONNECT						7
#define HTTP_IND_PATCH                          8

//Method names array
//extern cpcstr_t httpRequests[];

//HTTP status codes (according RFC 7231, p 6 unless otherwise specified).
//Informational 1xx:
#define HTTP_STS_100_CONTINUE                    100
#define HTTP_STS_101_SWITCHING_PROTOCOLS         101
//Successful 2xx:
#define HTTP_STS_200_OK                          200
#define HTTP_STS_201_CREATED                     201
#define HTTP_STS_202_ACCEPTED                    202
#define HTTP_STS_203_NON_AUTHORITATIVE_INFO      203
#define HTTP_STS_204_NO_CONTENT                  204
#define HTTP_STS_205_RESET_CONTENT               205
#define HTTP_STS_206_PARTIAL_CONTENT             206
//Redirection 3xx:
#define HTTP_STS_300_MULTIPLE_CHOICES            300
#define HTTP_STS_301_MOVED_PERMANENTLY           301
#define HTTP_STS_302_FOUND                       302
#define HTTP_STS_303_SEE_OTHER                   303
#define HTTP_STS_304_NOT_MODIFIED                304
#define HTTP_STS_305_USE_PROXY                   305
//#define HTTP_STS_306_x                         306 //Unused, code is reserved
#define HTTP_STS_307_TEMPORARY_REDIRECT          307
#define HTTP_STS_308_PERMANENT_REDIRECT          308 //RFC 7538, p 3
//Client Error 4xx:
#define HTTP_STS_400_BAD_REQUEST                 400
#define HTTP_STS_401_UNAUTHORIZED                401
#define HTTP_STS_402_PAYMENT_REQUIRED            402
#define HTTP_STS_403_FORBIDDEN                   403
#define HTTP_STS_404_NOT_FOUND                   404
#define HTTP_STS_405_METHOD_NOT_ALLOWED          405
#define HTTP_STS_406_NOT_ACCEPTABLE              406
#define HTTP_STS_407_PROXY_AUTH_REQUIRED         407
#define HTTP_STS_408_REQUEST_TIMEOUT             408
#define HTTP_STS_409_CONFLICT                    409
#define HTTP_STS_410_GONE                        410
#define HTTP_STS_411_LENGTH_REQUIRED             411
#define HTTP_STS_412_PRECONDITION_FAILED         412
#define HTTP_STS_413_REQUEST_ENTITY_TOO_LARGE    413
#define HTTP_STS_414_REQUEST_URI_TOO_LONG        414
#define HTTP_STS_415_UNSUPPORTED_MEDIA_TYPE      415
#define HTTP_STS_416_REQ_RANGE_NOT_SATISFIABLE   416
#define HTTP_STS_417_EXPECTATION_FAILED          417
#define HTTP_STS_418_I_AM_A_TEAPOT               418 //It's a joke
#define HTTP_STS_426_UPGRADE_REQUIRED            426
#define HTTP_STS_428_PRECONDITION_REQUIRED       428 //RFC 6585, p 3
#define HTTP_STS_429_TOO_MANY_REQUESTS           429 //RFC 6585, p 4
#define HTTP_STS_431_REQUEST_HDRFIELDS_TOO_LARGE 431 //RFC 6585, p 5
#define HTTP_STS_451_UNAVAIL_FOR_LEGAL_REASONS   451 //RFC 7725
//Server Error 5xx:
#define HTTP_STS_500_SERVER_INTERNAL_ERROR       500
#define HTTP_STS_501_NOT_IMPLEMENTED             501
#define HTTP_STS_502_BAD_GATEWAY                 502
#define HTTP_STS_503_SERVICE_UNAVAILABLE         503
#define HTTP_STS_504_GATEWAY_TIMEOUT             504
#define HTTP_STS_505_VERSION_NOT_SUPPORTED       505
#define HTTP_STS_511_NETWORK_AUTH_REQUIRED       511 //RFC 6585, p 6

//HTTP packet headers
#define HTTP_HDR_SERVER                         "Server"
#define HTTP_HDR_HOST                           "Host"
#define HTTP_HDR_CONTTYPE                       "Content-Type"
#define HTTP_HDR_CONTLENGTH                     "Content-Length"
#define HTTP_HDR_CONNECTION                     "Connection"

//#define HTTP_HDR_PROXY_AUTHENTICATE           "Proxy-Authenticate"
//#define HTTP_HDR_PROXY_AUTHORIZATION          "Proxy-Authorization"
#define HTTP_HDR_WWW_AUTHENTICATE               "WWW-Authenticate"
#define HTTP_HDR_AUTHORIZATION                  "Authorization"

//"Content-Type" header values
//(according RFC 2045, RFC 2046, RFC 4288, RFC 4289, RFC 4855)
#define HTTP_CTYPE_APP_XWWW_FORM_URLENCODED      "application/x-www-form-urlencoded"
#define HTTP_CTYPE_TEXT_HTML                     "text/html"
#define HTTP_CTYPE_TEXT_PLAIN                    "text/plain"
#define HTTP_CTYPE_IMG_PNG                       "image/png"
#define HTTP_CTYPE_IMG_SVG                       "image/svg+xml"


//Codes for "Connection" header (HTTPPacket::connection)
#define HTTP_CONN_CLOSE_CODE                     0
#define HTTP_CONN_KEEPALIVE_CODE                 1

//One line (header: value) buffer length
#define HTTP_LINE_BUFFER_LENGTH                  384

//Request method name maximum length, chars
#define HTTP_MAX_METHOD_NAME_LENGTH              12

//URI maximum length, chars
#define HTTP_MAX_URI_LENGTH                      64

//Content-Type maximum length, chars
#define HTTP_MAX_CONTTYPE_LENGTH                 52


//URL parameter name maximum length
#define HTTP_MAX_PARNAME_LENGTH                  32

//URL parameter value maximum length
#define HTTP_MAX_PARVALUE_LENGTH                 96


//HTTP user name maximum length, chars
#define HTTP_USERNAME_MAXLENGTH                  24

//HTTP user password maximum length, chars
#define HTTP_PASSWORD_MAXLENGTH                  24



//HTTP packet flags (HTTPPacket::flags)
//Indicate that parsed packet is SIP request
#define HTTP_FL_ISREQUEST						0x0001

//Indicates that request URI too long
#define HTTP_FL_REQUEST_URI_TOO_LONG            0x0002

//Indicates that request/response has unsupported version
#define HTTP_FL_VERSION_NOT_SUPPORTED			0x0004

//Indicates that "Authorization" header absent or
//authorization response is invalid
#define HTTP_FL_UNAUTHORIZED                    0x0008


/* One URL parameter (name/value) */
typedef struct tag_HTTPURLParams
{
    //Parameter name
    char name[HTTP_MAX_PARNAME_LENGTH];

    //Parameter value
    char value[HTTP_MAX_PARVALUE_LENGTH];

    //Flags
    u16 flags;

    //Current length
    u16 currlen;

    //Temp variable
    u16 tmp;
} HTTPURLParam;



/* User data */
typedef struct tag_HTTPUser
{
    //Username
    char name[HTTP_USERNAME_MAXLENGTH];

    //Password
    char password[HTTP_PASSWORD_MAXLENGTH];
} HTTPUser;



/* User auth parameters */
typedef struct tag_HTTPUserAuth
{
    //User name/password
    HTTPUser user;

    //Auth data
    AuthDigestRequest req;

    //Digest auth: stored "nc" value
    u32 nc;

    //User ID
    u16 uid;

    //Auth data expiration timer, sec
    u16 exptimer;
} HTTPUserAuth;


typedef struct tag_HTTPPacket	HTTPPacket;

typedef void (*HTTPURLParamHandler)(void *ctrl, HTTPURLParam *param);
typedef HTTPURLParamHandler			LPHTTPURLParamHandler;

typedef void (*HTTPAUTHHANDLER)(HTTPPacket *http,
                                const char *authScheme,
                                void *authParams,
                                u32 isAuthRequest);

typedef HTTPAUTHHANDLER			LPHTTPAUTHHANDLER;


/* Represents HTTP packet */
typedef struct tag_HTTPPacket
{
    //Request method (see HTTP_MET_x)
    char method[HTTP_MAX_METHOD_NAME_LENGTH];

    //URI+params (see "HTTPPacket::uriParams")
    char uri[HTTP_MAX_URI_LENGTH];

    //"Content-Type" header
    char contentType[HTTP_MAX_CONTTYPE_LENGTH];

    //Buffer of current line (request/response)
    char currLine[HTTP_LINE_BUFFER_LENGTH];

    //Pointer to start of URI parameters string ("HTTPPacket::uri")
    char *uriParams;

    //"Content-Length" header
    u32 contentLength;

    //Request/response line counter
    u16 lineCounter;

    //Response status code (see HTTP_STS_x)
    u16 status;

    //"Connection" header code (see HTTP_CONN_x)
    u8 connection;

    //HTTP version at bcd format: "HTTP/1.1" => 11
    u8 version;    

    //Packet flags (see HTTP_FL_x)
    u16 flags;

    //Pointer to user auth data
    HTTPUserAuth *uauth;

    //Handler that called when authentication header detected
    LPHTTPAUTHHANDLER authHandler;
} HTTPPacket;



/* Initialize URL parameters control structure */
void HTTP_InitializeUrlParam(HTTPURLParam *param);

/* */
const char *HTTP_GetURIParamByName(char *dest, const char *name, const char *uriParams);

/* Parse URL parameters */
s32 HTTP_ParseUrlParams(const char *buffer, u32 length,
                        void *ctrl,
                        HTTPURLParam *param,
                        HTTPURLParamHandler handler);

/* Parse URL parameters, last part */
s32 HTTP_ParseUrlParamsEnd(void *ctrl,
                           HTTPURLParam *param,
                           HTTPURLParamHandler handler);

/* Change URI of specified HTTP request */
s32 HTTPChangeURI(HTTPPacket *http, const char *newuri);

/* Copy one HTTP packet to another */
void HTTPCopyPacket(HTTPPacket *dest, const HTTPPacket *src);

/* Clear HTTP request structure with default values */
void ClearHTTPPacket(HTTPPacket *http);

/* Parse HTTP packet */
PARSLT HTTPParse(const char *buffer, u32 length,
                u32 *parseOffset, HTTPPacket *http);

#endif /* !__HTTP_H */

/*===========================================================================
 * End of file HTTP.h
 *===========================================================================*/
