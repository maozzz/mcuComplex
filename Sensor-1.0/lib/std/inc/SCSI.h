/*===========================================================================
 *
 *  Standard library
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
 *  File SCSI.h: Small Computer System Interface (SCSI) definitions
 *               and function prototypes.
 *
 *===========================================================================*/

#ifndef __SCSI_H
#define __SCSI_H

#include "../../bastypes.h"

//SCSI command (CBW) signature: 'U' 'S' 'B' 'C' (tx lsb first)
#define SCSI_CBW_SIGNATURE              0x43425355

//SCSI status (CSW) signature: 'U' 'S' 'B' 'S' (tx lsb first)
#define SCSI_CSW_SIGNATURE              0x53425355

//SCSI commands (CBWCB[0] byte)
#define SCSI_CMD_TEST_UNIT_READY       0x00
#define SCSI_CMD_REQUEST_SENSE		   0x03
#define SCSI_CMD_INQUIRY               0x12
#define SCSI_CMD_MODE_SENSE_6   	   0x1A
#define SCSI_CMD_START_STOP_UNIT       0x1B
#define SCSI_CMD_PREVENT_REMOVAL       0x1E
#define SCSI_CMD_READ_CAPACITY_10      0x25
#define SCSI_CMD_READ_10               0x28
#define SCSI_CMD_WRITE_10   		   0x2A
#define SCSI_CMD_VERIFY				   0x2F

//SCSI status codes
#define SCSI_STS_PASSED                0x00
#define SCSI_STS_FAIL                  0x01

//Maximum length of CDB array, bytes
#define SCSI_CDB_LENGTH                 16

//SCSI command block total length (including CDB), bytes
#define SCSI_CBW_LENGTH                 31

//SCSI status length, bytes
#define SCSI_CSW_LENGTH                 13

//Maximum response length for each command, bytes
#define SCSI_CMD_TEST_UNIT_READY_RSPLENGTH      0
#define SCSI_CMD_REQUEST_SENSE_RSPLENGTH        18
#define SCSI_CMD_INQUIRY_RSPLENGTH              36
#define SCSI_CMD_MODE_SENSE_6_RSPLENGTH         192
#define SCSI_CMD_START_STOP_UNIT_RSPLENGTH      0
#define SCSI_CMD_PREVENT_REMOVAL_RSPLENGTH      0
#define SCSI_CMD_READ_CAPACITY_10_RSPLENGTH     8


/* SCSI command block (CBW) */
typedef struct tag_SCSICommandBlock
{
    //Signature: 0x43425355 ('U' 'S' 'B' 'C', LSB first)
    u32 signature;

    //Tag (command id)
    u32 tag;

    //Total transfer length, bytes
    u32 dataTransferLength;

    //Flags
    u8  flags;

    //Bit field:
    //Target[6..4]:
    //LUN[3..0]   : logical unit number
    u8  LUN;

    //Length of CDB field: [0..31]
    u8  CDBLength;

    //Command data
    u8  CDB[SCSI_CDB_LENGTH];
} SCSICommandBlock;



/* SCSI status block (CSW) */
typedef struct tag_SCSIStatusBlock
{
    //Signature: 0x53425355 ('U' 'S' 'B' 'S', LSB first)
    u32 signature;

    //Tag. Must be same as scsi command tag (SCSICommandBlock::tag)
    u32 tag;

    //Residue of data
    u32 dataResidue;

    //Status code (SCSI_STS_x)
    u8 code;
} SCSIStatusBlock;



//Field "SCSICommandBlock::CDB" contains command data:

//Command "TEST UNIT READY"
//CDBLength: 6
//CDB[0] Opcode: 0x00
//CDB[1..4]: Zeros
//CDB[5] Control: 0x00
// bits[7..6]: Vendor specific
// bits[5..3]: Reserved
// bit[2]    : NACA
// bits[1..0]: Obsolete


//Command "REQUEST SENSE"
//CDBLength: 6
//CDB[0] Opcode: 0x03
//CDB[1] Flags:
// bits[7..1]: Reserved
// bit[0]    : DESC
//CDB[2,3]: Zeros
//CDB[4] Allocation length
//CDB[5] Control: 0x00


//Command "INQUIRY"
//CDBLength: 6
//CDB[0] Opcode: 0x12
//CDB[1] Flags:
// bits[7..2]: Reserved
// bit[1]    : 0 - CMDT=0
// bit[0]    : 0 - EVPD=0
//CDB[2] Page code: 0
//CDB[3,4] Allocation length: MSB, LSB
//CDB[5] Control: 0x00


//Command "MODE SENSE(6)"
//CDBLength: 6
//CDB[0] Opcode: 0x1A
//CDB[1] DBD: 0
//CDB[2] Page control:
// bits[7..6]: 0x00 - Page control
// bits[5..0]: 0x3F - SPC-2 page code (all mode pages)
//CDB[3,4] Allocation length: MSB, LSB
//CDB[5] Control: 0x00


//Command "START/STOP UNIT"
//CDBLength: 6
//CDB[0] Opcode: 0x1B
//CDB[1] Immed flags:
// bits[7..1]: Reserved
// bit[0]    : IMMEDIATE 1/0 - true/false
//CDB[2..3]: Zeros
//CDB[4] Pwr flags:
// bits[7..4]: Power conditions 0 - no change
// bits[3..2]: Reserved
// bit[1]    : LOEJ
// bit[0]    : Start 1/0 - start/stop
//CDB[5] Control: 0x00


//Command "PREVENT/ALLOW MEDIUM REMOVAL"
//CDBLength: 6
//CDB[0] Opcode: 0x1E
//CDB[1..3]: Zeros
//CDB[4] Flags:
// bits[7..1]: Reserved
// bit[1]    : 1/0 - PREVENT/ALLOW
//CDB[5] Control: 0x00


//Command "READ CAPACITY(10)"
//CDBLength: 10
//CDB[0] Opcode: 0x25
//CDB[1..8]: Zeros
//CDB[9] Control: 0x00


//Command "READ(10)"
//CDBLength: 10
//CDB[0] Opcode: 0x28
//CDB[1] Flags:
// bit[7..5]: RDPROTECT
// bit[4]   : DPO (Disable page out) 0 - cache data
// bit[3]   : FUA    0 - read cache if possible
// bit[2]   : Reserved
// bit[1]   : FUA_NV 0 - read from volatile or non-volatile cache permitted
// bit[0]   : Reserved
//CDB[2..5]:  Logical Block Address (LBA) [MSB..LSB]
//CDB[6] Group:
// bits[7..5]: Reserved
// bits[4..0]: Group
//CDB[7..8]: Transfer length [MSB..LSB]
//CDB[9] Control: 0x00


//Command "WRITE(10)"
//CDBLength: 10
//CDB[0] Opcode: 0x2A
//CDB[1] Flags:
// bit[7..5]: WRPROTECT
// bit[4]   : DPO (Disable page out) 0 - cache data
// bit[3]   : FUA    0 - read cache if possible
// bit[2]   : Reserved
// bit[1]   : FUA_NV 0 - read from volatile or non-volatile cache permitted
// bit[0]   : Reserved
//CDB[2..5]:  Logical Block Address (LBA) [MSB..LSB]
//CDB[6] Group:
// bits[7..5]: Reserved
// bits[4..0]: Group
//CDB[7..8]: Transfer length [MSB..LSB]
//CDB[9] Control: 0x00

#endif /* __SCSI_H */

/*===========================================================================
 * End of file SCSI.h
 *===========================================================================*/
