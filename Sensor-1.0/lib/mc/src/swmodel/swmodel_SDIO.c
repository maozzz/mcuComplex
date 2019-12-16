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
 *  File swmodel_SDIO.c: SDIO functions.
 *
 *===========================================================================*/

#include "../../inc/SDIO.h"

volatile u32 sdioTimerMs = 0;

/* Initialize SDIO controller */
MCRESULT InitializeSDIO(const SDIOConfig *cfg)
{
    return MCR_OK;
}



/* Turn SDIO block */
void TurnSDIO(SDIOBLOCK sdio, u8 on)
{

}



/* Set SDIO speed (frequency on clock line) */
void SDIOSetSpeed(SDIOBLOCK sdio, u32 speed)
{

}



/* Get SDIO speed (frequency on clock line), Hz */
u32 GetSDIOSpeed(SDIOBLOCK sdio)
{
    return 0;
}



/* Tx 8 clocks, CMD line at high state */
void SDIOTxClock(SDIOBLOCK sdio)
{

}



/* Send command */
s32 SDIOWriteCommand(SDIOBLOCK sdio, u32 idcmd, u32 arg)
{
    return -1;
}



/* Read response */
s32 SDIOReadResponse(SDIOBLOCK sdio, u32 *dest, u32 bitlen, u32 tmout)
{
    return -1;
}



/* Read specified count of data */
s32 SDIORead(SDIOBLOCK sdio, void *dest, s32 count)
{
    return 0;
}



/* Write specified count of data */
s32 SDIOWrite(SDIOBLOCK sdio, const void *src, s32 count)
{
    return 0;
}

/*===========================================================================
 * End of file swmodel_SDIO.c
 *===========================================================================*/
