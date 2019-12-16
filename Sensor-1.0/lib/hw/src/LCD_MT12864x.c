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
 *  File LCD_MT12864x.c: LCD MT-12864x functions.
 *                       Vdd=3.3V;
 *
 *===========================================================================*/

#include "../inc/LCD.h"

#include <string.h>

//LCD wait counters
//("__LCD_USE_EXT_BUS" must be defined at "targetlcd.h")
#ifdef __LCD_USE_EXT_BUS

//LCD_INV_CLKCYC = 1 / (clkCyc * 1000)
#ifndef LCD_INV_CLKCYC
#if defined(__CMCARM__)
#define LCD_INV_CLKCYC      0.000180F
#else
#define LCD_INV_CLKCYC      0.000250F
#endif /* !__CMCARM__ */
#endif /* !LCD_INV_CLKCYC */

#else /* __LCD_USE_EXT_BUS */

//LCD_INV_CLKCYC = 1 / (clkCyc * 1000)
#ifndef LCD_INV_CLKCYC
#define LCD_INV_CLKCYC      0.000125F
#endif /* !LCD_INV_CLKCYC */

static u32 LCD_tas      = 0;
static u32 LCD_tah      = 0;
static u32 LCD_PWeh     = 0;

#endif /* !__LCD_USE_EXT_BUS */

//Port IO counters
static u32 LCD_Pause    = 0;
static u32 LCD_ResetAct = 0;
static u32 LCD_ResetRdy = 0;


//Wait specified interval
#define WaitInterval(cnt)   cycCnt = (cnt); while(cycCnt--);

/* */
static u32 LCDGetCounter(u32 tmNs, u32 hclkMHz)
{
        //tm = cycCnt * clkCyc / hclk
        //cycCnt = tm * hclk / clkCyc
        //cycCnt = tmNs * 10^-9 * hclkMHz * 10^6 / clkCyc;
        //cycCnt = tmNs * hclkMHz / (clkCyc * 1000);
		return (u32)(tmNs * hclkMHz * LCD_INV_CLKCYC);
}


/* */
void LCDSetHCLK(u32 hclk)
{
        //Get hclk at MHz
        u32 hclkMHz = (u32)(hclk / 1000000);

        #ifdef __LCD_USE_EXT_BUS

        u32 wsSetup, wsActive, wsHold;

        wsSetup  = CalcClocks(LCD_tas_ns,  hclkMHz);
        wsActive = CalcClocks(LCD_PWeh_ns, hclkMHz);
        wsHold   = CalcClocks(LCD_tah_ns,  hclkMHz);

        //Setup extbus clocks
        EBusSetClockEx(LCD_CMD1, wsSetup, wsActive, wsHold);
        EBusSetClockEx(LCD_CMD2, wsSetup, wsActive, wsHold);

        EBusSetClockEx(LCD_DATA1, wsSetup, wsActive, wsHold);
        EBusSetClockEx(LCD_DATA2, wsSetup, wsActive, wsHold);

        //Use LCD as RAM
        EBusSelectMemory(EXTBUS_RAM);

        #else /* __LCD_USE_EXT_BUS */

        //Port I/O counters calculation
        LCD_tas      = LCDGetCounter(LCD_tas_ns, hclkMHz);
        LCD_PWeh     = LCDGetCounter(LCD_PWeh_ns, hclkMHz);
        LCD_tah      = LCDGetCounter(LCD_tah_ns, hclkMHz);

        #endif /* !__LCD_USE_EXT_BUS */


        //Common counters calculation
        LCD_Pause    = LCDGetCounter(LCD_Pause_ns, hclkMHz);
        LCD_ResetAct = LCDGetCounter(LCD_ResetAct_ns, hclkMHz);
        LCD_ResetRdy = LCDGetCounter(LCD_ResetRdy_ns, hclkMHz);
}




/* LCD initialization */
void InitializeLCD(u32 hclk)
{
        //This function must be defined at "targetlcd.h"
        InitializeLCDPorts();

        LCDSetHCLK(hclk);

        LCD_CE_Off();
}




#ifdef __LCD_USE_EXT_BUS

/* Write address using external bus */
#define WriteAddr(address, data)            *((vu8*)(address)) = (u8)(data)

/* Read address using external bus */
#define ReadAddr(address)                   *((vu8*)(address))

#else /* __LCD_USE_EXT_BUS */

