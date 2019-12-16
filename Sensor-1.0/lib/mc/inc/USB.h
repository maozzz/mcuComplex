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
 *  File USB.h: USB protocol definitions.
 *
 *  Notes:
 *   USB 1.0 (1996)
 *    Low-Speed   1.5Mbit/s, max cable length (no shield)   3m
 *    Full-Speed   12Mbit/s, max cable length (with shield) 5m
 *    Max current 500mA
 *   USB 1.1 (1998)
 *    Fixed errors in USB 1.0
 *   USB 2.0 (2000)
 *    Low-Speed  10-1500Kbit/s
 *    Full-Speed  0.5-12Mbit/s
 *    High-Speed  25-480Mbit/s
 *   USB 3.0 (2008)
 *    Super-speed 5Gbit/s
 *    Max current 900mA
 *   USB 3.1 (2013)
 *    Super-speed   5Gbit/s (USB 3.1 Gen 1)
 *    Super-speed+ 10Gbit/s (USB 3.1 Gen 2)
 *   USB 3.2 (2017)
 *    Up to 20Gbit/s
 *===========================================================================*/

#ifndef __USB_H
#define __USB_H

#include "../../bastypes.h"

//---------------------------------------------------------------------------
//Language IDs (Windows Language Identifiers)
//"http://www.voidtools.com/support/everything/language_ids"
#define USB_LANGID_NEUTRAL              0x0000 //Neutral language
#define USB_LANGID_DEFAULT              0x0400 //Process or User Default Language
#define USB_LANGID_DE                   0x0407 //German (Standard)
#define USB_LANGID_US                   0x0409 //English (United States)
#define USB_LANGID_ES                   0x040a //Spanish (Spain, Traditional Sort)
#define USB_LANGID_FI                   0x040b //Finnish
#define USB_LANGID_FR                   0x040c //French (Standard)
#define USB_LANGID_IT                   0x0410 //Italian (Standard)
#define USB_LANGID_NL                   0x0413 //Dutch (Netherlands)
#define USB_LANGID_RU                   0x0419 //Russian
#define USB_LANGID_SV                   0x041d //Swedish
#define USB_LANGID_TR                   0x041f //Turkish
#define USB_LANGID_PT                   0x0816 //Portuguese (Portugal)


//---------------------------------------------------------------------------
//bcdUSB, USB version at BCD format: [0xMN, 0xJJ] => [minor, major]:
#define USB_BCD_10                      0x00, 0x01 //USB 1.0
#define USB_BCD_11                      0x10, 0x01 //USB 1.1
#define USB_BCD_20                      0x00, 0x02 //USB 2.0

//---------------------------------------------------------------------------
//Length of various descriptor types (bLength), bytes
#define USB_BLEN_DEVICE                  18 //Device descriptor length
#define USB_BLEN_QUALIFIER               10 //Qualifier descriptor length
#define USB_BLEN_CONFIGURATION           9  //Configuration descriptor length
#define USB_BLEN_INTERFACE               9  //Interface descriptor length
#define USB_BLEN_HID                     9  //HID descriptor length
#define USB_BLEN_ENDPOINT                7  //Endpoint descriptor length

