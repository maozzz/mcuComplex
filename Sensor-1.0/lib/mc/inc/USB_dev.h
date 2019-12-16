/*===========================================================================
 *
 *  Sample code
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2018
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File USB_dev.h: USB device definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __USB_DEV_H
#define __USB_DEV_H

#include "types.h"
#include "CPU.h"
#include "USB.h"

//USB device events
#define USBD_EVTYPE_TRAN_IN         1 //In transaction completed
#define USBD_EVTYPE_TRAN_OUT        2 //Out transaction completed
#define USBD_EVTYPE_TRAN_SETUP      3 //Setup transaction completed
#define USBD_EVTYPE_STATE_CHANGED   4 //Device state is changed

//USB device options (USBDConfig::opts)
//Speed
#define USBD_OPTS_LOWSPEED          0 //Low speed, 1.5Mbit/s
#define USBD_OPTS_FULLSPEED         1 //Full speed, 12Mbit/s
#define USBD_OPTS_MASKSPEED         3 //mask

//Enable specified endpoint
#define USBD_OPTS_EP0EN             (1 << 2)
#define USBD_OPTS_EP1EN             (1 << 3)
#define USBD_OPTS_EP2EN             (1 << 4)
#define USBD_OPTS_EP3EN             (1 << 5)

//Enable isochronous mode of specified endpoint
#define USBD_OPTS_EP0ISOEN          (1 << 6)
#define USBD_OPTS_EP1ISOEN          (1 << 7)
#define USBD_OPTS_EP2ISOEN          (1 << 8)
#define USBD_OPTS_EP3ISOEN          (1 << 9)


//Count of language IDs (must be 2)
#define USBD_LANGIDS_MAX_COUNT      2

/* Represents USB device configuration */
typedef struct tag_USBDConfig
{
    //USB block (see USB_Nx)
    USBBLOCK usb;

    //Options (see USBD_OPTS_x)
    u32 opts;

    //Language IDs
    //index[0] - first language ID  (english or local)
    //index[1] - second language ID (local or zero)
    u16 languageID[USBD_LANGIDS_MAX_COUNT];


    //Device descriptor
    const u8 *dscDevice;

    //Qualifier descriptor
    const u8 *dscQualifier;

    //Configuration descriptor
    const u8 *dscConfiguration;


    //String: manufacturer (languageID[0]), UTF-8
    const char *strManufacturer;       

    //String: product description (languageID[0]), UTF-8
    const char *strProductID;


    //String: manufacturer (languageID[1]), UTF-8
    const char *strManufacturer1;

    //String: product description (languageID[1]), UTF-8
    const char *strProductID1;


    //String: serial number (all language IDs), UTF-8
    const char *strSerialNumber;


    //Pointer to data handler
    void (*evhandler)(u8 epIndex, u32 evtype);
} USBDConfig;


/* Initialize USB device using pointer to specified configuration */
MCRESULT InitializeUSBD(const USBDConfig *cfg);

/* Turn USB device */
void TurnUSBD(USBBLOCK usb, u8 on);

/* Get USB device clock, Hz */
u32 GetUSBDClk(USBBLOCK usb);

/* Get USB device speed, bit/s */
u32 GetUSBDSpeed(USBBLOCK usb);

/* Initialize data stage */
void USBDInitDataStage(USBBLOCK usb, const void *src, s32 count);

/* Get last setup request */
USBSetupRequest *USBDGetSetupRequest(USBBLOCK usb);

/* Get additional data of setup request */
const u8 *USBDGetSetupData(USBBLOCK usb, s32 *count);

/* Get current USB device address */
u8 USBDGetAddress(USBBLOCK usb);

/* Get current device state */
u8 USBDGetDeviceState(USBBLOCK usb);

/* Get number of selected configuration */
u8 USBDGetConfiguration(USBBLOCK usb);

/* Get count of bytes stored at rx FIFO of specified endpoint */
u32 USBDGetInFIFOCount(USBBLOCK usb, u8 epIndex);

/* Indicates that specified endpoint in "ready state" */
u32 USBDIsReady(USBBLOCK usb, u8 epIndex);


/* Enable/disable send STALL packets */
void USBDSetStall(USBBLOCK usb, u8 epIndex, u8 stall);

/* Set specified endpoint to ready/not state */
void USBDSetReady(USBBLOCK usb, u8 epIndex, u8 ready);

/* Enable/disable specified endpoint */
void USBDSetEnable(USBBLOCK usb, u8 epIndex, u8 enable);


/* Read data from rx FIFO of specified end point */
s32 USBDRead(USBBLOCK usb, u8 epIndex, void *dest, s32 count);

/* Write data to tx FIFO of specified end point */
s32 USBDWrite(USBBLOCK usb, u8 epIndex, const void *src, s32 count);

#endif /* __USB_DEV_H */

/*===========================================================================
 * End of file USB_dev.h
 *===========================================================================*/
