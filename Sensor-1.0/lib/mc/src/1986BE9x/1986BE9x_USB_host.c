/*===========================================================================
 *
 *  MCU library.
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
 *  File 1986BE9x_USB_host.c. USB host functions.
 *
 *===========================================================================*/

#include "../../inc/USB_host.h"
#include <string.h>

/*===========================================================================
 *
 * Setup transactions flow (USB 1.0, USB 1.1)
 *
 * Three stages: setup, data and status stage
 *
 * Host                    Device   Host                    Device
 * |      SET requests        |      |      GET requests        |
 * |       Setup stage        |      |       Setup stage        |
 * |--setup--data0(set)------>|(1)   |--setup--data0(get)------>|(1)
 * |<--------------------ack--|      |<--------------------ack--|
 * |                          |      |                          |
 * |  Data stage (optional)   |      |  Data stage (optional)   |
 * |--out--data1------------->|(2)   |--in--------------------->|
 * |<--------------------ack--|      |<------------------data1--|(2)
 * |--out--data0------------->|      |--ack-------------------->|
 * |<--------------------ack--|      |--in--------------------->|
 * |           ....           |      |<------------------data0--|
 * |--out--data0/1----------->|      |--ack-------------------->|
 * |<--------------------ack--|      |          ....            |
 * |                          |      |--in--------------------->|
 * |  Status stage (success)  |      |<----------------data0/1--|
 * |--in--------------------->|      |--ack-------------------->|
 * |<----------data1(empty)---|(3)   |                          |
 * |--ack-------------------->|      |  Status stage (success)  |
 *                                   |--out--data1(empty)------>|(3)
 *                                   |<--------------------ack--|
 *
 * Notes:
 * (1) - Always DATA0 packet after SETUP token.
 * (2) - Always DATA1 packet at begin of data stage,
 *       subsequently toggle between DATA1/DATA0.
 * (3) - Always DATA1 packet at status stage.
 *
 *
 *===========================================================================*/

//Debug messages (comment to disable)
#if defined(__USB_DEBUG_MESSAGES)
#include <stdcf.h>
#define PrintMsg(...)           std_printf(__VA_ARGS__)
#else
#define PrintMsg(...)
#endif



#if defined(__CC_ARM)
#define __USBH_FATTR2      __attribute__((weak))

#elif defined(__ICCARM__)
#define __USBH_FATTR2

#elif defined (__CMCARM__)
#define __USBH_FATTR2

#elif defined(__GNUC__)
#define __USBH_FATTR2	    __attribute__((weak))

#else
#define __USBH_FATTR2

#endif


//Time interval after device connected and before USB reset, ms
#ifndef USBH_TIME_BEFORERESET
#define USBH_TIME_BEFORERESET           100 //>=100ms
#endif

//Time USB device reset (DP=DM=0), ms
#ifndef USBH_TIME_DEVRESET
#define USBH_TIME_DEVRESET              20 //must >10ms
#endif

//Time interval after USB reset, ms
#ifndef USBH_TIME_AFTERRESET
#define USBH_TIME_AFTERRESET            0 //may be 0
#endif

//Count of transaction attempts (when NAK,RXTO received)
#ifndef USBH_TRANATMPT_COUNT
#define USBH_TRANATMPT_COUNT            ((4000 * 1000) / USBH_TRANREPEAT_INTERVAL)
#endif

//Used when solved "usbhDelayK"
#ifndef USBH_DELAY_COEF
#define USBH_DELAY_COEF                 16U
#endif /* USBH_DELAY_COEF */


//USB request: "GET DEVICE DESCRIPTOR"
static const USBSetupRequest GET_DEVICE_DESCRIPTOR =
{
    (USB_BMREQ_DIR_DEV2HOST|USB_BMREQ_TYPE_STANDARD|USB_BMREQ_RCPNT_DEVICE), //bmRequestType
    USB_BREQ_GET_DESCRIPTOR,   //bRequest
    (USB_BDTYPE_DEVICE << 8),  //wValue
    0,                         //wIndex
    USB_BLEN_DEVICE            //wLength
};


//USB request: "SET ADDRESS",
//"wValue" must be filled with actual device address
static const USBSetupRequest SET_ADDRESS_REQUEST =
{
    (USB_BMREQ_DIR_HOST2DEV|USB_BMREQ_TYPE_STANDARD|USB_BMREQ_RCPNT_DEVICE), //bmRequestType
    USB_BREQ_SET_ADDRESS,   //bRequest
    0,                      //wValue (must device address)
    0,                      //wIndex
    0                       //wLength
};


//USB request: "GET CONFIGURATION DESCRIPTOR",
//"wLength" must be set to full configuration length
static const USBSetupRequest GET_CONFIGURATION_DESCRIPTOR =
{
    (USB_BMREQ_DIR_DEV2HOST|USB_BMREQ_TYPE_STANDARD|USB_BMREQ_RCPNT_DEVICE), //bmRequestType
    USB_BREQ_GET_DESCRIPTOR,   //bRequest
    (USB_BDTYPE_CONFIGURATION << 8),  //wValue
    0,                         //wIndex
    USB_BLEN_CONFIGURATION     //wLength - configuration descriptor length,
                               //          without interface and endp. descriptors
};


//USB request: "SET CONFIGURATION",
//"wValue" must be filled with actual configuration number
static const USBSetupRequest SET_CONFIGURATION_REQUEST =
{
    (USB_BMREQ_DIR_HOST2DEV|USB_BMREQ_TYPE_STANDARD|USB_BMREQ_RCPNT_DEVICE), //bmRequestType
    USB_BREQ_SET_CONFIGURATION, //bRequest
    1,                          //wValue (must configuration number from descriptor, 1 by default)
    0,                          //wIndex
    0                           //wLength
};


//USB request: "GET STRING DESCRIPTOR" (supported laguages)
static const USBSetupRequest GET_STRING_DESCRIPTOR =
{
    (USB_BMREQ_DIR_DEV2HOST|USB_BMREQ_TYPE_STANDARD|USB_BMREQ_RCPNT_DEVICE), //bmRequestType
    USB_BREQ_GET_DESCRIPTOR,   //bRequest
    (USB_BDTYPE_STRING << 8),  //wValue low byte must contains string number
    0,                         //wIndex: 0 - no specified language
    6                          //wLength: string descriptor max length;
                               //6 = blen[1] + btype[1] + lang1[2] + lang2[2]
};


//USB host time counter, msecs
volatile u32 usbhTimerMs = 0;

//Device address counter [1..127], initial 0
static u8 usbhAddressCounter = 0;

//Current setup request
static USBSetupRequest usbhCurrSetupRequest;

//Host configuration (low-level)
static USBHostConfig usbhHardwareConfig;

