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
 *  File 1986BE9x_DMA.c: DMA functions.
 *
 *===========================================================================*/

#include "../../inc/DMA.h"
#include <string.h>

/* */
#define DMA_CHNUM_MASK			(DMA_CHNL_COUNT - 1)

/* */
#define DMAChMask(chnum)		(1 << ((chnum) & DMA_CHNUM_MASK))

/* */
#define DMA_CHCFG_PROT_MASK (DMA_CHCFG_PROT_CACHE  |\
							 DMA_CHCFG_PROT_BUFFER |\
							 DMA_CHCFG_PROT_PRIV)

/* */
#define CHDATA_TABLE_SIZE	(DMA_CHNL_COUNT * (1 + DMA_USE_ALT_CHCTRL))

//Select data alignment
#if (DMA_CHNL_COUNT == 1)
#define DATA_ALIGN	32
#elif (DMA_CHNL_COUNT == 2)
#define DATA_ALIGN	64
#elif ((DMA_CHNL_COUNT >= 3) && (DMA_CHNL_COUNT <= 4))
#define DATA_ALIGN	128
#elif ((DMA_CHNL_COUNT >= 5) && (DMA_CHNL_COUNT <= 8))
#define DATA_ALIGN	256
#elif ((DMA_CHNL_COUNT >= 9) && (DMA_CHNL_COUNT <= 16))
#define DATA_ALIGN	512
#elif ((DMA_CHNL_COUNT >= 17) && (DMA_CHNL_COUNT <= 32))
#define DATA_ALIGN	1024
#endif

#if defined ( __CC_ARM )
	#if (TARGET_MCU == MCU_1986BE1x)
		DMAChData chDataTable[CHDATA_TABLE_SIZE] __attribute__((section("EXECUTABLE_MEMORY_SECTION"))) __attribute__ ((aligned (DATA_ALIGN)));
	#else
		DMAChData chDataTable[CHDATA_TABLE_SIZE] __attribute__ ((aligned (DATA_ALIGN)));
	#endif

#elif defined ( __ICCARM__ )
	#pragma data_alignment = DATA_ALIGN
	#if (TARGET_MCU == MCU_1986BE1x)
		DMAChData chDataTable[CHDATA_TABLE_SIZE] IAR_SECTION ("EXECUTABLE_MEMORY_SECTION");
	#else
		DMAChData chDataTable[CHDATA_TABLE_SIZE];
	#endif

#elif defined ( __GNUC__ )
	#if (TARGET_MCU == MCU_1986BE1x)
		DMAChData chDataTable[CHDATA_TABLE_SIZE] __attribute__((section(".ramfunc"), aligned(DATA_ALIGN)));
	#else
		DMAChData chDataTable[CHDATA_TABLE_SIZE] __attribute__((aligned(DATA_ALIGN)));
	#endif

#elif defined ( __TASKING__ )
	DMAChData chDataTable[CHDATA_TABLE_SIZE];

#elif defined ( __CMCARM__ )
	#if (TARGET_MCU == MCU_1986BE1x)
		#pragma locate chDataTable 0x20100000 noinit
		DMAChData chDataTable[CHDATA_TABLE_SIZE];
	#else
		#pragma locate chDataTable 0x20000000 noinit
		DMAChData chDataTable[CHDATA_TABLE_SIZE];
	#endif
#endif



/* */
MCRESULT InitializeDMA(DMAConfig *cfg)
{
	//Enable DMA clock
	ClkEnable(CLK_DMA);	

	//Clear previuos configuraion
	DMA->CFG = (u32)0;
	DMA->CHNL_ENABLE_CLR   = (u32)~1;
	DMA->CHNL_PRIORITY_CLR = (u32)~1;
	DMA->CHNL_PRI_ALT_CLR  = (u32)~1;
	DMA->CHNL_REQ_MASK_CLR = (u32)~1;
	DMA->CHNL_USEBURST_CLR = (u32)~1;
	DMA->ERR_CLR           = 1;

	//CFG[7..5] chnl_prot_ctrl: access with cache,buffer,priv
	if (cfg)
		DMA->CFG = ((cfg->prot & DMA_CHCFG_PROT_MASK) << 5);

	//Set base address primary control structures
	DMA->CTRL_BASE_PTR = (u32)chDataTable;

	return MCR_OK;
}



/* */
MCRESULT InitializeDMAChannel(DMAChConfig *cfg)
{	
	DMAChData *pChData;
	u32 mask;

	if (!cfg ||
		 cfg->chNumber >= DMA_CHNL_COUNT ||
		 (!cfg->pri && !cfg->alt))
		return MCR_FAIL;

	mask = DMAChMask(cfg->chNumber);

	if (cfg->pri)
	{
		pChData = GetChPriData(cfg->chNumber);
		memcpy(pChData, cfg->pri, sizeof(DMAChData));
	}

	#if DMA_USE_ALT_CHCTRL
	if (cfg->alt)
	{
		pChData = GetChAltData(cfg->chNumber);
		memcpy(pChData, cfg->alt, sizeof(DMAChData));
	}
	#endif /* DMA_USE_ALT_CHCTRL */

	//Using '=' instead '|=': write zero bits to register has no effect
	if (cfg->useBurst)
		DMA->CHNL_USEBURST_SET = mask;
	else
		DMA->CHNL_USEBURST_CLR = mask;

	if (cfg->useAltData)
		DMA->CHNL_PRI_ALT_SET = mask; //using alter
	else
		DMA->CHNL_PRI_ALT_CLR = mask; //using primary

	if (cfg->priority == DMA_CHCFG_PRIOR_HIGH)
		DMA->CHNL_PRIORITY_SET = mask;
	else
		DMA->CHNL_PRIORITY_CLR = mask;

	DMA->CHNL_REQ_MASK_CLR = mask;

	//DMA->CHNL_ENABLE_SET   = mask;

	return MCR_OK;
}



/* Turn ON/OFF DMA controller */
void TurnDMA(u8 on)
{
	//master_enable[0] = 1/0: turn ON/OFF DMA
	if (on)
		DMA->CFG |= 1;
	else
		DMA->CFG &= ~1;
}



/* Turn ON/OFF the specified DMA Channel */
void TurnDMAChannel(u32 chNumber, u8 on)
{
	if (on)
		DMA->CHNL_ENABLE_SET = DMAChMask(chNumber);
	else
		DMA->CHNL_ENABLE_CLR = DMAChMask(chNumber);
}



/* Generates software request for specified DMA channel */
void GenDMARequest(u32 chNumber)
{
	DMA->CHNL_ENABLE_SET = DMAChMask(chNumber);
	DMA->CHNL_SW_REQUEST |= DMAChMask(chNumber);
}



/* Reset DMA error flags */
void ResetDMAError()
{
	DMA->ERR_CLR = 0x01;
}



/* */
DMAChData *GetChPriData(u32 chNumber)
{
	return (DMAChData*)&chDataTable[(chNumber & DMA_CHNUM_MASK)];
}



/* */
DMAChData *GetChAltData(u32 chNumber)
{
#if DMA_USE_ALT_CHCTRL
	return (DMAChData*)&chDataTable[((chNumber & DMA_CHNUM_MASK) + DMA_CHNL_COUNT)];
#else
	return 	(DMAChData*)0;
#endif /* DMA_USE_ALT_CHCTRL */
}

/*===========================================================================
 * End of file 1986BE9x_DMA.c
 *===========================================================================*/
