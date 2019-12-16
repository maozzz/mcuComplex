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
 *  File DateTime.c: Date/time functions.
 *
 *===========================================================================*/

#define _CRT_SECURE_NO_WARNINGS 1

#include "../../inc/DateTime.h"
#include "../../inc/stdcf.h"
#include <stdio.h>

#define DTM_BASE_YEAR           1970

#define DTM_SEC_DAY             (u32)86400      //Count of sec at day
#define DTM_SEC_YEAR            (u32)31536000   //Count of sec at year
#define DTM_SEC_LYEAR           (u32)31622400   //Count of sec at leap year


//Count of days at each month
const u8 dtMonthDays[12] = {  31, 28, 31,  30, 31, 30,
                              31, 31, 30,  31, 30, 31  };

DateTime *BinaryToDateTime(const u32 value, DateTime *dt)
{
        u32 secCnt = 0;
        u32 tmp;

        dt->year = DTM_BASE_YEAR;

        //Set month to 0 to use it as index
        //at dtMonthDays array
        dt->month = 0;

        //Year
        while(1)
        {
                tmp = (dt->year % 4) ? DTM_SEC_YEAR : DTM_SEC_LYEAR;

                if (secCnt + tmp > value)
                        break;

                secCnt += tmp;
                dt->year++;
        }

        //Month
        while(1)
        {
                //Now dt->month: [0..11]
                tmp = dtMonthDays[dt->month] * DTM_SEC_DAY;

                //if Feb (1) check leap year and add one day
                if (dt->month == 1 && !(dt->year % 4))
                        tmp += DTM_SEC_DAY;

                if (secCnt + tmp > value)
                        break;

                secCnt += tmp;
                dt->month++;
        }

        //Now dt->month: [0..11], set to: [1..12]
        dt->month++;

        //Day
        secCnt = value - secCnt;
        dt->day = (u8)(secCnt / DTM_SEC_DAY);

        //Hour
        secCnt -= dt->day * DTM_SEC_DAY;
        dt->hour = (u8)(secCnt / 3600);

        //Min
        secCnt -= dt->hour * 3600;
        dt->min = (u8)(secCnt / 60);

        //Sec
        dt->sec = (u16)(secCnt - dt->min * 60);

        //Now day 0..30, set 1..31
        dt->day++;

        return dt;
}




u32 DateTimeToBinary(const DateTime *dt, u32 *value)
{
        u32 cnt = DTM_BASE_YEAR;
        *value = 0;

        //Year
        while(cnt < dt->year)
        {
                *value += (cnt % 4) ? DTM_SEC_YEAR : DTM_SEC_LYEAR;
                cnt++;
        }

        //Month, Jan = 1
        cnt = 1;
        while(cnt < dt->month)
        {
                *value += dtMonthDays[cnt - 1] * DTM_SEC_DAY;
                cnt++;

                //if Feb (2) check leap year and add one day
                if (cnt < dt->month &&
                        cnt == 2 && !(dt->year % 4))
                        *value += DTM_SEC_DAY;
        }

        *value += (dt->day - 1) * DTM_SEC_DAY +
                                dt->hour * 3600 +
                                dt->min * 60 +
                                dt->sec;

        return *value;
}



char *DateTimeToString(char *txtBuffer, const DateTime *dt)
{
        if (!txtBuffer || !dt)
                return 0;

		std_sprintf(txtBuffer, "%04u-%02u-%02u %02u:%02u:%02u",
                        dt->year, dt->month, dt->day,
                        dt->hour, dt->min,   dt->sec);

        return txtBuffer;
}

/*===========================================================================
 * End of file DateTime.h
 *===========================================================================*/

