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
 *  File 1986BE9x_EEPROM.c: EEPROM functions.
 *
 *===========================================================================*/

#include "../../inc/EEPROM.h"

#if defined(__CC_ARM)
#define __EEPROM_FATTR2      __attribute__((section("EXECUTABLE_MEMORY_SECTION"), weak))

#elif defined(__ICCARM__)
#define __EEPROM_FATTR2      __ramfunc
//#define __EEPROM_FATTR2    IAR_SECTION("EXECUTABLE_MEMORY_SECTION")

#elif defined (__CMCARM__)
#define __EEPROM_FATTR2      __ramfunc

#elif defined(__GNUC__)
#define __EEPROM_FATTR2	    __attribute__((section(".ramfunc"), weak))

#else
#define __EEPROM_FATTR2

#endif

//EEPROM KEY
#define EEPROM_KEY_VALUE	0x8AAA5551

//EEPROM registers
#define EEPROM_CMD			(*(__io_reg *)(BASE_EEPROM + 0x00))
#define EEPROM_ADR			(*(__io_reg *)(BASE_EEPROM + 0x04))
#define EEPROM_DI			(*(__io_reg *)(BASE_EEPROM + 0x08))
#define EEPROM_DO			(*(__io_reg *)(BASE_EEPROM + 0x0C))
#define EEPROM_KEY			(*(__io_reg *)(BASE_EEPROM + 0x10))

//EEPROM_CMD flags: Delay[5..3] is not defined here
#define CON					0x0001
#define WR					0x0002
#define RD					0x0004
#define XE					0x0040
#define YE					0x0080
#define SE					0x0100
#define IFREN				0x0200
#define ERASE				0x0400
#define MAS1				0x0800
#define PROG				0x1000
#define NVSTR				0x2000

//Delay[5..3] mask
#define DELAY_MASK			0x0038

//Disable all IRQs
#define DISABLE_ALL_IRQs()	__disable_irq(); ram_EEPROMWaitInterval(100)

//Enable all IRQs
#define ENABLE_ALL_IRQs()	ram_EEPROMWaitInterval(100); __enable_irq()


//Switch EEPROM to programming mode
//IFREN = 0/1 - code/data eeprom
#define BEGIN_PROG_MODE() \
  EEPROM_KEY = EEPROM_KEY_VALUE;\
  cmd = (EEPROM_CMD & DELAY_MASK) | CON;\
  if (eeprom == EEPROM_CODE) { baseAddr = EEPROM_CODE_BASE; }\
  else                       { baseAddr = EEPROM_DATA_BASE; cmd |= IFREN; }\
  EEPROM_CMD = cmd;

//End of programming mode
#define END_PROG_MODE() \
  cmd &= DELAY_MASK;\
  EEPROM_CMD = cmd;\
  EEPROM_KEY = 0;


//
static int usecScaleValue = 1;

/* */
__EEPROM_FATTR2 void ram_EEPROMWaitInterval(int usec)
{
    usec *= usecScaleValue;

    do
    {
		usec++;
		usec -= 2;
    }
    while (usec != 0);
}


 
/* Initialize EEPROM */
MCRESULT InitializeEEPROM(const void *cfg)
{    
    ClkEnable(CLK_EEPROM);

    usecScaleValue = (int)(GetHCLK() / FREQ_HSI);

    if (!usecScaleValue)
	usecScaleValue = 1;

    return MCR_OK;
}



/* Erase full eeprom (RAM function) */
__EEPROM_FATTR s32 ram_EEPROMEraseChip(u8 eeprom)
{
    u32 cmd, sector, baseAddr;

    BEGIN_PROG_MODE()

    //Erase full eeprom: erase A,B,C,D sectors
    for (sector = 0; sector < 16; sector += 4)
    {
	    //ADR[3..2] - sector address [0..3]
	    EEPROM_ADR = baseAddr + sector;

	    EEPROM_CMD &= ~(XE|YE|SE|MAS1|ERASE|NVSTR);

	    EEPROM_CMD |= XE|MAS1|ERASE;
		ram_EEPROMWaitInterval(6);     //~6us

	    EEPROM_CMD |= NVSTR;
		ram_EEPROMWaitInterval(50000); //~50ms

	    EEPROM_CMD &= ~ERASE;
		ram_EEPROMWaitInterval(110);   //~110us

	    EEPROM_CMD &= ~(XE|MAS1|NVSTR);
		ram_EEPROMWaitInterval(2);     //~2us
    }

    END_PROG_MODE()

    return 0;
}



/* Erase selected page (RAM function) */
__EEPROM_FATTR s32 ram_EEPROMErasePage(u8 eeprom, u32 pgaddr)
{
    u32 cmd, sector, baseAddr;

    BEGIN_PROG_MODE()

    if (pgaddr < baseAddr)
		pgaddr = baseAddr;

    //Erase selected page: erase A,B,C,D sectors
    for (sector = 0; sector < 16; sector += 4)
    {
	    //ADR[16..12] - page address [0..31]
	    //ADR[3..2]   - sector address [0..3]
	    EEPROM_ADR = pgaddr + sector;

	    EEPROM_CMD &= ~(XE|YE|SE|MAS1|ERASE|NVSTR);

	    EEPROM_CMD |= XE|ERASE;
		ram_EEPROMWaitInterval(6);     //~6us

	    EEPROM_CMD |= NVSTR;
		ram_EEPROMWaitInterval(50000); //~50ms

	    EEPROM_CMD &= ~ERASE;
		ram_EEPROMWaitInterval(6);   //~6us

	    EEPROM_CMD &= ~(XE|NVSTR);
		ram_EEPROMWaitInterval(2);     //~2us
    }

    END_PROG_MODE()

    return 0;
}



