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
 *  File 1986BE9x_USB_dev.c: USB device functions.
 *
 *===========================================================================*/

#include "../../inc/USB_dev.h"
#include <CPU.h>
#include <string.h>
#include <consp.h>

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
#define PrintMsg(...)       //ConPrintf(__VA_ARGS__)

#define USB_SSTAGE_SETUP        0
#define USB_SSTAGE_DATA         1
#define USB_SSTAGE_STATUS       2
#define USB_SSTAGE_UPADDR       3

/* Runtime configuration */
typedef struct tag_USBRtmConfig
{
    //Device configuration (descriptors)
    USBDConfig devConfig;

    //Received setup request (setup transaction)
    USBSetupRequest recvRequest;

    //Additional data followed setup request (out transaction)
    u8 recvReqData[USB_EP_FIFO_RX_LENGTH];

    //Setup request state
    u8 setupStage;

    //Device state
    u8 state;

    //Device address
    u8 address;

    //Index of selected configuration
    u8 cfgIndex;    

    //Count of bytes in "recvReqData" buffer
    s32 recvReqDataCount;

    //Pointer to transmitted data (to host)
    const u8 *pTxData;

    //Length of transmitted data
    s32 txLength;

    //Pointer to data handler
    void (*evhandler)(u8 epIndex, u32 evtype);
} USBRtmConfig;


/* Reset end points to initial state */
static void ResetEndPoints();


//Runtime configuration
static USBRtmConfig usbRtmConfig;


//Call user-defined event handler
#define CallEventHandler(ep, evtype) if (usbRtmConfig.evhandler) \
                                         usbRtmConfig.evhandler(ep, evtype)


//Set request error: EPSTALL[3]=1 of endpoint #0
#define USBEp0SetRequestError()     USB->SEP0_CTRL |= PN3


/* Initialize USB device */
MCRESULT InitializeUSBD(const USBDConfig *cfg)
{
    u32 val;

    //Clear structure
    memset(&usbRtmConfig, 0, sizeof(USBRtmConfig));

    //Save requested configuration and pointer to data handler
    memcpy(&usbRtmConfig.devConfig, cfg, sizeof(USBDConfig));
    usbRtmConfig.evhandler = cfg->evhandler;

    usbRtmConfig.state = USB_DEVSTATE_INITIAL;

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
    //USB_C1_SEL[1..0]: [0,1,2,3] => [HSI,HSI/2,HSE,HSE/2]
    RST_CLK->USB_CLOCK = PN8 | PN2 | 2;


    //HSCR register:
    //DM_PULLDOWN[7]: 1/0 - on/off D- pulldown
    //DM_PULLUP[6]  : 1/0 - on/off D- pullup
    //DP_PULLDOWN[5]: 1/0 - on/off D+ pulldown
    //DP_PULLUP[4]  : 1/0 - on/off D- pullup
    //EN_RX[3]      : 1/0 - on/off USB receiver
    //EN_TX[2]      : 1/0 - on/off USB transmitter
    //RESET_CORE[1] : 1/0 - reset/work mode
    //HOST_MODE[0]  : 1/0 - host/device mode
    if (cfg->opts & USBD_OPTS_FULLSPEED)
        USB->HSCR = PN4 | PN1; //DP_PULLUP=1 - full speed
    else
        USB->HSCR = PN6 | PN1; //DM_PULLUP=1 - low speed

    //Waits more than 10 CPU ticks while USB reset
    for (val = 0; val <= 40; val++) __NOP();

    //Clear RESET_CORE[1] bit
    if (cfg->opts & USBD_OPTS_FULLSPEED)
        USB->HSCR = PN4; //DP_PULLUP=1 - full speed
    else
        USB->HSCR = PN6; //DM_PULLUP=1 - low speed

    //SC register:
    //SCFSR[5]: 1/0 - 12/1.5 Mbit/s (speed)
    //SCFSP[4]: 1/0 - full/low speed (polarity)
    //SCDC[3] : 1/0 - enable/disable direct control (D+,D-)
    //SCTXL[2]: 1 - D+ (when SCDC=1)
    //SCTXL[1]: 1 - D- (when SCDC=1)
    //SCGEN[0]: 1/0 - enable/disable endpoints
    if (cfg->opts & USBD_OPTS_FULLSPEED)
        USB->SC = PN5 | PN4 | PN0; //Full speed
    else
        USB->SC = PN0; //Low speed

    //Reset endpoints to initial state
    ResetEndPoints();

    if (cfg->evhandler)
    {
        //SIM register (1/0 - enable/disable interrupts):
        //SCNAKSENTIE [4]: NAK packet sent
        //SCSOFRECIE  [3]: SOF packet received
        //SCRESETEVIE [2]: reset event
        //SCRESUMEIE  [1]: repeat state
        //SCTODONEIE  [0]: transaction completed
        USB->SIM = PN2 | PN0;

        //Enable interrupt from USB controller
        IRQEnable(IRQ_USB);
    }
    else
    {
        USB->SIM = 0;

        //Disable interrupt from USB controller
        IRQDisable(IRQ_USB);
    }

    return MCR_OK;
}



/* Turn USB device */
void TurnUSBD(USBBLOCK usb, u8 on)
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