//Array of USB devices
USBHDeviceParams usbhDevices[USBH_DEVPARAMS_COUNT];


/* Delay 10ms, N=8
MCU 1901BC1T, 80MHz
-Ofast  8ms
-Os    12ms
-O2     8ms
-O1    10ms
-O0    16ms

MCU 1901BC1T, 96MHz
-O1    8.4ms

MCU 1901BC1T, 40MHz
-O1    20ms
*/
static u32 usbhDelayK = 0;

__USBH_FATTR2 void USBHDelay(u32 usec)
{
    usec = usbhDelayK * usec;
    while(usec--) __NOP();
}



/* Get endpoints maximum packet size using configuration descriptor */
void USBGetEndpointsMaxSize(const u8 *descCfg,
                            u16 *epINMaxSizeList,
                            u16 *epOUTMaxSizeList)
{
    u16 offset, wTotalLength, wMaxPacketSize;
    u8 epaddr;

    //Check configuration descriptor fields
    if (descCfg[0] != USB_BLEN_CONFIGURATION ||
        descCfg[1] != USB_BDTYPE_CONFIGURATION)
        return;

    //bytes[3,2]: wTotalLength (con)
    wTotalLength = (descCfg[3] << 8) | descCfg[2];

    if (wTotalLength == 0xFFFF)
        return;

    offset = USB_BLEN_CONFIGURATION;
    while(offset < wTotalLength)
    {
        if (descCfg[offset]     == USB_BLEN_ENDPOINT &&
            descCfg[offset + 1] == USB_BDTYPE_ENDPOINT)
        {
            //byte[2]: endpoint address
            epaddr = descCfg[(offset + 2)];

            //bytes[5,4]: wMaxPacketSize
            wMaxPacketSize = ((descCfg[(offset + 5)]) << 8) | (descCfg[(offset + 4)]);

            if (wMaxPacketSize > 1024)
                wMaxPacketSize = 1024;
            else  if (wMaxPacketSize < 8)
                wMaxPacketSize = 8;

            //bit[7]=1:   IN endpoint
            //bits[3..0]: address [1..15]
            if (epaddr & 0x80)
                epINMaxSizeList[(epaddr & 0x0F)] = wMaxPacketSize;
            else
                epOUTMaxSizeList[(epaddr & 0x0F)] = wMaxPacketSize;

            offset += USB_BLEN_ENDPOINT;
        }
        else
        {
            //Skip other descriptors
            if (descCfg[offset] == 0)
                offset++;
            else
                offset += descCfg[offset];
        }
    } //while
}



/* Initialize USB host */
MCRESULT InitializeUSBH(const USBHostConfig *cfg)
{
    u32 val;

    //Save host configuration
    if (cfg)
        memcpy(&usbhHardwareConfig, cfg, sizeof(USBHostConfig));
    else
        memset(&usbhHardwareConfig, 0, sizeof(USBHostConfig));


    //Clear device array
    memset(usbhDevices, 0, USBH_DEVPARAMS_COUNT * sizeof(USBHDeviceParams));

    //Tune K multiplier (gcc compiler with -O1)        
    usbhDelayK = (u32)((USBH_DELAY_COEF * GetHCLK()) / 80000000U);

    //Set address counter to 0
    usbhAddressCounter = 0;


    //Start hardware initialization
    //Enable USB clock
    ClkEnable(CLK_USB);

    //PLL_CONTROL register:
    //PLL_USB_MUL[7..4]: frequency multiplier
    //PLL_USB_RLD[1]   : reload PLL block
    //                   (after change frequency multiplier set it to 1)
    //PLL_USB_ON [0]: turn ON/OFF USB block
    val = RST_CLK->PLL_CONTROL;

    //PLL_USB_MUL = 48MHz / HSE - 1
    val = (val & ~(0x0F << 4)) | (((u32)(48000000 / (FREQ_HSE)) - 1U) << 4);

    //PLL in work mode: set PLL_USB_RLD[1]=1
    if (RST_CLK->CLOCK_STATUS & PN0)
        val |= PN1;

    //PLL_USB_ON[0]=1
    val |= PN0;

    //Update register
    RST_CLK->PLL_CONTROL = val;


    //While PLL is not ready (PLL_USB_RDY[0] is 0)
    while(!(RST_CLK->CLOCK_STATUS & PN0)) __NOP();

    //USB_CLOCK register:
    //USB_CLK_EN[8]: 1/0 - enable/disable USB clock
    //USB_C3_SEL[4]: 1/0 - USB_C3 = USB_C2 / (USB_C3_SEL + 1)
    //USB_C2_SEL[2]: 1   - USB_C2 = USB_C1 * (PLL_USB_MUL + 1)
    //               0   - USB_C2 = USB_C1
    //USB_C1_SEL[1..0]: [0,1,2,3] - USB_C1 = [HSI,HSI/2,HSE,HSE/2]
    RST_CLK->USB_CLOCK = PN8 | PN2 | 2;    

    //----------------------------------------------------------------------------------------
    //2. Host_init();
    //HSCR
    //RESET_CORE[1]=1: reset USB controller
    val = USB->HSCR;
    val |= PN1;
    USB->HSCR = val;

    //Wait reset interval
    for(val = 0; val < 0x1000; val++);

    //RESET_CORE[1]=0: normal work
    val = USB->HSCR;
    val &= ~PN1;
    USB->HSCR = val;


    //HSCR
    //DM_PULLDOWN[7]=DP_PULLDOWN[5]=1: 15KOhm pull down for host
    //EN_RX[3]=EN_TX[2]=0            : Tx/Rx currently disabled
    //RESET_CORE[1]=0                : normal work
    //HOST_MODE[0]=1                 : host mode
    val = USB->HSCR;    
    val = PN7|PN5 |PN0;
    USB->HSCR = val;


    //HTXLC
    //FSLR[4]=0: 1.5Mbit/s,
    //FSPL[3]=0: low speed polarity
    //DC[2]=0: normal work
    //TXLC[1,0]: [DP,DM] line state
    //This register updated after device connected
    USB->HTXLC = 0;


    //HTXSE: enable tx SOF - prevent from suspend
    //SOFEN[0]=1 - when FSPL=1: tx SOF,
    //             when FSPL=0: tx low speed EOP;
    //         0 - no tx SOF/EOP.
    if (usbhHardwareConfig.opts & USBH_HWOPTS_TX_SOF)
    {
        USB->HTXSE = 1;
    }
    else
    {
        USB->HTXSE = 0;        
        usbhHardwareConfig.opts &= ~(USBH_HWOPTS_SYNC_TX_WITH_SOF|USBH_HWOPTS_SYNC_RX_WITH_SOF);
    }


    //Current software version not used interrupts:
    //disable interrupt from USB controller
    USB->HIM = 0;    
    IRQDisable(IRQ_USB);

    /*
    //Sample of enabling interrupts
    //HIM register (1/0 - enable/disable interrupts):
    //SOFIE[3]  : SOF send
    //CONEVIE[2]: Connection event
    //RESUMIE[1]: Repeat
    //TDONEIE[0]: Transaction completed
    USB->HIM = PN2 | PN0;

    //Enable interrupt from USB controller
    IRQEnable(IRQ_USB);
    */

    return MCR_OK;
}