/* Write data to LCD without pause */
static void WriteAddr(u32 addr, u8 value)
{
        u32 cycCnt;
        u16 vtmp;

        //E (CLOCK) = 0: off
        LCD_E_Off();

        //Set address
        LCD_ADDR_Set(addr, vtmp);

        //RW (WE) = 0: enable write
        LCD_WE_Act();

        //Turn data port to output
        LCD_DB_Out();

        //Write data[7..0]
        LCD_DB_Write(value, vtmp);

        //Wait tas
        WaitInterval(LCD_tas);

        //E (CLOCK) = 1: active
        LCD_E_Act();

        //Wait PWeh
        WaitInterval(LCD_PWeh);

        //E (CLOCK) = 0: off
        LCD_E_Off();

        //Wait tah
        WaitInterval(LCD_tah);

        //RW (WE) = 1: disable write
        LCD_WE_Off();

        //Turn data port to input
        LCD_DB_In();
}


/* Read data from LCD without pause */
static u8 ReadAddr(u32 addr)
{
        u32 cycCnt;
        u16 vtmp;
        u16 value;

        //E (CLOCK) = 0: off
        LCD_E_Off();

        //Set address
        LCD_ADDR_Set(addr, vtmp);

        //Turn data port to input
        LCD_DB_In();

        //RW (WE) = 1: enable read
        LCD_WE_Off();

        //Wait tas
        WaitInterval(LCD_tas);

        //E (CLOCK) = 1: on
        LCD_E_Act();

        //Wait PWeh
        WaitInterval(LCD_PWeh);

        //Read data[7..0]
        LCD_DB_Read(value);

        //E (CLOCK) = 0: off
        LCD_E_Off();

        //Wait tah
        WaitInterval(LCD_tah);

        return (u8)value;
}

#endif /* !__LCD_USE_EXT_BUS */


#ifdef __LCD_USE_FLAG_BUSY

__STATIC_INLINE void WaitReady()
{
	while((ReadAddr(LCD_CMD1) & LCD_FLAG_BUSY) ||
		  (ReadAddr(LCD_CMD2) & LCD_FLAG_BUSY));
}

#else /* __LCD_USE_FLAG_BUSY */

//#define WaitReady()     WaitInterval(LCD_Pause)
__STATIC_INLINE void WaitReady()
{
	u32 cycCnt;
	WaitInterval(LCD_Pause);	
}

#endif /* !__LCD_USE_FLAG_BUSY */




/* */
void LCDSetStartLine(u32 line)
{        
        u8 cmd = LCD_CMD_STARTLINE | line;

        WriteAddr(LCD_CMD1, cmd);
        WriteAddr(LCD_CMD2, cmd);
        WaitReady();
}



/* */
void LCDReset(u32 addr)
{
        u32 cycCnt;

        //Set RESET to active state
        LCD_RES_Act();
        //Wait active interval
        WaitInterval(LCD_ResetAct);

        //Set RESET to inactive state
        LCD_RES_Off();
        //Wait wait inactive interval
        WaitInterval(LCD_ResetRdy);

        //Set 0 start line
        LCDSetStartLine(0);
}



/* */
void TurnLCD(u8 turn)
{        
        u8 cmd = turn ? LCD_CMD_ON : LCD_CMD_OFF;

        WriteAddr(LCD_CMD1, cmd);
        WriteAddr(LCD_CMD2, cmd);
        WaitReady();
}



/* Select I2C address */
void LCDSelectI2CAddress(u8 i2caddr)
{
    //Not I2C interface: no actions
}