/* Reset end points to initial state */
static void ResetEndPoints()
{
    USBDConfig *dcfg;

    //SEPx_CTRL register:
    //EPISOEN[4]  : 1/0 - isochronous mode (no send ACKs) /none isochronous
    //EPSTALL[3]  : 1/0 - send STALL (when EPEN=EPRDY=1 and EPISOEN=0) / no send
    //EPDATASEQ[2]: 1/0 - tx data with DATA1/DATA0 (IN transaction)
    //EPRDY[1]    : 1/0 - endpoint is ready/not ready
    //              NOTE: automatically reset to 0 when transaction completed
    //EPEN[0]     : 1/0 - enable/disable endpoint
    //              NOTE:
    //              EPEN=0 - endpoint is not respond to transaction
    //              EPEN=1 and EPRDY=EPISOEN=0 - endpoint send NAK

    //SEPx_RXFC : clear rx FIFO
    //SEPx_TXFDC: clear tx FIFO

    dcfg = &usbRtmConfig.devConfig;


    //Endpoint #0 (SETUP), always enabled, tx with DATA1:
    //EPDATASEQ[2]=1; EPRDY[1]=EPEN[0]=1;
    USB->SEP0_CTRL  = PN2|PN1|PN0;
    USB->SEP0_RXFC  = 1;
    USB->SEP0_TXFDC = 1;


    //Endpoint #1:
    if (dcfg->opts & USBD_OPTS_EP1EN)
    {
        //EPISOEN[4]=1/0; EPDATASEQ[2]=0; EPRDY[1]=EPEN[0]=1
        if (dcfg->opts & USBD_OPTS_EP1ISOEN)
            USB->SEP1_CTRL = PN4|PN1|PN0;
        else
            USB->SEP1_CTRL = PN1|PN0;

        USB->SEP1_RXFC  = 1;
        USB->SEP1_TXFDC = 1;
    }
    else
        USB->SEP1_CTRL = 0;


    //Endpoint #2:
    if (dcfg->opts & USBD_OPTS_EP2EN)
    {
        //EPISOEN[4]=1/0; EPDATASEQ[2]=0; EPRDY[1]=EPEN[0]=1
        if (dcfg->opts & USBD_OPTS_EP2ISOEN)
            USB->SEP2_CTRL = PN4|PN1|PN0;
        else
            USB->SEP2_CTRL = PN1|PN0;

        USB->SEP2_RXFC  = 1;
        USB->SEP2_TXFDC = 1;
    }
    else
        USB->SEP2_CTRL = 0;


    //Endpoint #3:
    if (dcfg->opts & USBD_OPTS_EP3EN)
    {
        //EPISOEN[4]=1/0; EPDATASEQ[2]=0; EPRDY[1]=EPEN[0]=1
        if (dcfg->opts & USBD_OPTS_EP3ISOEN)
            USB->SEP3_CTRL = PN4|PN1|PN0;
        else
            USB->SEP3_CTRL = PN1|PN0;

        USB->SEP3_RXFC  = 1;
        USB->SEP3_TXFDC = 1;
    }
    else
        USB->SEP3_CTRL = 0;
}



/* Get USB clock, Hz */
u32 GetUSBDClk(USBBLOCK usb)
{
    u32 speed_USB_C1 = 0, speed_USB_C2;

    //USB_CLOCK register:
    //USB_CLK_EN[8]: 1/0 - enable/disable USB clock
    //USB_C3_SEL[4]: 1/0 - USB_C3 = USB_C2 / (USB_C3_SEL + 1)
    //USB_C2_SEL[2]: 1   - USB_C2 = USB_C1 * (PLL_USB_MUL + 1)
    //               0   - USB_C2 = USB_C1
    //USB_C1_SEL[1..0]: [0,1,2,3] => [HSI,HSI/2,HSE,HSE/2]

    //Select USB_C1 speed
    switch(RST_CLK->USB_CLOCK & 0x03)
    {
        case 0: { speed_USB_C1 =  FREQ_HSI;       break; }
        case 1: { speed_USB_C1 = (FREQ_HSI >> 1); break; }
        case 2: { speed_USB_C1 =  FREQ_HSE;       break; }
        case 3: { speed_USB_C1 = (FREQ_HSE >> 1); break; }
    }

    //Select USB_C2: USB_C1 or pllUSBo
    if (RST_CLK->USB_CLOCK & PN2)
    {
        //PLL_USB_ON[0]=1
        if (RST_CLK->PLL_CONTROL & PN0)
        {
            u32 pll_USB_MUL = (RST_CLK->PLL_CONTROL & 0x00F0) >> 4;
            speed_USB_C2 = speed_USB_C1 * (pll_USB_MUL + 1);
        }
        else
            speed_USB_C2 = 0;
    }
    else    
        speed_USB_C2 = speed_USB_C1;

    //USB_C3_SEL[4]: 1 - USB_C3 = USB_C2/2;
    //               0 - USB_C3 = USB_C2
    if (RST_CLK->USB_CLOCK & PN4)
        return (speed_USB_C2 >> 1);
    else
        return (speed_USB_C2);
}



/* Get USB speed, bit/s */
u32 GetUSBDSpeed(USBBLOCK usb)
{
    u32 sls;

    //SLS register:
    //SCRXLS[1..0]: [0,1,2] => [reset, low speed, full speed]
    sls = (USB->SLS & 0x03);

    if (sls == 1)
        return 1500000;
    else if (sls == 2)
        return 12000000;
    else
        return 0;
}



/* */
void USBDSetStall(USBBLOCK usb, u8 epIndex, u8 stall)
{
    //SEPx_CTRL register:
    //EPSTALL[3]  : 1 - send STALL (when EPEN=EPRDY=1 and EPISOEN=0)
    //              0 - no send STALL
    if (stall)
    {
        switch (epIndex)
        {            
            case 1: { USB->SEP1_CTRL |= PN3; break; }
            case 2: { USB->SEP2_CTRL |= PN3; break; }
            case 3: { USB->SEP3_CTRL |= PN3; break; }
            case 0: { USB->SEP0_CTRL |= PN3; break; }
        }
    }
    else
    {
        switch (epIndex)
        {            
            case 1: { USB->SEP1_CTRL &= ~PN3; break; }
            case 2: { USB->SEP2_CTRL &= ~PN3; break; }
            case 3: { USB->SEP3_CTRL &= ~PN3; break; }
            case 0: { USB->SEP0_CTRL &= ~PN3; break; }
        }
    }
}



/* */
void USBDSetReady(USBBLOCK usb, u8 epIndex, u8 ready)
{
    //SEPx_CTRL register:
    //EPRDY[1]    : 1/0 - endpoint is ready/not ready
    //              NOTE: automatically reset to 0 when transaction completed
    if (ready)
    {
        switch (epIndex)
        {            
            case 1: { USB->SEP1_CTRL |= PN1; break; }
            case 2: { USB->SEP2_CTRL |= PN1; break; }
            case 3: { USB->SEP3_CTRL |= PN1; break; }
            case 0: { USB->SEP0_CTRL |= PN1; break; }
        }
    }
    else
    {
        switch (epIndex)
        {            
            case 1: { USB->SEP1_CTRL &= ~PN1; break; }
            case 2: { USB->SEP2_CTRL &= ~PN1; break; }
            case 3: { USB->SEP3_CTRL &= ~PN1; break; }
            case 0: { USB->SEP0_CTRL &= ~PN1; break; }
        }
    }
}



/* Indicates that specified endpoint in "ready state" */
u32 USBDIsReady(USBBLOCK usb, u8 epIndex)
{
    switch (epIndex)
    {
        case 1: return (USB->SEP1_CTRL & PN1);
        case 2: return (USB->SEP2_CTRL & PN1);
        case 3: return (USB->SEP3_CTRL & PN1);
        case 0: return (USB->SEP0_CTRL & PN1);
        default: return 0;
    }
}



