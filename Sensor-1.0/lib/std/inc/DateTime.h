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
 *  File DateTime.h: Date/time definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __DATETIME_H
#define __DATETIME_H

#include "../../bastypes.h"

#define DATETIME_MIN_YEAR       1970
#define DATETIME_MAX_YEAR       2106

typedef struct tag_DateTime
{
        //Second [0..59]
        u32 sec:16;

        //Minute [0..59]
        u32 min:8;

        //Hour [0..23]
        u32 hour:8;

        //Day [1..31]
        u32 day:8;

        //Month [1..12]
        u32 month:8;

        //Year [DATETIME_MIN_YEAR .. DATETIME_MAX_YEAR]
        u32 year:16;

} DateTime;

/** Converts count of seconds since 1970-01-01 00:00:00
    to date and time */
DateTime *BinaryToDateTime(const u32 value, DateTime *dt);

/** Converts date and time to count of seconds
    since 1970-01-01 00:00:00 */
u32 DateTimeToBinary(const DateTime *dt, u32 *value);

/** Converts date and time to its string representation */
char *DateTimeToString(char *txtBuffer, const DateTime *dt);

#endif

/*===========================================================================
 * End of file DateTime.h
 *===========================================================================*/

