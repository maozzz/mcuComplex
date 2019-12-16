/*===========================================================================
 *
 *  MCU library
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2014-2016
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File RTC.h: Real time clock definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __RTC_H
#define __RTC_H

#include "types.h"
#include "CPU.h"

//RTC number
#define RTC_N1          0

typedef u8              RTCBLOCK;

/* Represents RTC configuration */
typedef struct tag_RTCCOnfig
{
        //RTC (see RTC_Nx)
        RTCBLOCK rtc;

        //Calibrating
        u16 cal;

        //RTC speed, Hz
        u32 speed;

        //RTC clock source (see CLKSRC_x)
        u32 clkSrc;

        //Tick handler
        void (*tick)(void);

} RTCConfig;


/* Initialize RTC */
MCRESULT InitializeRTC(const RTCConfig *cfg);

/* Turn ON/OFF RTC */
void TurnRTC(RTCBLOCK rtc, u8 turn);

/* Write RTC value */
void RTCWrite(RTCVALUE val);

/* Read RTC value */
RTCVALUE RTCRead(void);

#endif /* __RTC_H */

/*===========================================================================
 * End of file RTC.h
 *===========================================================================*/