/* */
void USBDSetEnable(USBBLOCK usb, u8 epIndex, u8 enable)
{
    //SEPx_CTRL register:
    //EPEN[0]     : 1/0 - enable/disable endpoint
    //              NOTE:
    //              EPEN=0 - endpoint is not respond to transaction
    //              EPEN=1 and EPRDY=EPISOEN=0 - endpoint send NAK
    if (enable)
    {
        switch (epIndex)
        {            
            case 1: { USB->SEP1_CTRL |= PN0; break; }
            case 2: { USB->SEP2_CTRL |= PN0; break; }
            case 3: { USB->SEP3_CTRL |= PN0; break; }
            case 0: { USB->SEP0_CTRL |= PN0; break; }
        }
    }
    else
    {
        switch (epIndex)
        {            
            case 1: { USB->SEP1_CTRL &= ~PN0; break; }
            case 2: { USB->SEP2_CTRL &= ~PN0; break; }
            case 3: { USB->SEP3_CTRL &= ~PN0; break; }
            case 0: { USB->SEP0_CTRL &= ~PN0; break; }
        }
    }
}



/* Get count of bytes stored at rx FIFO */
u32 USBDGetInFIFOCount(USBBLOCK usb, u8 epIndex)
{
    //SEPx_RXFDC_H register: count of bytes at rx FIFO
    switch(epIndex)
    {        
        case 1: return (u32)(USB->SEP1_RXFDC_H);
        case 2: return (u32)(USB->SEP2_RXFDC_H);
        case 3: return (u32)(USB->SEP3_RXFDC_H);
        case 0: return (u32)(USB->SEP0_RXFDC_H);
        default: return 0;
    }
}



/* Read data from FIFO of specified end point */
s32 USBDRead(USBBLOCK usb, u8 epIndex, void *dest, s32 count)
{
    s32 i, SEPx_RXFDC_H;
    __io_reg *pSEPx_RXFD;
    u8 *pdest;

    //SEPx_RXFDC_H: count of bytes at rx FIFO
    //pSEPx_RXFD  : pointer to rx FIFO
    switch(epIndex)
    {        
        case 1:
        {
            SEPx_RXFDC_H = (s32)USB->SEP1_RXFDC_H;
            pSEPx_RXFD   = &USB->SEP1_RXFD;
            break;
        }

        case 2:
        {
            SEPx_RXFDC_H = (s32)USB->SEP2_RXFDC_H;
            pSEPx_RXFD   = &USB->SEP2_RXFD;
            break;
        }

        case 3:
        {
            SEPx_RXFDC_H = (s32)USB->SEP3_RXFDC_H;
            pSEPx_RXFD   = &USB->SEP3_RXFD;
            break;
        }

        case 0:
        {
            SEPx_RXFDC_H = (s32)USB->SEP0_RXFDC_H;
            pSEPx_RXFD   = &USB->SEP0_RXFD;
            break;
        }

        default: return 0;
    }

    pdest = (u8*)dest;

    if (count > SEPx_RXFDC_H)
        count = SEPx_RXFDC_H;   

    //Read data from rx FIFO
    for(i = 0; i < count; i++)
        pdest[i] = *pSEPx_RXFD;

    //SEPx_RXFC=1: clear rx FIFO
    switch(epIndex)
    {        
        case 1: { USB->SEP1_RXFC = 1; break; }
        case 2: { USB->SEP2_RXFC = 1; break; }
        case 3: { USB->SEP3_RXFC = 1; break; }
        case 0: { USB->SEP0_RXFC = 1; break; }
    }

    return i;
}



/* Write data to tx FIFO of specified endpoint */
s32 USBDWrite(USBBLOCK usb, u8 epIndex, const void *src, s32 count)
{
    s32 i;
    __io_reg *pSEPx_TXFD;
    const u8* psrc = (u8*)src;

    //SEPx_TXFDC=1: clear tx FIFO
    //SEPx_TXFD   : pointer to tx FIFO
    switch(epIndex)
    {        
        case 1:
        {
            USB->SEP1_TXFDC = 1;
            pSEPx_TXFD = &USB->SEP1_TXFD;
            break;
        }

        case 2:
        {
            USB->SEP2_TXFDC = 1;
            pSEPx_TXFD = &USB->SEP2_TXFD;
            break;
        }

        case 3:
        {
            USB->SEP3_TXFDC = 1;
            pSEPx_TXFD = &USB->SEP3_TXFD;
            break;
        }

        case 0:
        {
            USB->SEP0_TXFDC = 1;
            pSEPx_TXFD = &USB->SEP0_TXFD;
            break;
        }

        default: return 0;
    }

    if (count > USB_EP_FIFO_TX_LENGTH)
        count = USB_EP_FIFO_TX_LENGTH;

    for(i = 0; i < count; i++)
        *pSEPx_TXFD = psrc[i];

    return i;
}



/* Get current USB device address */
u8 USBDGetAddress(USBBLOCK usb)
{
    //SA register:
    //SDEVADDR[6..0]: device address
    return (USB->SA & 0x7F);
}



/* Get last setup request */
USBSetupRequest *USBDGetSetupRequest(USBBLOCK usb)
{
    return &usbRtmConfig.recvRequest;
}



/* Get additional data of setup request */
const u8 *USBDGetSetupData(USBBLOCK usb, s32 *count)
{
    *count = usbRtmConfig.recvReqDataCount;
    return usbRtmConfig.recvReqData;
}



/* Get current device state */
u8 USBDGetDeviceState(USBBLOCK usb)
{
    return usbRtmConfig.state;
}



/* Get number of selected configuration */
u8 USBDGetConfiguration(USBBLOCK usb)
{
    return usbRtmConfig.cfgIndex;
}

/*===========================================================================
 * SETUP REQUEST HANDLER: DEVICE
 *===========================================================================*/


