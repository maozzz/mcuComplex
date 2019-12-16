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
 *  File base64.h: Base-64 (RFC 1421) scheme encoder/decoder definitions and 
 * 		   functions prototypes.
 *
 *===========================================================================*/

#ifndef __BASE64_H
#define __BASE64_H

#include "../../bastypes.h"

//Comment to disable base-64 encoder/decoder tables
//(to reduce memory usage)
#define USE_BASE64_CODE_TABLES		1

/* Encode data to base-64 string */
s32 EncodeBase64(const u8 *buf, s32 length, u8 *b64buf);

/* Decode data from base-64 string */
s32 DecodeBase64(const u8 *b64buf, s32 length, u8 *buf);

#endif /* __BASE64_H */

/*===========================================================================
 * End of file base64.h
 *===========================================================================*/