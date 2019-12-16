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
 *  File swmodel_SSP.c: SSP functions.
 *
 *===========================================================================*/

#include "../../inc/SSP.h"
#include <string.h>

static SSPConfig sspConfigs[8];

/* */
MCRESULT InitializeSSP(const SSPConfig *cfg)
{	
    memcpy(&sspConfigs[(cfg->ssp)], cfg, sizeof(SSPConfig));
	return MCR_OK;
}



/* */
u32 GetSSPClk(SSPBLOCK ssp)
{
    if (sspConfigs[ssp].mode == SSP_MODE_SLAVE)
        return (sspConfigs[ssp].speed * 12);
    else
        return (sspConfigs[ssp].speed * 2);
}



/* */
u32 GetSSPSpeed(SSPBLOCK ssp)
{
    return sspConfigs[ssp].speed;
}



/* */
MCRESULT SSPSetSpeed(SSPBLOCK ssp, u32 speed)
{
    sspConfigs[ssp].speed = speed;
	return MCR_OK;
}



/* */
void TurnSSP(SSPBLOCK ssp, u8 turn)
{

}



/* */
void TurnSSPTx(SSPBLOCK ssp, u8 turn)
{

}



/* */
u8 IsSSPOutFifoFull(SSPBLOCK ssp)
{
	return 0;
}



/* */
void SSPInitOutFifo(SSPBLOCK ssp, u32 v)
{

}



/* */
void SSPFlush(SSPBLOCK ssp)
{

}



/* */
s32 SSPRead(SSPBLOCK ssp, void *dest, s32 count)
{
	return 0;
}



/* */
s32 SSPWrite(SSPBLOCK ssp, const void *src, s32 count)
{
	return 0;
}


u8 IsSSPOutFifoEmpty(SSPBLOCK ssp)
{
    return 1;
}



/* Indicates that SSP busy: tx/rx data or tx FIFO not empty */
u8 IsSSPBusy(SSPBLOCK ssp)
{
    return 0;
}



/* Read one element from SSP fifo */
s32 SSPReadFifo(SSPBLOCK ssp, u16 *data)
{
    *data = 0;
    return 1;
}



/* Write one element to SSP fifo */
s32 SSPWriteFifo(SSPBLOCK ssp, u16 data)
{
    return 1;
}

/*===========================================================================
 * End of file swmodel_SSP.c
 *===========================================================================*/
