/*===========================================================================
 *
 *  Hardware library
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
 *  File targetlcd.h: LCD PC1602x. 8-bit interface [DB7..DB0].
 *                    MCU 1986BE1T evaluation board is used.
 *                    It's not recommended change any definitions at this file.
 *
 *===========================================================================*/

#ifndef __TARGETLCD_H
#define __TARGETLCD_H

//Select taget LCD (see LCD_x at "lib/hw/inc/LCD.h"):
#define TARGET_LCD		LCD_PC1602LRSFNHx

//Uncomment to enable control LCD within extension bus
//#define __LCD_USE_EXT_BUS	1


//LCD baseaddr: bits[31..29] - region
#define LCD_BASEADDR    0x60000000U

//LCD address mapping.
//[E12] -> [ADDR26] -> [RS]
#define LCD_CMD1        (LCD_BASEADDR)      //RS=0
#define LCD_DATA1       (LCD_BASEADDR|PN26) //RS=1


//Port options: data (note that pull down).
#define LCD_PORT_OPTS_DATA   (PN_PWR_FASTEST |\
			      PN_NOPULLUP | PN_PULLDOWN   |\
			      PN_NOSHMIT  | PN_CTRLDRIVER |\
			      PN_NOINFILTER)

//Port options: address (note that pull down).
#define LCD_PORT_OPTS_ADDR   (PN_PWR_FASTEST |\
			      PN_NOPULLUP | PN_PULLDOWN   |\
			      PN_NOSHMIT  | PN_CTRLDRIVER |\
			      PN_NOINFILTER)

//Port options: E (CLOCK), RW (WE) (note that pull up)
#define LCD_PORT_OPTS_CTRL   (PN_PWR_FASTEST |\
			      PN_PULLUP  | PN_NOPULLDOWN |\
			      PN_NOSHMIT | PN_CTRLDRIVER |\
			      PN_NOINFILTER)


//LCD_RES_{Act/Off}: sets active/inactive LCD reset (optional)
#define LCD_RES_Act()         //nothing for PC1602
#define LCD_RES_Off()         //nothing for PC1602


#ifdef __LCD_USE_EXT_BUS

//Read something to set E1, E2 to 0
#define LCD_CE_Act()            //nothing
#define LCD_CE_Off()            { u8 tmp = *((vu8*)LCD_BASEADDR); }


//[A7..A0] -> [DB7..DB0]
//[C2,C0]  -> [E (CLOCK), RW (WE)] (different port functions)
//[E12]    -> [ADDR26] -> [RS]
__STATIC_INLINE void InitializeLCDPorts()
{
	InitializePORTEx(PORT_A, 0xFF, 0, PN_BASIC | LCD_PORT_OPTS_DATA);
	InitializePORTEx(PORT_C, PN0,  0, PN_BASIC | LCD_PORT_OPTS_CTRL);
	InitializePORTEx(PORT_C, PN2,  0, PN_ALTER | LCD_PORT_OPTS_CTRL);
	InitializePORTEx(PORT_E, PN12, 0, PN_ALTER | LCD_PORT_OPTS_ADDR);
}

#else  /* __LCD_USE_EXT_BUS */

#define LCD_INV_CLKCYC			0.000160F

//Reset [E12] -> [RS] to 0
#define LCD_CE_Act()            //nothing
#define LCD_CE_Off()            PE_Reset(PN12)

//[C2] -> [E (CLOCK)] 1/0 - set/reset strobe
#define LCD_E_Act()             PC_Set(PN2)
#define LCD_E_Off()             PC_Reset(PN2)

//[C0] -> [RW (WE)] (0 - write, active; 1 - read)
#define LCD_WE_Act()            PC_Reset(PN0)
#define LCD_WE_Off()            PC_Set(PN0)

//Select E2, E1, A0
//[E12] -> [ADDR26] -> [RS]
#define LCD_ADDR_Set(addr, tmp) PE_Write( (PE_Read() & ~(1 << 12)) |\
                                          ((addr & (PN26)) >> 14) )

//[A0..A7] -> [DB0..DB7]
#define LCD_DB_Out()            PA_Out(0xFF)
#define LCD_DB_In()             PA_In(0xFF)
#define LCD_DB_Write(val, tmp)  PA_Write( (PA_Read() & ~0xFF) | (val & 0xFF) )
#define LCD_DB_Read(val)        val = PA_Read() & 0xFF

//[A7..A0] -> [DB7..DB0]
//[C2,C0]  -> [E (CLOCK), RW (WE)]
//[E12]    -> [ADDR26] -> [RS]
__STATIC_INLINE void InitializeLCDPorts()
{
	InitializePORTEx(PORT_A, 0xFF,    0, PN_PORTIO | LCD_PORT_OPTS_DATA);
	InitializePORTEx(PORT_C, PN2|PN0, 0, PN_PORTIO | LCD_PORT_OPTS_CTRL);
	InitializePORTEx(PORT_E, PN12,    0, PN_PORTIO | LCD_PORT_OPTS_ADDR);
}

#endif /* !__LCD_USE_EXT_BUS */

#endif /* !__TARGETLCD_H */

/*===========================================================================
 * End of file targetlcd.h
 *===========================================================================*/