/* Turn USB host */
void TurnUSBH(USBBLOCK usb, u8 on)
{
    //HSCR register:
    //EN_RX[3]: 1/0 - on/off USB receiver
    //EN_TX[2]: 1/0 - on/off USB transmitter
    if (on)
    {
        //EN_RX[3]=EN_TX[2]=1
        USB->HSCR |= (PN3 | PN2);
    }
    else
    {
        //EN_RX[3]=EN_TX[2]=0
        USB->HSCR &= ~(PN3 | PN2);
    }
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
    //HRXFC=1: clear tx/rx fifo    
    USB->HRXFC  = 1;

    //HTXA[6..0]: device address
    USB->HTXA = usbDev->address;
    USB->HTXE = usbDev->ioepoint;
    USB->HTXT = 1; //always 1 - IN transaction

    //HIS (clear with writing 1s):
    //SOFS[3]:   1 - SOF was send
    //CONEV[2]:  1 - connection/disconnection event
    //RESUME[1]: 1 - repeat
    //TDONE[0]:  1 - transaction completed

    //HTXC
    //ISOEN[3]: 1 isochronous enabled
    //PREEN[2]: 1 preambula enabled, only when [host]-[hub]-[low speed device]
    //SOFS[1] : 1 sync transmission with SOF
    //TREQ[0] : 1 request transmission

    //TDONE[0]=1: reset bit
    USB->HIS |= PN0;

    //SOFS[1]=1 sync transmission with SOF
    //TREQ[0]=1 request transmission
    if (usbhHardwareConfig.opts & USBH_HWOPTS_SYNC_RX_WITH_SOF)
        USB->HTXC |= PN1 | PN0;
    else
        USB->HTXC = (USB->HTXC & ~0x03) | PN0;
}



/* Start SETUP/OUT transaction */
void USBHStartOutTransaction(USBHDeviceParams *usbDev)
{
    u32 i, n;
    const u8 *ptr = (u8*)usbDev->iodata;
    u16 epOutMaxSize;

    //HTXFDC=1: clear tx fifo
    USB->HTXFDC = 1;    

    //HTXA[6..0]: device address
    USB->HTXA = usbDev->address;
    USB->HTXE = usbDev->ioepoint;
    USB->HTXT = usbDev->iotype; //0,2,3 - setup,data0,data1

    n = usbDev->iolength;

    epOutMaxSize = usbDev->epOUTMaxSizeList[(usbDev->ioepoint)];

    if (n > USB_EP_FIFO_TX_LENGTH)
        n = USB_EP_FIFO_TX_LENGTH;    

    if (n > epOutMaxSize)
        n = epOutMaxSize;

    //Write tx data to tx FIFO (HTXFD register)
    for (i = 0; i < n; i++)
    {
        USB->HTXFD = *ptr++;
    }

    //HIS (clear with writing 1s):
    //SOFS[3]:   1 - SOF was send
    //CONEV[2]:  1 - connection/disconnection event
    //RESUME[1]: 1 - repeat
    //TDONE[0]:  1 - transaction completed
    //TDONE[0]=1: reset bit
    USB->HIS |= PN0;

    //HTXC
    //ISOEN[3]: 1 isochronous enabled
    //PREEN[2]: 1 preambula enabled, only when [host]-[hub]-[low speed device]
    //SOFS[1] : 1 sync transmission with SOF
    //TREQ[0] : 1 request transmission

    //SOFS[1]=1 sync transmission with SOF
    //TREQ[0]=1 request transmission        
    if (usbhHardwareConfig.opts & USBH_HWOPTS_SYNC_TX_WITH_SOF)
        USB->HTXC |= PN1 | PN0;
    else
        USB->HTXC = (USB->HTXC & ~0x03) | PN0;
}





/* Put DP=DM=0 */
static void USBHPutDPDMToLow()
{
    //HTXLC
    //DC[2]=1:     direct line control
    //TXLC[1,0]=0: put DP and DM to low state
    USB->HTXLC |= PN2;
    USB->HTXLC &= ~(PN1|PN0);
}



/* Go to work mode */
static void USBHGoToWorkMode()
{
    //DC[2]=0: normal work
    USB->HTXLC &= ~PN2;
}



