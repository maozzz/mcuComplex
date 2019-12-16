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
 *  File PVD.h: Power detector definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __PVD_H
#define __PVD_H

#include "types.h"
#include "CPU.h"

//PVD blocks
#define PVD_N1      0

//Event rise when Ucc < Uthold (otherwise Ucc > Uthold)
#define PVD_OPTS_EVENTBY_ULESSUTHOLD       0x01

typedef u8 PVDBLOCK;

/* PVD configuration.
   Uthold and BUthold at BCD format, for sample:
   3.2V = 32
   2.6V = 26 */
typedef struct tag_PVDConfig
{
    //PVD block number
    PVDBLOCK pvd;

    //Options (see PVD_OPTS_x)
    u8 opts;

    //Source power threshold, BCD format
    u8 Uthold;

    //Battery power threshold, BCD format
    u8 BUthold;

    //User-defined handler
    void (*pvdEvent)(void);
} PVDConfig;

/* */
MCRESULT InitializePVD(const PVDConfig *cfg);

/* Turn ON/OFF */
void TurnPVD(PVDBLOCK pvd, u8 on);

/* Indicates that Ucc > Uthold
   (or Ucc < Uthold when PVD_OPTS_UlessUthold is set) */
u32 IsPVDPowerEvent(PVDBLOCK pvd);

/* Indicates that BUcc > BUthold
   (or BUcc < BUthold when PVD_OPTS_UlessUthold is set) */
u32 IsPVDBatteryEvent(PVDBLOCK pvd);

#endif /* __PVD_H */

/*===========================================================================
 * End of file PVD.h
 *===========================================================================*/
