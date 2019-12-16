/*===========================================================================
 *
 *  MCU library
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2014-2016
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File swmodel_DSP.c: DSP core functions.
 *
 *===========================================================================*/

#include "../../inc/DSP.h"
#include <string.h>

static u32 currDSPCLK = 0;

#define DSP_CODE_SIZE	    (DSP_CODE_MAX_SIZE + 128)
#define DSP_DATA_SIZE	    (DSP_DATA_MAX_SIZE + 128)

#define DSP_MEM_BEGADDR	    (DSP_DATA_ARM_ADDR)
#define DSP_MEM_ENDADDR	    (DSP_DATA_ARM_ADDR + ((DSP_DATA_SIZE + DSP_CODE_SIZE) << 1))

static u16 dspMemorySpace[DSP_CODE_SIZE + DSP_DATA_SIZE];

/* */
MCRESULT SetDSPCLK(u32 hclk, u32 clkSrc)
{
	memset(dspMemorySpace, 0, (DSP_CODE_SIZE + DSP_DATA_SIZE) * sizeof(u16));
	currDSPCLK = hclk;
	return MCR_OK;
}



/* */
u32 GetDSPCLK(void)
{
	return currDSPCLK;
}



/* Load DSP data/code to DSP data/code memory */
void DSPMemWrite(const u16 *src, u32 destArmAddr, s32 wSize)
{
    s32 i = 0;
    while(i < wSize && destArmAddr < DSP_MEM_ENDADDR)
    {
	if (destArmAddr >= DSP_MEM_BEGADDR)
	    dspMemorySpace[((destArmAddr - DSP_MEM_BEGADDR) >> 1)] = src[i];

	destArmAddr += 2;
	i++;
    }
}



/* Clear DSP data memory */
void DSPMemDataClear(void)
{
    memset(dspMemorySpace, 0, DSP_DATA_SIZE * sizeof(u16));
}



/* Set/clear reset signal for DSP peripherals */
void DSPPerReset(u8 on)
{

}



/* Set/clear reset signal for DSP memory */
void DSPMemReset(u8 on)
{

}



/* Set/clear reset signal for DSP core */
void DSPCoreReset(u8 on)
{

}



/* Set/clear reset signal for DSP subsystem */
void DSPBlockReset(u8 on)
{

}

/*===========================================================================
 * End of file swmodel_DSP.c
 *===========================================================================*/
