/*===========================================================================
 *
 *  MCU library
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2014-2019
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File USB_host.h: USB host definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __USB_HOST_H
#define __USB_HOST_H

#include "types.h"
#include "CPU.h"
#include "USB.h"


/*===========================================================================
 * USB device definitions
 *===========================================================================*/

//Interval before repeat transaction, usec
//(when NAK received)
#ifndef USBH_TRANREPEAT_INTERVAL
#define USBH_TRANREPEAT_INTERVAL         200  //usec
#endif


//Maximum count of device re-initializations
//(when setup transactions fail)
#ifndef USBH_DEVICE_REINIT_MAXCOUNT
#define USBH_DEVICE_REINIT_MAXCOUNT         3 // 8 //[1..254]
#endif /* USBH_DEVICE_REINIT_MAXCOUNT */



//Device states (USBHDeviceParams::devState)
#define USBH_DEVSTATE_INITIAL           0

#define USBH_DEVSTATE_ATTACHED          1  //Device attached
#define USBH_DEVSTATE_IDLE              2  //Wait before reset state
#define USBH_DEVSTATE_RESET             3  //DP=DM=0 for time >10ms

#define USBH_DEVSTATE_GET_DEVICE0       4  //Get device descriptor (zero address)

#define USBH_DEVSTATE_SET_ADDRESS       5  //Set device address

#define USBH_DEVSTATE_GET_DEVICE        6  //Get device descriptor (after address is set)
#define USBH_DEVSTATE_GET_CONFIG0       7  //Get configuration descriptor (header only)
#define USBH_DEVSTATE_GET_CONFIG        8  //Get configuration descriptor (full descriptor)
#define USBH_DEVSTATE_GET_SUPLANGS      9  //Get supported langiages (string descriptors)
#define USBH_DEVSTATE_SET_CONFIG        10 //Set configuration
#define USBH_DEVSTATE_ERROR             11 //Work with error, no actions
#define USBH_DEVSTATE_2ND_LEVEL         12 //2nd level of USB protocol (device specified)

//"USBH_DEVSTATE_x" at [11..255] used at 2nd level protocol


//---------------------------------------------------------------------------
//Device flags (USBHDeviceParams::flags)
#define USBH_DEVFLAG_DETACHED           (0 << 0) //Detached state
#define USBH_DEVFLAG_LOWSPEED           (1 << 0) //Low-speed device
#define USBH_DEVFLAG_FULLSPEED          (2 << 0) //Full-speed device
#define USBH_DEVFLAG_HIGHSPEED          (3 << 0) //High-speed device
#define USBH_DEVFLAG_SPEEDMASK          (3 << 0) //mask

#define USBH_DEVFLAG_ADDRESSED          (1 << 4)  //Addressed
#define USBH_DEVFLAG_CONFIGURED         (1 << 5)  //Configured
#define USBH_DEVFLAG_DRIVERLOADED       (1 << 6)  //Driver loaded
#define USBH_DEVFLAG_ERROR              (1 << 31) //Work with errors


//USB request states (USBHDeviceParams::reqState)
#define USBH_REQSTAGE_INIT              0 //Initial state
#define USBH_REQSTAGE_TXREQUEST         1 //Transmit request (command)
#define USBH_REQSTAGE_DATA              2 //Data stage (tx/rx data to/from device)
#define USBH_REQSTAGE_STATUS            3 //Status stage
#define USBH_REQSTAGE_CLEARFEATURE      4 //Clear feature

//Transaction types
#define USBH_TRANTYPE_SETUP             0 //Setup
#define USBH_TRANTYPE_IN                1 //In
#define USBH_TRANTYPE_DATA0             2 //Data0
#define USBH_TRANTYPE_DATA1             3 //Data1

//Transaction states
#define USBH_TRANSTATE_PROGRESS         0    //in progress
#define USBH_TRANSTATE_SUCCESS          1    //success
#define USBH_TRANSTATE_INVDATASEQ       2    //invalid dataseq
#define USBH_TRANSTATE_FAIL             255  //fail