/* Start reset USB bus */
void USBHResetDeviceStart(USBHDeviceParams *usbDev)
{
    u32 i, regHRXCS;
    USBBLOCK usbhost;
    u8 reInitCounter;

    //Select USB controller using "usbDev->usbhost" field
    //Not need here: MCU 1986x,1901x has one USB controller

    regHRXCS = (USB->HRXCS & 3);

    //Save some fields of control structure
    reInitCounter = usbDev->reInitCounter;
    usbhost       = usbDev->usbhost;

    //Clear device structure
    memset(usbDev, 0, sizeof(USBHDeviceParams));

    //Restore re-init counter and number of USB host
    usbDev->reInitCounter = reInitCounter;
    usbDev->usbhost       = usbhost;

    if (regHRXCS == 1 || regHRXCS == 2)
    {
        //Device connected        
        //HRXCS=1: low-speed
        if (regHRXCS == 1)
        {
            //LOW SPEED device connected
            //FSLR[4]=FSPL[3]=0: 1.5Mbit, low speed polarity
            USB->HTXLC &= ~(PN4|PN3);

            usbDev->flags = USBH_DEVFLAG_LOWSPEED;
        }
        else
        {
            //FULL SPEED device connected
            //FSLR[4]=FSPL[3]=1: 12Mbit, full speed polarity
            USB->HTXLC |= (PN4|PN3);

            usbDev->flags = USBH_DEVFLAG_FULLSPEED;
        }

        //Set endpoints default packet size: 8 bytes (minimum)
        for(i = 0; i < 16; i++)
        {
            usbDev->epINMaxSizeList[i]  = 8;
            usbDev->epOUTMaxSizeList[i] = 8;
        }

        //Wait before reset bus
        usbDev->devState = USBH_DEVSTATE_ATTACHED;
        usbDev->timerInterval = USBH_TIME_BEFORERESET;
        usbDev->timer = usbhTimerMs;

        //Call event handler
        if (usbhHardwareConfig.attached)
            usbhHardwareConfig.attached(usbDev);
    }
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
    if (!usbDev)
        return;

    //Increment counter
    usbDev->reInitCounter++;

    //Check counter value
    if (usbDev->reInitCounter >= USBH_DEVICE_REINIT_MAXCOUNT)
        USBHMoveDeviceToErrorState(usbDev); //Go to error state
    else
        USBHResetDeviceStart(usbDev);       //Start reset device
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
    u32 regHIS, regHRXS, i, rxcount, rn;

    regHIS = USB->HIS;

    //HIS.TDONE[0]=0: transaction in progress
    if ((regHIS & PN0) == 0)
        return USBH_TRANSTATE_PROGRESS;

    //HIS.TDONE[0]=1: transaction completed
    //Clear HIS.TDONE[0] with writing 1s
    USB->HIS |= PN0;

    //HRXS (rx status)
    //DATASEQ[7]  : 1/0 - rx DATA1/DATA0
    //ACKRXED[6]  : 1 - ACK received
    //STALLRXED[5]: 1 - STALL received
    //NAKRXED[4]  : 1 - NAK received
    //RXTO[3]     : 1 - Rx timeout (no device response)
    //RXOF[2]     : 1 - Rx FIFO overflow
    //BSERR[1]    : 1 - bit stuff error
    //CRCERR[0]   : 1 - CRC error

    regHRXS = USB->HRXS;

    if (regHRXS & PN4)
    {
        //NAKRXED[4]=1 - NAK received:
        //repeat IN transaction after some interval

        //Check counter
        if (usbDev->ioMaxAtCount)
        {
            usbDev->ioAtCounter++;
            if (usbDev->ioAtCounter >= usbDev->ioMaxAtCount)
            {
                //Transaction failed: return error code
                return USBH_TRANSTATE_FAIL;
            }
        }

        //Wait specified delay before repeat IN transaction
        USBHDelay(USBH_TRANREPEAT_INTERVAL);

        //Repeat IN transaction
        USBHStartInTransaction(usbDev);

        //Rx in progress
        return USBH_TRANSTATE_PROGRESS;
    }
    else if (regHRXS & (PN3|PN1|PN0))
    {
        //RXTO[3]=1   - response timeout (no response from device),
        //BSERR[1]=1  - bit stuff error,
        //CRCERR[0]=1 - CRC error:
        //repeat IN transaction

        //Check counter
        usbDev->ioAtCounter++;
        if (usbDev->ioAtCounter >= USBH_TRANATMPT_COUNT)
        {
            //Transaction failed: return error code
            PrintMsg("=========== RX.ERROR: RXTO=%i, BSERR=%i, CRCERR=%i\n",
                     (regHRXS & (PN3)) ? 1 : 0,
                     (regHRXS & (PN1)) ? 1 : 0,
                     (regHRXS & (PN0)) ? 1 : 0);
            return USBH_TRANSTATE_FAIL;
        }      

        //Wait specified delay before repeat IN transaction
        USBHDelay(USBH_TRANREPEAT_INTERVAL);

        //Repeat IN transaction
        USBHStartInTransaction(usbDev);

        //Rx in progress
        return USBH_TRANSTATE_PROGRESS;
    }
    else if (regHRXS & (PN5|PN2))
    {
        //STALLRXED[5]: 1 - STALL received
        //RXOF[2]     : 1 - Rx FIFO overflow
        return USBH_TRANSTATE_FAIL;
    }
    else if ((regHRXS == PN7 && usbDev->iotype == USBH_TRANTYPE_DATA1) ||
             (regHRXS == 0   && usbDev->iotype == USBH_TRANTYPE_DATA0))
    {
        //Current transaction success when
        //rx DATA1 and expected DATA1 OR
        //rx DATA0 and expected DATA0
        u32 epInMaxSize;

        epInMaxSize = usbDev->epINMaxSizeList[usbDev->ioepoint];

        //Reset attempts counter
        usbDev->ioAtCounter = 0;

        //Toggle between expected data0/data1
        if (usbDev->iotype == USBH_TRANTYPE_DATA0)
            usbDev->iotype = USBH_TRANTYPE_DATA1;
        else
            usbDev->iotype = USBH_TRANTYPE_DATA0;

        //Not use "USB->HRXFDC_L" register: always 0
        rxcount = USB->HRXFDC_H;

        if (usbDev->iodata && usbDev->iolength)
        {
            u8 *dest;
            rn = rxcount;

            if (usbDev->iolength < rn)
                rn = usbDev->iolength;

            //Copy rx data from endpoint FIFO to memory buffer
            dest = (u8*)usbDev->iodata;

            for(i = 0; i < rn; i++)
                dest[i] = USB->HRXFD;

            //Advance pointer to start of next portion of data
            usbDev->iodata = &usbDev->iodata[rn];
            usbDev->iolength -= rn;
        }

        //HRXFC=1: clear rx fifo
        USB->HRXFC = 1;

        if (usbDev->iolength == 0 || rxcount < epInMaxSize)
        {
            //All data was completely received from remote device:
            //return success
            return USBH_TRANSTATE_SUCCESS;
        }
        else
        {
            //Receive next portion of data
            USBHStartInTransaction(usbDev);

            //Tx in progress
            return USBH_TRANSTATE_PROGRESS;
        }
    }
    else
    {
        //Expected DATA0/DATA1 error, repeat IN transaction
        //HRXFC=1: clear rx fifo
        USB->HRXFC = 1;

        //Check counter
        usbDev->ioAtCounter++;
        if (usbDev->ioAtCounter >= USBH_TRANATMPT_COUNT)
        {
            //Transaction failed: return error code
            PrintMsg("=========== RX.ERROR: REPEAT\n");
            return USBH_TRANSTATE_FAIL;
        }

        //Wait specified delay before repeat IN transaction
        USBHDelay(USBH_TRANREPEAT_INTERVAL);

        //Repeat IN transaction
        USBHStartInTransaction(usbDev);

        //Rx in progress
        return USBH_TRANSTATE_PROGRESS;
    }
}



