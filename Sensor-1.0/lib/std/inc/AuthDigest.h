/*===========================================================================
 *
 *  Standard library
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
 *  File AuthDigest.h: Digest Access Authentication (RFC 2617) definitions 
 *                     and functions prototypes.
 *
 *===========================================================================*/

#ifndef __AUTHDIGEST_H
#define __AUTHDIGEST_H

#include "../../bastypes.h"

//Digest scheme name
#define AUTH_SCHEME_DIGEST				"Digest\0"

//Field names
#define AUTHD_FLD_REALM					"realm\0"
#define AUTHD_FLD_QOP					"qop\0"
#define AUTHD_FLD_NONCE					"nonce\0"
#define AUTHD_FLD_OPAQUE				"opaque\0"
#define AUTHD_FLD_STALE					"stale\0"
#define AUTHD_FLD_ALGORITHM				"algorithm\0"

#define AUTHD_FLD_USERNAME				"username\0"
#define AUTHD_FLD_URI					"uri\0"
#define AUTHD_FLD_NC					"nc\0"
#define AUTHD_FLD_CNONCE				"cnonce\0"
#define AUTHD_FLD_RESPONSE				"response\0"

//Field values
//"qop"
#define AUTHD_QOP_AUTH					"auth\0"
#define AUTHD_QOP_AUTHINT				"auth-int\0"
//"stale"
#define AUTHD_STALE_FALSE				"FALSE\0"
#define AUTHD_STALE_TRUE				"TRUE\0"
//"algorithm"
#define AUTHD_ALGORITHM_MD5				"MD5\0"
#define AUTHD_ALGORITHM_MD5SESS			"MD5-sess\0"

//Fields lengths
#define AUTHD_REALM_MAX_LENGTH			64
#define AUTHD_QOP_MAX_LENGTH			16
#define AUTHD_NONCE_MAX_LENGTH			64
#define AUTHD_OPAQUE_MAX_LENGTH			64
#define AUTHD_STALE_MAX_LENGTH			6
#define AUTHD_ALGORITHM_MAX_LENGTH		10

#define AUTHD_USERNAME_MAX_LENGTH		32
#define AUTHD_URI_MAX_LENGTH			64
#define AUTHD_CNONCE_MAX_LENGTH			64
#define AUTHD_RESPONSE_MAX_LENGTH		36


//"nonce" parameter length, chars [2..AUTHD_NONCE_MAX_LENGTH-2]
#ifndef AUTHD_NONCE_LENGTH
#define AUTHD_NONCE_LENGTH              20
#endif /* AUTHD_NONCE_LENGTH */

//"opaque" parameter length, chars [2..AUTHD_OPAQUE_MAX_LENGTH-2]
#ifndef AUTHD_OPAQUE_LENGTH
#define AUTHD_OPAQUE_LENGTH             20
#endif /* AUTHD_OPAQUE_LENGTH */

//"cnonce" parameter length, chars [2..AUTHD_CNONCE_MAX_LENGTH-2]
#ifndef AUTHD_CNONCE_LENGTH
#define AUTHD_CNONCE_LENGTH             8
#endif /* AUTHD_CNONCE_LENGTH */

//"nc" parameter maximum value (must greater than 1)
#ifndef HTTP_AUTHD_NC_MAXVALUE
#define HTTP_AUTHD_NC_MAXVALUE          20
#endif /* HTTP_AUTHD_NC_MAXVALUE */


#if ((AUTHD_NONCE_LENGTH + 2) > AUTHD_NONCE_MAX_LENGTH)
#error "Digest authorization: 'nonce' length too long. Check 'AUTHD_NONCE_LENGTH'"
#endif

#if ((AUTHD_OPAQUE_LENGTH + 2) > AUTHD_OPAQUE_MAX_LENGTH)
#error "Digest authorization: 'opaque' length too long. Check 'AUTHD_OPAQUE_LENGTH'"
#endif

#if ((AUTHD_CNONCE_LENGTH + 2) > AUTHD_CNONCE_MAX_LENGTH)
#error "Digest authorization: 'cnonce' length too long. Check 'AUTHD_CNONCE_LENGTH'"
#endif



/* Digest request (server -> request -> client) */
typedef struct tag_AuthDigestRequest
{
	//"realm" (required field)
	char realm[AUTHD_REALM_MAX_LENGTH];

	//"qop" (see AUTHD_QOP_x)
	char qop[AUTHD_QOP_MAX_LENGTH];

	//"nonce" (required field)
	char nonce[AUTHD_NONCE_MAX_LENGTH];

	//"opaque"
	char opaque[AUTHD_OPAQUE_MAX_LENGTH];
	
	//"stale" (see AUTHD_STALE_x)
	char stale[AUTHD_STALE_MAX_LENGTH];
	
	//"algorithm" (see AUTHD_ALGORITHM_x)
	char algorithm[AUTHD_ALGORITHM_MAX_LENGTH];

} AuthDigestRequest;



/* Digest response (client -> response -> server) */
typedef struct tag_AuthDigestResponse
{
	//"realm" (required field)
	char realm[AUTHD_REALM_MAX_LENGTH];

	//"qop" (see AUTHD_QOP_x)
	char qop[AUTHD_QOP_MAX_LENGTH];

	//"nonce" (required field)
	char nonce[AUTHD_NONCE_MAX_LENGTH];

	//"opaque"
	char opaque[AUTHD_OPAQUE_MAX_LENGTH];

	//"algorithm" (see AUTHD_ALGORITHM_x)
	char algorithm[AUTHD_ALGORITHM_MAX_LENGTH];

	//"username" (required field)
	char username[AUTHD_USERNAME_MAX_LENGTH];

	//"uri" (required field)
	char uri[AUTHD_URI_MAX_LENGTH];		

	//"cnonce"
	char cnonce[AUTHD_CNONCE_MAX_LENGTH];

	//"response" (required field)
	char response[AUTHD_RESPONSE_MAX_LENGTH];

	//"nc" (nonce-count 32-bit unsigned integer)
	u32 nc;	
} AuthDigestResponse;


/* */
void AuthDigestRequestFill(AuthDigestRequest *req,
							const char *realm,
							const char *requiredQop,
							const char *requiredAlg,							
							const char *opaque,
							const char *stale);


/* */
void AuthDigestResponseFill(const AuthDigestRequest *req,
							AuthDigestResponse *rsp,	
							const char *requiredQop,
							const char *httpmethod,	
							const char *uri,
							const char *username,
							const char *password,
							const char *entityBody);

/* */
char *AuthDigestRequestToString(char *strBuf, AuthDigestRequest *req);

/* */
char *AuthDigestResponseToString(char *strBuf, AuthDigestResponse *rsp);

#endif /* __AUTHDIGEST_H */

/*===========================================================================
 *  End of file AuthDigest.c
 *===========================================================================*/
