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
 *  File swmodel_RTC.c: RTC functions.
 *
 *===========================================================================*/

#include "../../inc/RTC.h"

static RTCVALUE current_rtc_value = 0;

/* */
MCRESULT InitializeRTC(const RTCConfig *cfg)
{
	//Check configuration
	if (!cfg)
		return MCR_INVALID_CFG;

	return MCR_OK;
}



/* */
void TurnRTC(RTCBLOCK rtc, u8 turn)
{
}



/* */
void RTCWrite(RTCVALUE val)
{
    current_rtc_value = val;
}



/* Read RTC value */
RTCVALUE RTCRead(void)
{
    return current_rtc_value;
}



/* */
void BACKUP_IRQHandler(void)
{
}
 
/*===========================================================================
 * End of file swmodel_RTC.c
 *===========================================================================*/
