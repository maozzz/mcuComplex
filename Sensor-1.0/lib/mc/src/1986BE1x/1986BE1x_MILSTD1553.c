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
 *  File 1986BE1x_MILSTD1553.c: MILSTD1553 (GOST R 52070-2003) functions.
 *
 *===========================================================================*/

#include "../../inc/MILSTD1553.h"
#include <string.h>

static const u32 milstdAddr[2] = { BASE_MILSTD1, BASE_MILSTD2 };
#define get_milstd(__x)       ((_milstd*)milstdAddr[((__x) & 1)])

#define MILSTD_PORT_OUT_OPTS    (PN_PWR_FASTEST|PN_NOINFILTER|PN_NOPULLDOWN|\
                                 PN_NOPULLUP|PN_NOSHMIT|PN_CTRLDRIVER)

#define MILSTD_PORT_IN_OPTS     (PN_INPUT|PN_NOINFILTER|PN_NOPULLDOWN|\
                                 PN_NOPULLUP|PN_NOSHMIT|PN_CTRLDRIVER)


/* Control structure (runtime) */
typedef struct tag_MILSTDRtmConfig
{        
    //Last tx command (master only)
    u16 lastCommand;

    //Last tx status (slave only)
    u16 lastStatus;

    //Active channel
    u16 lastChannel;   

    //Reserved
    u16 reserved;    

    //Event handler
    //"saddr" - slave address
    //"cmd"   - see MILSTD_CMD_x
    void (*evhandler)(s32 saddr, s32 cmd);
} MILSTDRtmConfig;


//Runtime configuration
static MILSTDRtmConfig milstdRtmConfig[2];


/* Tune MILSTD clock parameters */
static MCRESULT TuneMILSTDClkParams(u32 *pmanBRG, u32 *pdiv)
{
    u32 div, hclk, manBRG;

    hclk = GetHCLK();

    //MAN_BRG at [0..7]
    manBRG = 0;
    while(manBRG < 8)
    {
        //MAN_CLK = HCLK / (1 << MAN_BRG)
        div = (u32)(hclk / (1 << manBRG));

        if (div > 120000000)
            manBRG++;
        else
            break;
    }

    //DIV[17..11]: div = MAN_CLK / 1MHz, div=[1..120]
    *pdiv = (u32)(div / 1000000);
    *pmanBRG = manBRG;

    return MCR_OK;
}



/* */
static MCRESULT MILSTDMapPortPins(const MILSTD1553BConfig *cfg)
{
    #if (TARGET_MCU == MCU_1986BE1x)
    //MCU 1918BE1x
    switch(cfg->mil)
    {
        case MILSTD_N1:
        {
            if (cfg->mapRxTx == MILSTD1_MAP_RXTX_C13D6)
            {
                //Outputs
                //[PRDA+,PRDA-,PRD_PRMA, PRDB+,PRDB-,PRD_PRMB] => [D1,D2,D5, D3,D4,D6]
                InitializePORTEx(PORT_D, (PN1|PN2|PN5| PN3|PN4|PN6), 0,
                                 PN_BASIC|MILSTD_PORT_OUT_OPTS);

                //Inputs
                //[PRMA+,PRMA-, PRMB+,PRMB-] => [C13,C14, C15,D0]
                InitializePORTEx(PORT_C, (PN13|PN14|PN15), 0,
                                 PN_BASIC|MILSTD_PORT_IN_OPTS);
                InitializePORTEx(PORT_D, (PN0), 0,
                                 PN_BASIC|MILSTD_PORT_IN_OPTS);

                return MCR_OK;
            }
            else
                return MCR_INVALID_MAPPING;
        }

        case MILSTD_N2:
        {
            if (cfg->mapRxTx == MILSTD2_MAP_RXTX_F3F12)
            {
                //Outputs
                //[PRDC+,PRDC-,PRD_PRMC, PRDD+,PRDD-,PRD_PRMD] => [F7,F8,F11, F9,F10,F12]
                InitializePORTEx(PORT_F, (PN7|PN8|PN11| PN9|PN10|PN12), 0,
                                 PN_BASIC|MILSTD_PORT_OUT_OPTS);

                //Inputs
                //[PRMC+,PRMC-, PRMD+,PRMD-] => [F3,F4, F5,F6]
                InitializePORTEx(PORT_F, (PN3|PN4|PN5|PN6), 0,
                                 PN_BASIC|MILSTD_PORT_IN_OPTS);

                return MCR_OK;
            }
            else if (cfg->mapRxTx == MILSTD2_MAP_RXTX_A8A15)
            {
                //Outputs
                //[PRD_PRMC, PRD_PRMD] => [F11, F12]
                InitializePORTEx(PORT_F, (PN11|PN12), 0,
                                 PN_BASIC|MILSTD_PORT_OUT_OPTS);

                //[PRDC+,PRDC-, PRDD+,PRDD-] => [A12,A13, A14,A15]
                InitializePORTEx(PORT_A, (PN12|PN13| PN14|PN15), 0,
                                 PN_REMAP|MILSTD_PORT_OUT_OPTS);

                //Inputs
                //[PRMC+,PRMC-, PRMD+,PRMD-] => [A8,A9, A10,A11]
                InitializePORTEx(PORT_A, (PN9|PN0|PN10|PN11), 0,
                                 PN_REMAP|MILSTD_PORT_IN_OPTS);

                return MCR_OK;
            }
            else
                return MCR_INVALID_MAPPING;
        }

        default:
            return MCR_INVALID_MAPPING;
    }

    #else
    return MCR_INVALID_MAPPING;

    #endif
}