/* Transmission actions */
u32 USBHTxDataActions(USBHDeviceParams *usbDev)
{
    u32 regHIS, regHRXS, n;
    u16 epOutMaxSize;

    regHIS = USB->HIS;

    //HIS.TDONE[0]=0: transaction in progress
    if ((regHIS & PN0) == 0)
        return USBH_TRANSTATE_PROGRESS;

    //HIS.TDONE[0]=1: transaction completed
    //Clear HIS.TDONE[0] with writing 1s
    USB->HIS |= PN0;

    //HRXS (tx status)
    //DATASEQ[7]  : 1/0 - rx DATA1/DATA0 (IN transaction only)
    //ACKRXED[6]  : 1 - ACK received
    //STALLRXED[5]: 1 - STALL received
    //NAKRXED[4]  : 1 - NAK received
    //RXTO[3]     : 1 - response timeout (no response from device)
    //RXOF[2]     : 1 - Rx FIFO overflow (IN transaction only)
    //BSERR[1]    : 1 - bit stuff error
    //CRCERR[0]   : 1 - CRC error

    regHRXS = USB->HRXS;

    //ACKRXED[6]=1: ACK received, success
    if (regHRXS == PN6)
    {
        //Current transaction success
        //Reset counter
        usbDev->ioAtCounter = 0;

        //[2,3]=>[out0,out1]: toggle between data0/data1 transactions
        if (usbDev->iotype == USBH_TRANTYPE_DATA0)
            usbDev->iotype = USBH_TRANTYPE_DATA1;
        else
            usbDev->iotype = USBH_TRANTYPE_DATA0;

        //Length of portion data
        n = usbDev->iolength;

        epOutMaxSize = usbDev->epOUTMaxSizeList[usbDev->ioepoint];

        if (n > USB_EP_FIFO_TX_LENGTH)
            n = USB_EP_FIFO_TX_LENGTH;

        if (n > epOutMaxSize)
            n = epOutMaxSize;

        //Decrement count of bytes that must transmitted
        usbDev->iolength -= n;

        //Advance pointer to start of next portion of data
        if (usbDev->iodata)
            usbDev->iodata = &usbDev->iodata[n];        

        if (usbDev->iolength > 0)
        {            
            //Send next portion of data
            USBHStartOutTransaction(usbDev);

            //Tx in progress
            return USBH_TRANSTATE_PROGRESS;
        }
        else
        {
            //All data was completely send to remote device:
            //return success
            return USBH_TRANSTATE_SUCCESS;
        }
    }
    else if (regHRXS & PN4)
    {
        //NAKRXED[4]=1 - NAK received:
        //repeat OUT transaction after some interval

        //Check counter
        if (usbDev->ioMaxAtCount)
        {
            usbDev->ioAtCounter++;
            if (usbDev->ioAtCounter >= usbDev->ioMaxAtCount)
            {
                //Transaction failed: return error code
                return USBH_TRANSTATE_FAIL;
            }
        }

        //Wait specified delay before repeat OUT transaction
        USBHDelay(USBH_TRANREPEAT_INTERVAL);

        //Repeat portion of data
        USBHStartOutTransaction(usbDev);

        //Tx in progress
        return USBH_TRANSTATE_PROGRESS;
    }
    else if (regHRXS & (PN3|PN1|PN0))
    {
        //RXTO[3]=1   - response timeout (no response from device),
        //BSERR[1]=1  - bit stuff error,
        //CRCERR[0]=1 - CRC error:
        //repeat OUT transaction

        //Check counter
        usbDev->ioAtCounter++;
        if (usbDev->ioAtCounter >= USBH_TRANATMPT_COUNT)
        {
            //Transaction failed: return error code
            PrintMsg("=========== TX.ERROR: ACK=%i, RXTO=%i, BSERR=%i, CRCERR=%i\n",
                     (regHRXS & (PN6)) ? 1 : 0,
                     (regHRXS & (PN3)) ? 1 : 0,
                     (regHRXS & (PN1)) ? 1 : 0,
                     (regHRXS & (PN0)) ? 1 : 0);
            return USBH_TRANSTATE_FAIL;
        }

        //Wait specified delay before repeat OUT transaction
        USBHDelay(USBH_TRANREPEAT_INTERVAL);

        //Repeat portion of data
        USBHStartOutTransaction(usbDev);

        //Tx in progress
        return USBH_TRANSTATE_PROGRESS;
    }
    else
    {
        //All other cases return error:
        PrintMsg("=========== TX.ERROR: STALL=%i, RXOF=%i\n",
                 (regHRXS & PN5) ? 1 : 0,
                 (regHRXS & PN2) ? 1 : 0);

        //STALLRXED[5]=1 - STALL received
        //RXOF[2]=1      - Rx FIFO overflow
        return USBH_TRANSTATE_FAIL;
    }
}



/* Send "GET" request, receive data from device, send status */
u32 USBHGetRequestActions(USBHDeviceParams *usbDev,
                          const USBSetupRequest *req,
                          u8 *resp, u32 respLength)
{
    u32 res;

    switch (usbDev->reqState)
    {
        default:
        {
            //Start setup stage (tx USB request "GET")
            usbDev->reqState = USBH_REQSTAGE_TXREQUEST;

            USBHStartTxData(usbDev, 0, USBH_TRANTYPE_SETUP, USBH_TRANATMPT_COUNT,
                            req, USB_SETUP_REQUEST_LENGTH);

            break;
        }


        case USBH_REQSTAGE_TXREQUEST:
        {
            //Tx request in progress
            res = USBHTxDataActions(usbDev);

            if (res == USBH_TRANSTATE_FAIL)
            {
                //PrintMsg("Tx req FAIL\n");
                return USBH_TRANSTATE_FAIL;
            }
            else if (res == USBH_TRANSTATE_SUCCESS)
            {
                //PrintMsg("Tx req OK\n");

                usbDev->reqState = USBH_REQSTAGE_DATA;

                //Start data stage, expected "data1" packet from device
                USBHStartRxData(usbDev, 0, USBH_TRANTYPE_DATA1, USBH_TRANATMPT_COUNT,
                                resp, respLength);
            }


            break;
        }


        case USBH_REQSTAGE_DATA:
        {
            //Rx data in progress
            res = USBHRxDataActions(usbDev);

            if (res == USBH_TRANSTATE_FAIL)
            {                
                return USBH_TRANSTATE_FAIL;
            }
            else if (res == USBH_TRANSTATE_SUCCESS)
            {                
                //Start status stage: tx empty "data1" packet
                usbDev->reqState = USBH_REQSTAGE_STATUS;
                USBHStartTxData(usbDev, 0, USBH_TRANTYPE_DATA1, USBH_TRANATMPT_COUNT,
                                0, 0);
            }

            break;
        }


        case USBH_REQSTAGE_STATUS:
        {
            //Tx status in progress (empty "data1" packet)
            res = USBHTxDataActions(usbDev);

            if (res == USBH_TRANSTATE_FAIL || res == USBH_TRANSTATE_SUCCESS)
                return res;

            break;
        }
    } //switch

    return USBH_TRANSTATE_PROGRESS;
}



