/*===========================================================================
 *
 *  Samples
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
 *  File globaldef.h: Global definitions (MCU 1986BE1T).
 *
 *===========================================================================*/

#ifndef __GLOBALDEF_H
#define __GLOBALDEF_H

/*===========================================================================
 * Software mode
 *===========================================================================*/

//Debug: evaluation board
#define SOFT_MODE_DEBUG                         1
//Release: real device
#define SOFT_MODE_RELEASE                       2

//Current software mode
#define SOFT_MODE                               SOFT_MODE_DEBUG

/*===========================================================================
 * Console output. See "lib/std/inc/consp.h"
 *===========================================================================*/

//Print messages within UART (comment to disable)
#define PRINT_OUTPUT                            PRINT_UART

/*===========================================================================
 * File system configuration. See "lib/sys/inc/filesys.h"
 *===========================================================================*/

#define FS_FAT_EN			1

#define FS_DEV_EN           0

#define FS_PROC_EN          0

#define FS_VROOT_APPS_EN    0

/*===========================================================================
 * Audio configuration. See "lib/sys/inc/audio.h"
 *===========================================================================*/

//Enable/disable audio modules (1/0 - enabled/disabled)
//Audio module (encode/decode audio)
#define AUD_EN_MOD_AUDIO                        1

//Signal generator
#define AUD_EN_MOD_GENERATOR                    1

//Oscilloscope
#define AUD_EN_MOD_OSC                          0

//G.711 a/u-Law
#define AUD_EN_CODEC_G711                       1

//G.726
#define AUD_EN_CODEC_G726                       0

//GSM 06.10
#define AUD_EN_CODEC_GSM                        0

//DVI 32kbit
#define AUD_EN_CODEC_DVI                        0

//G.7231
#define AUD_EN_CODEC_G7231                      0

//DTMF
#define AUD_EN_CODEC_DTMF                       1

//Audio sources count
#define AUD_ASRC_COUNT                          1

/*===========================================================================
 * Screen configuration. See "lib/sys/inc/screen.h"
 *===========================================================================*/

//Enable/disable color functions
//1-bit color
#define SCREEN_EN_COLOR_1b                      1

//4-bit color
#define SCREEN_EN_COLOR_4b                      0

//8-bit color
#define SCREEN_EN_COLOR_8b                      0

//16-bit color
#define SCREEN_EN_COLOR_16b                     0

//24-bit color
#define SCREEN_EN_COLOR_24b                     0

//32-bit color
#define SCREEN_EN_COLOR_32b                     0

//Add graphic primitives library
#define SCREEN_EN_GRPRIM                        1

//Add text primitives library
#define SCREEN_EN_TXTPRIM                       1

/*===========================================================================
 * Enable/disable control modules
 *===========================================================================*/

#define CTRLMOD_EN_PROGRESSBAR           0

#define CTRLMOD_EN_SLIDERBAR             0

#define CTRLMOD_EN_SCROLLBAR             0

#define CTRLMOD_EN_TEXTBOX               1

#define CTRLMOD_EN_MENUBAR               0

#define CTRLMOD_EN_BUTTON                0

#define CTRLMOD_EN_RADIOBUTTON           0

#define CTRLMOD_EN_CHECKBOX              0

#define CTRLMOD_EN_LISTBOX               0

#define CTRLMOD_EN_COMBOBOX              0

#define CTRLMOD_EN_TASKBAR               0

#define CTRLMOD_EN_FILELISTBOX           0

/*===========================================================================
 * Network configuration. See "lib/sys/inc/network.h"
 *===========================================================================*/

//Maximum count of network interfaces including localhost
//(i.e. minimum value should 2: localhost and one real interface)
#define NET_IFC_MAXCOUNT                        2

//Check ethernet link interval, msec
//#define NET_ARP_CHECK_LINK_INTERVAL           250

//Probe handler interval, msec
//#define NET_ARP_PROBE_HANDLER_INTERVAL        5

//Maximum size of ARP table
#define NET_ARP_TABLE_SIZE                      12

//Maximum size of IPv4 route table
#define NET_ROUTE_TABLE_SIZE                    8

//IGMP state update interval, msec
//#define NET_IGMP_STATE_UPDINTERVAL            100

//Maximum size of interface reception state (for all interfaces)
#define NET_IGMP_STATE_SIZE                     6

//Enable/disable source list support
#define NET_IGMP_EN_SRCLIST                     0

//Maximum size of source list
#define NET_IGMP_SRCLIST_SIZE                   8

//Robustness Variable. MUST NOT be 0, and SHOULD NOT be 1
//#define NET_IGMP_ROBUST_VAR                     2

//Unsolicited Report Interval, msec
//#define NET_IGMP_REPORT_INTERVAL                1000

//Query Response Interval, msec
//#define NET_IGMP_QRESPONSE_INTERVAL             10000

//Last Member Query Interval, msec
//#define NET_IGMP_LASTMEMBER_INTERVAL            1000

//Maximum size of DNS cache
#define NET_DNS_CACHE_SIZE                      4

//Maximum count of DNS servers
#define NET_DNS_NSERVER_COUNT                   2

//Number of stored IP addresses
#define NET_DNS_ENTRY_IPCOUNT                   2

//Enable/disable TCP module
#define NET_TCP_MODULE_ENABLE                   1

//Maximum size of TCB table (maximum count of TCP connections)
//#define NET_TCP_TCBTABLE_SIZE                 12

//Maximum size of TCP listeners table (maximum count of opened ports)
//#define NET_TCP_TCPLTABLE_SIZE                12

#endif /* !__GLOBALDEF_H */

/*===========================================================================
 * End of file globaldef.h
 *===========================================================================*/