//---------------------------------------------------------------------------
//Device classes (bDeviceClass)
#define USB_BDEVCLASS_OWN                0x00 //Each interface has own class
#define USB_BDEVCLASS_AUDIO              0x01 //Sound card, MIDI
#define USB_BDEVCLASS_CDC                0x02 //Communication device (modem, network card, serial port)
#define USB_BDEVCLASS_HID                0x03 //HID devices (keyboard, mouse, e.t.c)
#define USB_BDEVCLASS_PID                0x05 //Joystick with Force Feedback
#define USB_BDEVCLASS_IMAGE              0x06 //Web-camera, scanner
#define USB_BDEVCLASS_PRINTER            0x07 //Printer
#define USB_BDEVCLASS_MSD                0x08 //USB-storage, digital camera
#define USB_BDEVCLASS_HUB                0x09 //USB-hub
#define USB_BDEVCLASS_CDCDATA            0x0A //Communication device data interface
#define USB_BDEVCLASS_CCID               0x0B //Smart card reader
#define USB_BDEVCLASS_CONTSECURITY       0x0D //Biometric scanner
#define USB_BDEVCLASS_VDC                0x0E //Video device class (web-camera)
#define USB_BDEVCLASS_HEALTHCARE         0x0F //Medical equipment
#define USB_BDEVCLASS_DIAGNOSTIC         0xDC //Diagnostic device
#define USB_BDEVCLASS_WIRELESS           0xE0 //Bluetooth adapter
#define USB_BDEVCLASS_MISC               0xEF //ActiveSync devices
#define USB_BDEVCLASS_APPSPEC            0xFE //IrDA device, firmware update mode
#define USB_BDEVCLASS_VENDOR             0xFF //Vendor specific



//Sbclasses (bDeviceSubClass, bInterfaceSubClass)
#define USB_BDEVSCLASS_VENDOR            0xFF //Vendor specific

//CDC class subclass codes (according "Universal Serial Bus Communications Class", Table 4)
#define USB_BDEVSCLASS_DLM               0x01 //Direct Line Control Model         (USBPSTN)
#define USB_BDEVSCLASS_ACM               0x02 //Abstract Control Model            (USBPSTN)
#define USB_BDEVSCLASS_TCM               0x03 //Telephone Control Model           (USBPSTN)
#define USB_BDEVSCLASS_MCCM              0x04 //Multi-Channel Control Model       (USBISDN)
#define USB_BDEVSCLASS_CAPI              0x05 //CAPI Control Model                (USBISDN)
#define USB_BDEVSCLASS_ECM               0x06 //Ethernet Networking Control Model (USBECM)
#define USB_BDEVSCLASS_ATM               0x07 //ATM Networking Control Model      (USBATM)
#define USB_BDEVSCLASS_WHCM              0x08 //Wireless Handset Control Model    (USBWMC)
#define USB_BDEVSCLASS_WDM               0x09 //Device Management                 (USBWMC)
#define USB_BDEVSCLASS_WDLM              0x0A //Mobile Direct Line Model          (USBWMC)
#define USB_BDEVSCLASS_OBEX              0x0B //OBEX                              (USBWMC)
#define USB_BDEVSCLASS_EEM               0x0C //Ethernet Emulation Model          (USBEEM)
//Values [0x00, 0x0D..0x7F] reserved
//Values [0x80..0xFF] reserved, vendor specified

//HID class subclass codes
#define USB_BDEVSCLASS_BOOTIFC           0x01 //Boot interface

//Printer class subclass codes
#define USB_BDEVSCLASS_PRINTER           0x01 //Printer subclass

//MSD class subclass codes
#define USB_BDEVSCLASS_SCSI              0x06 //SCSI

//---------------------------------------------------------------------------
//Device protocol (bDeviceProtocol, bInterfaceProtocol)
//CDC cass protocol codes
//(according "Universal Serial Bus Communications Class", Table 5)
#define USB_BDEVPROTO_NOPROTO            0x00 //No class specific protocol required
#define USB_BDEVPROTO_V250               0x01 //AT Commands: V.250 etc
#define USB_BDEVPROTO_PCCA101            0x02 //AT Commands: PCCA-101
#define USB_BDEVPROTO_PCCA101_ANXO       0x03 //AT Commands: PCCA-101 and Annex O
#define USB_BDEVPROTO_GSM0707            0x04 //AT Commands: GSM 07.07
#define USB_BDEVPROTO_3GPP27007          0x05 //AT Commands: 3GPP 27.007
#define USB_BDEVPROTO_CS00170            0x06 //AT Commands: TIA for CDMA
#define USB_BDEVPROTO_USBEEM             0x07 //Ethernet Emulation Model
#define USB_BDEVPROTO_EXTERNAL           0xFE //External Protocol: defined by Command Set descriptor
#define USB_BDEVPROTO_VENDOR             0xFF //Vendor specific
//Values [0x08..0xFD] reserved