/* Send "SET" requests and request data, receive status */
u32 USBHSetRequestActions(USBHDeviceParams *usbDev,
                          const USBSetupRequest *req,
                          const u8 *txdata, u32 txLength)
{
    u32 res;

    switch (usbDev->reqState)
    {
        default:
        {
            //Start setup stage (tx USB request "SET")
            usbDev->reqState = USBH_REQSTAGE_TXREQUEST;

            USBHStartTxData(usbDev, 0, USBH_TRANTYPE_SETUP, USBH_TRANATMPT_COUNT,
                            req, USB_SETUP_REQUEST_LENGTH);

            break;
        }


        case USBH_REQSTAGE_TXREQUEST:
        {
            //Tx request in progress
            res = USBHTxDataActions(usbDev);

            if (res == USBH_TRANSTATE_FAIL)
            {
                //PrintMsg("Tx req FAIL\n");
                return USBH_TRANSTATE_FAIL;
            }
            else if (res == USBH_TRANSTATE_SUCCESS)
            {
                //PrintMsg("Tx req OK\n");

                if (txdata && txLength)
                {
                    //Data not empty
                    usbDev->reqState = USBH_REQSTAGE_DATA;

                    //Start data stage, send "data1" packet to device
                    USBHStartTxData(usbDev, 0, USBH_TRANTYPE_DATA1, USBH_TRANATMPT_COUNT,
                                    txdata, txLength);
                }
                else
                {
                    //Start status stage: rx empty "data1" packet
                    usbDev->reqState = USBH_REQSTAGE_STATUS;
                    USBHStartRxData(usbDev, 0, USBH_TRANTYPE_DATA1, USBH_TRANATMPT_COUNT,
                                    0, 0);
                }
            }


            break;
        }


        case USBH_REQSTAGE_DATA:
        {
            //Rx data in progress
            res = USBHTxDataActions(usbDev);

            if (res == USBH_TRANSTATE_FAIL)
            {
                //PrintMsg("Tx data FAIL\n");
                return USBH_TRANSTATE_FAIL;
            }
            else if (res == USBH_TRANSTATE_SUCCESS)
            {
                //PrintMsg("Tx data OK\n");

                //Start status stage: rx empty "data1" packet
                usbDev->reqState = USBH_REQSTAGE_STATUS;
                USBHStartRxData(usbDev, 0, USBH_TRANTYPE_DATA1, USBH_TRANATMPT_COUNT,
                                0, 0);
            }

            break;
        }

        case USBH_REQSTAGE_STATUS:
        {
            //Rx status in progress (empty "data1" packet)
            res = USBHRxDataActions(usbDev);

            if (res == USBH_TRANSTATE_FAIL)
            {                
                //return USBH_TRANSTATE_FAIL;
                return USBH_TRANSTATE_SUCCESS;
            }
            else if (res == USBH_TRANSTATE_SUCCESS)
            {                
                return USBH_TRANSTATE_SUCCESS;
            }

            break;
        }
    } //switch

    return USBH_TRANSTATE_PROGRESS;
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
    #define STRINGDESC_MAXLENGTH        255

    //USB request
    USBSetupRequest req;
    //Local variables
    u32 i, k, n, res;
    //String descriptor at UTF-16LE encoding (2 bytes per symbol)
    u8 destUTF16[STRINGDESC_MAXLENGTH];

    //String descriptor index must greater than 0,
    //0 - no string descriptor, return
    if (strIndex == 0)
    {
        dest[0] = 0;
        return dest;
    }

    //langIndex:
    //1 - first language (default: EN/US)
    //2 - second language (optional)
    if (langIndex != 2)
        langIndex = 1;

    //Prepare "GET_STRING" request
    req.bmRequestType = (USB_BMREQ_DIR_DEV2HOST|USB_BMREQ_TYPE_STANDARD|USB_BMREQ_RCPNT_DEVICE);
    req.bRequest      = USB_BREQ_GET_DESCRIPTOR;
    req.wValue        = (USB_BDTYPE_STRING << 8) | strIndex;
    req.wIndex        = usbDev->langs[langIndex];
    req.wLength       = STRINGDESC_MAXLENGTH; //request maximum length

    //EN/US by default
    if (req.wIndex == 0)
        req.wIndex = USB_LANGID_US;

    //Clear string descriptor buffer (UTF-16LE encoding)
    memset(destUTF16, 0, STRINGDESC_MAXLENGTH);

    //Clear destination buffer (string at 8-bit encoding)
    memset(dest, 0, maxLen);

    usbDev->reqState = USBH_REQSTAGE_INIT;

    while(1)
    {
        //Try read string descriptor
        res = USBHGetRequestActions(usbDev, &req, destUTF16, STRINGDESC_MAXLENGTH);

        if (res == USBH_TRANSTATE_FAIL ||
            res == USBH_TRANSTATE_SUCCESS)
            break;        
    }

    //Invalid descriptor type: return
    if (destUTF16[1] != USB_BDTYPE_STRING)
        return dest;

    //destUTF16[0]: string descriptor length at bytes
    n = destUTF16[0];

    k = 0;

    //Reserve one byte to zero terminate
    maxLen--;

    //String descriptor at UTF-16LE encoding (2 bytes per symbol):
    //convert to 8-bit encoding, after convertation from UTF-16LE
    //string data at Windows-1251 encoding
    for (i = 2; i < n && k < maxLen; i += 2)
        dest[k++] = destUTF16[i];

    return dest;

    #undef STRINGDESC_MAXLENGTH
}



/* Read device descriptor */
u32 USBHReadDeviceDescriptor(USBHDeviceParams *usbDev)
{
    u32 res;

    usbDev->reqState = USBH_REQSTAGE_INIT;

    while(1)
    {
        //Try read device descriptor
        res = USBHGetRequestActions(usbDev,
                                    &GET_DEVICE_DESCRIPTOR,
                                    (u8*)&usbDev->descDevice, USB_BLEN_DEVICE);

        if (res == USBH_TRANSTATE_FAIL ||
            res == USBH_TRANSTATE_SUCCESS)
            break;
    }

    return res;
}



/* Update USB protocol state.
   Call once at main cycle */
