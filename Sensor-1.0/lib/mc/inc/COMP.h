/*===========================================================================
 *
 *  MCU library
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
 *  File COMP.h: Comparator definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __COMP_H
#define __COMP_H

#include "types.h"
#include "CPU.h"

//Comparator number
#define COMP_N1         0

//Comparator Vref source
#define COMP_VREFNONE           0x00 //No source, compare 2 inputs
#define COMP_VREFEXT            0x01 //External source
#define COMP_VREFINT            0x02 //Internal source

typedef u8                      COMPBLOCK;


typedef struct tag_COMPConfig
{
        //Selected comparator (see COMP_Nx)
        COMPBLOCK comp;

        //Reference voltage source (see COMP_VREFx)
        u8 vRefSrc;

        //Scale vRefSrc (see MC documentation)
        u16 vRefi;

        //Result handler
        void (*result)(u32 *v);

        //Mapping inputs (see COMP_MAP_IN_x)
        u16 mapIn;

        //Mapping outputs (see COMP_MAP_OUT_x)
        u16 mapOut;
} COMPConfig;


/* Initialize comparator */
MCRESULT InitializeCOMP(const COMPConfig *cfg);

/* Turn comparator */
void TurnCOMP(COMPBLOCK comp, u8 turn);

/* Select input "+" */
void COMPSelInP(COMPBLOCK comp, u8 inP);

/* Select input "-" */
void COMPSelInM(COMPBLOCK comp, u8 inM);

/* Select input "+", "-" */
void COMPSelInPM(COMPBLOCK comp, u8 inP, u8 inM);

/* */
u32 COMPRead();

#endif /* __COMP_H */

/*===========================================================================
 * End of file COMP.h
 *===========================================================================*/