//Maximum length of device configuration that
//can stored at device parameters (+2: device descriptor has 18 bytes length)
#define USBH_DESCCONFIG_MAXLENGTH       (256 + 2)


typedef struct tag_USBHDeviceParams USBHDeviceParams;

typedef void (*USBHDeviceProtoHandler)(USBHDeviceParams *usbDev, u32 evtype);
typedef USBHDeviceProtoHandler LPUSBHDeviceProtoHandler;


/* */
typedef struct tag_USBHDeviceParams
{
    //Host controller to which device attached (see USB_Nx)
    USBBLOCK usbhost;

    //Device flags (see USBH_DEVFLAG_x)
    u32 flags;

    //Timer, ms
    u32 timer;

    //Interval, ms
    u32 timerInterval;


    //Device state (see USBH_DEVSTATE_x)
    u8 devState;

    //USB request state (see USBH_REQSTATE_x)
    u8 reqState;

    //Device address [0,1..127]
    u8 address;

    //Count of device re-initializations.
    //When USB transaction fail occurs or response from device is not
    //recognized (invalid data at descriptors) device will reset and
    //this counter incremented by 1. When counter achieve value defined
    //at USBH_DEVICE_REINIT_MAXCOUNT, device go to error state.
    u8 reInitCounter;


    //-----------------------------------------------
    //I/O fields
    //Endpoint number [0,1...]
    u8 ioepoint;

    //Transaction type (see USBH_TRANTYPE_x)
    u8 iotype;    

    //Count of transaction attempts
    u32 ioAtCounter;

    //Maximum count of transaction attempts
    u32 ioMaxAtCount;


    //Pointer to START of buffer that contains data for transmission
    //or buffer that will store received data
    const u8 *iodata0;

    //Pointer to current portion of tx/rx data
    const u8 *iodata;

    //Requested tx/rx length
    u32 iolength;


    //-----------------------------------------------
    //IN endpoints maximum packet size
    u16 epINMaxSizeList[16];

    //OUT endpoints maximum packet size
    u16 epOUTMaxSizeList[16];

    //Device descriptor
    USBDeviceDescriptor descDevice;

    //Full configuration descriptor (config+interface+endpoints descriptors)
    u8 descConfig[USBH_DESCCONFIG_MAXLENGTH];

    //Total length of configuration descriptor
    u16 descConfigTotalLength;

    //Supported languages (see USB_LANGID_x)
    //lang[0] - stuf only
    //lang[1] - default language (EN/US)
    //lang[2] - second language (when none-zero)    
    u16 langs[3];

    //Poniter to parent device
    USBHDeviceParams *parent;    

    //Pointer to device control structure
    void *deviceCtrl;

    //Pointer to protocol handler (second level, device specified)
    LPUSBHDeviceProtoHandler deviceProtoHandler;

    //Indicates that device driver ready
    void (*ready)(USBHDeviceParams *usbDev, u32 status);
} USBHDeviceParams;


/*===========================================================================
 * USB host definitions
 *===========================================================================*/

//USB host options (USBHostConfig::opts)
//Tx SOF/EOP packets: special packets to prevent device go to suspend state,
//tx with interval 1ms
#define USBH_HWOPTS_TX_SOF                  0x01
//Sync transmission with SOF packet
 #define USBH_HWOPTS_SYNC_TX_WITH_SOF       0x02
//Sync receive with SOF packet
#define USBH_HWOPTS_SYNC_RX_WITH_SOF        0x04


/* Represents USB host configuration */
typedef struct tag_USBHostConfig
{
    //USB # (see USB_Nx)
    USBBLOCK usb;

    //USB host options (see USBH_HWOPTS_x)
    u32 opts;

    //Device detached event handler
    void (*detached)(USBHDeviceParams *usbDev);

    //Device attached event handler
    void (*attached)(USBHDeviceParams *usbDev);

    //Device configured event handler
    void (*configured)(USBHDeviceParams *usbDev);

    //Device error event handler
    void (*error)(USBHDeviceParams *usbDev);    
} USBHostConfig;


/*===========================================================================
 * Functions
 *===========================================================================*/

//Maximum count of USB devices
#define USBH_DEVPARAMS_COUNT     1