//HID class protocol codes
#define USB_BDEVPROTO_KEYBOARD           0x01 //Keyboard
#define USB_BDEVPROTO_MOUSE              0x02 //Mouse

//Printer class protocol codes
#define USB_BDEVPROTO_UNDEFINED          0x00 //Undefined
#define USB_BDEVPROTO_UNIDIR             0x01 //Unidirectional interface
#define USB_BDEVPROTO_BIDIR              0x02 //Bidirectional protocol
#define USB_BDEVPROTO_1284_4             0x03 //1284.4 protocol
#define USB_BDEVPROTO_BULKONLY           0x50 //Bulk-only
//Values [0x04..0xFE] - reserved


//---------------------------------------------------------------------------
//According "Universal Serial Bus Specification Revision 2.0", p 9.3:
//bmRequestType field (USBSetupRequest::bmRequestType):
//Request direction (bit [7]):
#define USB_BMREQ_DIR_DEV2HOST          (1 << 7) //Device-to-host (GET requests)
#define USB_BMREQ_DIR_HOST2DEV          (0 << 7) //Host-to-device (SET requests)

//Request type (bits [6..5]):
#define USB_BMREQ_TYPE_STANDARD         (0 << 5) //Standard
#define USB_BMREQ_TYPE_CLASS            (1 << 5) //Class-specific
#define USB_BMREQ_TYPE_VENDOR           (2 << 5) //Vendor-specific
#define USB_BMREQ_TYPE_MASK             (3 << 5) //mask

//Recipient (bits [4..0]):
#define USB_BMREQ_RCPNT_DEVICE          0x00 //Device
#define USB_BMREQ_RCPNT_INTERFACE       0x01 //Interface
#define USB_BMREQ_RCPNT_ENDPOINT        0x02 //Endpoint
#define USB_BMREQ_RCPNT_OTHER           0x03 //Other
#define USB_BMREQ_RCPNT_MASK            0x1F //mask
//Values [4..31] - reserved

//---------------------------------------------------------------------------
//bRequest field (USBSetupRequest::bRequest).
//STANDARD requests (bmRequestType = x0000000b):
#define USB_BREQ_GET_STATUS             0
#define USB_BREQ_CLEAR_FEATURE          1
#define USB_BREQ_SET_FEATURE            3
#define USB_BREQ_SET_ADDRESS            5
#define USB_BREQ_GET_DESCRIPTOR         6
#define USB_BREQ_SET_DESCRIPTOR         7
#define USB_BREQ_GET_CONFIGURATION      8
#define USB_BREQ_SET_CONFIGURATION      9
#define USB_BREQ_GET_INTERFACE          10
#define USB_BREQ_SET_INTERFACE          11
#define USB_BREQ_SYNCH_FRAME            12
//Values [2,4] reserved

//Defined at "Universal Serial Bus Communications Class",
//CLASS specific requests (bmRequestType = x0100001b)
#define USB_BREQ_SEND_ENCAPCOMMAND      0x00 //p 6.2.1
#define USB_BREQ_GET_ENCAPRESPONSE      0x01 //p 6.2.2


