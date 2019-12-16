/*===========================================================================
 *
 *  MCU library
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2014-2018
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File swmodel_USB_dev.c: USB device functions.
 *
 *===========================================================================*/

#include "../../inc/USB_dev.h"


/* Initialize USB device */
MCRESULT InitializeUSBD(const USBDConfig *cfg)
{
    return MCR_OK;
}



/* Turn USB device */
void TurnUSBD(USBBLOCK usb)
{

}



/* Get USB clock, Hz */
u32 GetUSBDClk(USBBLOCK usb)
{
    return 0;
}



/* Get USB speed, bit/s */
u32 GetUSBDSpeed(USBBLOCK usb)
{
    return 0;
}



/* Get current USB device address */
u8 GetUSBDAddress(USBBLOCK usb)
{
    return (0 & 0x7F);
}



/* */
void USBDSetStall(USBBLOCK usb, u8 epIndex, u8 stall)
{
}



/* */
void USBDSetReady(USBBLOCK usb, u8 epIndex, u8 ready)
{
}



/* */
void USBDSetEnable(USBBLOCK usb, u8 epIndex, u8 enable)
{
}



/* Get count of bytes stored at rx FIFO */
u32 USBDGetInFIFOCount(u8 epIndex)
{
    return 0;
}



/* Read data from FIFO of specified end point */
s32 USBRead(USBBLOCK usb, u8 epIndex, void *dest, s32 count)
{
    return -1;
}



/* Write data to tx FIFO of specified endpoint */
s32 USBWrite(USBBLOCK usb, u8 epIndex, const void *src, s32 count)
{
    return -1;
}



/* USB interrupt handler */
void USB_IRQHandler(void)
{
}
 
/*===========================================================================
 * End of file swmodel_USB_dev.c
 *===========================================================================*/
