/*===========================================================================
 *
 *  Hardware library
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
 *  File SDCard-cmn.h: SD card common macros and functions prototypes
 *                     (for SSP and SDIO modes).
 *
 *===========================================================================*/

#ifndef __SDCARD_CMN_H
#define __SDCARD_CMN_H

#include "../../bastypes.h"

//Error codes
//Success
#define ESDOK						0
//Common error
#define ESDERROR					-1
//Response timeout
#define ESDRTMOUT					-2
//CRC error
#define ESDCRCERROR					-3
//Unusable SD card
#define ESDUNUSCARD					-4
//Not SD card
#define ESDNOTCARD					-5
//Incompatible voltage range
#define ESDINCVRANGE				-6
//Invalid data
#define ESDINVDATA					-7

/* According SD Physical Layer Simplified Specification Version 3.01,
   p 4.2.3:
 "The host shall set ACMD41 timeout more than 1000 ms to abort
  repeat of issuing ACMD41 when the card does not indicate ready" */
#define SD_ACMD41_TIMEOUT			1500 //msec


//Block size, bytes (must 512 bytes: 1 << 9)
#define SD_BLOCK_SIZE_BITS			9
#define SD_BLOCK_SIZE				(1 << SD_BLOCK_SIZE_BITS)

//CSD register length, bytes
#define SD_CSD_LENGTH				16

//CID register length, bytes
#define SD_CID_LENGTH				16

//MIDs (Manufacturer IDs) (CID register)
#define SD_CID_MID_APACER			0x28
#define SD_CID_MID_CHROMEBOOK		0x45
#define SD_CID_MID_CORSAIR			0x1D
#define SD_CID_MID_CRUCIAL			0x36
#define SD_CID_MID_KINGSTON1		0x27
#define SD_CID_MID_KINGSTON2		0x41
#define SD_CID_MID_PANASONIC		0x01
#define SD_CID_MID_PRETEC			0x88
#define SD_CID_MID_SAMSUNG			0x1B
#define SD_CID_MID_SANDISK			0x03
#define SD_CID_MID_TEAM				0x89
#define SD_CID_MID_TOSHIBA			0x02
#define SD_CID_MID_TRANSCEND1		0x74
#define SD_CID_MID_TRANSCEND2		0x1C
#define SD_CID_MID_TRANSCEND3		0x1E
#define SD_CID_MID_UNBRANDED		0x12
#define SD_CID_MID_WINTEC			0x6F

/* Convert CID register to string representaion */
char *SDCIDToString(const u8 *cid, char *dest);

/* Gets SD card capacity (bytes) using CSD register */
s32 SDCSDGetCapacity(const u8 *csd, u64 *capacity);

#endif /* !__SDCARD_CMN_H */

/*===========================================================================
 * End of file SDCard-cmn.h
 *===========================================================================*/
