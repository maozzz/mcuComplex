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
 *  File SDIOCard.h: SD card macros and functions prototypes.
 *                   SDIO mode: command, clock and 4 data lines.
 *
 *===========================================================================*/

#ifndef __SDIOCARD_H
#define __SDIOCARD_H

#include "../../bastypes.h"
#include "../../mc/inc/CPU.h"
#include "../../mc/inc/SDIO.h"
#include "SDCard-cmn.h"

/* Control structure */
typedef struct SDIOCardCtrl
{
    //SDIO block number
    SDIOBLOCK sdio;

    //Assigned relative card address (RCA)
    u16 rca;

    //CCS bit: 1 - SDHC/SDXC; 0 - SDSC
    u16 ccs;

    //CID register value
    u8 cid[SD_CID_LENGTH];

    //CSD register value
    u8 csd[SD_CSD_LENGTH];
} SDIOCardCtrl;

/* Reset SD card and put it to data transfer mode (SD mode) */
s32 SDIOCardReset(SDIOCardCtrl *pcard);

/* */
s32 SDIOReadBlock(SDIOCardCtrl *pcard, u32 index, u8 *data);

/* */
s32 SDIOWriteBlock(SDIOCardCtrl *pcard, u32 index, const u8 *data);

#endif /* !__SDIOCARD_H */

/*===========================================================================
 * End of file SDIOCard.h
 *===========================================================================*/