/* Setup request handler: device (get requests) */
static void USBSetupRequestHandler_device_get(USBRtmConfig *rtm)
{
    USBSetupRequest *req  = &rtm->recvRequest;
    USBDConfig      *dcfg = &rtm->devConfig;

    switch (req->bRequest)
    {
        case USB_BREQ_GET_DESCRIPTOR:
        {
            //"wValue" high byte: descriptor type
            u8 bdtype = (u8)(req->wValue >> 8);

            switch (bdtype)
            {
                case USB_BDTYPE_DEVICE:
                {
                    if (dcfg->dscDevice != 0)
                    {
                        rtm->pTxData  = (u8*)dcfg->dscDevice;
                        rtm->txLength = dcfg->dscDevice[0];
                    }
                    break;
                } //USB_BDTYPE_DEVICE


                case USB_BDTYPE_QUALIFIER:
                {
                    if (dcfg->dscQualifier != 0)
                    {
                        rtm->pTxData  = (u8*)dcfg->dscQualifier;
                        rtm->txLength = dcfg->dscQualifier[0];
                    }
                    break;
                } //USB_BDTYPE_QUALIFIER


                case USB_BDTYPE_CONFIGURATION:
                {
                    if (dcfg->dscConfiguration != 0)
                    {
                        //dscConfiguration[3,2]: total length (high, low bytes)
                        rtm->pTxData  = (u8*)dcfg->dscConfiguration;
                        rtm->txLength = dcfg->dscConfiguration[3];
                        rtm->txLength = (rtm->txLength << 8) | dcfg->dscConfiguration[2];
                    }
                    break;
                } //USB_BDTYPE_CONFIGURATION


                case USB_BDTYPE_STRING:
                {
                    //"wIndex" contains language ID

                    //"wValue" low byte:  requested string index
                    //"wValue" high byte: descriptor type (string)
                    u8 reqStrIndex = (u8)req->wValue;

                    if (reqStrIndex == 0)
                    {
                        //Zero index: send available language IDs
                        u16 i, dscLength = 1;
                        u16 dscLANGIDs[(USBD_LANGIDS_MAX_COUNT + 1)];

                        for(i = 0; i < USBD_LANGIDS_MAX_COUNT; i++)
                        {
                            if (dcfg->languageID[i] != USB_LANGID_NEUTRAL)
                                dscLANGIDs[dscLength++] = dcfg->languageID[i];
                        }

                        //Get descriptor length at bytes
                        dscLength <<= 1;

                        dscLANGIDs[0] = (USB_BDTYPE_STRING << 8) | dscLength;

                        USBDWrite(USB_N1, 0, dscLANGIDs, dscLength);
                    }
                    else if (reqStrIndex == dcfg->dscDevice[16])
                    {
                        //iSerialNumber: one for all language IDs
                        rtm->pTxData  = (u8*)dcfg->strSerialNumber;
                    }
                    else
                    {
                        rtm->pTxData = 0;

                        if (req->wIndex == dcfg->languageID[1])
                        {
                            //Second language ID (local)
                            if (reqStrIndex == dcfg->dscDevice[14])      //iManufacturerID
                                rtm->pTxData = (u8*)dcfg->strManufacturer1;
                            else if (reqStrIndex == dcfg->dscDevice[15]) //iProductID
                                rtm->pTxData = (u8*)dcfg->strProductID1;
                        }

                        if (rtm->pTxData == 0)
                        {
                            //First language ID (english or local), default
                            if (reqStrIndex == dcfg->dscDevice[14])      //iManufacturerID
                                rtm->pTxData = (u8*)dcfg->strManufacturer;
                            else if (reqStrIndex == dcfg->dscDevice[15]) //iProductID
                                rtm->pTxData = (u8*)dcfg->strProductID;
                        }

                        //Requested string descriptor is unknown:
                        //call user-defined handler
                        if (rtm->pTxData == 0 && rtm->evhandler)
                            rtm->evhandler(0, USBD_EVTYPE_TRAN_SETUP);
                    }

                    break;
                } //USB_BDTYPE_STRING

                case USB_BDTYPE_INTERFACE:
                {                    
                    break;
                } //USB_BDTYPE_INTERFACE
            } //switch (req->bDescriptorType)

            break;
        } //USB_BREQ_GET_DESCRIPTOR


        case USB_BREQ_GET_STATUS:
        {
            //Response to GET_STATUS request:
            //bits[15..2]: reserved and 0
            //bit[1]     : 1 - Remote Wakeup
            //bit[0]     : 1 - Self Powered

            //dscConfiguration[7]: bmAttributes
            //bit[7]: Bus powered
            //bit[6]: Self powered
            //bit[5]: Remote wakeup
            s32 txcount;
            u8 attr = dcfg->dscConfiguration[7];
            u8 status[2];

            status[1] = 0;
            status[0] = ((attr & PN6) ? PN0 : 0) |
                        ((attr & PN5) ? PN1 : 0);

            txcount = 2;

            if (txcount > req->wLength)
                txcount = req->wLength;

            //Only two bytes requested: write FIFO here
            USBDWrite(USB_N1, 0, status, txcount);
            break;
        } //USB_BREQ_GET_STATUS


        case USB_BREQ_GET_CONFIGURATION:
        {
            //Return selected configuration
            //Address state:    the value zero must be returned
            //Configured state: the non-zero bConfigurationValue of the
            //                  current configuration must be returned.

            //Only one byte requested: write FIFO here
            USBDWrite(USB_N1, 0, &rtm->cfgIndex, 1);
            break;
        } //USB_BREQ_GET_CONFIGURATION


        default:
        {
            //Call user-defined handler
            if (rtm->evhandler)
                rtm->evhandler(0, USBD_EVTYPE_TRAN_SETUP);
            break;
        }
    }//switch (bRequest)
}



