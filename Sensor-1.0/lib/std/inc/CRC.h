/*===========================================================================
 *
 *  Standard library
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2014
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File CRC.h: Cyclic Redundancy Check (CRC) definitions
 *              and functions prototypes.
 *
 *===========================================================================*/

#ifndef __CRC_H
#define __CRC_H

#include "../../bastypes.h"

//USB token packets
#define CRC5_USB_POL1           0x05
#define CRC5_USB_POL2           0x14
#define CRC5_USB_POL3           0x12

//(Telecommunication, ITU-T G.707[14], ITU-T G.832[15], MMC, SD)
#define CRC7_POL1				0x09
#define CRC7_POL2				0x48
#define CRC7_POL3				0x44

//(ATM HEC), ISDN Header Error Control and Cell Delineation ITU-T I.432.1 (02/99)
#define CRC8_CCIT_POL1          0x07
#define CRC8_CCIT_POL2          0xE0
#define CRC8_CCIT_POL3          0x83

//Bisync, Modbus, USB, ANSI X3.28[20]. As known CRC-16 or CRC-16-ANSI
#define CRC16_IBM_POL1          0x8005
#define CRC16_IBM_POL2          0xA001
#define CRC16_IBM_POL3          0xC002

//X.25, HDLC, XMODEM, Bluetooth, SD
#define CRC16_CCIT_POL1         0x1021
#define CRC16_CCIT_POL2         0x8408
#define CRC16_CCIT_POL3         0x8810

//SCSI DIF
#define CRC16_T10_DIF_POL1      0x8BB7
#define CRC16_T10_DIF_POL2      0xEDD1
#define CRC16_T10_DIF_POL3      0xC5DB

//DNP, IEC 870, M-Bus
#define CRC16_DNP_POL1          0x3D65
#define CRC16_DNP_POL2          0xA6BC
#define CRC16_DNP_POL3          0x9EB2

//V.42, MPEG-2, PNG, POSIX checksum
#define CRC32_IEEE8023_POL1     0xEDB88320
#define CRC32_IEEE8023_POL2     0x04C11DB7
#define CRC32_IEEE8023_POL3     0x82608EDB

//iSCSI, G.hn payload (C - Castagnoli)
#define CRC32_C_POL1            0x1EDC6F41
#define CRC32_C_POL2            0x82F63B78
#define CRC32_C_POL3            0x8F6E37A0

//(K - Koopman)
#define CRC32_K_POL1            0x741B8CD7
#define CRC32_K_POL2            0xEB31D82E
#define CRC32_K_POL3            0xBA0DC66B

//Aviation, AIXM
#define CRC32_Q_POL1            0x814141AB
#define CRC32_Q_POL2            0xD5828281
#define CRC32_Q_POL3            0xC0A0A0D5

//Initial values
#define CRC5_BEGIN_VALUE        0x1F
#define CRC8_BEGIN_VALUE        0xFF
#define CRC16_BEGIN_VALUE       0xFFFF
#define CRC32_BEGIN_VALUE       0xFFFFFFFF

u32 GetCRC32R(u32 pol, const u8 *data, u32 length);

u16 GetCRC16R(u16 pol, const u8 *data, u32 length);

/* Solve CRC-7 (SD card commands) */
u8 GetCRC7SD(const u8 *data, u32 length);

/* Solve CRC-16 (SD card data) */
u16 GetCRC16SD(const u8 *data, u32 length);

/* Solve CRC-16 (SD card data) */
u16 GetCRC16SDI(const u8 *data, u32 length, u16 incrc);

/* Solve CRC-8 (1-Wire interface, polinome X^8+X^5+X^4+1) */
u8 GetCRC8IWire(const u8 *data, u32 length);

#endif /* __CRC_H */

/*===========================================================================
 * End of file CRC.h
 *===========================================================================*/