/* Write page to LCD using specified starting address and length */
s32 LCDWritePage(u32 page, u32 addr, u8 *data, s32 length)
{
		s32 celli = 0;

        if (!data)
                return 0;

        page = LCD_CMD_SETPAGE | page;

        if (addr < LCD_CR_WIDTH &&
            addr + length > LCD_CR_WIDTH)
        {
                //Data at first AND second crystal
				u32 addr2;
				s32 offset2, length2, count;
                u8 *data2;

                //Select page
                WriteAddr(LCD_CMD1, page);
                WriteAddr(LCD_CMD2, page);
                WaitReady();

                //Select address on page
                WriteAddr(LCD_CMD1, LCD_CMD_SETADDR | addr);
                WriteAddr(LCD_CMD2, LCD_CMD_SETADDR);
                WaitReady();

                addr2 = 0;
                offset2 = LCD_CR_WIDTH - addr;
                length2 = addr + length - LCD_CR_WIDTH;

                data2 = &data[offset2];

                count = offset2 > length2 ?
                        offset2 : length2;

                while(celli < count)
                {
                        //Write to first crystal
                        if (addr < LCD_CR_WIDTH)
                        {
                                WriteAddr(LCD_DATA1, data[celli]);
                                addr++;
                        }

                        //Write to second crystal
                        if (addr2 < LCD_CR_WIDTH &&
                              celli < length2)
                        {
                                WriteAddr(LCD_DATA2, data2[celli]);
                                addr2++;
                        }

                        //Wait pause between operations
                        WaitReady();

                        celli++;
                }

                return LCD_CR_WIDTH + addr2 - addr;
        }
        else
        {
                //Data at first OR second crystal
				u32 cmdAddr, dataAddr;

                if (addr < LCD_CR_WIDTH)
                {
                        cmdAddr = LCD_CMD1;
                        dataAddr = LCD_DATA1;
                }
                else
                {
                        cmdAddr = LCD_CMD2;
                        dataAddr = LCD_DATA2;
                        addr -= LCD_CR_WIDTH;
                }

                WriteAddr(cmdAddr, page);
                WaitReady();

                WriteAddr(cmdAddr, LCD_CMD_SETADDR | addr);
                WaitReady();

                while(celli < length &&
                       addr < LCD_CR_WIDTH)
                {
                        WriteAddr(dataAddr, data[celli++]);
                        addr++;
                        WaitReady();
                }

                return celli;
        }
}



/* Read page from LCD using specified starting address and length */
s32 LCDReadPage(u32 page, u32 addr, u8 *data, s32 length)
{
		s32 celli = 0;

        if (!data)
                return 0;

        page = LCD_CMD_SETPAGE | page;

        if (addr < LCD_CR_WIDTH &&
            addr + length > LCD_CR_WIDTH)
        {
                //Data at first AND second crystal
				u32 addr2;
				s32 offset2, length2, count;
				u8 *data2, tmp;

                WriteAddr(LCD_CMD1, page);
                WriteAddr(LCD_CMD2, page);
                WaitReady();

                WriteAddr(LCD_CMD1, LCD_CMD_SETADDR | addr);
                WriteAddr(LCD_CMD2, LCD_CMD_SETADDR);
                WaitReady();

                //Additional reading to get valid data at cycle
                tmp = ReadAddr(LCD_DATA1);
                tmp = ReadAddr(LCD_DATA2);
                WaitReady();

                addr2 = 0;
                offset2 = LCD_CR_WIDTH - addr;
                length2 = addr + length - LCD_CR_WIDTH;

                data2 = &data[offset2];

                count = offset2 > length2 ?
                        offset2 : length2;

                while(celli < count)
                {
                        //Read from first crystal
                        if (addr < LCD_CR_WIDTH)
                        {
                                data[celli] = ReadAddr(LCD_DATA1);
                                addr++;
                        }

                        //Read from second crystal
                        if (addr2 < LCD_CR_WIDTH &&
                            celli < length2)
                        {
                                data2[celli] = ReadAddr(LCD_DATA2);
                                addr2++;
                        }

                        //Wait pause between operations
                        WaitReady();

                        celli++;
                }

                return LCD_CR_WIDTH + addr2 - addr;
        }
        else
        {
                //Data at first OR second crystal
				u32 cmdAddr, dataAddr;
                u8 tmp;

                if (addr < LCD_CR_WIDTH)
                {
                        cmdAddr = LCD_CMD1;
                        dataAddr = LCD_DATA1;
                }
                else
                {
                        cmdAddr = LCD_CMD2;
                        dataAddr = LCD_DATA2;
                        addr -= LCD_CR_WIDTH;
                }

                //Select page
                WriteAddr(cmdAddr, page);
                WaitReady();

                //Select addres on page
                WriteAddr(cmdAddr, LCD_CMD_SETADDR | addr);
                WaitReady();

                //Additional reading to get valid data at cycle
                tmp = ReadAddr(dataAddr);
                WaitReady();

                while(celli < length &&
                       addr < LCD_CR_WIDTH)
                {
                        data[celli++] = ReadAddr(dataAddr);
                        addr++;
                        WaitReady();
                }

                return celli;
        }
}




