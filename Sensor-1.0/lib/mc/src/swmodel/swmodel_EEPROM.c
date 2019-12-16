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
 *  File swmodel_EEPROM.c: EEPROM functions.
 *
 *===========================================================================*/

#include "../../inc/EEPROM.h"
#include <string.h>

//EEPROM CODE
u8 eeprom_code_buffer[EEPROM_CODE_SIZE];

//EEPROM DATA
u8 eeprom_data_buffer[EEPROM_DATA_SIZE];



/* Initialize EEPROM */
MCRESULT InitializeEEPROM(const void *cfg)
{
    memset(eeprom_code_buffer, 0xFF, EEPROM_CODE_SIZE);
    memset(eeprom_data_buffer, 0xFF, EEPROM_DATA_SIZE);
    return MCR_OK;
}



/* Erase full chip */
s32 EEPROMEraseChip(u8 eeprom)
{
    if (eeprom == EEPROM_CODE)
        memset(eeprom_code_buffer, 0xFF, EEPROM_CODE_SIZE);
    else
        memset(eeprom_data_buffer, 0xFF, EEPROM_DATA_SIZE);

    return 0;
}



/* Erase selected page */
s32 EEPROMErasePage(u8 eeprom, u32 pgaddr)
{
    pgaddr &= ~(0x03U);

    if (eeprom == EEPROM_CODE)
    {
        if (pgaddr < EEPROM_CODE_BASE)
            pgaddr = EEPROM_CODE_BASE;

        memset(&eeprom_code_buffer[(pgaddr - EEPROM_CODE_BASE)], 0xFF, EEPROM_CODE_PAGE_SIZE);
    }
    else
    {
        if (pgaddr < EEPROM_DATA_BASE)
            pgaddr = EEPROM_DATA_BASE;

        memset(&eeprom_data_buffer[(pgaddr - EEPROM_DATA_BASE)], 0xFF, EEPROM_DATA_PAGE_SIZE);
    }

    return 0;
}



/* Write data to EEPROM */
s32 EEPROMWrite(u8 eeprom, u32 daddr, const void *src, s32 count)
{
    u8 *eepromBuf;
    const u8 *srcU8 = (u8*)src;
    s32 i;
    u32 endaddr;

    daddr &= ~(0x03U);

    if (eeprom == EEPROM_CODE)
    {
        if (daddr < EEPROM_CODE_BASE)
            daddr = EEPROM_CODE_BASE;

        eepromBuf = &eeprom_code_buffer[(daddr - EEPROM_CODE_BASE)];

        endaddr = EEPROM_CODE_BASE + EEPROM_CODE_SIZE;
    }
    else
    {
        if (daddr < EEPROM_DATA_BASE)
            daddr = EEPROM_DATA_BASE;

        eepromBuf = &eeprom_data_buffer[(daddr - EEPROM_DATA_BASE)];

        endaddr = EEPROM_DATA_BASE + EEPROM_DATA_SIZE;
    }

    i = 0;
    while(i < count && daddr < endaddr)
    {
        eepromBuf[i] = eepromBuf[i] & srcU8[i];
        //printf("%02X ", eepromBuf[i]);
        i++;
    }

    //printf("\r\n");
    return i;
}



/* Read data from EEPROM */
s32 EEPROMRead(u8 eeprom, u32 saddr, void *dest, s32 count)
{
    u8 *eepromBuf;
    u8 *destU8 = (u8*)dest;
    s32 i;
    u32 endaddr;

    saddr &= ~(0x03U);

    if (eeprom == EEPROM_CODE)
    {
        if (saddr < EEPROM_CODE_BASE)
            saddr = EEPROM_CODE_BASE;

        eepromBuf = &eeprom_code_buffer[(saddr - EEPROM_CODE_BASE)];

        endaddr = EEPROM_CODE_BASE + EEPROM_CODE_SIZE;
    }
    else
    {
        if (saddr < EEPROM_DATA_BASE)
            saddr = EEPROM_DATA_BASE;

        eepromBuf = &eeprom_data_buffer[(saddr - EEPROM_DATA_BASE)];

        endaddr = EEPROM_DATA_BASE + EEPROM_DATA_SIZE;
    }

    i = 0;
    while(i < count && saddr < endaddr)
    {
        destU8[i] = eepromBuf[i];
        i++;
    }

    return i;
}

/*===========================================================================
 * End of file swmodel_EEPROM.c
 *===========================================================================*/