/* Write data to EEPROM (RAM function) */
__EEPROM_FATTR s32 ram_EEPROMWrite(u8 eeprom, u32 daddr, const void *src, s32 count)
{
    u32 cmd, sector, baseAddr;
    u32 udata, srcOffset, countUnBytes, bcnt;
    u8 *psrc;

    if (!count)
		return 0;

    BEGIN_PROG_MODE()

    if (daddr < baseAddr)
		daddr = baseAddr;

    //Address must be aligned by 4
    daddr &= ~(0x03U);

    //Get pointer to data
    psrc = (u8*)src;

    //Count of unaligned bytes
    countUnBytes = (count & 0x03);

    //For each sector: [A,B,C,D]
    sector = 0;
	udata  = 0;
    while(sector < (u32)count)
    {
	    EEPROM_ADR = daddr + sector;
	    EEPROM_CMD |= XE|PROG;
		ram_EEPROMWaitInterval(6);  //~6us

	    EEPROM_CMD |= NVSTR;
		ram_EEPROMWaitInterval(11); //~11us

	    //Write current sector
	    srcOffset = sector;
	    while(srcOffset < (u32)count)
	    {
		    if ((srcOffset + countUnBytes) == (u32)count)
		    {
				udata = 0xFFFFFFFFU;
				bcnt = countUnBytes;
		    }
		    else
				bcnt = 4;

		    //Get 32-bit word from source buffer
		    while(bcnt--)
				udata = (udata << 8) | ((u32)psrc[srcOffset + bcnt]);

		    EEPROM_ADR = daddr + srcOffset;
		    EEPROM_DI  = udata;

		    EEPROM_CMD |= YE;
			ram_EEPROMWaitInterval(50); //~50us

		    EEPROM_CMD &= ~YE;
			ram_EEPROMWaitInterval(1);  //~1us

		    //Next address at current sector:
		    //+16 bytes (+4 32-bit words)
		    srcOffset += 16;
	    }

	    EEPROM_CMD &= ~PROG;
		ram_EEPROMWaitInterval(6); //~6us

	    EEPROM_CMD &= ~(XE|NVSTR);
		ram_EEPROMWaitInterval(1); //~1us

	    //Next sector address: +4 bytes (+1 32-bit word)
	    sector += 4;
    }

    END_PROG_MODE()

    return count;
}



/* Read data from EEPROM (RAM function) */
__EEPROM_FATTR s32 ram_EEPROMRead(u8 eeprom, u32 saddr, void *dest, s32 count)
{
    u32 cmd, baseAddr;
    u32 udata, destOffset, countUnBytes, bcnt;
    u8 *pdest;

    if (!count)
		return 0;

	BEGIN_PROG_MODE()

    //Check source address
    if (saddr < baseAddr)
		saddr = baseAddr;

    //Address must be aligned by 4
    saddr &= ~(0x03U);

    //Get pointer to data
    pdest = (u8*)dest;

    //Count of unaligned bytes
    countUnBytes = (count & 0x03);

    destOffset = 0;
    while(destOffset < (u32)count)
    {
	    EEPROM_ADR = saddr + destOffset;

	    EEPROM_CMD |= XE|YE|SE;
		ram_EEPROMWaitInterval(1);

	    udata = EEPROM_DO;

	    EEPROM_CMD &= ~(XE|YE|SE);
		ram_EEPROMWaitInterval(1);

	    bcnt = ((destOffset + countUnBytes) == (u32)count) ? countUnBytes : 4;

	    //Put 32-bit word to destination buffer
	    while(bcnt--)
	    {
			pdest[destOffset++] = (u8)(udata & 0xFF);
			udata >>= 8;
	    }
    }

	END_PROG_MODE()

    return count;
}



/* Erase full EEPROM */
s32 EEPROMEraseChip(u8 eeprom)
{
    s32 r;
    DISABLE_ALL_IRQs();
    r = ram_EEPROMEraseChip(eeprom);
    ENABLE_ALL_IRQs();
    return r;
}



/* Erase selected page */
s32 EEPROMErasePage(u8 eeprom, u32 pgaddr)
{
    s32 r;
    DISABLE_ALL_IRQs();
	r = ram_EEPROMErasePage(eeprom, pgaddr);
    ENABLE_ALL_IRQs();
    return r;
}



/* Write data to EEPROM */
s32 EEPROMWrite(u8 eeprom, u32 daddr, const void *src, s32 count)
{
    s32 r;
    DISABLE_ALL_IRQs();
    r = ram_EEPROMWrite(eeprom, daddr, src, count);
    ENABLE_ALL_IRQs();
    return r;
}



/* Read data from EEPROM */
s32 EEPROMRead(u8 eeprom, u32 saddr, void *dest, s32 count)
{
    s32 r;
    DISABLE_ALL_IRQs();
    r = ram_EEPROMRead(eeprom, saddr, dest, count);
    ENABLE_ALL_IRQs();
    return r;
}

/*===========================================================================
 * End of file 1986BE9x_EEPROM.c
 *===========================================================================*/