/* Write LCD cell */
s32 LCDWriteCell(u32 addr, u32 value)
{        
        u32 cmdAddr, dataAddr;
        u8 page, cellAddr;

        if (addr < LCD_BASEADDR)
                addr = LCD_BASEADDR;

        addr = addr - LCD_BASEADDR;

        #if (LCD_WIDTH == 128)

        page = (u8)(addr >> 7);
        cellAddr = (addr & 0x7F);

        #else

        page = (u8)(addr / LCD_WIDTH);
        cellAddr = addr % LCD_WIDTH;

        #endif /* LCD_WIDTH != 128 */

        //Check page index
        if (page >= LCD_PAGECNT)
                return 0;

        //Select crystal
        if (cellAddr < LCD_CR_WIDTH)
        {
                cmdAddr = LCD_CMD1;
                dataAddr = LCD_DATA1;
        }
        else
        {
                cellAddr -= LCD_CR_WIDTH;
                cmdAddr = LCD_CMD2;
                dataAddr = LCD_DATA2;
        }

        //Select page
        WriteAddr(cmdAddr, LCD_CMD_SETPAGE | page);
        WaitReady();

        //Select address on page
        WriteAddr(cmdAddr, LCD_CMD_SETADDR | cellAddr);
        WaitReady();

        //Write cell
        WriteAddr(dataAddr, (u8)value);
        WaitReady();

        return 1;
}



/* Read LCD cell */
u32 LCDReadCell(u32 addr)
{        
        u32 cmdAddr, dataAddr, value;
        u8 page, cellAddr;

        if (addr < LCD_BASEADDR)
                addr = LCD_BASEADDR;

        addr = addr - LCD_BASEADDR;

        #if (LCD_WIDTH == 128)

        page = (u8)(addr >> 7);
        cellAddr = (addr & 0x7F);

        #else /* LCD_WIDTH != 128 */

        page = (u8)(addr / LCD_WIDTH);
        cellAddr = addr % LCD_WIDTH;

        #endif

        //Check page index
        if (page >= LCD_PAGECNT)
                return 0;

        //Select crystal
        if (cellAddr < LCD_CR_WIDTH)
        {
                cmdAddr = LCD_CMD1;
                dataAddr = LCD_DATA1;
        }
        else
        {
                cellAddr -= LCD_CR_WIDTH;
                cmdAddr = LCD_CMD2;
                dataAddr = LCD_DATA2;
        }

        //Select page
        WriteAddr(cmdAddr, LCD_CMD_SETPAGE | page);
        WaitReady();

        //Select address on page
        WriteAddr(cmdAddr, LCD_CMD_SETADDR | cellAddr);
        WaitReady();

        //Read cell twice to get valid value
        value = (u8)ReadAddr(dataAddr);
        WaitReady();

        value = (u8)ReadAddr(dataAddr);
        WaitReady();

        return value;
}



/* */
s32 LCDWrite(u32 addr, u8 *data, s32 length)
{
        u32 page, cellAddr;
        s32 wrCount = 0;

        if (addr < LCD_BASEADDR)
                addr = LCD_BASEADDR;

        addr = addr - LCD_BASEADDR;

        //Start page and start address on page
        #if (LCD_WIDTH == 128)

        page     = (u32)(addr >> 7);
        cellAddr = addr % 0x7F;

        #else

        page     = (u32)(addr / LCD_WIDTH);
        cellAddr = addr % LCD_WIDTH;

        #endif /* LCD_WIDTH != 128 */

        while(page < LCD_PAGECNT &&
              length - wrCount > 0)
        {
                wrCount += LCDWritePage(page, cellAddr,
                                        &data[wrCount],
                                        length - wrCount);

                page++;
                cellAddr = 0;
        }

        return wrCount;
}



/* */
s32 LCDRead(u32 addr, u8 *data, s32 length)
{
        u32 page, cellAddr;
        s32 rdCount = 0;

        if (addr < LCD_BASEADDR)
                addr = LCD_BASEADDR;

        addr = addr - LCD_BASEADDR;

        //Start page and start address on page
        #if (LCD_WIDTH == 128)

        page     = (u32)(addr >> 7);
        cellAddr = addr % 0x7F;

        #else

        page     = (u32)(addr / LCD_WIDTH);
        cellAddr = addr % LCD_WIDTH;

        #endif /* LCD_WIDTH != 128 */

        while(page < LCD_PAGECNT &&
              length - rdCount > 0)
        {
                rdCount += LCDReadPage(page, cellAddr,
                                        &data[rdCount],
                                        length - rdCount);

                page++;
                cellAddr = 0;
        }

        return rdCount;
}



