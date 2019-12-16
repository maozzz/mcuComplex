/*===========================================================================
 *
 *  Hardware library
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
 *  File targetlcd.h: LCD PC2004A. 8-bit interface.
 *                    MCU 1986BE91T evaluation board is used.
 *                    It's not recommended change any definitions at this file.
 *
 *===========================================================================*/

#ifndef __TARGETLCD_H
#define __TARGETLCD_H

//Select target LCD (see LCD_x at "lib/hw/inc/LCD.h")
#define TARGET_LCD      LCD_PC2004A

//Uncomment to enable control LCD within extension bus
//#define __LCD_USE_EXT_BUS   1

//Uncomment to use poll busy flag instead of waiting cycle
//#define __LCD_USE_FLAG_BUSY   1

#define LCD_BASEADDR	0x60000000

//LCD address mapping.
//E[11,5,4] => [region][ADDR27] =>
//          => [31..28][RS]
//Command: RS=0
#define LCD_CMD1        (LCD_BASEADDR)

//Data: RS=1
#define LCD_DATA1       (LCD_BASEADDR|PN27)

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
//[C9] -> [RESET]
#define LCD_RES_Act()         //PC_Reset(PN9)
#define LCD_RES_Off()         //PC_Set(PN9)							 
							 

#ifdef __LCD_USE_EXT_BUS

//Read something to set E1, E2 to 0
#define LCD_CE_Act()			//nothing
#define LCD_CE_Off()			{ u8 tmp = *((vu8*)LCD_BASEADDR); }

//Ports initialization
//[A7..A0] -> [DB7..DB0]
//[C7, C2] -> [E, RW]
//[E11]    -> [RS]
__STATIC_INLINE void InitializeLCDPorts()
{
    InitializePORTEx(PORT_A, 0xFF,    0, PN_BASIC | LCD_PORT_OPTS_DATA);
    InitializePORTEx(PORT_C, PN7|PN2, 0, PN_BASIC | LCD_PORT_OPTS_CTRL);
    InitializePORTEx(PORT_E, PN11,    0, PN_BASIC | LCD_PORT_OPTS_ADDR);
}

#else  /* __LCD_USE_EXT_BUS */

#define LCD_INV_CLKCYC			0.000160F

//Reset [E11] -> [RS] to 0
#define LCD_CE_Act()			//nothing
#define LCD_CE_Off()			PE_Reset((PN11))

//[C7] -> [E (CLOCK)] 1/0 - set/reset strobe
#define LCD_E_Act()             PC_Set(PN7)
#define LCD_E_Off()             PC_Reset(PN7)

//[C2] -> [RW (WE)] (0 - write, active; 1 - read)
#define LCD_WE_Act()            PC_Reset(PN2)
#define LCD_WE_Off()            PC_Set(PN2)


//Select RS
//E[11] -> [ADDR27] -> [RS]
#define LCD_ADDR_Set(addr, tmp) PE_Write( (PE_Read() & ~(PN11)) |\
                                          ((addr & (PN27)) >> 16) )


//[A0..A7] -> [DB0..DB7]
#define LCD_DB_Out()            PA_Out(0xFF)
#define LCD_DB_In()             PA_In(0xFF)
#define LCD_DB_Write(val, tmp)  PA_Write( (PA_Read() & ~0xFF) | (val & 0xFF) )
#define LCD_DB_Read(val)        val = PA_Read() & 0xFF

//Ports initialization
//[A7..A0] -> [DB7..DB0]
//[C7, C2] -> [E, RW]
//[E11]    -> [RS]
__STATIC_INLINE void InitializeLCDPorts()
{
    InitializePORTEx(PORT_A, 0xFF,    0, PN_PORTIO | LCD_PORT_OPTS_DATA);
    InitializePORTEx(PORT_C, PN7|PN2, 0, PN_PORTIO | LCD_PORT_OPTS_CTRL);
    InitializePORTEx(PORT_E, PN11,    0, PN_PORTIO | LCD_PORT_OPTS_ADDR);
}

#endif /* !__LCD_USE_EXT_BUS */

#endif /* __TARGETLCD_H */

/*===========================================================================
 * End of file targetlcd.h
 *===========================================================================*/