/* Initialize controller MILSTD1553B */
MCRESULT InitializeMILSTD1553B(const MILSTD1553BConfig *cfg)
{	
    _milstd *pmil;
    u32 div, control, trba, manBRG, opt;

    switch(cfg->mil)
    {
        case MILSTD_N1:
        {
            memset(&milstdRtmConfig[0], 0, sizeof(MILSTDRtmConfig));            
            milstdRtmConfig[0].evhandler = cfg->evhandler;

            ClkEnable(CLK_MILSTD1);
            pmil = MILSTD1;           
            break;
        }

        case MILSTD_N2:
        {
            memset(&milstdRtmConfig[1], 0, sizeof(MILSTDRtmConfig));            
            milstdRtmConfig[1].evhandler = cfg->evhandler;

            ClkEnable(CLK_MILSTD2);
            pmil = MILSTD2;
            break;
        }

        default:
            return MCR_INVALID_CFG;
    }

    //Map Rx/Tx pins
    if (MILSTDMapPortPins(cfg) != MCR_OK)
        return MCR_INVALID_MAPPING;

    //Tune MILSTD clock
    TuneMILSTDClkParams(&manBRG, &div);

    //MAN_CLK_EN[25]: 1/0 - enable/disable MILSTD clock
    //MAN_BRG[15..8]: [0..7]
    RST_CLK->ETH_CLOCK &= ~(PN25 | (0xFF << 8));
    RST_CLK->ETH_CLOCK |= PN25 | (manBRG << 8);

    //NOAUTOTUNE[21]     : 1 - disable autotune
    //ENFILTER[20]       : 1 - enable filter of NRZ stream
    //INVTR[19]          : 1 - invert PRM_PRDx signals
    //RERR[18]           : 1 - reset errors by MR=1; 0 - reset auto (idle)
    //DIV[15..11]        : div = MAN_CLK / 1MHz
    //RTA[10..6]         : slave address
    //TRB[5],TRA[4]      : secondary/primary channel
    //RTMODE[3],BCMODE[2]: [10,01,11] => [slave,master,monitor]
    //BCSTART[1]         : 1 - start tx data (master mode only)
    //MR[0]              : 1/0 - reset/work mode

    //MR[0] = 1 - reset
    pmil->CONTROL = PN0;

    //[15..13] - additonal options
    opt = (cfg->mode >> 12) & 0x0F;

    //Register value
    control = (opt << 18) |
              (div << 11) |
              ((cfg->saddr & 0x1F) << 6) | PN0;

    if ((cfg->mode & 0x03) == MILSTD_MODE_MASTER)
    {
        //MASTER mode
        //Select secondary (B, TRB[5]) or primary (A, TRA[4]) channel
        trba = (cfg->mode & MILSTD_MODE_SECONDARY) ? PN5 : PN4;

        //BCMODE[2]=1 - master, MR[0]=1 - reset
        control |= (trba|PN2);
    }
    else if ((cfg->mode & 0x03) == MILSTD_MODE_SLAVE)
    {
        //SlAVE mode
        //TRB[5]=TRA[4]=1 - both channels, RTMODE[3]=1 - slave
        control |= (PN5|PN4|PN3);

        //Set "StatusWord"
        //RTA[15..11]: slave address
        milstdRtmConfig[cfg->mil].lastStatus = ((cfg->saddr & 0x1F) << 11);
        pmil->StatusWord1 = milstdRtmConfig[cfg->mil].lastStatus;
    }
    else
    {
        //MONITOR mode
        //TRB[5]=TRA[4]=1 - both channels, RTMODE[3]=BCMODE[2]=1 - monitor
        control |= (PN5|PN4|PN3|PN2);
    }

    //Set control register
    pmil->CONTROL = control;

    if (cfg->evhandler != 0)
    {
        //Enable interrupts
        //ERRIE[3]     = 1
        //VALMESSIE[2] = 1
        //RFLAGNIE[1]  = 1
        //IDLEIE[0]    = 1
        pmil->INTEN = (PN3|PN2|PN1|PN0);

        switch(cfg->mil)
        {
            case MILSTD_N1: { IRQEnable(IRQ_MILSTD1); break; }
            case MILSTD_N2: { IRQEnable(IRQ_MILSTD2); break; }
        }
    }    

	return MCR_OK;
}



