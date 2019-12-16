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
 *  File IPv6.h: Internet Protocol version 6 (RFC 2460)
 *				 definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __IPV6_H
#define __IPV6_H

#include "../../bastypes.h"

/* Convert IPv6 address to it string representation */
char *IPv6ToStr(const u8 *ipv6, char *str);

#endif /* __IPV6_H */

/*===========================================================================
 * End of file IPv6.h
 *===========================================================================*/

