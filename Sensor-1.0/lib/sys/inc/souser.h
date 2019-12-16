/*===========================================================================
 *
 *  Simple Operation System (SimOS)
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
 *  File sysapi.h: SimOS user.
 *
 *===========================================================================*/

#ifndef __SOUSER_H
#define __SOUSER_H

#include "../../bastypes.h"

#define SOUSER_NAME_SIZE		32
#define SOUSER_HOMEDIR_SIZE		(16 + SOUSER_NAME_SIZE)

/* */
typedef struct tag_SimosUser
{
	/* User identifier */
	int uid;

	/* Group identifier */
	int gid;

	/* User name */
	char name[SOUSER_NAME_SIZE];

	/* Home directory */
	char home[SOUSER_HOMEDIR_SIZE];
} SimosUser;

#endif /* !__SOUSER_H */

/*===========================================================================
 *
 *===========================================================================*/
