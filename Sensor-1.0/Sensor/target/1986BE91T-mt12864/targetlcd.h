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
 *  File targetlcd.h: LCD MT-12864J. MCU 1986BE91T evaluation board is used.
 *                    It's not recommended change any definitions at this file.
 *
 *===========================================================================*/

#ifndef __TARGETLCD_H
#define __TARGETLCD_H

//Select target LCD (see LCD_x at "lib/hw/inc/LCD.h")
#define TARGET_LCD      LCD_MT12864x

//Uncomment to enable control LCD within extension bus
//#define __LCD_USE_EXT_BUS   1

//Uncomment to use poll busy flag instead of waiting cycle
//#define __LCD_USE_FLAG_BUSY   1

#define LCD_BASEADDR	0x60000000

//LCD address mapping.
//E[11,5,4] => [region][ADDR27,ADDR21,ADDR20] =>
//          => [31..28][A0,    E2,    E1]
//Command cr.1:   0110, 0000,  0001
#define LCD_CMD1        (LCD_BASEADDR|PN20)
//Command cr.2:   0110, 0000,  0010
#define LCD_CMD2        (LCD_BASEADDR|PN21)

//Data    cr.1:   0110, 1000,  0001
#define LCD_DATA1       (LCD_BASEADDR|PN27|PN20)
//Data    cr.2:   0110, 1000,  0010
#define LCD_DATA2       (LCD_BASEADDR|PN27|PN21)

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
//[A7..A0]      -> [DB7..DB0]
//[C7, C2]      -> [E, RW]
//[E11, E5, E4] -> [A0, E2, E1]
//[C9]          -> [RESET]
__STATIC_INLINE void InitializeLCDPorts()
{
	InitializePORTEx(PORT_A, 0xFF,         0, PN_BASIC | LCD_PORT_OPTS_DATA);
	InitializePORTEx(PORT_C, PN7|PN2,      0, PN_BASIC | LCD_PORT_OPTS_CTRL);
	InitializePORTEx(PORT_E, PN11|PN5|PN4, 0, PN_BASIC | LCD_PORT_OPTS_ADDR);
		
	//LCD MT-12864x doesn't have CE pin
	//InitializePORTEx(PORT_x, PNi, 1, PN_PORTIO | LCD_PORT_OPTS_CTRL);
		
	//Initialize LCD reset pin (optional)
	//InitializePORTEx(PORT_C, PN9, 0, PN_PORTIO | LCD_PORT_OPTS_CTRL);
}

#else  /* __LCD_USE_EXT_BUS */

#define LCD_INV_CLKCYC			0.000160F

//Reset [E11, E5, E4] -> [A0, E2, E1] to 0
#define LCD_CE_Act()			//nothing
#define LCD_CE_Off()			PE_Reset((PN11|PN5|PN4))

//[C7] -> [E (CLOCK)] 1/0 - set/reset strobe
#define LCD_E_Act()             PC_Set(PN7)
#define LCD_E_Off()             PC_Reset(PN7)

//[C2] -> [RW (WE)] (0 - write, active; 1 - read)
#define LCD_WE_Act()            PC_Reset(PN2)
#define LCD_WE_Off()            PC_Set(PN2)


//Select E2, E1, A0
//E2, E1, A0 at same ports
//E[11,5,4] -> [ADDR27,ADDR21,ADDR20] -> [A0,E2,E1]
#define LCD_ADDR_Set(addr, tmp) PE_Write( (PE_Read() & ~(PN11|PN5|PN4)) |\
                                          ((addr & (PN27|PN21|PN20)) >> 16) )


//[A0..A7] -> [DB0..DB7]
#define LCD_DB_Out()            PA_Out(0xFF)
#define LCD_DB_In()             PA_In(0xFF)
#define LCD_DB_Write(val, tmp)  PA_Write( (PA_Read() & ~0xFF) | (val & 0xFF) )
#define LCD_DB_Read(val)        val = PA_Read() & 0xFF

//Ports initialization
//[A7..A0]      -> [DB7..DB0]
//[C7, C2]      -> [E, RW]
//[E11, E5, E4] -> [A0, E2, E1]
//[C9]          -> [RESET]
__STATIC_INLINE void InitializeLCDPorts()
{
	InitializePORTEx(PORT_A, 0xFF,         0, PN_PORTIO | LCD_PORT_OPTS_DATA);
	InitializePORTEx(PORT_C, PN7|PN2,      0, PN_PORTIO | LCD_PORT_OPTS_CTRL);
	InitializePORTEx(PORT_E, PN11|PN5|PN4, 0, PN_PORTIO | LCD_PORT_OPTS_ADDR);
		
	//LCD MT-12864x doesn't have CE pin
	//InitializePORTEx(PORT_x, PNi, 1, PN_PORTIO | LCD_PORT_OPTS_CTRL);
		
	//Initialize LCD reset pin (optional)
	//InitializePORTEx(PORT_C, PN9, PN9, PN_PORTIO | LCD_PORT_OPTS_CTRL);
}

#endif /* !__LCD_USE_EXT_BUS */

#endif /* __TARGETLCD_H */

/*===========================================================================
 * End of file targetlcd.h
 *===========================================================================*/

