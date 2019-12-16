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
 *  File DMA.h: DMA definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __DMA_H
#define __DMA_H

#include "types.h"

#ifndef DMA_USE_ALT_CHCTRL
#define DMA_USE_ALT_CHCTRL			1
#endif /* !DMA_USE_ALT_CHCTRL */

//Source/destination pointer increment (DMAChTxCfg::src_inc,dst_inc)
#define DMA_CHCFG_INC_BYTE			0
#define DMA_CHCFG_INC_SHORT			1
#define DMA_CHCFG_INC_LONG			2
#define DMA_CHCFG_INC_NONE			3
//Source/destination pointer size (DMAChTxCfg::src_size,dst_size)
#define DMA_CHCFG_SIZE_BYTE			0
#define DMA_CHCFG_SIZE_SHORT		1
#define DMA_CHCFG_SIZE_LONG			2
//Source/destination HPROT (DMAChTxCfg::src_prot_ctrl,dst_prot_ctrl)
#define DMA_CHCFG_PROT_CACHE		4
#define DMA_CHCFG_PROT_BUFFER		2
#define DMA_CHCFG_PROT_PRIV			1
//DMA cycle mode (DMAChTxCfg::cycle_ctrl)
#define DMA_CHCFG_CYCLE_NONE		0
#define DMA_CHCFG_CYCLE_BASIC		1
#define DMA_CHCFG_CYCLE_AUTOREQ		2
#define DMA_CHCFG_CYCLE_PINGPONG	3
#define DMA_CHCFG_CYCLE_MEMPRI		4
#define DMA_CHCFG_CYCLE_MEMALT		5
#define DMA_CHCFG_CYCLE_PERPRI		6
#define DMA_CHCFG_CYCLE_PERALT		7
//DMA channel priority (DMAChConfig::priority)
#define DMA_CHCFG_PRIOR_HIGH		1
#define DMA_CHCFG_PRIOR_DEFAULT		0

/* */
#define dma_channel_cfg(dinc, dsize, sinc, ssize, dprot, sprot, R, slength, useburst, cycle) \
   ( ((dinc)  << 30)|\
	 ((dsize) << 28)|\
	 ((sinc)  << 26)|\
	 ((ssize) << 24)|\
	 ((dprot) << 21)|\
	 ((sprot) << 18)|\
	 ((R)     << 14)|\
	 ((slength - 1) << 4)|\
	 ((useburst) << 3)|\
	 ((cycle)    << 0) )

/* DMA channel transmition configuration */
typedef struct tag_DMAChTxCfg
{
	//DMA cycle mode (see DMA_CHCFG_CYCLE_x)
	u32 cycle_ctrl:3;

	//
	u32 next_useburst:1;

	//Overall count of transmitions at DMA cycle [0..1023]
	u32 n_minus_1:10;

	//Count of DMA transmitions before arbitrage (2^R_power, R_power=[0..10])
	u32 R_power:4;

	//Source HPROT (see DMA_CHCFG_PROT_x)
	u32 src_prot_ctrl:3;

	//Destination HPROT (see DMA_CHCFG_PROT_x)
	u32 dst_prot_ctrl:3;

	//Source pointer size (see DMA_CHCFG_SIZE_x)
	u32 src_size:2;

	//Source pointer increment (see DMA_CHCFG_INC_x)
	u32 src_inc:2;

	//Destination pointer size (see DMA_CHCFG_SIZE_x)
	u32 dst_size:2;

	//Destination pointer increment (see DMA_CHCFG_INC_x)
	u32 dst_inc:2;
} DMAChTxConfig;



/* DMA channel data configuration */
typedef struct tag_DMAChData
{
	//Source data end pointer
	volatile u32 src_end_ptr;

	//Destination end pointer
	volatile u32 dest_end_ptr;

	//Channel configuration
	volatile u32 channel_cfg;

	//Unused
	u32 unused;
} DMAChData;



/* DMA channel configuration */
typedef struct tag_DMAChConfig
{
	//Pointer to primary data configuration
	DMAChData *pri;

	//Pointer to alter data configuration
	DMAChData *alt;

	//Channel number
	u8 chNumber;

	//Channel priority (see DMA_CHCFG_PRIOR_x)
	u8 priority:6;

	//Use burst mode
	u8 useBurst:1;

	//Use alternate dat structure
	u8 useAltData:1;

	//Reserved
	u8 reserved[2];
} DMAChConfig;


/* DMA controller configuration */
typedef struct tag_DMAConfig
{
	//HPROT (see DMA_CHCFG_PROT_x)
	u32 prot;
} DMAConfig;

/* Initialize DMA controller */
MCRESULT InitializeDMA(DMAConfig *cfg);

/* Initialize specified DMA channel */
MCRESULT InitializeDMAChannel(DMAChConfig *cfg);

/* Turn ON/OFF DMA controller */
void TurnDMA(u8 on);

/* Turn ON/OFF the specified DMA Channel */
void TurnDMAChannel(u32 chNumber, u8 on);

/* Generates software request for specified DMA channel */
void GenDMARequest(u32 chNumber);

/* Reset DMA error flags */
void ResetDMAError();

/* */
DMAChData *GetChPriData(u32 chNumber);

/* */
DMAChData *GetChAltData(u32 chNumber);

#endif /* __DMA_H */

/*===========================================================================
 * End of file DMA.h
 *===========================================================================*/