/* Clear all crystals on LCD */
void LCDClear(u32 addr)
{
		u32 pgIndex, celli;

        //For each page
        pgIndex = 0;
        while(pgIndex < LCD_PAGECNT)
        {
                //Select page
                WriteAddr(LCD_CMD1, (u8)(LCD_CMD_SETPAGE | pgIndex));
                WriteAddr(LCD_CMD2, (u8)(LCD_CMD_SETPAGE | pgIndex));
                WaitReady();

                //Set address at current page to 0
                WriteAddr(LCD_CMD1, LCD_CMD_SETADDR);
                WriteAddr(LCD_CMD2, LCD_CMD_SETADDR);
                WaitReady();

                pgIndex++;

                //For each cell at page
                //Write zeros to first and second crystals
                celli = 0;
                while(celli++ < LCD_CR_WIDTH)
                {
                        WriteAddr(LCD_DATA1, 0x00);
                        WriteAddr(LCD_DATA2, 0x00);
                        WaitReady();
                }
        }


        //Select 0 page
        WriteAddr(LCD_CMD1, LCD_CMD_SETPAGE);
        WriteAddr(LCD_CMD2, LCD_CMD_SETPAGE);
        WaitReady();

        //Set 0 address
        WriteAddr(LCD_CMD1, LCD_CMD_SETADDR);
        WriteAddr(LCD_CMD2, LCD_CMD_SETADDR);
        WaitReady();
}


/* */
u32 GetPixel1b(u8 *buffer, s32 width, s32 x, s32 y);

/* */
void DrawChar1b(u32 charIndex,
                u8 *buffer, s32 width, s32 x, s32 y,
                FONT *font, u32 fColorIndex, u32 bColorIndex);

/* Draw text on LCD using specified parameters */
void LCDDrawText(ScreenParams *scp)
{
        //Buffer size: 2 LCD pages
        u8 pageBuf0[(LCD_WIDTH << 1)];
        u8 *pageBuf1;
        u8 *buf;
        s32 xpix, ypix;
        u32 fColorIndex, bColorIndex;
        u16 fontW, fontH;
        s16 xchar, ychar, xcharMax, ycharMax;
        u8 bch;

        if (!scp->font || scp->font->height > 16)
                return;

        //Clear buffer
        memset(pageBuf0, 0, (LCD_WIDTH << 1));

        fontW = scp->font->width;
        fontH = scp->font->height;

        //Get pointer to 2nd page
        pageBuf1 = &pageBuf0[LCD_WIDTH];

        xcharMax = (s16)(LCD_WIDTH  / fontW);
        ycharMax = (s16)(LCD_HEIGHT / fontH);

        if (xcharMax > scp->width)
            xcharMax = scp->width;

        if (ycharMax > scp->height)
            ycharMax = scp->height;

        //For each text line
        //xchar, ychar - char coordinates at text ScreenParams::buffer
        //xpix, ypix - pixel coordinates at pageBuf
        //Reset Y-pixel position
        ypix = 0;
        for(ychar = 0; ychar < ycharMax; ychar++)
        {
                //Get pointer to begin of text line
				buf = (u8*)&scp->buffer[(ychar * scp->width)];

                //Reset X-pixel position
                xpix = 0;

                //Draw chars of current line
                for(xchar = 0; xchar < xcharMax; xchar++)
                {
                        //Get fore color index
                        fColorIndex = GetPixel1b(scp->fColors, scp->width, xchar, ychar);

                        //Get back color index
                        bColorIndex = GetPixel1b(scp->bColors, scp->width, xchar, ychar);

                        //Convert encoding
                        if (scp->cp)
                            bch = GetLetterIndex(scp->cp, buf[xchar]);
                        else
                            bch = buf[xchar];

                        //Convert char index to graphic data
                        DrawChar1b(bch,
                                   pageBuf0, LCD_WIDTH, xpix, ypix & 0x07,
                                   scp->font, fColorIndex, bColorIndex);

                        xpix += fontW;
                }



                //Write graphic data to device
                if (fontH < 16)
                {
                        u32 pageIndex = ypix >> 3;

                        //Draw graphical data to LCD
                        LCDWritePage(pageIndex, 0, pageBuf0, xpix);

                        //Increment Y-pixel position to font height
                        ypix += fontH;

                        if (fontH != 8 && ((ypix & 0x07) == 0 || ychar + 1 == ycharMax))
                        {
                                LCDWritePage(pageIndex + 1, 0, pageBuf1, xpix);

                                //Clear two pages
                                memset(pageBuf0, 0, (LCD_WIDTH << 1));
                        }
                        else
                        {
                                //Copy second page to first page
                                memcpy(pageBuf0, pageBuf1, LCD_WIDTH);

                                //Clear second page
                                memset(pageBuf1, 0, LCD_WIDTH);
                        }
                }
                else
                {
                        //Draw graphical data to LCD
                        //Font with height 16 fill two pages on LCD
                        LCDWritePage((ychar << 1),     0, pageBuf0, xpix);
                        LCDWritePage((ychar << 1) + 1, 0, pageBuf1, xpix);

                        //Clear two pages
                        memset(pageBuf0, 0, (LCD_WIDTH << 1));
                }
        } //for each line
}