//Defined at "Universal Serial Bus Communications Class Subclass Specification for PSTN Devices",
//CLASS specific requests (bmRequestType = x0100001b)
#define USB_BREQ_SET_COMM_FEATURE       0x02 //p 6.3.1
#define USB_BREQ_GET_COMM_FEATURE       0x03 //p 6.3.2
#define USB_BREQ_CLEAR_COMM_FEATURE     0x04 //p 6.3.3
#define USB_BREQ_SET_AUX_LINE_STATE     0x10 //p 6.3.4
#define USB_BREQ_SET_HOOK_STATE         0x11 //p 6.3.5
#define USB_BREQ_PULSE_SETUP            0x12 //p 6.3.6
#define USB_BREQ_SEND_PULSE             0x13 //p 6.3.7
#define USB_BREQ_SET_PULSE_TIME         0x14 //p 6.3.8
#define USB_BREQ_RING_AUX_JACK          0x15 //p 6.3.9
#define USB_BREQ_SET_LINE_CODING        0x20 //p 6.3.10
#define USB_BREQ_GET_LINE_CODING        0x21 //p 6.3.11
#define USB_BREQ_SET_CONTROL_LINE_STATE 0x22 //p 6.3.12
#define USB_BREQ_SEND_BREAK             0x23 //p 6.3.13
#define USB_BREQ_SET_RINGER_PARMS       0x30 //p 6.3.14
#define USB_BREQ_GET_RINGER_PARMS       0x31 //p 6.3.15
#define USB_BREQ_SET_OPERATION_PARMS    0x32 //p 6.3.16
#define USB_BREQ_GET_OPERATION_PARMS    0x33 //p 6.3.17
#define USB_BREQ_SET_LINE_PARMS         0x34 //p 6.3.18
#define USB_BREQ_GET_LINE_PARMS         0x35 //p 6.3.19
#define USB_BREQ_DIAL_DIGITS            0x36 //p 6.3.20
//Values [0x16..0x1F], [0x24..0x2F] reserved


//---------------------------------------------------------------------------
//Descriptor types (high byte of USBSetupRequest::wValue)
#define USB_BDTYPE_DEVICE               1
#define USB_BDTYPE_CONFIGURATION        2
#define USB_BDTYPE_STRING               3
#define USB_BDTYPE_INTERFACE            4
#define USB_BDTYPE_ENDPOINT             5
#define USB_BDTYPE_QUALIFIER            6
#define USB_BDTYPE_OTHER_SPEED          7
//Defined at "USB Interface Power Management Specification"
#define USB_BDTYPE_INTERFACE_POWER      8

//HID device request
#define USB_BDTYPE_HID                  0x21
//HID device report
#define USB_BDTYPE_HIDREPORT            0x22

//Communication Device Class (CDC), functional descriptor
#define USB_BDTYPE_CS_INTERFACE         0x24

//Communication Device Class (CDC), descriptor subtypes
//(functional descriptors, according "Universal Serial Bus Communications Class", Table 13)
#define USB_BDSTYPE_HEADER              0x00 //Header Functional Descriptor marks the beginning of the
                                             //concatenated set of functional descriptors
#define USB_BDSTYPE_CALLMGMT            0x01 //Call Management
#define USB_BDSTYPE_ACMGMT              0x02 //Abstract Control Management
#define USB_BDSTYPE_DLMGMT              0x03 //Direct Line Management
#define USB_BDSTYPE_TLRINGER            0x04 //Telephone Ringer
#define USB_BDSTYPE_TLCALL              0x05 //Telephone Call and Line State Reporting Capabilities
#define USB_BDSTYPE_UNION               0x06 //Union
#define USB_BDSTYPE_COUNTRY             0x07 //Country Selection
#define USB_BDSTYPE_TLOPMODES           0x08 //Telephone Operational Modes
#define USB_BDSTYPE_USBTERMINAL         0x09 //USB Terminal
#define USB_BDSTYPE_NETTERMINAL         0x0A //Network Channel Terminal
#define USB_BDSTYPE_PROTOUNIT           0x0B //Protocol Unit
#define USB_BDSTYPE_EXTUNIT             0x0C //Extension Unit
#define USB_BDSTYPE_MCHMGMT             0x0D //Multi-Channel Management
#define USB_BDSTYPE_CAPIMGMT            0x0E //CAPI Control Management
#define USB_BDSTYPE_ETHNET              0x0F //Ethernet Networking
#define USB_BDSTYPE_ATMNET              0x10 //ATM Networking
#define USB_BDSTYPE_WHCM                0x11 //Wireless Handset Control Model
#define USB_BDSTYPE_MDLM                0x12 //Mobile Direct Line Model (MDLM)
#define USB_BDSTYPE_MDLMDTL             0x13 //MDLM Detail
#define USB_BDSTYPE_DEVMGMT             0x14 //Device Management Model
#define USB_BDSTYPE_OBEX                0x15 //OBEX
#define USB_BDSTYPE_CMDSET              0x16 //Command Set
#define USB_BDSTYPE_CMDSETDTL           0x17 //Command Set Detail
#define USB_BDSTYPE_TLCM                0x18 //Telephone Control Model
#define USB_BDSTYPE_OBEXSVCID           0x19 //OBEX Service Identifier
//Values [0x1A..0x7F] reserved (future use)
//Values [0x80..0xFE] reserved (vendor specific)

