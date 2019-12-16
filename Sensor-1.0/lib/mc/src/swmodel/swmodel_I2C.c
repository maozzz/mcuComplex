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
 *  File swmodel_I2C.c: I2C functions.
 *
 *===========================================================================*/

#include "../../inc/I2C.h"

/* Initialize I2C controller */
MCRESULT InitializeI2C(const I2CConfig *cfg)
{
    return MCR_OK;
}



/* Turn I2C */
void TurnI2C(I2CBLOCK i2c, u8 turn)
{

}



/* Get I2C speed, bit/s */
u32 GetI2CSpeed(I2CBLOCK i2c)
{
    return 0;
}



/* Write data to selected I2C controller */
s32 I2CWrite(I2CBLOCK i2c, const void *src, s32 count)
{
    return 0;
}



/* Read data from selected I2C controller */
s32 I2CRead(I2CBLOCK i2c, void *dest, s32 count)
{
    return 0;
}
 
/*===========================================================================
 * End of file swmodel_I2C.c
 *===========================================================================*/