/* Draw graphic on LCD using specified parameters */
void LCDDrawGraphic(ScreenParams *scp)
{
        //Pointer to start page
        u8 *pgptr = (u8*)&scp->buffer[0];
        //Page wigth
        s16 pgWidth = scp->width;
        //Convert height at pixels to count of pages (h/8)
        s16 pgCount = scp->height >> 3;
        //Page counter
        s16 pgi = 0;        

        //Check width
        if (pgWidth > LCD_WIDTH)
                pgWidth = LCD_WIDTH;

        //Check count of pages
        if (pgCount > LCD_PAGECNT)
                pgCount = LCD_PAGECNT;

        //Write data to LCD
        while(pgi < pgCount)
        {           
                LCDWritePage(pgi++, 0, pgptr, pgWidth);
                pgptr += (u32)scp->width;
        }
}

/*===========================================================================
 * Test functions
 *===========================================================================*/

#ifdef __LCD_TEST_FUNC

#define LCD_READBUF_LENGTH      (128 * 8)
u8 lcdReadBuf[LCD_READBUF_LENGTH] = { 0 };

/* Write/Read test */
s32 LCDTest0()
{
        u32 cycCnt, p, x;
        s32 result = 0;
        u8 data;

        memset(lcdReadBuf, 0, LCD_READBUF_LENGTH);

        //Clear LCD
        LCDReset(0);
        LCDClear(0);

        //Write data
        p = 0;
        while(p < 8)
        {
                WriteAddr(LCD_CMD1, (LCD_CMD_SETPAGE | p));
                WriteAddr(LCD_CMD2, (LCD_CMD_SETPAGE | p));
                WaitReady();

                WriteAddr(LCD_CMD1, LCD_CMD_SETADDR);
                WriteAddr(LCD_CMD2, LCD_CMD_SETADDR);
                WaitReady();

                if (p & 1)
                        data = 128;
                else
                        data = 0;

                x = 0;
                while(x < 64)
                {
                        WriteAddr(LCD_DATA1, (u8)data);
                        WriteAddr(LCD_DATA2, (u8)(data + 64));
                        WaitReady();
                        x++;
                        data++;
                }

                p++;
        }

        //Read data
        p = 0;
        while(p < 8)
        {
                //page offset
                u32 offset = p * 128;
                u8 tmp;

                WriteAddr(LCD_CMD1, (LCD_CMD_SETPAGE | p));
                WriteAddr(LCD_CMD2, (LCD_CMD_SETPAGE | p));
                WaitReady();

                WriteAddr(LCD_CMD1, LCD_CMD_SETADDR);
                WriteAddr(LCD_CMD2, LCD_CMD_SETADDR);
                WaitReady();

                //Additional reading to get valid data at cycle
                tmp = ReadAddr(LCD_DATA1);
                tmp = ReadAddr(LCD_DATA2);
                WaitReady();

                x = 0;
                while(x < 64)
                {
                        lcdReadBuf[offset] = ReadAddr(LCD_DATA1);
                        lcdReadBuf[offset + 64] = ReadAddr(LCD_DATA2);
                        WaitReady();

                        x++;
                        offset++;
                }

                p++;
        }


        //Check received data
        x = 0;
        data = 0;
        while(x < LCD_READBUF_LENGTH)
        {
                if ((u8)data != (u8)lcdReadBuf[x])
                {
                        result = (s32)(PN31 | x);
                        break;
                }

                data++;
                x++;
        }

        return result;
}

#endif /* __LCD_TEST_FUNC */

/*===========================================================================
 * End of file LCD_MT12864x.c
 *===========================================================================*/

