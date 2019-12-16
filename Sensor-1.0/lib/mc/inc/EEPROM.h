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
 *  File EEPROM.h: EEPROM definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __EEPROM_H
#define __EEPROM_H

#include "CPU.h"

//EEPROM sections.
//Warning! Be careful when writing to 'EEPROM CODE' section.
#define EEPROM_CODE     0 //Executable code
#define EEPROM_DATA     1 //User data

/* Initialize EEPROM */
MCRESULT InitializeEEPROM(const void *cfg);

/* Erase EEPROM */
s32 EEPROMEraseChip(u8 eeprom);

/* Erase selected page */
s32 EEPROMErasePage(u8 eeprom, u32 pgaddr);

/* Write data to EEPROM */
s32 EEPROMWrite(u8 eeprom, u32 daddr, const void *src, s32 count);

/* Read data from EEPROM */
s32 EEPROMRead(u8 eeprom, u32 saddr, void *dest, s32 count);

#endif /* !__EEPROM_H */

/*===========================================================================
 * End of file EEPROM.h
 *===========================================================================*/
