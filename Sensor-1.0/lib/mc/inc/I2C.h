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
 *  File I2C.h: I2C interface definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __I2C_H
#define __I2C_H

#include "types.h"
#include "CPU.h"

//I2C number
#define I2C_N1		0


//I2C options
#define I2C_OPTS_MASTER         0
#define I2C_OPTS_SLAVE          1

typedef u8 I2CBLOCK;

//Return codes
//Presence signal detected
#define I2C_RESULT_OK            0
//NACK received (no device with specified address)
#define I2C_RESULT_NACK         -1
//SCL and/or SDA at low state (possible short curcuit)
#define I2C_RESULT_BUSY         -2
//CRC error
#define I2C_RESULT_CRCERROR     -3


/* Represents I2C configuration */
typedef struct tag_I2CCOnfig
{
    //I2C block number (see I2C_Nx)
	I2CBLOCK i2c;

    //Requested speed, bit/s
    u32 speed;

    //Options (see I2C_OPTS_x)
    u32 opts;

    //Tx/Rx data completed
    void (*completed)(void);

    //(see I2Cx_MAP_x)
    u32 mappins;
} I2CConfig;


//I2C time counter, msecs
extern volatile u32 i2cTimerMs;


/* Update SDIO timers.
   Call this function with frequency 1000Hz */
__STATIC_INLINE void UpdateI2CTimers()
{
    i2cTimerMs++;
}


/* Initialize I2C controller */
MCRESULT InitializeI2C(const I2CConfig *cfg);

/* Turn I2C */
void TurnI2C(I2CBLOCK i2c, u8 turn);

/* Get I2C speed, bit/s */
u32 GetI2CSpeed(I2CBLOCK i2c);

/* Write data to selected I2C controller */
s32 I2CWrite(I2CBLOCK i2c, const void *src, s32 count);

/* Read data from selected I2C controller */
s32 I2CRead(I2CBLOCK i2c, void *dest, s32 count);

#endif /* __I2C_H */

/*===========================================================================
 * End of file I2C.h
 *===========================================================================*/

