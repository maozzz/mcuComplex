/*===========================================================================
 *
 *  MCU library
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2019
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File swmodel_IWire.c: 1-Wire functions.
 *
 *===========================================================================*/

#include "../../inc/IWire.h"

/* Initialize 1-Wire */
MCRESULT InitializeIWire(const IWireConfig *cfg)
{
    return MCR_OK;
}



/* Turn 1-Wire ON/OFF */
void TurnIWire(IWIREBLOCK iwire, u8 on)
{

}



/* Transmit "reset" and try detect "presence" singal.
   Returns: one of "IWIRE_RESULT_x" values */
s32 IWireResetPresence(IWIREBLOCK iwire)
{
    return IWIRE_RESULT_NOPRESENCE;
}



/* Transmit one byte */
s32 IWireWriteByte(IWIREBLOCK iwire, u8 val)
{
    return 0;
}



/* Read one byte */
u8 IWireReadByte(IWIREBLOCK iwire)
{
    return 0;
}



/* Transmit specified count of bytes */
s32 IWireWrite(IWIREBLOCK iwire, const void *src, s32 n)
{
    return 0;
}



/* Read specified count of bytes */
s32 IWireRead(IWIREBLOCK iwire, void *dest, s32 n)
{
    return 0;
}

/*===========================================================================
 * End of file swmodel_IWire.c
 *===========================================================================*/
