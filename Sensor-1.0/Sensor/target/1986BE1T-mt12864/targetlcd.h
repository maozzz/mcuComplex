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
 *  File targetlcd.h: LCD MT-12864A. MCU 1986BE1T evaluation board is used.
 *                    It's not recommended change any definitions at this file.
 *
 *===========================================================================*/

#ifndef __TARGETLCD_H
#define __TARGETLCD_H

//Select taget LCD (see LCD_x at "lib/hw/inc/LCD.h"):
#define TARGET_LCD		LCD_MT12864x

//Uncomment to enable control LCD within extension bus
#define __LCD_USE_EXT_BUS	1

//Uncomment to use poll busy flag instead of waiting cycle
//#define __LCD_USE_FLAG_BUSY   1


//LCD baseaddr
#define LCD_BASEADDR    0x60000000U

//LCD address mapping.
//E[14..12] => [region][ADDR28,ADDR27,ADDR26] =>
//          => [31..29][E2,    E1,A0][25,24]
//Command cr.1:    011, 0,     1, 0,  0, 0
#define LCD_CMD1        (LCD_BASEADDR|PN27)//0x68000000
//Command cr.2:    011, 1,     0, 0,  0, 0
#define LCD_CMD2        (LCD_BASEADDR|PN28)//0x70000000

//Data    cr.1:    011, 0,     1, 1,  0, 0
#define LCD_DATA1       (LCD_BASEADDR|PN27|PN26)//0x6C000000
//Data    cr.2:    011, 1,     0, 1,  0, 0
#define LCD_DATA2       (LCD_BASEADDR|PN28|PN26)//0x74000000


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
//[F0] -> [RESET]
#define LCD_RES_Act()         //PF_Reset(PN0)
#define LCD_RES_Off()         //PF_Set(PN0)


#ifdef __LCD_USE_EXT_BUS

//Read something to set E1, E2 to 0
#define LCD_CE_Act()            //nothing
#define LCD_CE_Off()            { u8 tmp = *((vu8*)LCD_BASEADDR); }


//[A7..A0]      -> [DB7..DB0]
//[C2,C0]       -> [E (CLOCK), RW (WE)] (different port functions)
//[E14,E13,E12] -> [ADDR28..ADDR26] -> [E2,E1,A0]
//[F0]          -> [RESET]
__STATIC_INLINE void InitializeLCDPorts()
{
	InitializePORTEx(PORT_A, 0xFF, 0, PN_BASIC | LCD_PORT_OPTS_DATA);
	InitializePORTEx(PORT_C, PN0,  0, PN_BASIC | LCD_PORT_OPTS_CTRL);
	InitializePORTEx(PORT_C, PN2,  0, PN_ALTER | LCD_PORT_OPTS_CTRL);
	InitializePORTEx(PORT_E, PN14|PN13|PN12, 0, PN_ALTER | LCD_PORT_OPTS_ADDR);

        //LCD MT-12864x doesn't have CE pin
	/*InitializePORTEx(PORT_x, PNi, 1, PN_PORTIO | LCD_PORT_OPTS_CTRL);*/

        //Initialize LCD reset pin (optional)
	//InitializePORTEx(PORT_F, PN0, 0, PN_PORTIO | LCD_PORT_OPTS_CTRL);
}

#else  /* __LCD_USE_EXT_BUS */

#define LCD_INV_CLKCYC			0.000125F

//Reset [E14, E13, E12] -> [E2, E1, A0] to 0
#define LCD_CE_Act()            //nothing
#define LCD_CE_Off()            PE_Reset((PN14|PN13|PN12))

//[C2] -> [E (CLOCK)] 1/0 - set/reset strobe
#define LCD_E_Act()             PC_Set(PN2)
#define LCD_E_Off()             PC_Reset(PN2)

//[C0] -> [RW (WE)] (0 - write, active; 1 - read)
#define LCD_WE_Act()            PC_Reset(PN0)
#define LCD_WE_Off()            PC_Set(PN0)

//Select E2, E1, A0
//E2, E1, A0 at same ports
//E[14..12] => [ADDR28..ADDR26] => [E2,E1,A0]
#define LCD_ADDR_Set(addr, tmp) PE_Write( (PE_Read() & ~(7 << 12)) |\
                                          ((addr & (PN28|PN27|PN26)) >> 14) )

//[A0..A7] -> [DB0..DB7]
#define LCD_DB_Out()            PA_Out(0xFF)
#define LCD_DB_In()             PA_In(0xFF)
#define LCD_DB_Write(val, tmp)  PA_Write( (PA_Read() & ~0xFF) | (val & 0xFF) )
#define LCD_DB_Read(val)        val = PA_Read() & 0xFF

//[A7..A0]      -> [DB7..DB0]
//[C2,C0]       -> [E (CLOCK), RW (WE)]
//[E14,E13,E12] -> [ADDR28..ADDR26] -> [E2,E1,A0]
//[F0]          -> [RESET]
__STATIC_INLINE void InitializeLCDPorts()
{
	InitializePORTEx(PORT_A, 0xFF,    0, PN_PORTIO | LCD_PORT_OPTS_DATA);
	InitializePORTEx(PORT_C, PN2|PN0, 0, PN_PORTIO | LCD_PORT_OPTS_CTRL);
	InitializePORTEx(PORT_E, PN14|PN13|PN12, 0, PN_PORTIO | LCD_PORT_OPTS_ADDR);

        //LCD MT-12864x doesn't have CE pin
	//InitializePORTEx(PORT_x, PNi, 1, PN_PORTIO | LCD_PORT_OPTS_CTRL);

        //Initialize LCD reset pin (optional)
	//InitializePORTEx(PORT_F, PN0, 0, PN_PORTIO | LCD_PORT_OPTS_CTRL);
}

#endif /* !__LCD_USE_EXT_BUS */

#endif /* !__TARGETLCD_H */

/*===========================================================================
 * End of file targetlcd.h
 *===========================================================================*/