/* Set mode (master/slave) */
void MILSTD1553BSetMode(MILSTDBLOCK mil, u32 mode)
{
    _milstd *pmil = get_milstd(mil);
    u32 control, trba;

    //RTA[10..6]         : slave address
    //TRB[5],TRA[4]      : secondary/primary channel
    //RTMODE[3],BCMODE[2]: [10,01,11] => [slave,master,monitor]
    //BCSTART[1]         : 1 - start tx data (master mode only)
    //MR[0]              : 1/0 - reset/work mode

    //Save register value, keep all bits except [5..1]
    control = pmil->CONTROL & ~0x3E;

    //MR[0]=1 - reset
    pmil->CONTROL |= PN0;

    if ((mode & 0x03) == MILSTD_MODE_MASTER)
    {
        //MASTER mode
        //Select secondary (B, TRB[5]) or primary (A, TRA[4]) channel
        trba = (mode & MILSTD_MODE_SECONDARY) ? PN5 : PN4;

        //Set TRB[5] or TRA[4], BCMODE[2]=1 - master
        control |= trba | PN2;
    }
    else if ((mode & 0x03) == MILSTD_MODE_SLAVE)
    {
        //SLAVE mode
        //Set TRB[5]=TRA[4]=1 - both channels, RTMODE[3]=1 - slave
        control |= (PN5|PN4|PN3);

        //Set "StatusWord": RTA[15..11] - slave address
        milstdRtmConfig[mil].lastStatus = (((control >> 6) & 0x1F) << 11);
        pmil->StatusWord1 = milstdRtmConfig[mil].lastStatus;
    }
    else
    {
        //MONITOR mode
        //Set TRB[5]=TRA[4]=1 - B/A channels, RTMODE[3]=BCMODE[2]=1 - monitor
        control |= (PN5|PN4|PN3|PN2);
    }

    //Restore register value
    pmil->CONTROL = control;
}