//---------------------------------------------------------------------------
//PSTN Subclass Specific Notifications
//(according "Universal Serial Bus Communications Class Subclass Specification for PSTN Devices", p 6.5)
#define USB_NOTIFY_NETWORK_CONNECTION   0x00 //NETWORK_CONNECTION
#define USB_NOTIFY_RESPONSE_AVAILABLE   0x01 //RESPONSE_AVAILABLE
#define USB_NOTIFY_AUX_JACK_HOOK_STATE  0x08 //AUX_JACK_HOOK_STATE
#define USB_NOTIFY_RING_DETECT          0x09 //RING_DETECT
#define USB_NOTIFY_SERIAL_STATE         0x20 //SERIAL_STATE
#define USB_NOTIFY_CALL_STATE_CHANGE    0x28 //CALL_STATE_CHANGE
#define USB_NOTIFY_LINE_STATE_CHANGE    0x23 //LINE_STATE_CHANGE

//---------------------------------------------------------------------------
//Configuration attributes (configuration descriptor, bmAttributes)
#define USB_BMATTR_BUSPOWERED           (1 << 7) //Bus powered, MUST BE SET for USB 1.1 and higher
#define USB_BMATTR_SELFPOWERED          (1 << 6) //Self powered
#define USB_BMATTR_REMWAKEUP            (1 << 5) //Remote wakeup

//Endpoint attributes (bmAttributes)
#define USB_EPATTR_CONTROL              0 //Control endpoint
#define USB_EPATTR_ISOCHRONOUS          1 //Isochronous mode
#define USB_EPATTR_BULK                 2 //Bulk mode
#define USB_EPATTR_INTERRUPT            3 //Interrupt mode

//---------------------------------------------------------------------------
//USB device states
#define USB_DEVSTATE_INITIAL             0x00
#define USB_DEVSTATE_ATTACHED            0x01
#define USB_DEVSTATE_POWERED             0x02
#define USB_DEVSTATE_DEFAULT             0x03
#define USB_DEVSTATE_ADDRESSED           0x04
#define USB_DEVSTATE_CONFIGURED          0x05
#define USB_DEVSTATE_SUSPENDED           0x06


//---------------------------------------------------------------------------
//Peripheral device
typedef u32                              USBBLOCK;

//USB controller number
#define USB_N1                           0
#define USB_N2                           1
#define USB_N3                           2
#define USB_N4                           3

//Length of setup request (see "USBSetupRequest" structure)
#define USB_SETUP_REQUEST_LENGTH         8