void UpdateUSBHostState(USBBLOCK usb)
{
    u32 res, regHIS, regHRXCS;
    USBHDeviceParams *usbDev = &usbhDevices[0];    

    //HIS register, bits cleared with writing 1s:
    //SOFS[3]  : SOF send
    //CONEV[2] : Connection/disconnection event
    //RESUME[1]: Repeat
    //TDONE[0] : Transaction completed

    //HRXS (rx status)
    //DATASEQ[7]  : 1/0 - rx DATA1/DATA0
    //ACKRXED[6]  : 1 - ACK received
    //STALLRXED[5]: 1 - STALL received
    //NAKRXED[4]  : 1 - NAK received    
    //RXTO[3]     : 1 - Rx timeout (no device response)
    //RXOF[2]     : 1 - Rx FIFO overflow
    //BSERR[1]    : 1 - bit stuff error
    //CRCERR[0]   : 1 - CRC error

    //HTXLC
    //FSLR[4]: 1/0 12/1.5Mbit/s,
    //FSPL[3]: 1/0 full/low speed polarity
    //DC[2]:   1/0 direct/normal work
    //TXLC[1,0]: [DP,DM] line state

    regHIS = USB->HIS;

    if (regHIS & PN2)
    {
        //HIS.CONEV[2]=1: connection/disconnection event
        //Clear bit with writing 1s
        USB->HIS |= PN2;

        //HRXCS
        //RXLS[1..0]: [0,1,2] => [disconnect,low speed,full speed]
        regHRXCS = (USB->HRXCS & 3);

        if (regHRXCS == 0)
        {
            //Device disconnected
            if ((usbDev->flags & USBH_DEVFLAG_SPEEDMASK) != 0)
            {
                //Device was connected: call event handler
                if (usbhHardwareConfig.detached)
                    usbhHardwareConfig.detached(usbDev);
            }

            //Clear device structure after "detached" event handler
            memset(usbDev, 0, sizeof(USBHDeviceParams));                        
        }        
        else
        {
            //Device connected
            //Clear device structure
            memset(usbDev, 0, sizeof(USBHDeviceParams));

            //Save host controller number
            usbDev->usbhost = usb;

            //Start device reset
            USBHResetDeviceStart(usbDev);
        }

        return;
    }


    /*
    //HIS.SOFS[3]=1: SOF send, each 1 msec.
    //("USBH_HWOPTS_TX_SOF" must be set at USBHostConfig::opts)
    if (regHIS & PN3)
    {
        //Clear bit with writing 1s
        USB->HIS |= PN3;

        //To do actions here (if need)
    }*/


    //Wait specified time interval
    if (usbDev->timerInterval > 0)
    {
        if ((usbDev->timer + usbDev->timerInterval) > usbhTimerMs)
            return;
        else
            usbDev->timerInterval = 0;
    }


    //Call second level of protocol handler (device specified)
    if (usbDev->devState >= USBH_DEVSTATE_2ND_LEVEL)
    {        
        if (usbDev->deviceProtoHandler)
            usbDev->deviceProtoHandler(usbDev, 0);
        return;
    }


    //First level, basic USB requests:
    //get device descriptor, set address, get configuration
    switch(usbDev->devState)
    {
        case USBH_DEVSTATE_ATTACHED:
        {
            //Start reset USB bus: DP=DM=0
            USBHPutDPDMToLow();

            //Reset bus for time >10ms
            usbDev->timerInterval = USBH_TIME_DEVRESET;
            usbDev->timer = usbhTimerMs;

            usbDev->devState = USBH_DEVSTATE_RESET;
            usbDev->reqState = USBH_REQSTAGE_INIT;

            break;
        } //USBH_DEVSTATE_ATTACHED


        case USBH_DEVSTATE_RESET:
        {            
            //DP,DM to work mode
            USBHGoToWorkMode();            

            usbDev->timerInterval = USBH_TIME_AFTERRESET;
            usbDev->timer = usbhTimerMs;

            usbDev->devState = USBH_DEVSTATE_GET_DEVICE0;            
            usbDev->reqState = USBH_REQSTAGE_INIT;

            break;
        } //USBH_DEVSTATE_RESET


        case USBH_DEVSTATE_GET_DEVICE0:
        {
            //"GET DEVICE DESCRIPTOR" before address is set
            res = USBHGetRequestActions(usbDev,
                                        &GET_DEVICE_DESCRIPTOR,
                                        (u8*)&usbDev->descDevice, USB_BLEN_DEVICE);

            if (res == USBH_TRANSTATE_FAIL)
            {
                PrintMsg("GET_DEV0 FAIL\n");
                USBHReInitDevice(usbDev);
            }
            else if (res == USBH_TRANSTATE_SUCCESS)
            {
                //Check received descriptor
                if (!(usbDev->descDevice.bLength == USB_BLEN_DEVICE &&
                      usbDev->descDevice.bDescriptorType == USB_BDTYPE_DEVICE))
                {
                    USBHReInitDevice(usbDev);
                    break;
                }

                PrintMsg("GET_DEV0, EP0 FIFO %i OK\n", usbDev->descDevice.bMaxPacketSize0);

                //byte[7]: bMaxPacketSize0 - maximum ep0 packet size
                usbDev->epINMaxSizeList[0]  = usbDev->descDevice.bMaxPacketSize0;
                usbDev->epOUTMaxSizeList[0] = usbDev->descDevice.bMaxPacketSize0;

                //Start address setup
                //Increment address counter, device address is 7 bits [1..127]
                usbhAddressCounter++;
                if (usbhAddressCounter > 127)
                    usbhAddressCounter = 1;

                //Put address to request: fill "wValue" field
                memcpy(&usbhCurrSetupRequest, &SET_ADDRESS_REQUEST, USB_SETUP_REQUEST_LENGTH);
                usbhCurrSetupRequest.wValue = usbhAddressCounter;

                usbDev->devState = USBH_DEVSTATE_SET_ADDRESS;
                usbDev->reqState = USBH_REQSTAGE_INIT;
            }

            break;
        } //USBH_DEVSTATE_GET_DEVICE0


        case USBH_DEVSTATE_SET_ADDRESS:
        {            
            //"SET ADDRESS" (no tx data)
            res = USBHSetRequestActions(usbDev, &usbhCurrSetupRequest, 0, 0);

            if (res == USBH_TRANSTATE_FAIL)
            {                
                PrintMsg("SET_ADDR FAIL\n");
                USBHReInitDevice(usbDev);
            }
            else if (res == USBH_TRANSTATE_SUCCESS)
            {
                PrintMsg("SET_ADDR %i OK\n", usbhAddressCounter);

                //Save device address (must none-zero)
                usbDev->address  = usbhAddressCounter;

                usbDev->flags |= USBH_DEVFLAG_ADDRESSED;

                usbDev->devState = USBH_DEVSTATE_GET_DEVICE;
                usbDev->reqState = USBH_REQSTAGE_INIT;
            }

            break;
        } //USBH_DEVSTATE_SET_ADDRESS


        case USBH_DEVSTATE_GET_DEVICE:
        {
            //"GET DEVICE DESCRIPTOR" after address is set
            res = USBHGetRequestActions(usbDev,
                                        &GET_DEVICE_DESCRIPTOR,
                                        (u8*)&usbDev->descDevice, USB_BLEN_DEVICE);

            if (res == USBH_TRANSTATE_FAIL)
            {
                PrintMsg("GET_DEV FAIL\n");
                USBHReInitDevice(usbDev);
            }
            else if (res == USBH_TRANSTATE_SUCCESS)
            {
                //Check received descriptor
                if (!(usbDev->descDevice.bLength == USB_BLEN_DEVICE &&
                      usbDev->descDevice.bDescriptorType == USB_BDTYPE_DEVICE))
                {
                    USBHReInitDevice(usbDev);
                    break;
                }

                PrintMsg("GET_DEV OK\n");
                usbDev->devState = USBH_DEVSTATE_GET_CONFIG0;
                usbDev->reqState = USBH_REQSTAGE_INIT;
            }

            break;
        } //USBH_DEVSTATE_GET_DEVICE


        case USBH_DEVSTATE_GET_CONFIG0:
        {
            //"GET CONFIGURATION": get first 9 bytes to define total cfg length
            res = USBHGetRequestActions(usbDev,
                                        &GET_CONFIGURATION_DESCRIPTOR,
                                        usbDev->descConfig, USB_BLEN_CONFIGURATION);

            if (res == USBH_TRANSTATE_FAIL)
            {
                PrintMsg("GET_CFG0 FAIL\n");
                USBHReInitDevice(usbDev);
            }
            else if (res == USBH_TRANSTATE_SUCCESS)
            {                
                u16 totalLength;

                //Check received descriptor
                if (!(usbDev->descConfig[0] == USB_BLEN_CONFIGURATION &&
                      usbDev->descConfig[1] == USB_BDTYPE_CONFIGURATION))
                {
                    USBHReInitDevice(usbDev);
                    break;
                }

                PrintMsg("GET_CFG0 OK\n");

                //Get total configuration length from config descriptor
                totalLength = (usbDev->descConfig[3] << 8) | (usbDev->descConfig[2]);

                //Check length
                if (totalLength > USBH_DESCCONFIG_MAXLENGTH)
                    totalLength = USBH_DESCCONFIG_MAXLENGTH;
                else if (totalLength < USB_BLEN_CONFIGURATION)
                    totalLength = USB_BLEN_CONFIGURATION;

                memcpy(&usbhCurrSetupRequest, &GET_CONFIGURATION_DESCRIPTOR, USB_SETUP_REQUEST_LENGTH);
                usbhCurrSetupRequest.wLength = totalLength;

                PrintMsg("TOT_LEN %i\n", totalLength);

                usbDev->descConfigTotalLength = totalLength;
                usbDev->devState = USBH_DEVSTATE_GET_CONFIG;
                usbDev->reqState = USBH_REQSTAGE_INIT;
            }

            break;
        } //USBH_DEVSTATE_GET_CONFIG0


        case USBH_DEVSTATE_GET_CONFIG:
        {
            //"GET CONFIGURATION": get full configuration descriptor
            res = USBHGetRequestActions(usbDev,
                                        &usbhCurrSetupRequest,
                                        usbDev->descConfig, usbDev->descConfigTotalLength);

            if (res == USBH_TRANSTATE_FAIL)
            {
                PrintMsg("GET_CFG FAIL\n");
                USBHReInitDevice(usbDev);
            }
            else if (res == USBH_TRANSTATE_SUCCESS)
            {                
                //Check received descriptor
                if (!(usbDev->descConfig[0] == USB_BLEN_CONFIGURATION &&
                      usbDev->descConfig[1] == USB_BDTYPE_CONFIGURATION))
                {
                    USBHReInitDevice(usbDev);
                    break;
                }

                //Define maximum packet size for each endpoint
                //using configuration descriptor
                USBGetEndpointsMaxSize(usbDev->descConfig,
                                       usbDev->epINMaxSizeList,
                                       usbDev->epOUTMaxSizeList);

                usbDev->devState = USBH_DEVSTATE_GET_SUPLANGS;
                usbDev->reqState = USBH_REQSTAGE_INIT;
            }

            break;
        } //USBH_DEVSTATE_GET_CONFIG


        case USBH_DEVSTATE_GET_SUPLANGS:
        {
            //"GET STRING": get supported languages
            res = USBHGetRequestActions(usbDev,
                                        &GET_STRING_DESCRIPTOR,
                                        (u8*)usbDev->langs, 6);

            if (res == USBH_TRANSTATE_FAIL ||
                res == USBH_TRANSTATE_SUCCESS)
            {
                //This request MUST always completed successfully:
                //some devices does not have any string descriptors
                //and return "STALL" response to this request
                if (res == USBH_TRANSTATE_FAIL)
                {
                    //US/EN laguage supported by default
                    usbDev->langs[1] = USB_LANGID_US;
                    usbDev->langs[2] = 0;
                }                

                PrintMsg("GET_LANGS OK: %04X %04X\n",
                         usbDev->langs[1], usbDev->langs[2]);

                //Start set configuration
                //Fill "wValue" field to value get from config descriptor:
                //byte[5] of configuration descriptor
                memcpy(&usbhCurrSetupRequest, &SET_CONFIGURATION_REQUEST, USB_SETUP_REQUEST_LENGTH);
                usbhCurrSetupRequest.wValue = usbDev->descConfig[5];

                PrintMsg("SET_CFG %i...\n", usbhCurrSetupRequest.wValue);

                usbDev->devState = USBH_DEVSTATE_SET_CONFIG;
                usbDev->reqState = USBH_REQSTAGE_INIT;
            }

            break;
        } //USBH_DEVSTATE_GET_SUPLANGS


        case USBH_DEVSTATE_SET_CONFIG:
        {
            //"SET CONFIGURATION"
            res = USBHSetRequestActions(usbDev, &usbhCurrSetupRequest, 0, 0);

            if (res == USBH_TRANSTATE_FAIL)
            {
                PrintMsg("SET_CFG FAIL\n");
                USBHReInitDevice(usbDev);
            }
            else if (res == USBH_TRANSTATE_SUCCESS)
            {
                PrintMsg("SET_CFG OK\n");

                usbDev->flags |= USBH_DEVFLAG_CONFIGURED;                

                usbDev->devState = USBH_DEVSTATE_2ND_LEVEL;
                usbDev->reqState = USBH_REQSTAGE_INIT;

                //Now device at configured state.
                //Call event handler
                if (usbhHardwareConfig.configured)
                    usbhHardwareConfig.configured(usbDev);
            }

            break;
        } //USBH_DEVSTATE_SET_CONFIG


        case USBH_DEVSTATE_ERROR:
        {
            //Device at error state: no actions
            break;
        } //USBH_DEVSTATE_ERROR
    } //switch(usbDev->devState)   
}

/*===========================================================================
 * End of file 1986BE9x_USB_host.c
 *===========================================================================*/