/* Select channel (primary/secondary, master only) */
void MILSTD1553BSelectChannel(MILSTDBLOCK mil, u32 chindex)
{
    _milstd *pmil = get_milstd(mil);
    u32 control, trba;

    //RTMODE[3],BCMODE[2]: [10,01,11] => [slave,master,monitor]
    if (pmil->CONTROL & PN3)
    {
        //SLAVE/MONITOR MODE
        //no actions, both channels used, return
        return;
    }

    //Save register value
    control = pmil->CONTROL;

    //MR[0]=1 - reset
    pmil->CONTROL |= PN0;

    //Master mode
    //Select secondary (B, TRB[5]) or primary (A, TRA[4]) channel
    trba = (chindex) ? PN5 : PN4;

    //TRB[5] or TRA[4]
    control = (control & ~(PN5|PN4)) | trba;

    //Restore register value
    pmil->CONTROL = control;
}



/* Turn MILSTD1553B */
void TurnMILSTD1553B(MILSTDBLOCK mil, u8 on)
{
    u32 val;
    _milstd *pmil = get_milstd(mil);

    val = pmil->CONTROL;

    //MR[0]:  1/0 - reset/work mode
    if (on)
        val &= ~PN0;
    else
        val |= PN0;

    pmil->CONTROL = val;
}



/* Reset MILSTD1553B */
void ResetMILSTD1553B(MILSTDBLOCK mil)
{
    u32 i, val;
    _milstd *pmil = get_milstd(mil);

    val = pmil->CONTROL;

    //MR[0]: 1/0 - reset/work mode
    if (val & PN0)
    {
        //At reset mode: no actions, return
        return;
    }
    else
    {
        //At work mode:
        //Go to reset mode
        val |= PN0;
        pmil->CONTROL = val;

        //Wait some interval
        i = 0;
        while(i++ < 100) __NOP();

        //Return to work mode
        val &= ~PN0;
        pmil->CONTROL = val;
    }
}



/* Sets vector word */
void MILSTD1553BSetVecWord(MILSTDBLOCK mil, u32 vword)
{
    _milstd *pmil = get_milstd(mil);
    pmil->ModeData = (vword & 0xFFFF);
}



/* Gets received vector word */
u32 MILSTD1553BGetVecWord(MILSTDBLOCK mil)
{
    _milstd *pmil = get_milstd(mil);
    return (u32)(pmil->ModeData);
}



/* Sets status word */
void MILSTD1553BSetStatusWord(MILSTDBLOCK mil, u32 sword)
{
    _milstd *pmil = get_milstd(mil);
    u16 lsts;

    //RTMODE[3],BCMODE[2]: [10,01,11] => [slave,master,monitor]
    //RTMODE[3]=1: slave or monitor mode
    if (pmil->CONTROL & PN3)
    {
        //SLAVE/MONITOR MODE
        lsts = milstdRtmConfig[mil].lastStatus;

        //StatusWord: bits[15..11] - RTA (without changes), bits[7..5] - not used
        lsts = (lsts & (0x1F << 11)) | (sword & (PN10|PN9|PN8|0x1F));

        milstdRtmConfig[mil].lastStatus = lsts;
        pmil->StatusWord1 = lsts;
    }
    else
    {
        //MASTER MODE
        //"StatusWord" read only: no actions
    }
}



/* Gets received status word */
u32 MILSTD1553BGetStatusWord(MILSTDBLOCK mil, u32 swordN)
{
    _milstd *pmil = get_milstd(mil);

    //RTMODE[3],BCMODE[2]: [10,01,11] => [slave,master,monitor]
    //RTMODE[3]=1: slave or monitor mode
    if (pmil->CONTROL & PN3)
    {
        if (pmil->CONTROL & PN2)
        {
            //MONITOR MODE            
            if (swordN)
                return pmil->StatusWord2;
            else
                return pmil->StatusWord1;
        }
        else
        {
            //SLAVE MODE
            //"StatusWord" contains status from second slave (slave-slave transaction)
            return pmil->StatusWord1;
        }
    }
    else
    {
        //MASTER MODE
        if (swordN)
            return pmil->StatusWord2;
        else
            return pmil->StatusWord1;
    }
}