/* USB setup request */
typedef struct tag_USBSetupRequest
{
    //Characteristics of the specific request (see USB_BMREQ_x)
    //bit[7]    : request direction 1/0 -> device-to-host/host-to-device (GET/SET)
    //bits[6..5]: request type [0,1,2] -> [standard,class,vendor]
    //bits[4..0]: recipient [0,1,2] -> [deivce,interface,endpoint]
    u8 bmRequestType;

    //Particular request (see USB_BREQ_x)
    u8 bRequest;

    //Word-sized field that varies according to request.
    //Often contains:
    //Low byte:  descriptor index
    //High byte: descriptor type (see USB_BDTYPE_x)
    u16 wValue;

    //The contents of this field vary according to the request
    //Is often used in requests to specify an endpoint or an interface
    u16 wIndex;

    //The length of the data transferred during
    //the second phase of the control transfer
    u16 wLength;
} USBSetupRequest;


/* USB Communication Device Class (CDC), PSTN subclass, Line Coding Structure.
   (According "Universal Serial Bus Communications Class Subclass Specification
    for PSTN Devices", p 6.3.11, table 17) */
#define USB_CDC_LINECODING_STRUCTURE_LENGTH     7
typedef struct tag_USBCDCLineCoding
{
    //Data terminal rate, bit/s
    u32 dwDTERate;

    //Stop bits [0,1,2] -> [1,1.5,2]
    u8 bCharFormat;

    //Parity type [0,1,2,3,4] -> [none,odd,even,mark,space]
    u8 bParityType;

    //Databits [5,6,7,8,16]
    u8 bDataBits;

    //Reserved (for align by 4 bytes)
    u8 reserved;
} USBCDCLineCoding;



/* Device descriptor structure */
typedef struct tag_USBDeviceDescriptor
{
    //Length: must 18
    u8 bLength;

    //Type: must 0x01
    u8 bDescriptorType;

    //USB version at BCD format (sample: 2.00 is 0x0200)
    u16 bcdUSB;

    //Device class (when 0 - defined at interface level)
    u8 bDeviceClass;

    //Device sublcass (when 0 - defined at interface level)
    u8 bDeviceSubClass;

    //Device protocol (when 0 - defined at interface level)
    u8 bDeviceProtocol;

    //Endpoint #0 FIFO length
    u8 bMaxPacketSize0;

    //ID vendor
    u16 idVendor;

    //ID product
    u16 idProduct;

    //Device version at BCD format: 1.00 => 0x0100
    u16 bcdDevice;

    //String descriptor number (vendor description)
    u8 iManufacturer;

    //String descriptor number (product description)
    u8 iProduct;

    //String descriptor number (serial number)
    u8 iSerialNmuber;

    //Count of configurations (at least 1)
    u8 bNumConfigurations;

} USBDeviceDescriptor;



/* Configuration descriptor structure */
typedef struct tag_USBConfigurationDescriptor
{
    //Length: must 9
    u8 bLength;

    //Type: must 0x02
    u8 bDescriptorType;

    //Total length of configuration descriptor, low byte
    u8 wTotalLengthL;

    //Total length of configuration descriptor, high byte
    u8 wTotalLengthH;

    //Count of interfaces
    u8 bNumInterfaces;

    //Configuration number
    u8 bConfigurationValue;

    //String descriptor number (configuration description)
    u8 iConfiguration;

    //Attributes
    u8 bmAttributes;

    //Maximum power, 2mA units (100 => 200mA)
    u8 bMaxPower;
} USBConfigurationDescriptor;



/* Interface descriptor structure */
typedef struct tag_USBInterfaceDescriptor
{
    //Length: must 9
    u8 bLength;

    //Type: must 0x04
    u8 bDescriptorType;

    //Interface number (zero based: 0,1,2...)
    u8 bInterfaceNumber;

    //Alternate setting number (zero based: 0,1,2...)
    u8 bAlternateSetting;

    //Count of endpoints at interface
    u8 bNumEndpoints;

    //Interface class
    u8 bInterfaceClass;

    //Interface subclass
    u8 bInterfaceSubClass;

    //Interface protocol
    u8 bInterfaceProtocol;

    //String descriptor number (interface description)
    u8 iInterface;
} USBInterfaceDescriptor;



