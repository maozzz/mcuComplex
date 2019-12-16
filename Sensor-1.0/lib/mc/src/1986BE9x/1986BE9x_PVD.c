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
 *  File 1986BE9x_PVD.c. Power detector functions.
 *
 *===========================================================================*/

#include "../../inc/PVD.h"
#include <string.h>

/*
PVDCS register:
INV[11]:    set PVD  flag 0 -  Ucc > Uthold;  1 -  Ucc < Uthold
INVB[10]:   set PVBD flag 0 - BUcc > BUthold; 1 - BUcc < BUthold
IEPVD[9]:   1/0 - enable/disable interrupt from PVD sensor  (clear with 0)
IEPVBD[8]:  1/0 - enable/disable interrupt from PVBD sensor (clear with 0)
PVD[7]:     flag (clear with 0)
PVBD[6]:    flag (clear with 0)
PLS[5..3]:  Uthold  [0..7] => [2.0V..3.4V] (step 0.2V)
PBLS[2..1]: BUthold [0..3] => [1.8V..3.0V] (step 0.4V)
PVDEN[0]:   1/0 - ON/OFF PVD block
*/

void (*pvdN1Event)(void);

/* */
MCRESULT InitializePVD(const PVDConfig *cfg)
{
    u32 pvdcs;
    u8 Uthold, BUthold;

    if (!cfg)
        return MCR_INVALID_CFG;

    pvdN1Event = cfg->pvdEvent;

    //Enable PVD clock
    ClkEnable(CLK_POWER);

    //Turn PVD OFF
    POWER->PVDCS = 0;

    pvdcs = 0;

    //INV[11]=1: invert flag
    if (cfg->opts & PVD_OPTS_EVENTBY_ULESSUTHOLD)
        pvdcs |= PN11;

    //Set threshold
    Uthold  = cfg->Uthold;
    BUthold = cfg->BUthold;

    if (Uthold < 20)      Uthold = 20;
    else if (Uthold > 34) Uthold = 34;

    if (BUthold < 18)      BUthold = 18;
    else if (BUthold > 30) BUthold = 30;

    //PLS  = (U - 2.0V) / 0.2V
    Uthold  = (Uthold  - 20) >> 1;
    //PBLS = (BU - 1.8V) / 0.4V
    BUthold = (BUthold - 18) >> 2;

    //Set PLS[5..3] and PBLS[2..1]
    pvdcs |= (Uthold << 3) | (BUthold << 1);

    if (cfg->pvdEvent)
    {
        //IEPVD[9]=1; IEPVBD[8]=1: enable interupts
        pvdcs |= (PN9|PN8);

        IRQEnable(IRQ_POWER);
    }
    else
    {
        //Disable interrupts
        IRQDisable(IRQ_POWER);
    }

    //Set register
    POWER->PVDCS = pvdcs;

    return MCR_OK;
}



/* Turn ON/OFF */
void TurnPVD(PVDBLOCK pvd, u8 on)
{
    //PVDEN[0]: 1/0 - turn ON/OFF
    if (on)
        POWER->PVDCS |= PN0;
    else
        POWER->PVDCS &= ~PN0;
}



/* Indicates that Ucc > Uthold
   (or Ucc < Uthold when PVD_OPTS_UlessUthold is set) */
u32 IsPVDPowerEvent(PVDBLOCK pvd)
{
    //PVD[7]=1: power sensor event
    return (POWER->PVDCS & PN7) ? 1 : 0;
}



/* Indicates that BUcc > BUthold
   (or BUcc < BUthold when PVD_OPTS_UlessUthold is set) */
u32 IsPVDBatteryEvent(PVDBLOCK pvd)
{
    //PVBD[6]=1: battery sensor event
    return (POWER->PVDCS & PN6) ? 1 : 0;
}



/* IRQ handler */
void POWER_IRQHandler()
{
    u32 pvdcs = POWER->PVDCS;

    //IEPVD[9]=PVD[7]=1 (power sensor event)
    if (pvdcs & (PN9|PN7))
    {
        //Call user-defined handler
        if (pvdN1Event)
            pvdN1Event();

        //Clear flags: IEPVD[9]=PVD[7]=0
        pvdcs &= ~(PN9|PN7);
    }

    //IEPVBD[8]=PVBD[6]=1 (battery sensor event)
    if (pvdcs & (PN8|PN6))
    {
        //Call user-defined handler
        if (pvdN1Event)
            pvdN1Event();

        //Clear flags: IEPVBD[8]=PVBD[6]=0
        pvdcs &= ~(PN8|PN6);
    }

    //Update register
    POWER->PVDCS = pvdcs;
}

/*===========================================================================
 * End of file 1986BE9x_PVD.c
 *===========================================================================*/
