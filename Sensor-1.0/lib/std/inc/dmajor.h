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
 *  File dmajor.h: Definitions for major device numbers.
 *
 *===========================================================================*/

#ifndef __DMAJOR_H
#define __DMAJOR_H

#define UNNAMED_MAJOR		0
#define MEM_MAJOR			1
#define RAMDISK_MAJOR		1
#define FLOPPY_MAJOR		2
#define HD_MAJOR			3
#define TTY_MAJOR			4
#define TTYAUX_MAJOR		5
#define LP_MAJOR			6

#define INPUT_MAJOR			13
#define SOUND_MAJOR			14

#define NETLINK_MAJOR		36
#define NBD_MAJOR			43   //Network block device

#define SCSI_DISK0_MAJOR	8
#define SCSI_DISK1_MAJOR	65
#define SCSI_DISK2_MAJOR	66
#define SCSI_DISK3_MAJOR	67
#define SCSI_DISK4_MAJOR	68

#define IDE0_MAJOR			HD_MAJOR
#define IDE1_MAJOR			22
#define IDE2_MAJOR			33
#define IDE3_MAJOR			34

#define RAW_MAJOR			162
#define USB_CHAR_MAJOR		180

#define CPUID_MAJOR			203

#define PORT_MAJOR			240
#define ADC_MAJOR			241
#define DAC_MAJOR			242
#define SSP_MAJOR			243
#define RTC_MAJOR			254

#endif /* !__DMAJOR_H */

/*===========================================================================
 * End of file dmajor.h
 *===========================================================================*/
