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
 *  File swmodel_USB_host.c: USB host functions (stubs).
 *
 *===========================================================================*/

#include "../../inc/USB_host.h"
#include <string.h>

//USB host time counter, msecs
volatile u32 usbhTimerMs = 0;

//Array of USB devices
USBHDeviceParams usbhDevices[USBH_DEVPARAMS_COUNT];

//Host configuration (low-level)
static USBHostConfig usbhHardwareConfig;

/* */
void USBHDelay(u32 usec)
{
}



/* Get endpoints maximum packet size using configuration descriptor */
void USBGetEndpointsMaxSize(const u8 *descCfg,
                            u16 *epINMaxSizeList,
                            u16 *epOUTMaxSizeList)
{
}



/* Initialize USB host */
MCRESULT InitializeUSBH(const USBHostConfig *cfg)
{
    //Save host configuration
    if (cfg)
        memcpy(&usbhHardwareConfig, cfg, sizeof(USBHostConfig));
    else
        memset(&usbhHardwareConfig, 0, sizeof(USBHostConfig));


    //Clear device array
    memset(usbhDevices, 0, USBH_DEVPARAMS_COUNT * sizeof(USBHDeviceParams));

    return MCR_OK;
}



/* Turn USB host */
void TurnUSBH(USBBLOCK usb, u8 on)
{
}



/* */
void USBHSetSyncTxWithSOF(USBBLOCK usb, u8 on)
{
    if (on)
        usbhHardwareConfig.opts |= USBH_HWOPTS_SYNC_TX_WITH_SOF;
    else
        usbhHardwareConfig.opts &= ~USBH_HWOPTS_SYNC_TX_WITH_SOF;
}



/* */
void USBHSetSyncRxWithSOF(USBBLOCK usb, u8 on)
{
    if (on)
        usbhHardwareConfig.opts |= USBH_HWOPTS_SYNC_RX_WITH_SOF;
    else
        usbhHardwareConfig.opts &= ~USBH_HWOPTS_SYNC_RX_WITH_SOF;
}



/* */
void USBHStartInTransaction(USBHDeviceParams *usbDev)
{
}



/* Start SETUP/OUT transaction */
void USBHStartOutTransaction(USBHDeviceParams *usbDev)
{
}



/* Start reset USB bus */
void USBHResetDeviceStart(USBHDeviceParams *usbDev)
{
}



/* Move device to error state */
void USBHMoveDeviceToErrorState(USBHDeviceParams *usbDev)
{
    if (!usbDev)
        return;

    usbDev->flags |= USBH_DEVFLAG_ERROR;
    usbDev->devState = USBH_DEVSTATE_ERROR;
    usbDev->reqState = USBH_REQSTAGE_INIT;

    //Call event handler
    if (usbhHardwareConfig.error)
        usbhHardwareConfig.error(usbDev);
}



/* Re-initalize device */
void USBHReInitDevice(USBHDeviceParams *usbDev)
{
}



/* Start receive data */
void USBHStartRxData(USBHDeviceParams *usbDev,
                     u8 epoint, u8 expttype, u32 nattempts,
                     void *dest, u32 n)
{
    //Save endpoint address and transaction type
    usbDev->ioepoint = epoint;
    usbDev->iotype   = expttype; //expected type: [2,3]=>[data0/data1]

    //Save pointer to out data and length
    usbDev->iodata0  = dest;
    usbDev->iodata   = dest;
    usbDev->iolength = n;

    //Reset attempt counters
    //usbDev->ioReqReInitCounter = 0;
    usbDev->ioAtCounter  = 0;
    usbDev->ioMaxAtCount = nattempts;

    //Start send first portion of out data
    USBHStartInTransaction(usbDev);
}



/* Start transmit data */
void USBHStartTxData(USBHDeviceParams *usbDev,
                     u8 epoint, u8 ttype, u32 nattempts,
                     const void *src, u32 n)
{
    //Save endpoint address and transaction type
    usbDev->ioepoint = epoint;
    usbDev->iotype   = ttype; //[setup,data0,data1]=>[0,2,3]

    //Save pointer to out data and length
    usbDev->iodata0  = src;
    usbDev->iodata   = src;
    usbDev->iolength = n;

    //Reset attempt counters
    usbDev->ioAtCounter  = 0;
    usbDev->ioMaxAtCount = nattempts;

    //Start send first portion of out data
    USBHStartOutTransaction(usbDev);
}



/* Receive actions */
u32 USBHRxDataActions(USBHDeviceParams *usbDev)
{
    return USBH_TRANSTATE_FAIL;
}



/* Transmission actions */
u32 USBHTxDataActions(USBHDeviceParams *usbDev)
{
    return USBH_TRANSTATE_FAIL;
}



/* Send "GET" request, receive data from device, send status */
u32 USBHGetRequestActions(USBHDeviceParams *usbDev,
                          const USBSetupRequest *req,
                          u8 *resp, u32 respLength)
{
    return USBH_TRANSTATE_FAIL;
}



/* Send "SET" requests and request data, receive status */
u32 USBHSetRequestActions(USBHDeviceParams *usbDev,
                          const USBSetupRequest *req,
                          const u8 *txdata, u32 txLength)
{
    return USBH_TRANSTATE_FAIL;
}



/* Read string descriptor: manufacturer description */
char *USBHReadManufacturerString(USBHDeviceParams *usbDev, u8 langIndex, char *dest, u16 maxLen)
{
    return USBHReadString(usbDev, usbDev->descDevice.iManufacturer, langIndex, dest, maxLen);
}



/* Read string descriptor: product description */
char *USBHReadProductString(USBHDeviceParams *usbDev,
                            u8 langIndex, char *dest, u16 maxLen)
{
    return USBHReadString(usbDev, usbDev->descDevice.iProduct, langIndex, dest, maxLen);
}



/* Read string descriptor: serial number */
char *USBHReadSerialNumberString(USBHDeviceParams *usbDev,
                                 u8 langIndex, char *dest, u16 maxLen)
{
    return USBHReadString(usbDev, usbDev->descDevice.iSerialNmuber, langIndex, dest, maxLen);
}



/* Read string descriptor with specified index */
char *USBHReadString(USBHDeviceParams *usbDev,
                     u8 strIndex, u8 langIndex, char *dest, u16 maxLen)
{
    dest[0] = 0;
    return dest;
}



/* Read device descriptor */
u32 USBHReadDeviceDescriptor(USBHDeviceParams *usbDev)
{
    return USBH_TRANSTATE_FAIL;
}



/* Update USB protocol state.
   Call once at main cycle */
void UpdateUSBHostState(USBBLOCK usb)
{

}
 
/*===========================================================================
 * End of file swmodel_USB_host.c
 *===========================================================================*/