//USB device control structures
extern USBHDeviceParams usbhDevices[];


/* Time counter, msec */
extern volatile u32 usbhTimerMs;

/* Update USB host timers.
   Call this function with frequency 1000Hz */
__STATIC_INLINE void UpdateUSBHostTimers()
{
    usbhTimerMs++;
}


/* Initialize USB host */
MCRESULT InitializeUSBH(const USBHostConfig *cfg);

/* Turn USB host */
void TurnUSBH(USBBLOCK usb, u8 on);

/* Delay specified number of usec */
void USBHDelay(u32 usec);

/* Sync transmission with SOF ON/OFF */
void USBHSetSyncTxWithSOF(USBBLOCK usb, u8 on);

/* Sync receive with SOF ON/OFF */
void USBHSetSyncRxWithSOF(USBBLOCK usb, u8 on);

/* Start receive specified count of data from device endpoint.
   expttype: expected type of FIRST IN transaction (USBH_TRANTYPE_DATA0 or USBH_TRANTYPE_DATA1) */
void USBHStartRxData(USBHDeviceParams *usbDev,
                     u8 epoint, u8 expttype, u32 nattempts,
                     void *dest, u32 n);

/* Start transmit specified count of data to device endpoint.
   ttype: type of FIRST OUT transaction (USBH_TRANTYPE_DATA0 or USBH_TRANTYPE_DATA1) */
void USBHStartTxData(USBHDeviceParams *usbDev,
                     u8 epoint, u8 ttype, u32 nattempts,
                     const void *src, u32 n);


/* Receive actions.
   Return: one of "USBH_TRANSTATE_x" values.
   Notes:
    1. Initialize data receiving with call of "USBHStartRxData" function
    2. This function must be called while it return "USBH_TRANSTATE_PROGRESS" value */
u32 USBHRxDataActions(USBHDeviceParams *usbDev);

/* Transmission actions.
   Return: one of "USBH_TRANSTATE_x" values.
   Notes:
    1. Initialize data transmission with call of "USBHStartTxData" function
    2. This function must be called while it return "USBH_TRANSTATE_PROGRESS" value */
u32 USBHTxDataActions(USBHDeviceParams *usbDev);



/* Send "GET" request, receive data from device, send status.
   Return: one of "USBH_TRANSTATE_x" values */
u32 USBHGetRequestActions(USBHDeviceParams *usbDev,
                          const USBSetupRequest *req,
                          u8 *resp, u32 respLength);

/* Send "SET" request and data to device, receive status.
   Return: one of "USBH_TRANSTATE_x" values */
u32 USBHSetRequestActions(USBHDeviceParams *usbDev,
                          const USBSetupRequest *req,
                          const u8 *txdata, u32 txLength);

/* Read string descriptor: manufacturer description */
char *USBHReadManufacturerString(USBHDeviceParams *usbDev,
                                 u8 langIndex, char *dest, u16 maxLen);

/* Read string descriptor: product description */
char *USBHReadProductString(USBHDeviceParams *usbDev,
                            u8 langIndex, char *dest, u16 maxLen);

/* Read string descriptor: serial number */
char *USBHReadSerialNumberString(USBHDeviceParams *usbDev,
                                 u8 langIndex, char *dest, u16 maxLen);

/* Read string descriptor with specified index */
char *USBHReadString(USBHDeviceParams *usbDev,
                     u8 strIndex, u8 langIndex, char *dest, u16 maxLen);

/* Read device descriptor */
u32 USBHReadDeviceDescriptor(USBHDeviceParams *usbDev);

/* Move device to error state */
void USBHMoveDeviceToErrorState(USBHDeviceParams *usbDev);

/* Start reset device */
void USBHResetDeviceStart(USBHDeviceParams *usbDev);

/* Re-initalize device */
void USBHReInitDevice(USBHDeviceParams *usbDev);

/* Update USB protocol state.
   Call once at main cycle */
void UpdateUSBHostState(USBBLOCK usb);

#endif /* __USB_HOST_H */

/*===========================================================================
 * End of file USB_host.h
 *===========================================================================*/