/* Gets last error */
u32 MILSTD1553BGetError(MILSTDBLOCK mil)
{
    _milstd *pmil = get_milstd(mil);
    return (u32)(pmil->ERROR);
}



/* Indicates that transaction in progress (master only) */
s32 IsMILSTDTranInProgress(MILSTDBLOCK mil)
{
    _milstd *pmil = get_milstd(mil);

    //RTMODE[3],BCMODE[2]: [10,01,11] => [slave,master,monitor]
    //RTMODE[3]=1: slave or monitor mode
    if (pmil->CONTROL & PN3)
    {
        //SLAVE/MONITOR MODE
        return 0;
    }
    else
    {
        //MASTER MODE
        //BCSTART[1] = 1: transmission in progress
        if (pmil->CONTROL & PN1)
            return 1;
        else
            return 0;
    }
}



/* Send command to specified slave device */
s32 MILSTD1553BSendCommand(MILSTDBLOCK mil, s32 daddr, s32 command, s32 dataword)
{
    s32 K;
    u32 val;
    _milstd *pmil = get_milstd(mil);

    //RTMODE[3],BCMODE[2]: [10,01,11] => [slave,master,monitor]
    //RTMODE[3]=1: slave or monitor mode
    if (pmil->CONTROL & PN3)
    {
        //SLAVE/MONITOR MODE: return
        return -1;
    }

    //MASTER MODE

    //Commands size: 5 bits
    command &= 0x1F;

    if (command & 0x10)
    {
        //Commands with dataword
        if (command == MILSTD_CMD_GETVECWORD ||
            command == MILSTD_CMD_GETLASTCMD ||
            command == MILSTD_CMD_GETBITWORD)
        {
            //Request data word from slave (K[10]=1)
            K = PN10;
        }
        else
        {
            //Send data word to slave (K[10]=0)
            K = 0;

            //ModeData[15..0]: data word
            pmil->ModeData = (dataword & 0xFFFF);
        }

    }
    else
    {
        //Commands without data word (always K[10]=1)
        K = PN10;
    }

    milstdRtmConfig[mil].lastCommand = ((daddr & 0x1F) << 11) | //RTA[15..1] - slave address
                                       (K) |      //K[10]=1/0 - get data word/send data word
                                       (command); //SUBA[9..5]=0; N[4..0] - command

    pmil->CommandWord1 = milstdRtmConfig[mil].lastCommand;

    //Clear CommandWord2
    pmil->CommandWord2 = (u32)0;

    //Set BCSTART[1]=1: start send command
    val = pmil->CONTROL | PN1;
    pmil->CONTROL = val;

    return 0;
}



/* Request data from slave */
s32 MILSTD1553BRequestData(MILSTDBLOCK mil, s32 daddr, s32 count)
{
    s32 nofWords;
    u32 subaddr, val;
    _milstd *pmil = get_milstd(mil);

    if (pmil->CONTROL & PN3)
    {
        //SLAVE/MONITOR MODE: return
        return -1;
    }    



    //Request data from slave
    //CommandWord1 register:
    //RTA[15..11]: destination address
    //K[10]      : 1/0 - get data from slave/send data to slave
    //SUBA[9..5] : [1..29] - subaddress of tx data,
    //              [0,31] - command (no data),
    //                [30] - circular data
    //N[4..0]    : count of words [1..31], N=0 - 32 words

    //Get subaddress from "daddr" parameter
    subaddr = (daddr >> 5) & 0x1F;

    //Count of requested words
    nofWords = ((count + 1) >> 1);

    //Use subaddress #1 by default
    if (subaddr == 0 || subaddr == 0x1F)
        subaddr = 1;    

    //Maximum 32 words
    if (nofWords > 32)
        nofWords = 32;

    milstdRtmConfig[mil].lastCommand = ((daddr & 0x1F) << 11) |
                                        (1 << 10)       | //K=1: get data from slave
                                        (subaddr << 5)  |
                                        (nofWords & 0x1F);

    pmil->CommandWord1 = milstdRtmConfig[mil].lastCommand;

    //Clear CommandWord2
    pmil->CommandWord2 = (u32)0;

    //Set BCSTART[1]=1: start send command word
    val = pmil->CONTROL | PN1;
    pmil->CONTROL = val;

    //Return from here
    return 0;
}