/* Setup request handler: device (set requests) */
static void USBSetupRequestHandler_device_set(USBRtmConfig *rtm)
{
    USBSetupRequest *req  = &rtm->recvRequest;
    //USBDConfig      *dcfg = &rtm->devConfig;

    switch (req->bRequest)
    {
        /* Optional request
        case USB_BREQ_SET_DESCRIPTOR:
        {
            //Clear tx FIFO of endpoint #0
            USBEp0ClearTxFIFO();
            break;
        } //USB_BREQ_SET_DESCRIPTOR
        */


        case USB_BREQ_SET_ADDRESS:
        {
            //"wValue" low 7 bits: assigned device address, save only.
            //Don't update device hardware address here (USB->SA = x),
            //wait status stage completed
            rtm->address    = (u8)(req->wValue & 0x7F);
            rtm->setupStage = USB_SSTAGE_UPADDR;

            //No data at response
            break;
        } //USB_BREQ_SET_ADDRESS


        case USB_BREQ_SET_CONFIGURATION:
        {
            //Get pointer to runtime configuration
            USBDConfig *dcfg = &rtm->devConfig;

            //"wValue": requested configuration index
            u8 reqCfgIndex = (u8)req->wValue;

            /* According "Universal Serial Bus Specification Revision 2.0", p 9.4.7:
               "Default state: <...> behavior <...> is not specified.
                Address state:
                If the specified configuration value is zero, then the device remains
                in the Address state. If the specified configuration value matches the
                configuration value from a configuration descriptor, then that
                configuration is selected and the device enters the Configured state.
                Otherwise, the device responds with a Request Error.
                Configured state:
                If the specified configuration value is zero, then the device enters
                the Address state. If the specified configuration value matches the
                configuration value from a configuration descriptor, then that configuration
                is selected and the device remains in the Configured state. Otherwise,
                the device responds with a Request Error." */

            //No data at response

            //dscConfiguration[5]: bConfigurationValue
            #define USBIsValidConfiguration(cfgi)   (cfgi == dcfg->dscConfiguration[5])

            if (rtm->state == USB_DEVSTATE_ADDRESSED)
            {                
                if (USBIsValidConfiguration(reqCfgIndex))
                {
                    //Device configured
                    rtm->state    = USB_DEVSTATE_CONFIGURED;
                    rtm->cfgIndex = reqCfgIndex;

                    if (rtm->evhandler)
                        rtm->evhandler(0, USBD_EVTYPE_STATE_CHANGED);
                }
                else if (reqCfgIndex == 0)
                {
                    //Remains at address state
                    //No call user-defined handler here
                }
                else
                {
                    USBEp0SetRequestError();
                }

            }
            else if (rtm->state == USB_DEVSTATE_CONFIGURED)
            {
                if (reqCfgIndex == 0)
                {
                    //Return to address state
                    rtm->state    = USB_DEVSTATE_ADDRESSED;
                    rtm->cfgIndex = 0;

                    if (rtm->evhandler)
                        rtm->evhandler(0, USBD_EVTYPE_STATE_CHANGED);
                }
                else if (USBIsValidConfiguration(reqCfgIndex))
                {                    
                    //New configuration selected
                    if (rtm->cfgIndex != reqCfgIndex)
                    {
                        rtm->state    = USB_DEVSTATE_CONFIGURED;
                        rtm->cfgIndex = reqCfgIndex;

                        if (rtm->evhandler)
                            rtm->evhandler(0, USBD_EVTYPE_STATE_CHANGED);
                    }
                }
                else
                {
                    USBEp0SetRequestError();
                }
            }

            #undef USBIsValidConfiguration
            break;
        } //USB_BREQ_SET_CONFIGURATION


        default:
        {
            //Call user-defined handler
            if (rtm->evhandler)
                rtm->evhandler(0, USBD_EVTYPE_TRAN_SETUP);
            break;
        }
    }//switch (bRequest)
}



/* Update device address */
static void USBDUpdateAddress(USBRtmConfig *rtm)
{
    /* According "Universal Serial Bus Specification Revision 2.0", p 9.4.6:
       "The USB device does not change its device address until after the
        Status stage of this request is completed successfully."
       "Default state:
        If the address specified is non-zero, then the device shall enter
        the Address state; otherwise, the device remains in the Default
        state (this is not an error condition).
        Address state:
        If the address specified is zero, then the device shall enter the
        Default state; otherwise, the device remains in the Address state
        but uses the newly-specified address.
        Configured state: <...> behavior <...> is not specified." */

    //Configured state and new address assigned:
    //go to address state
    if (rtm->state == USB_DEVSTATE_CONFIGURED &&
        (USB->SA != rtm->address))
    {
        rtm->state = USB_DEVSTATE_ADDRESSED;

        if (rtm->evhandler)
            rtm->evhandler(0, USBD_EVTYPE_STATE_CHANGED);
    }

    switch(rtm->state)
    {
        case USB_DEVSTATE_DEFAULT:
        {
            USB->SA = rtm->address;

            if (rtm->address != 0)
            {
                rtm->state = USB_DEVSTATE_ADDRESSED;

                if (rtm->evhandler)
                    rtm->evhandler(0, USBD_EVTYPE_STATE_CHANGED);
            }
            break;
        }

        case USB_DEVSTATE_ADDRESSED:
        {
            USB->SA = rtm->address;

            if (rtm->address == 0)
            {
                rtm->state = USB_DEVSTATE_DEFAULT;

                if (rtm->evhandler)
                    rtm->evhandler(0, USBD_EVTYPE_STATE_CHANGED);
            }
            break;
        }
    } //switch(rtm->state)
}


/*===========================================================================
 * SETUP REQUEST HANDLER: INTERFACE
 *===========================================================================*/

/* Setup request handler: interface (get requests) */
static void USBSetupRequestHandler_interface_get(USBRtmConfig *rtm)
{
    //USBSetupRequest *req  = &rtm->recvRequest;
    //USBDConfig      *dcfg = &rtm->devConfig;

    /*
    switch (req->bRequest)
    {
        case USB_BREQ_x: break;
    } //switch (bRequest)
    */

    //Call user-defined handler
    if (rtm->evhandler)
        rtm->evhandler(0, USBD_EVTYPE_TRAN_SETUP);
}



/* Setup request handler: interface (set requests) */
static void USBSetupRequestHandler_interface_set(USBRtmConfig *rtm)
{
    //USBSetupRequest *req  = &rtm->recvRequest;
    //USBDConfig      *dcfg = &rtm->devConfig;

    //wIndex contains interface index
    //bits[15..8]: reserved and 0
    //bit[7..0]  : interface number
    //ifcIndex = (req->wIndex & 0xFF);

    /*
    switch (req->bRequest)
    {
        case USB_BREQ_x: break;
    } //switch (bRequest)
    */

    //Call user-defined handler
    if (rtm->evhandler)
        rtm->evhandler(0, USBD_EVTYPE_TRAN_SETUP);
}

/*===========================================================================
 * SETUP REQUEST HANDLER: ENDPOINT
 *===========================================================================*/

