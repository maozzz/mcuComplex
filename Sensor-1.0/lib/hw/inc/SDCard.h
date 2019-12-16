/*===========================================================================
 *
 *  Hardware library
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2016
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File SDCard.h: SD card macros and functions prototypes.
 *                 SSP mode: tx, rx, clock lines.
 *
 *===========================================================================*/

#ifndef __SDCARD_H
#define __SDCARD_H

#include "../../bastypes.h"
#include "../../mc/inc/CPU.h"
#include "SDCard-cmn.h"
#include <targetsd.h>

//SD card response timeout, ms
#ifndef SD_RESP_TIMEOUT
#define SD_RESP_TIMEOUT				500
#endif /* !SD_RESP_TIMEOUT */

//SD card read data timeout, ms
#ifndef SD_READ_TIMEOUT
#define SD_READ_TIMEOUT				800
#endif /* !SD_READ_TIMEOUT */

//SD card read write timeout, ms
#ifndef SD_WRITE_TIMEOUT
#define SD_WRITE_TIMEOUT			800
#endif /* !SD_READ_TIMEOUT */

//The number of attempts to reset SD card
#ifndef SD_COUNT_RESET_ATTEMPTS
#define SD_COUNT_RESET_ATTEMPTS		3
#endif /* !SD_COUNT_RESET_ATTEMPTS */

//The number of attempts to read data
#ifndef SD_COUNT_READ_ATTEMPTS
#define SD_COUNT_READ_ATTEMPTS		8
#endif /* !SD_COUNT_READ_ATTEMPTS */

//The number of attempts to write data
#ifndef SD_COUNT_WRITE_ATTEMPTS
#define SD_COUNT_WRITE_ATTEMPTS		8
#endif /* !SD_COUNT_WRITE_ATTEMPTS */


//Host options (SDHostConfig::opts)
//High Capacity Support (HCS) enabled
#define SD_HOST_OPTS_HCS			0x01

/* Represents SD host configuration */
typedef struct tag_SDHostConfig
{
	//Timer speed, Hz
	u32 timSpeed;

	//Initial SSP speed, bit/s
	u32 sspSpeed;

	//Options (see SD_HOST_OPTS_x)
	u32 opts;
} SDHostConfig;


/* Update timers. Call with speed SDHostConfig::timSpeed */
void UpdataSDCardTimers();

/* Low-level initialization (MCU port/SPI) */
s32 InitializeSDCard(SDHostConfig *cfg);

/* Put SD card to data transfer mode */
s32 SDCardReset();

/* Reads CID register to specified buffer */
s32 SDReadCIDRegister(u8 *regbuf);

/* Reads CID register and converts it to readable string */
s32 SDReadCIDString(char *dest);

/* Reads CSD register to specified buffer */
s32 SDReadCSDRegister(u8 *regbuf);

/* Gets SD card serial number */
s32 SDGetSerialNumber(u32 *snumber);

/* Gets SD card capacity (bytes) */
s32 SDGetCapacity(u64 *capacity);

/* Gets SD card maximum i/o speed (bit/s) */
s32 SDGetMaxSpeed(u32 *speed);

/* Sets block size */
s32 SDSetBlockSize(s32 size);

/* Gets block size */
s32 SDGetBlockSize();

/* Read block with specified index from SD card */
s32 SDReadBlock(u32 index, u8 *data);

/* Write block with specified index to SD card */
s32 SDWriteBlock(u32 index, const u8 *data);

#endif /* !__SDCARD_H */

/*===========================================================================
 * End of file SDCard.h
 *===========================================================================*/