/* Read data */
s32 MILSTD1553BRead(MILSTDBLOCK mil, s32 suba, void *dest, s32 count)
{    
    s32 i;    
    u32 subaddr, val;
    volatile u32 *psrcU32;
    volatile u8 *pdest;
    _milstd *pmil = get_milstd(mil);
    MILSTDRtmConfig *prtm = &milstdRtmConfig[mil];

    //RTMODE[3],BCMODE[2]: [10,01,11] => [slave,master,monitor]
    if (pmil->CONTROL & PN3)
    {
        //SLAVE/MONITOR MODE
        if (suba > 0)
            subaddr = (u32)(suba & 0x1F);
        else
            subaddr = ((pmil->CommandWord1 >> 5) & 0x1F);        
    }
    else
    {
        //MASTER MODE
        if (suba > 0)
            subaddr = (u32)(suba & 0x1F);
        else
            subaddr = ((prtm->lastCommand >> 5) & 0x1F);        
    }

    //[0,0x1F] - not used, command control,
    //use subaddress #1 by default
    if (subaddr == 0 || subaddr == 0x1F)
        subaddr = 1;    

    //Maximum count
    if (count > MILSTD_MSG_DATA_MAXLENGTH)
        count = MILSTD_MSG_DATA_MAXLENGTH;

    //Data words stored at 32-bit cells:
    //Tx/Rx buffer size: 0x1000 = 4096 bytes = [32 subaddr] x [128 bytes] =
    //= [32 subaddr] x [32 words] x [4 bytes].
    //Offset = [subaddress] x 128,  subaddress = [1..29]
    psrcU32 = (mil == MILSTD_N1) ?
              ((volatile u32 *)(BASE_MILSTD1_DATA + (subaddr << 7))):
              ((volatile u32 *)(BASE_MILSTD2_DATA + (subaddr << 7)));

    pdest = (u8*)dest;    
    val = 0;

    for(i = 0; i < count; i++)
    {
        if ((i & 1) == 0)
        {
            //Low 8-bit of 16-bit word
            val = *psrcU32++;
            *pdest++ = (u8)(val);
        }
        else
        {
            //High 8-bit of 16-bit word
            *pdest++ = (u8)(val >> 8);
        }
    }

    return (s32)(i);
}



/* Start transmit data */
s32 MILSTD1553BWrite(MILSTDBLOCK mil, s32 daddr, const void *src, s32 count)
{
    s32 i, nofWords;
    u32 val, subaddr;
    _milstd *pmil;
    volatile u32 *pdestU32;
    u8 *psrc;    

    if (!src || !count)
        return 0;    

    //Select block
    pmil = get_milstd(mil);

    //Get subaddress from "daddr" parameter
    subaddr = (daddr >> 5) & 0x1F;

    //Use subaddress #1 by default
    if (subaddr == 0 || subaddr == 0x1F)
        subaddr = 1;

    //Maximum count
    if (count > MILSTD_MSG_DATA_MAXLENGTH)
        count = MILSTD_MSG_DATA_MAXLENGTH;

    //Data words stored at 32-bit cells:
    //Tx/Rx buffer size: 0x1000 = 4096 bytes = [32 subaddr] x [128 bytes] =
    //= [32 subaddr] x [32 words] x [4 bytes].
    //Offset = [subaddress] x 128,  subaddress = [1..29]
    pdestU32 = (mil == MILSTD_N1) ?
                ((volatile u32 *)(BASE_MILSTD1_DATA + (subaddr << 7))):
                ((volatile u32 *)(BASE_MILSTD2_DATA + (subaddr << 7)));

    psrc = (u8*)src;
    nofWords = 0;

    //Copy data to internal MILSTD buffer
    for(i = 0; i < count; i += 2)
    {
        //Odd count of bytes: zeros at high 8 bits
        val = (i + 1 < count) ? psrc[(i + 1)] : 0;

        *pdestU32++ = ((val << 8) | ((u32)psrc[i]));
        nofWords++;
    }       

    //RTMODE[3],BCMODE[2]: [10,01,11] => [slave,master,monitor]
    if (pmil->CONTROL & PN3)
    {
        //SLAVE/MONITOR MODE
        //no actions        
    }
    else
    {
        //MASTER MODE        
        //Set CommandWord1 register
        //RTA[15..11]: destination address
        //K[10]      : 1 - get data from slave;  0 - send data to slave
        //SUBA[9..5] : [1..29] - subaddress of tx data,
        //              [0,31] - command (no data),
        //                [30] - circular data
        //N[4..0]    : count of (16+4)bit words [1..31], N=0 - 32 words        
        milstdRtmConfig[mil].lastCommand = ((daddr & 0x1F) << 11) |
                                            (0 << 10) |       //K=0: send data to slave
                                            (subaddr << 5)  | //SUBA: [1..29]
                                            (nofWords & 0x1F);

        pmil->CommandWord1 = milstdRtmConfig[mil].lastCommand;

        //Clear CommandWord2
        pmil->CommandWord2 = (u32)0;

        //Set BCSTART[1]=1: start data transmission
        val = pmil->CONTROL | PN1;
        pmil->CONTROL = val;
    }

    return count;
}