/* Setup request handler: endpoint (get requests) */
static void USBSetupRequestHandler_endpoint_get(USBRtmConfig *rtm)
{
    USBSetupRequest *req  = &rtm->recvRequest;
    //USBDConfig      *dcfg = &rtm->devConfig;

    //wIndex contains endpoint index:
    //bits[15..8]: reserved and 0
    //bit[7]     : 1/0 IN/OUT direction
    //bit[6..4]  : reserved and 0
    //bit[3..0]  : endpoint number [0..15]    

    switch (req->bRequest)
    {
        case USB_BREQ_GET_STATUS:
        {
            u32 SEPx_CTRL = 0;
            u16 epIndex = (req->wIndex & 0x0F);
            u8 status[2];

            //Endpoint status
            //bits[15..1]: reserved and 0
            //bit[0]     : 1 - Halt bit

            //SEPx_CTRL register
            //EPEN[0]: 1/0 endpoint enabled/disabled

            if (epIndex < USB_EP_COUNT)
            {
                switch(epIndex)
                {
                    case 0: SEPx_CTRL = USB->SEP0_CTRL; break;
                    case 1: SEPx_CTRL = USB->SEP1_CTRL; break;
                    case 2: SEPx_CTRL = USB->SEP2_CTRL; break;
                    case 3: SEPx_CTRL = USB->SEP3_CTRL; break;
                }

                status[0] = (SEPx_CTRL & PN0) ? 0 : 1;
                status[1] = 0;

                USBDWrite(USB_N1, 0, status, 2);
            }

            break;
        } //USB_BREQ_GET_STATUS


        /*
        case USB_BREQ_SYNCH_FRAME:
        {
            //Response 2 bytes: Frame number
            if (rtm->state == USB_DEVSTATE_CONFIGURED)
            {                
                u16 frmnumber = 0;
                USBWrite(USB_N1, 0, &frmnumber, 2);
            }
            break;
        } //USB_BREQ_SYNCH_FRAME
        */

        default:
        {
            //Call user-defined handler
            if (rtm->evhandler)
                rtm->evhandler(0, USBD_EVTYPE_TRAN_SETUP);
            break;
        }
    } //switch (bRequest)
}



/* Setup request handler: endpoint (set requests) */
static void USBSetupRequestHandler_endpoint_set(USBRtmConfig *rtm)
{
    //USBSetupRequest *req  = &rtm->recvRequest;
    //USBDConfig      *dcfg = &rtm->devConfig;

    //wIndex contains endpoint index:
    //bits[15..8]: reserved and 0
    //bit[7]     : 1/0 IN/OUT direction
    //bit[6..4]  : reserved and 0
    //bit[3..0]  : endpoint number [0..15]
    //epIndex = (req->wIndex & 0x0F);

    /*
    switch (req->bRequest)
    {        
        case USB_BREQ_x: break;
    } //switch (bRequest)
    */

    //Call user-defined handler
    if (rtm->evhandler)
        rtm->evhandler(0, USBD_EVTYPE_TRAN_SETUP);
}

/*===========================================================================
 * SETUP REQUEST HANDLER
 *===========================================================================*/

/* Setup request handler (top level) */
static void USBSetupRequestHandler(USBRtmConfig *rtm)
{
    USBSetupRequest *req  = &rtm->recvRequest;
    //USBDConfig      *dcfg = &rtm->devConfig;

    u8 bmReqType = req->bmRequestType & USB_BMREQ_TYPE_MASK;    

    if (bmReqType == USB_BMREQ_TYPE_STANDARD)
    {
        //Standard requests
        if (req->bmRequestType & USB_BMREQ_DIR_DEV2HOST)
        {
            //Direction: Device-to-Host (get requests)
            PrintMsg("G");

            //Select destination
            switch (req->bmRequestType & USB_BMREQ_RCPNT_MASK)
            {
                case USB_BMREQ_RCPNT_DEVICE:
                    USBSetupRequestHandler_device_get(rtm);    break;

                case USB_BMREQ_RCPNT_INTERFACE:
                    USBSetupRequestHandler_interface_get(rtm); break;

                case USB_BMREQ_RCPNT_ENDPOINT:
                    USBSetupRequestHandler_endpoint_get(rtm);  break;                
            }
        }
        else
        {
            //Direction: Host-to-Device (set requests)
            PrintMsg("S");

            //Select destination
            switch (req->bmRequestType & USB_BMREQ_RCPNT_MASK)
            {
                case USB_BMREQ_RCPNT_DEVICE:
                    USBSetupRequestHandler_device_set(rtm);    break;

                case USB_BMREQ_RCPNT_INTERFACE:
                    USBSetupRequestHandler_interface_set(rtm); break;

                case USB_BMREQ_RCPNT_ENDPOINT:
                    USBSetupRequestHandler_endpoint_set(rtm);  break;                
            }
        }
    }
    else if (bmReqType == USB_BMREQ_TYPE_CLASS ||
             bmReqType == USB_BMREQ_TYPE_VENDOR)
    {
        //CLASS/VENDOR-specific requests
        //Call user-defined handler
        if (rtm->evhandler)
            rtm->evhandler(0, USBD_EVTYPE_TRAN_SETUP);
    }    
}



/* Initialize data stage */
void USBDInitDataStage(USBBLOCK usb, const void *src, s32 count)
{
    usbRtmConfig.pTxData = (u8*)src;
    usbRtmConfig.txLength = count;
}



/* Send portion of data to host */
static void USBDataStageHandler(USBRtmConfig *rtm, u8 isInitial)
{
    USBSetupRequest *req;

    req = &rtm->recvRequest;

    if (rtm->pTxData == 0)
        return;
    else if (rtm->txLength == 0)
    {
        if (!isInitial)
        {
            //Empty data packet: end of data
            USBDWrite(USB_N1, 0, 0, 0);
            rtm->pTxData = 0;
            return;
        }
    }    

    //"wValue" high byte: descriptor type
    if ((req->wValue >> 8) == USB_BDTYPE_STRING)
    {
        /* According "Universal Serial Bus Specification Revision 2.0", p 9.6.7:
           "String descriptors use UNICODE encodings <...>.
            The UNICODE string descriptor <...> is not NULL-terminated.
            The string length is computed by subtracting two from the
            value of the first byte of the descriptor." */

        //Send string descriptor
        s32 i, utf16Len;
        const u8 *psrc;
        u16 chUTF8, chUTF16;
        u8 tmpbuf[USB_EP_FIFO_TX_LENGTH];

        if (isInitial)
        {
            //Solve count of bytes at transmitted UTF-16 string
            rtm->txLength = strlen((char*)rtm->pTxData);
            i = 0;
            utf16Len = 0;
            while(i < rtm->txLength)
            {
                chUTF8 = rtm->pTxData[i];
                i = (chUTF8 < 128) ? (i + 1) : (i + 2);
                utf16Len += 2;
            }

            tmpbuf[0] = utf16Len + 2;
            tmpbuf[1] = USB_BDTYPE_STRING;
            i = 2;
        }
        else
        {
            i = 0;
        }

        //Get pointer
        psrc = rtm->pTxData;

        while(rtm->txLength > 0 &&
              i < USB_EP_FIFO_TX_LENGTH && i < req->wLength)
        {
            chUTF8 = *psrc++;

            if (chUTF8 < 0x80)
            {
                //Convert from 8-bit latin encoding to UTF-16 encoding
                chUTF16 = chUTF8;
                rtm->txLength--;
            }
            else
            {
                //Convert from UTF-8 encoding to UTF-16 encoding
                //UTF8 (2 bytes case):
                //110x xxxx 10xx xxxx, x - significant bits
                chUTF8  = (chUTF8 << 8) + (*psrc++);
                chUTF16 = ((chUTF8 & 0x1F00) >> 2) | (chUTF8 & 0x3F);
                rtm->txLength -= 2;
            }

            tmpbuf[i]     = (u8)chUTF16;
            tmpbuf[i + 1] = (u8)(chUTF16 >> 8);
            i += 2;
        }

        //Save pointer
        rtm->pTxData = psrc;

        //Write portion of string descriptor
        USBDWrite(USB_N1, 0, tmpbuf, i);
    }
    else
    {
        //Send other descriptors
        s32 txc;

        if (isInitial)
        {
            if (rtm->txLength > req->wLength)
                rtm->txLength = req->wLength;
        }

        txc = (rtm->txLength > USB_EP_FIFO_TX_LENGTH) ?
               USB_EP_FIFO_TX_LENGTH : rtm->txLength;

        USBDWrite(USB_N1, 0, rtm->pTxData, txc);

        rtm->pTxData  += txc;
        rtm->txLength -= txc;
    }
}