/* Endpoint descriptor structure */
typedef struct tag_USBEndpointDescriptor
{
    //Length: must 7
    u8 bLength;

    //Type: must 0x05
    u8 bDescriptorType;

    //Endpoint address:
    //bit[7]:     direction 1/0 - IN/OUT
    //bits[3..0]: address [1..15]
    u8 bEndpointAddress;

    //Attributes:
    //bits[1..0]: transfer type (see USB_EPATTR_x)
    u8 bmAttributes;

    //Maxmimum packet size, bytes (low byte)
    u8 wMaxPacketSizeL;

    //Maxmimum packet size, bytes (high byte)
    u8 wMaxPacketSizeH;

    //Interrupt/isochronous mode: request interval, ms
    u8 bInterval;
} USBEndpointDescriptor;



/* HID descriptor */
typedef struct tag_USBHIDDescriptor
{
    //Length: must 9
    u8 bLength;

    //Type: must 0x21
    u8 bDescriptorType;

    //Version at BCD format: minor
    u8 bcdHIDMinor;

    //Version at BCD format: major
    u8 bcdHIDMajor;

    //Country code
    u8 bCountryCode;

    //Number of HID descriptors
    u8 bNumHIDDescriptors;

    //HID descriptor type
    u8 bHIDDescriptorType;

    //HID Descriptor length (low byte)
    u8 wHIDDescriptorLengthL;

    //HID Descriptor length (high byte)
    u8 wHIDDescriptorLengthH;
} USBHIDDescriptor;


/* Convert USB device descriptor to string representation */
char *USBDeviceDescriptorToString(const USBDeviceDescriptor *dev, char *dest);

/* Convert USB configuration descriptor to string representation */
char *USBConfigDescriptorToString(const USBConfigurationDescriptor *config, char *dest);

/* Convert USB interface descriptor to string representation */
char *USBInterfaceDescriptorToString(const USBInterfaceDescriptor *ifc, char *dest);

/* Convert USB endpoint descriptor to string representation */
char *USBEndpointDescriptorToString(const USBEndpointDescriptor *epoint, char *dest);

/* Convert USB HID descriptor to string representation */
char *USBHIDDescriptorToString(const USBHIDDescriptor *hid, char *dest);


/* Convert USB full configuration descriptor to string representation
   (including interface, endpoints and other descriptors) */
char *USBFullConfigDescriptorToString(const u8 *descCfg, char *dest);


/* Get pointers to interfaces with specified number/alterantive settings number.
   When ifcNum < 0 will get all interfaces at configuration descriptor.
   When altSet < 0 will get all interfaces with specified interface number.
   Found interfaces is recorded to ifclist[] array.
   Return count of found interfaces. */
s16 USBGetInterfaceByNumber(const u8 *descCfg,
                            s16 ifcNum, s16 altSet,
                            USBInterfaceDescriptor *ifclist[]);

/* Get pointers to interfaces with specified interface class/subclass.
   When ifcClass < 0 will get all interfaces at configuration descriptor.
   When ifcSubClass < 0 will get interfaces with specified class.
   Found interfaces is recorded to ifclist[] array.
   Return count of found interfaces. */
s16 USBGetInterfaceByClass(const u8 *descCfg,
                           s16 ifcClass, s16 ifcSubClass,
                           USBInterfaceDescriptor *ifclist[]);

/* Get pointers to interface endpoints with specified bmAttributes.
   bmAttr is one of "USB_EPATTR_x" definitions.
   When bmAttr < 0 will get all endpoints at specified interface.
   Found endpoints is recorded to eplist[] array.
   Return count of found endpoints. */
s16 USBGetInterfaceEndpoints(const USBInterfaceDescriptor *ifc,
                             s16 bmAttr,
                             USBEndpointDescriptor *eplist[]);

#endif /* __USB_H */

/*===========================================================================
 * End of file USB.h
 *===========================================================================*/
