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
 *  File 1986BE9x_BKP.c. Read/write BKP registers (BKP_REG_00 .. BKP_REG_0D).
 *
 *===========================================================================*/

#include "../../inc/BKP.h"
#include <string.h>

//Count of BKP registers
#define BKP_REG_COUNT           14
//BKP space size, bytes
#define BKP_REG_ARR_SIZE        (BKP_REG_COUNT << 2)
//Pointer to registers array
#define BKP_REG_ARR             ((__io_reg*)(BASE_BKP))

//Wait while ready
#define WaitBKPReady()          //while(BKP->RTC_CS & PN6);

/* */
MCRESULT InitializeBKP(const void *cfg)
{
        ClkEnable(CLK_BKP);
        return MCR_OK;
}



/* */
s32 GetBKPSize()
{
	return (s32)BKP_REG_ARR_SIZE;
}



/* */
s32 BKPWrite(u32 offset, const void *buffer, s32 count)
{
        u32 regi, regend;
        u32 tmp[BKP_REG_COUNT];
        u8 *ptmp = (u8*)tmp;

        if (offset >= BKP_REG_ARR_SIZE || !buffer || !count)
                return 0;
        else if (offset + count > BKP_REG_ARR_SIZE)
                count = BKP_REG_ARR_SIZE - offset;

        //Read registers to temp buffer
        regi   = (offset >> 2);
        regend = ((offset + count + 3) >> 2);
        while(regi < regend)
        {
                WaitBKPReady();
                tmp[regi] = BKP_REG_ARR[regi];
                regi++;
        }

        //Write input buffer to temp buffer
        memcpy(&ptmp[offset], buffer, count);

        //Write temp buffer to registers
        regi   = (offset >> 2);
        while(regi < regend)
        {
                WaitBKPReady();
                BKP_REG_ARR[regi] = tmp[regi];
                regi++;
        }

        return count;
}



/* */
s32 BKPRead(u32 offset, void *buffer, s32 count)
{
        u32 regi, regend;
        u32 tmp[BKP_REG_COUNT];
        u8 *ptmp = (u8*)tmp;

        if (offset >= BKP_REG_ARR_SIZE || !buffer || !count)
                return 0;
        else if (offset + count > BKP_REG_ARR_SIZE)
                count = BKP_REG_ARR_SIZE - offset;

        //Read registers to temp buffer
        regi   = (offset >> 2);
        regend = ((offset + count + 3) >> 2);
        while(regi < regend)
        {
                WaitBKPReady();
                tmp[regi] = BKP_REG_ARR[regi];
                regi++;
        }

        //Write temp buffer to input buffer
        memcpy(buffer, &ptmp[offset], count);

        return count;
}



/* */
void BKPClear()
{
	s32 i = 0;
        while(i < BKP_REG_COUNT)
        {
                WaitBKPReady();
                BKP_REG_ARR[i++] = (u32)0;
        }
}

/*===========================================================================
 * End of file 1986BE9x_BKP.c
 *===========================================================================*/