/* */
static void MILSTD1553Bx_IRQHandler(_milstd *pmil, MILSTDRtmConfig *prtm)
{    
    u32 status, val, rtaddr, suba;

    //Read STATUS register
    status = pmil->STATUS;

    //RTMODE[3],BCMODE[2]: [10,01,11] => [slave,master,monitor]
    if (pmil->CONTROL & PN3)
    {
        //-----------------------------------------------------------
        //SLAVE/MONITOR MODE

        //RCVB[5]=1: secondary channel
        //RCVA[4]=1: primary channel
        if (status & PN5)
            prtm->lastChannel = 1;
        else if (status & PN4)
            prtm->lastChannel = 0;       

        //if (status & (PN2|PN1|PN0))
        if (status & (PN2|PN0))
        {
            //VALMESS[2] = 1: success transaction
            //RFLAGN[1]  = 1: rx command/response word
            //IDLE[0]    = 1: idle state
            val = pmil->CommandWord1;

            //Requested address
            rtaddr = (val >> 11) & 0x1F;

            //RTMODE[3]=BCMODE[2]=1: monitor mode, no check address
            //Not need check address here
            /* if ((pmil->CONTROL & PN2) == 0)
            {
                //CommandWord1.DADDR[15..11]: destination slave address;
                //CONTROL.RTA[10..6]: current slave address (RTA)
                //DADDR != [group addr] AND DADDR != RTA: return
                if (rtaddr != 0x1F &&
                    rtaddr != ((pmil->CONTROL >> 6) & 0x1F))
                    return;
            } */

            //Get subaddress (bits[9..5])
            suba = (val >> 5) & 0x1F;

            if (suba == 0 || suba == 0x1F)
            {
                //Subaddress is 0 or 31: command received - bits[4..0]
                if (prtm->evhandler)
                    prtm->evhandler(rtaddr, (val & 0x1F));
            }
            else if (val & PN10)
            {
                //CommandWord1.K[10]=1: data transmission completed
                if (prtm->evhandler)
                    prtm->evhandler(rtaddr, MILSTD_CMD_DATA_TXCOMPLETED);
            }
            else
            {
                //CommandWord1.K[10]=0: data received from master
                if (prtm->evhandler)
                    prtm->evhandler(rtaddr, MILSTD_CMD_DATA_RECEIVED);
            }
        }
        else if (status & PN3)
        {
            //ERR[3]=1: error during transaction
            rtaddr = ((pmil->CONTROL >> 6) & 0x1F);
            if (prtm->evhandler)
                prtm->evhandler(rtaddr, MILSTD_CMD_FAIL);
        }
    }
    else
    {
        //-----------------------------------------------------------
        //MASTER MODE        

        //if (status & (PN2|PN1))
        if (status & PN2)
        {
            //VALMESS[2] = 1: success transaction
            //RFLAGN[1]  = 1: rx command/response word
            val = pmil->StatusWord1;
            rtaddr = (val >> 11) & 0x1F;

            //Check address at status word
            if (((prtm->lastCommand >> 11) & 0x1F) != rtaddr)
                return;

            //Get subaddress
            suba = (prtm->lastCommand >> 5) & 0x1F;

            if (suba == 0 || suba == 0x1F)
            {
                //Subaddress is 0 or 31: response on command received
                if (prtm->evhandler)
                    prtm->evhandler(rtaddr, (prtm->lastCommand & 0x1F));
            }
            else if (prtm->lastCommand & PN10)
            {
                //K[10]=1: received data from slave
                if (prtm->evhandler)
                    prtm->evhandler(rtaddr, MILSTD_CMD_DATA_RECEIVED);
            }
            else
            {                
                //K[10]=0: data transmission completed
                if (prtm->evhandler)
                    prtm->evhandler(rtaddr, MILSTD_CMD_DATA_TXCOMPLETED);
            }
        }
        else if (status & PN3)
        {
            //ERR[3]=1: error
            rtaddr = ((prtm->lastCommand >> 11) & 0x1F);
            if (prtm->evhandler)
                prtm->evhandler(rtaddr, MILSTD_CMD_FAIL);
        }        
        else if (status & PN0)
        {
            //IDLE[0] = 1: idle state                        
        }        
    }

//#define __TEST_MILSTD_MSG
#ifdef __TEST_MILSTD_MSG
    //Test only
    status = pmil->STATUS;
    ConPrintf("%s: %s%s%s%s%s%s; ",
              (pmil->CONTROL & PN3) ? "SLV" : "MAS",
              (status & PN5) ? "RCVB "    : "",
              (status & PN4) ? "RCVA "    : "",
              (status & PN3) ? "ERR "     : "",
              (status & PN2) ? "VALMESS " : "",
              (status & PN1) ? "RFLAGN "  : "",
              (status & PN0) ? "IDLE "    : "");

    if ((pmil->CONTROL & PN3))
    {
        //SLAVE MODE
        if (status & PN3)
        {
            ConPrintf("\n");
        }
        else
        {
            val = pmil->CommandWord1;
            ConPrintf("RXCMD A%i K%i S%i N%i\n",
                      (val >> 11) & 0x1F,   //RTA
                      (val & PN10) ? 1 : 0, //K
                      (val >> 5) & 0x1F,    //SUBA
                      (val & 0x1F));        //N
        }
    }
    else
    {
        //MASTER MODE
        if (status & PN3)
        {
            ConPrintf("\n");
        }
        else
        {
            val = pmil->StatusWord1;
            ConPrintf("RXSTS A%i %s%s%s%s%s%s%s%s\n",
                      (val >> 11) & 0x1F,
                      (val & PN10) ? "MSERR " : "",
                      (val & PN9)  ? "TXWRD " : "",
                      (val & PN8)  ? "RQMNT " : "",
                      (val & PN4)  ? "RXGRP " : "",
                      (val & PN3)  ? "ABUSY " : "",
                      (val & PN2)  ? "AMALF " : "",
                      (val & PN1)  ? "GCTRL " : "",
                      (val & PN0)  ? "DMALF " : "");
        }
    }
#endif
}



/* */
void MILSTD1553B1_IRQHandler()
{
    MILSTD1553Bx_IRQHandler(MILSTD1, &milstdRtmConfig[0]);
}



/* */
void MILSTD1553B2_IRQHandler()
{
    MILSTD1553Bx_IRQHandler(MILSTD2, &milstdRtmConfig[1]);
}

/*===========================================================================
 * End of file 1986BE1x_MILSTD1553.c
 *===========================================================================*/