/*===========================================================================
 * INTERRUPT HANDLER
 *===========================================================================*/

//Transaction types
//SETUP transaction: Host -> request -> device
#define USB_TRAN_SETUP          0
//IN transaction:  Host <- data <- device
#define USB_TRAN_IN             1
//OUT transaction: Host -> data -> device
#define USB_TRAN_OUT            2

//Connection state: 0,1,2 - Reset, Low Speed, Full Speed
#define ADDR_USB_SLS            0x40010144
//Common USB state
#define ADDR_USB_SIS            0x40010148


/* USB interrupt handler */
void USB_IRQHandler(void)
{    
    u32 USB_SLS, USB_SIS;
    s32 RXFDC_H;
    USBSetupRequest *rxreq;
    u8 epTranType;    

    //Read SIS register
    USB_SIS = *((__io_reg*)ADDR_USB_SIS);

    //SIS register:
    //SCNAKSENT[4]=1: NAK sent
    //SCSOFRECV[3]=1: SOF received
    //SCRESETEV[2]=1: reset event
    //SCRESUME[1] =1: packet repeat
    //SCTDONE[0]  =1: transaction completed

    //SEPx_CTRL register:
    //EPISOEN[4]  : 1/0 - isochronous mode (no send ACKs) /none isochronous
    //EPSTALL[3]  : 1/0 - send STALL (when EPEN=EPRDY=1 and EPISOEN=0) / no send
    //EPDATASEQ[2]: 1/0 - tx data with DATA1/DATA0 (IN transaction)
    //EPRDY[1]    : 1/0 - endpoint is ready/not ready
    //              NOTE: automatically reset to 0 when transaction completed
    //EPEN[0]     : 1/0 - enable/disable endpoint
    //              NOTE:
    //              EPEN=0 - endpoint is not respond to transaction
    //              EPEN=1 and EPRDY=EPISOEN=0 - endpoint send NAK

    //SEPx_STS register:
    //SCDATASEQ[7]  : 1/0 rx data with DATA1/DATA0 (OUT transaction)
    //SCACKRXED[6]  : 1 - rx ACK from host
    //SCSTALLSENT[5]: 1 - STALL sent
    //SCNAKSENT[4]  : 1 - NAK sent
    //SCRXTO[3]     : 1 - response (from host) timeout
    //SCRXOFF[2]    : 1 - rx FIFO overflow
    //SCBSERR[1]    : 1 - STUFF error
    //SCCRCERR[0]   : 1 - CRC error

    //SEPx_TS register:
    //SCTTYPE[1..0]: [0,1,2,3] => [SETUP, IN, OUT, xxx]


    //SCRESETEV[2]=1: reset event
    if (USB_SIS & PN2)
    {
        //SLS register:
        //SCRXLS[1..0]: [0,1,2] => [reset, Low speed, Full speed]
        while(1)
        {
            __NOP();
            USB_SLS = *((__io_reg*)ADDR_USB_SLS);
            if (USB_SLS & 0x03)
                break;
        }

        //Clear assigned configuration index and address (logical level)
        usbRtmConfig.cfgIndex   = 0;
        usbRtmConfig.address    = 0;
        usbRtmConfig.setupStage = USB_SSTAGE_SETUP;
        usbRtmConfig.state = USB_DEVSTATE_DEFAULT;

        //Set device address to 0 (hardware level)
        USB->SA = 0;

        //Reset end points
        ResetEndPoints();

        CallEventHandler(0, USBD_EVTYPE_STATE_CHANGED);

        //SCRESETEV[2]=1: clear bit with record 1
        USB->SIS = PN2;

        //No another actions: return
        return;
    }

    //SCTDONE[0]=1: transaction completed
    if (USB_SIS & PN0)
    {
        //------------------------------------------------------------
        //END POINT #0: setup request/response only
        //EPRDY[1]=0: transaction completed
        //EPEN[0]=1 : endpoint enabled
        if ((USB->SEP0_CTRL & (PN1|PN0)) == PN0)
        {
            //SCCTYPE[1..0]: transaction type
            epTranType = (USB->SEP0_TS & 0x03);

            if (epTranType == USB_TRAN_SETUP)
            {
                //SETUP TRANSACTION (HOST -> DEVICE)

                //Endpoint #0 (SETUP),
                //EPSTALL[3]=0 - clear error;
                //TXFDC=1 - clear tx FIFO
                USB->SEP0_CTRL &= ~PN3;               
                USB->SEP0_TXFDC = 1;

                usbRtmConfig.setupStage = USB_SSTAGE_SETUP;
                usbRtmConfig.recvReqDataCount = 0;
                usbRtmConfig.pTxData    = 0;
                usbRtmConfig.txLength   = 0;

                RXFDC_H = (USB->SEP0_RXFDC_H);

                //Rx FIFO contains data
                if (RXFDC_H >= USB_SETUP_REQUEST_LENGTH)
                {
                    USBDRead(USB_N1, 0, &usbRtmConfig.recvRequest, USB_SETUP_REQUEST_LENGTH);                    

                    USBSetupRequestHandler(&usbRtmConfig);

                    //Start of data stage
                    USBDataStageHandler(&usbRtmConfig, 1);
                }

                //RXFC=1 - clear rx FIFO
                //EPDATASEQ[2]=1 - tx with DATA1; EPRDY[1]=1 - endpoint ready
                USB->SEP0_RXFC = 1;                
                USB->SEP0_CTRL |= (PN2|PN1);
            }
            else if (epTranType == USB_TRAN_IN)
            {
                //IN TRANSACTION COMPLETED (HOST <- DEVICE)
                //SCACKRXED[6]=1: ACK received, change DATA0/DATA1 (XOR operation)
                if ((USB->SEP0_STS) & PN6)
                {
                    USB->SEP0_CTRL = USB->SEP0_CTRL ^ PN2;
                }

                PrintMsg("I");

                //IN transaction completed (status stage completed):
                //update device address
                if (usbRtmConfig.setupStage == USB_SSTAGE_UPADDR)
                {
                    usbRtmConfig.setupStage = USB_SSTAGE_SETUP;

                    USBDUpdateAddress(&usbRtmConfig);

                    PrintMsg("A");
                }                

                //Continue of data stage
                USBDataStageHandler(&usbRtmConfig, 0);

                //Set EP0 ready
                USB->SEP0_CTRL |= PN1;
            }
            else if (epTranType == USB_TRAN_OUT)
            {
                PrintMsg("O");

                rxreq = &usbRtmConfig.recvRequest;

                //Received "SET" request and "wLength" > 0: read additional data
                if ((rxreq->bmRequestType & USB_BMREQ_DIR_DEV2HOST) == 0 &&
                     rxreq->wLength > 0)
                {
                    usbRtmConfig.recvReqDataCount =
                        USBDRead(USB_N1, 0, usbRtmConfig.recvReqData, USB_EP_FIFO_RX_LENGTH);

                    USBSetupRequestHandler(&usbRtmConfig);
                }

                //According "Universal Serial Bus Communications Class Subclass Specification for PSTN Devices",
                //p 6.3.10: "SET_LINE_CODING" request has additional data (Line Coding Structure, 7 bytes)
                /*if (rxreq->bmRequestType == (USB_BMREQ_TYPE_CLASS|USB_BMREQ_RCPNT_INTERFACE) &&
                    (rxreq->bRequest == USB_BREQ_SET_LINE_CODING   ||
                     rxreq->bRequest == USB_BREQ_SEND_ENCAPCOMMAND ||
                     rxreq->bRequest == USB_BREQ_SET_COMM_FEATURE))
                {
                    usbRtmConfig.recvReqDataCount =
                        USBDRead(USB_N1, 0, usbRtmConfig.recvReqData, USB_EP_FIFO_RX_LENGTH);

                    USBSetupRequestHandler(&usbRtmConfig);
                }*/

                //OUT TRANSACTION COMPLETED (HOST -> DEVICE)
                //Clear rx FIFO, set EP0 ready
                USB->SEP0_RXFC = 1;
                USB->SEP0_CTRL |= PN1;
            }
        }

        //------------------------------------------------------------
        //END POINT #1: rx/tx data
        //EPRDY[1]=0: transaction completed
        //EPEN[0]=1 : endpoint enabled
        if ((USB->SEP1_CTRL & (PN1|PN0)) == PN0)
        {
            //SCCTYPE[1..0]: transaction type
            epTranType   = (USB->SEP1_TS & 0x03);
            USB->SEP1_TS = (USB->SEP1_TS & ~0x03);

            if (epTranType == USB_TRAN_IN)
            {
                //IN TRANSACTION
                //SCACKRXED[6]=1: ACK received, change DATA0/DATA1 (XOR operation)
                if ((USB->SEP1_STS) & PN6)
                {
                    USB->SEP1_CTRL = USB->SEP1_CTRL ^ PN2;
                }

                CallEventHandler(1, USBD_EVTYPE_TRAN_IN);
            }
            else if (epTranType == USB_TRAN_OUT)
            {
                //OUT TRANSACTION
                CallEventHandler(1, USBD_EVTYPE_TRAN_OUT);
            }
        }

        //------------------------------------------------------------
        //END POINT #2: rx/tx data
        //EPRDY[1]=0: transaction completed
        //EPEN[0]=1 : endpoint enabled
        if ((USB->SEP2_CTRL & (PN1|PN0)) == PN0)
        {
            //SCCTYPE[1..0]: transaction type
            epTranType   = (USB->SEP2_TS & 0x03);
            USB->SEP2_TS = (USB->SEP2_TS & ~0x03);

           //ConPrintf("P(%i); ", epTranType);

            if (epTranType == USB_TRAN_IN)
            {
                //IN TRANSACTION
                //SCACKRXED[6]=1: ACK received, change DATA0/DATA1 (XOR operation)
                if ((USB->SEP2_STS) & PN6)
                {
                    USB->SEP2_CTRL = USB->SEP2_CTRL ^ PN2;
                }                

                CallEventHandler(2, USBD_EVTYPE_TRAN_IN);
            }
            else if (epTranType == USB_TRAN_OUT)
            {
                //OUT TRANSACTION
                CallEventHandler(2, USBD_EVTYPE_TRAN_OUT);
            }
        }

        //------------------------------------------------------------
        //END POINT #3: rx/tx data
        //EPRDY[1]=0: transaction completed
        //EPEN[0]=1 : endpoint enabled
        if ((USB->SEP3_CTRL & (PN1|PN0)) == PN0)
        {
            epTranType   = (USB->SEP3_TS & 0x03);
            USB->SEP3_TS = (USB->SEP3_TS & ~0x03);

            if (epTranType == USB_TRAN_IN)
            {
                //IN TRANSACTION
                //SCACKRXED[6]=1: ACK received, change DATA0/DATA1 (XOR operation)
                if ((USB->SEP3_STS) & PN6)
                {
                    USB->SEP3_CTRL = USB->SEP3_CTRL ^ PN2;
                }

                CallEventHandler(3, USBD_EVTYPE_TRAN_IN);
            }
            else if (epTranType == USB_TRAN_OUT)
            {
                //OUT TRANSACTION
                CallEventHandler(3, USBD_EVTYPE_TRAN_OUT);
            }
        }

      //End of SCTDONE event handler
    } //if (USB_SIS & PN0)

    //Clear event bits with record 1
    USB->SIS = 0x1F;
}

/*===========================================================================
 * End of file 1986BE9x_USB_dev.c
 *===========================================================================*/
