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
 *  File usbmsd.h: USB Mass-Storage class, SCSI subclass (HOST SIDE).
 *                 To get device side sample see file
 *                 "lib/docs/samples/sample-usbmsd-v1-main.c"
 *
 *===========================================================================*/

#ifndef __USBMSD_H
#define __USBMSD_H

#include "../../bastypes.h"
#include "../../std/inc/SCSI.h"
#include "../../mc/inc/USB_host.h"

//Maximum count of block read/write attempts
#ifndef USBH_MSDRWBLOCK_ATTEMPTS
#define USBH_MSDRWBLOCK_ATTEMPTS                8
#endif /* USBH_MSDREAD_ATTEMPTS */

//Wait device ready interval, msec (>2000ms recommended)
#ifndef USBH_MSDWAIT_READY_INTERVAL
#define USBH_MSDWAIT_READY_INTERVAL             8000
#endif /* USBH_MSDWAIT_READY_INTERVAL */


//Number of USB/SCSI transaction attempts (when NAK received)
#ifndef SCSI_CMD_NOF_ATTEMPTS
#define SCSI_CMD_NOF_ATTEMPTS                   ((6000 * 1000) / USBH_TRANREPEAT_INTERVAL)
#endif /* SCSI_CMD_NOF_ATTEMPTS */


//Command "Test Unit Ready" interval, msec
//(recommended >1000ms; when 0 - this command disabled)
#ifndef SCSI_TEST_UNIT_READY_INTERVAL
#define SCSI_TEST_UNIT_READY_INTERVAL           2000 //~2sec
#endif /* SCSI_TEST_UNIT_READY_INTERVAL */


//(USBSetupRequest::bRequest)
//Get maximum Logical Unit Number (LUN)
//USB flash sticks usually returns 0 (one byte at response) for this request
#define USB_BREQ_GET_MAXLUN        0xFE


//MSD states (USBHDeviceParams::devState)
#define USBH_DEVSTATE_MSD_GET_MAXLUN_INIT           (USBH_DEVSTATE_2ND_LEVEL + 0)
#define USBH_DEVSTATE_MSD_GET_MAXLUN                (USBH_DEVSTATE_2ND_LEVEL + 1)
#define USBH_DEVSTATE_MSD_INQUIRY                   (USBH_DEVSTATE_2ND_LEVEL + 2)
#define USBH_DEVSTATE_MSD_TEST_UNIT_READY0          (USBH_DEVSTATE_2ND_LEVEL + 3)
#define USBH_DEVSTATE_MSD_REQUEST_SENSE             (USBH_DEVSTATE_2ND_LEVEL + 4)
#define USBH_DEVSTATE_MSD_MODE_SENSE                (USBH_DEVSTATE_2ND_LEVEL + 5)
#define USBH_DEVSTATE_MSD_READ_CAPACITY             (USBH_DEVSTATE_2ND_LEVEL + 6)
#define USBH_DEVSTATE_MSD_PREVENT_REMOVAL           (USBH_DEVSTATE_2ND_LEVEL + 7)
#define USBH_DEVSTATE_MSD_TEST_UNIT_READY1          (USBH_DEVSTATE_2ND_LEVEL + 8)
#define USBH_DEVSTATE_MSD_IDLE1                     (USBH_DEVSTATE_2ND_LEVEL + 9)
#define USBH_DEVSTATE_MSD_TEST_UNIT_READY           (USBH_DEVSTATE_2ND_LEVEL + 10)
#define USBH_DEVSTATE_MSD_STOP_UNIT                 (USBH_DEVSTATE_2ND_LEVEL + 11)

#define USBH_DEVSTATE_MSD_CLEAR_FEATURE1            (USBH_DEVSTATE_2ND_LEVEL + 12)


#define USBH_DEVSTATE_MSD_IDLE                      (USBH_DEVSTATE_2ND_LEVEL + 13)


/* Mass Storage Device control structure */
typedef struct tag_USBHMSDDeviceParams
{
    //Pointer to base USB control structure
    USBHDeviceParams *usbdev;

    //Timer;
    u32 timer;

    //Command tag, must incremented when command is send
    u32 tag;

    //Capacity, blocks
    u32 capacity;

    //Block size, bytes
    u32 blockSize;

    //Maximum Logical Unit Number (LUN)
    u8 maxLUN;

    //OUT endpoint address (tx data to MSD)
    u8 epout;

    //IN endpoint address (rx data from MSD)
    u8 epin;

    //Type of OUT transaction (USBH_TRANTYPE_DATA0/USBH_TRANTYPE_DATA1)
    u8 ttypeout;

    //Type of expected IN transaction (USBH_TRANTYPE_DATA0/USBH_TRANTYPE_DATA1)
    u8 ttypein;    

    //MSD description (from response to INQUIRY command)
    char description[35];
} USBHMSDDeviceParams;


//Maximum count of MSD devices
#define USBH_MSDDEVPARAMS_COUNT     1

//MSD controls structures
extern USBHMSDDeviceParams usbhMSDDevices[];

/* Initialize MSD driver subsystem */
s32 InitializeUSBHMSD(void *cfg);

/* Setup MSD driver */
s32 USBHSetupMSD(USBHDeviceParams *usbDev,
                 USBInterfaceDescriptor *ifclist[],
                 s16 ifccount);

/* Stop unit */
s32 MSDStopUnit(USBHDeviceParams *usbDev, u8 lun);

/* Read one block from MSD device */
s32 MSDReadBlock(u32 index, u8 *data);

/* Write one block to MSD device */
s32 MSDWriteBlock(u32 index, const u8 *data);

#endif /* __USBMSD_H */

/*===========================================================================
 * End of file usbmsd.c
 *===========================================================================*/
