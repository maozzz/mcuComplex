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
 *  File 1986BE9x_CPU.c: MCU core speed setup, ports initialization.
 *
 *===========================================================================*/

#include "../../inc/CPU.h"


/*===========================================================================
 * Clock functions
 *===========================================================================*/

typedef struct tag_CLKCoreParams
{
        //Input parameters
	u32 iclk;     //Requested ARM core clock, Hz
        u32 clkSrc;   //Clock source (see CLKSRC_x)

        //Output parameters
	u32 oclk;      //Solved output ARM clock, Hz
	u8  mulc;      //PLL_CPU_MUL
	u8  divc;      //CPU_C3_SEL

        u16 res;       //Reserved
} CLKCoreParams;


//Current CPU clock source
static u32 cpuCurrClkSrc = 0;


/* Indicates that specified clock source is turned on and ready */
u8 IsClkSrcReady(u32 clkSrc)
{
        switch(clkSrc)
        {
                case CLKSRC_HSI:
                case CLKSRC_HSI_DIRECT:
						//HSI_ON[22] = 1 and HSI_RDY[23] = 1
                        return (BKP->REG_0F & PN22) && (BKP->REG_0F & PN23) ? 1 : 0;

                case CLKSRC_HSE:
						//HSE_ON[0] = 1 and HSE_RDY[2] = 1
                        return (RST_CLK->HS_CONTROL & PN0) && (RST_CLK->CLOCK_STATUS & PN2) ? 1 : 0;

                case CLKSRC_LSE:
						//LSE_ON[0] = 1
                        return (BKP->REG_0F & PN0) && (BKP->REG_0F & PN13) ? 1 : 0;

                case CLKSRC_LSI:
						//LSI_ON[15] = 1 and LSI_RDY[21] = 1
                        return (BKP->REG_0F & PN15) && (BKP->REG_0F & PN21) ? 1 : 0;

                default:
						//Unknown clock source: return 0
                        return 0;
        }
}



/* Turn on/off selected clock source */
u8 TurnClkSrc(u32 clkSrc, u8 turn)
{
        //Turn OFF clocks
        if (!turn)
        {
				//Reset {HSI,HSE,LSE,LSI}_ON bits
                switch(clkSrc)
                {
                        case CLKSRC_HSI:
                        case CLKSRC_HSI_DIRECT: BKP->REG_0F &= ~PN22;   return 1;
                        case CLKSRC_HSE: RST_CLK->HS_CONTROL &= ~PN0;   return 1;
                        case CLKSRC_LSE: BKP->REG_0F &= ~PN0;           return 1;
                        case CLKSRC_LSI: BKP->REG_0F &= ~PN15;          return 1;
                }

                return 0;
        }

        //Turn ON clocks
        switch(clkSrc)
        {
                case CLKSRC_HSI:
                case CLKSRC_HSI_DIRECT:
                {
						//Set HSI_ON[22] = 1, wait HSI_RDY[23] = 1
                        BKP->REG_0F |= PN22;            
                        while (!(BKP->REG_0F & PN23));  
                        return 1;
                }

                case CLKSRC_HSE:
                {
						//Set HSE_ON[0] = 1, HSE_BYP[1] = 0, wait HSE_RDY[2] = 1
                        RST_CLK->HS_CONTROL |= PN0;             
                        RST_CLK->HS_CONTROL &= ~PN1;            
                        while (!(RST_CLK->CLOCK_STATUS & PN2));
                        return 1;
                }

                case CLKSRC_LSE:
                {
						//Set LSE_ON[0] = 1, wait LSE_RDY[13] = 1
                        BKP->REG_0F |= PN0;             
                        while (!(BKP->REG_0F & PN13));  
                        return 1;
                }

                case CLKSRC_LSI:
                {
						//Set LSI_ON[15] = 1, wait LSI_RDY[21] = 1
                        BKP->REG_0F |= PN15;            
                        while (!(BKP->REG_0F & PN21));  
                        return 1;
                }
        } //switch(clkSrc)

        return 0;
}



/* */
s32 TuneClkSrcHSIInc(s32 trimInc)
{
        s32 trimVal;

	//Get current value
        trimVal = (s32)((BKP->REG_0F >> 24) & 0x3F);
				
	if (!trimInc)
	    //inc is 0 - return current value
	    return trimVal;
	else
	    //Set new trim value
	    return TuneClkSrcHSIVal(trimVal + trimInc);
}



/* */
s32 TuneClkSrcHSIVal(s32 trimVal)
{	
        //Check value
        if (trimVal > 63)
                trimVal = 63;
        else if (trimVal < 0)
                trimVal = 0;

        //Set new value
        BKP->REG_0F = (BKP->REG_0F & ~(0x3F << 24)) | (((u32)trimVal) << 24);

        //Wait while WEC[6] = 1 (write in progress)
        while(BKP->RTC_CS & PN6);

        //Return new value
        return (s32)((BKP->REG_0F >> 24) & 0x3F);
}



/* */
static MCRESULT TuneHCLKParams(CLKCoreParams *p)
{
        CLKCoreParams tmp;
        u32 diffHCLK;
        u32 selClock;

        if (!p)
                return MCR_SETUPCLK_FAIL;

        //Check hclk
        if (p->iclk > MAX_HCLK)
                p->iclk = MAX_HCLK;

        p->oclk = 0;
        p->mulc = 0;
        p->divc = 7;

        switch(p->clkSrc)
        {
                case CLKSRC_HSI:        selClock = FREQ_HSI; break;
                case CLKSRC_HSE:        selClock = FREQ_HSE; break;
                case CLKSRC_LSE:        p->oclk = FREQ_LSE;  return MCR_OK;
                case CLKSRC_LSI:        p->oclk = FREQ_LSI;  return MCR_OK;
                case CLKSRC_HSI_DIRECT: p->oclk = FREQ_HSI;  return MCR_OK;

                default:
                        return MCR_SETUPCLK_BADSRC;
        }

        //Set max difference
        diffHCLK = p->iclk;

        tmp.iclk = 0;

        //For each mul value
        tmp.mulc = 0;
        while(tmp.mulc <= 15)
        {
                //For each div value
                tmp.divc = 7;
                while(tmp.divc <= 15)
                {
                        u32 df;

                        //Solve result hclk with current parameters
                        tmp.iclk = (u32)selClock * (tmp.mulc + 1) / (1 << (tmp.divc - 7));

                        //Check hclk
                        if (tmp.iclk > MAX_VALID_HCLK)
                        {
                                tmp.divc++;
                                continue;
                        }

                        //Solve difference
                        df = tmp.iclk >= p->iclk ? (tmp.iclk - p->iclk) : (p->iclk - tmp.iclk);

                        //Save params
                        if (df < diffHCLK)
                        {
                              diffHCLK = df;
                              p->oclk = tmp.iclk;
                              p->divc = tmp.divc;
                              p->mulc = tmp.mulc;
                        }

                        if (df == 0)
                                break;

                        //Select next div
                        tmp.divc++;
                } //while

                if (diffHCLK == 0)
                        break;

                //Select next mul
                tmp.mulc++;
        } //while

        return p->oclk < MAX_VALID_HCLK ? MCR_OK : MCR_SETUPCLK_FAIL;
}



/* Setup CPU core speed */
MCRESULT SetHCLK(u32 hclk, u32 clkSrc)
{
        u8 delay;
        CLKCoreParams hp;        

        hp.iclk       = hclk;
        hp.clkSrc     = clkSrc;
		cpuCurrClkSrc = clkSrc;

        if (TuneHCLKParams(&hp) != MCR_OK)
                return MCR_SETUPCLK_FAIL;

        //Enable clocks
        ClkEnable(CLK_EEPROM);
        ClkEnable(CLK_RST_CLK);
        ClkEnable(CLK_DMA);
        ClkEnable(CLK_POWER);
        ClkEnable(CLK_WWDT);
        ClkEnable(CLK_IWDT);
        ClkEnable(CLK_BKP);

        //Select CPU delay
        if (hp.oclk < 25E6)      //<25MHz  0 clocks
                delay = 0;
        else if (hp.oclk < 50E6) //<50MHz  1 clock
                delay = 1;
        else if (hp.oclk < 75E6) //<75MHz  2 clocks
                delay = 2;
        else if (hp.oclk < 100E6)//<100MHz 3 clocks
                delay = 3;
        else if (hp.oclk < 125E6)//<125MHz 4 clocks
                delay = 4;
        else if (hp.oclk < 150E6)//<150MHz 5 clocks
                delay = 5;
        else if (hp.oclk < 175E6)//<175MHz 6 clocks
                delay = 6;
        else
                delay = 7;

        //Set EEPROM Delay [5..3],
        //RD[2] = 0,  WR[1] = 0,  CON[0] = 0
        EEPROM->CMD = (delay << 3);

        //Reset CPU_C1_SEL[1..0]
        //0 - HSI; 1 - HSI/2; 2 - HSE; 3 - HSE/2
        RST_CLK->CPU_CLOCK &= ~0x0003;

        if (!TurnClkSrc(clkSrc, 1))
                return MCR_SETUPCLK_FAIL;

        switch(clkSrc)
        {
                case CLKSRC_HSI:
                case CLKSRC_HSI_DIRECT:
                {
                        if (hp.clkSrc == CLKSRC_HSI_DIRECT)
                        {
								//HCLK_SEL[9..8] = 0 (HCLK = HSI)
                                RST_CLK->CPU_CLOCK &= ~0x0300;  
                                return MCR_OK;
                        }

                        //CPU_C1_SEL[1..0] = 0 (HSI) this bits already reset
                        //RST_CLK->CPU_CLOCK &= ~0x0003;
                        break;
                }

                case CLKSRC_LSE:
						//HCLK_SEL[9..8] = 2 (HCLK = LSE)
                        RST_CLK->CPU_CLOCK |= 0x0200;  
                        return MCR_OK;

                case CLKSRC_LSI:
						//HCLK_SEL[9..8] = 3 (HCLK = LSI)
                        RST_CLK->CPU_CLOCK |= 0x0300;  
                        return MCR_OK;

                case CLKSRC_HSE:
						//CPU_C1_SEL[1..0] = 2 (HSE)
                        RST_CLK->CPU_CLOCK |= 0x0002;  
                        break;

                default:
                        return MCR_SETUPCLK_BADSRC;
        }

        //Set clock multiplier value
		//PLL_CPUo = PLL_CPUi * (PLL_CPU_MUL[11..8] + 1)
		RST_CLK->PLL_CONTROL &= ~(0x0F << 8);
        RST_CLK->PLL_CONTROL |= (hp.mulc << 8);         

		//PLL_CPU_ON[2] = 1 - PPL on
		//Wait PLL_CPU_RDY[1] = 1 - wait PLL CPU ready
        RST_CLK->PLL_CONTROL |= PN2;                    
        while(!(RST_CLK->CLOCK_STATUS & PN1));

        //Set clock divider value
		//CPU_C3 = CPU_C2 / (1 << (CPU_C3_SEL[15..7] - 7))		
		//CPU_C3_SEL[7..4] - divider value
        RST_CLK->CPU_CLOCK &= ~(0x0F << 4);	
        RST_CLK->CPU_CLOCK |= (hp.divc << 4);             

		//HCLK_SEL[9..8] = 1 - CPU_C3 ([0..3] -> [HSI, CPU_C3, LSE, LSI])
		//CPU_C2_SEL[2] = 1/0 - PLL_CPUo/CPU_C1		
        RST_CLK->CPU_CLOCK |= PN8 | PN2;

        return MCR_OK;
}



/* Gets current CPU core speed, Hz */
u32 GetHCLK(void)
{
        u32 speed_CPU_C1  = 0;
        u32 speed_PLLCPUo = 0;
        u32 speed_CPU_C2;
        u32 speed_CPU_C3;
        u32 divc;

		//Selected HCLK source 
		//HCLK_SEL[9..8]: [0..3] -> [HSI, CPU_C3, LSE, LSI]
        switch ((RST_CLK->CPU_CLOCK & 0x0300) >> 8)
        {
                case 0: return IsClkSrcReady(CLKSRC_HSI) ? FREQ_HSI : 0;
                case 1: /* used CPU_C3 */ break;
                case 2: return IsClkSrcReady(CLKSRC_LSE) ? FREQ_LSE : 0;
                case 3: return IsClkSrcReady(CLKSRC_LSI) ? FREQ_LSI : 0;
        }

		//Selected clock source
		//CPU_C1_SEL[1..0]: [0..3] -> [HSI, HSI/2, HSE, HSE/2]
        switch(RST_CLK->CPU_CLOCK & 0x03) 
        {
                case 0x00: { speed_CPU_C1 = IsClkSrcReady(CLKSRC_HSI) ? FREQ_HSI : 0;        break; }                
                case 0x01: { speed_CPU_C1 = IsClkSrcReady(CLKSRC_HSI) ? (FREQ_HSI >> 1) : 0; break; }
                case 0x02: { speed_CPU_C1 = IsClkSrcReady(CLKSRC_HSE) ? FREQ_HSE : 0;        break; }
                case 0x03: { speed_CPU_C1 = IsClkSrcReady(CLKSRC_HSE) ? (FREQ_HSE >> 1) : 0; break; }
        }

		//PLL block
        //CPU_C2_SEL[2]: 1/0 -> pllCPUo/CPU_C1
        if (RST_CLK->CPU_CLOCK & PN2)
        {
				//PLL_CPU_ON[2]: 1/0 -> PLL on/off
                if (RST_CLK->PLL_CONTROL & PN2) 
                {
						//PLL_CPU_MUL[11..8]
                        int pll_CPU_MUL = (RST_CLK->PLL_CONTROL & 0x0F00) >> 8;
                        speed_PLLCPUo = speed_CPU_C1 * (pll_CPU_MUL + 1);
                }

				//CPU_C2 = PLL out
                speed_CPU_C2 = speed_PLLCPUo;
        }
        else
        {
				//CPU_C2 = CPU_C1
                speed_CPU_C2 = speed_CPU_C1;
        }

        //Division block
		//CPU_C3_SEL[7..4]: [7,8,9..15] = [/1,/2,/4../256]
        divc = (RST_CLK->CPU_CLOCK & 0xF0) >> 4;

        if (divc < 7)
                divc = 7;

		//CPU_C3 = CPU_C2 / (2 ^ CPU_C3_SEL)
        speed_CPU_C3 = (u32)(speed_CPU_C2 / (float)(1 << (divc - 7)));

        return speed_CPU_C3;
}



/* Gets CPU identifier */
u32 GetCPUID()
{
	return (u32)(*((__io_reg*)0xE000ED00));
}



/* */
s32 CPUSetOption(u8 cpu, u32 oid, const void *ival)
{
	switch(oid)
	{
		case CPU_OID_SPEED:
		{
			if (SetHCLK(*((u32*)ival), cpuCurrClkSrc) == MCR_OK)
				return 4;
			else
				return -1;
		}

		case CPU_OID_CPUID:	return 0;

		//Unknown option: return error
		default: return -1;
	}
}



/* */
s32 CPUGetOption(u8 cpu, u32 oid, void *oval)
{
	switch(oid)
	{
		case CPU_OID_SPEED:
		{
			*((u32*)oval) = GetHCLK();
			return 4;
		}

		case CPU_OID_CPUID:
		{
			*((u32*)oval) = GetCPUID();
			return 4;
		}

		//Unknown option: return error
		default: return -1;
	}
}

/*===========================================================================
 * Port functions
 *===========================================================================*/

//Array of port addresses
static const u32 portAddr[6] =
{
    BASE_PORTA,	BASE_PORTB, BASE_PORTC, BASE_PORTD, BASE_PORTE, BASE_PORTF
};

//Clock bit indexes (PER_CLOCK register)
static const u8 portClkNums[] =
{
    CLK_PORTA, CLK_PORTB, CLK_PORTC, CLK_PORTD, CLK_PORTE, CLK_PORTF
};

#define get_port(n)		((_port*)portAddr[n])
#define get_port_clknum(n)	portClkNums[n]


//INFILTER  CTRLDRIVER  SHMIT  PDOWN  PUP  POWER/INPUT   FUNC
//    [13]    [12..11]   [10]    [9]  [8]       [7..4] [3..0]
#define PN_CTRLDRIVER_SHIFT	(11)
#define PN_PWR_SHIFT		(4)
#define PN_FUNC_SHIFT		(0)

#define PN_CTRLDRIVER_MASK	(0x03 << PN_CTRLDRIVER_SHIFT)
#define PN_PWR_MASK		(0x0F << PN_PWR_SHIFT)
#define PN_FUNC_MASK		(0x0F << PN_FUNC_SHIFT)


/* Initialize port */
MCRESULT InitializePORTEx(PORTBLOCK port, u32 pinmask, u32 pinval, u32 opts)
{
    _port *reg;    
    u32 shift, mask, func, pwr;
    u32 regRXTX, regOE, regFUNC, regANALOG;
    u32 regPULL, regPD, regPWR,  regGFEN;

    if (port > PORT_F)
	return MCR_INVALID_PORT;

    reg = get_port(port);

    ClkEnable(get_port_clknum(port));

    //Load registers
    regRXTX   = reg->RXTX;
    regOE     = reg->OE;
    regFUNC   = reg->FUNC;
    regANALOG = reg->ANALOG;
    regPULL   = reg->PULL;
    regPD     = reg->PD;
    regPWR    = reg->PWR;
    regGFEN   = reg->GFEN;

    //Each port has 16 pins
    pinmask &= 0xFFFFU;

    shift = 0;
    mask  = pinmask;
    func  = ((opts & PN_FUNC_MASK) >> PN_FUNC_SHIFT) & 0x03;
    pwr   = ((opts & PN_PWR_MASK)  >> PN_PWR_SHIFT)  & 0x03;

    //Set FUNC and PWR for selected pins
    while(shift < 32)
    {
	if(mask & 0x01)
	{
	    regFUNC = (regFUNC & ~(0x03 << shift)) | (func << shift);
	    regPWR  = (regPWR  & ~(0x03 << shift)) | (pwr  << shift);
	}

	mask >>= 1;
	shift += 2;
    }

    //ANALOG = 1/0 (digital/analog)
    if ((opts & PN_FUNC_MASK) == PN_ANALOG)
	regANALOG &= ~pinmask; //analog
    else
	regANALOG |= pinmask;  //digital

    //OE = 1/0 (output/input)
    if ((opts & PN_PWR_MASK) == PN_INPUT)
    {
	//Input
	regOE &= ~pinmask;

	//GFEN[15..0] = 1/0 (Input filter ON/OFF)
	if (opts & PN_INFILTER)
	    regGFEN |=  pinmask;
	else
	    regGFEN &= ~pinmask;
    }
    else
    {
	//Output
	regOE |= pinmask;

	//No input filter
	regGFEN &= ~pinmask;

	//Set pins initial values
	regRXTX = (regRXTX & ~(pinmask)) | (pinval & pinmask);
    }

    //PULL[31..16] = 1/0 (pull up / no pull up)
    if (opts & PN_PULLUP)
	regPULL |=  (pinmask << 16);
    else
	regPULL &= ~(pinmask << 16);

    //PULL[15..0] = 1/0 (pull down / no pull down)
    if (opts & PN_PULLDOWN)
	regPULL |=  pinmask;
    else
	regPULL &= ~pinmask;

    //PD[31..16] = 1/0 (ON/OFF Shmit trigger)
    if (opts & PN_SHMIT)
	regPD |=  (pinmask << 16);
    else
	regPD &= ~(pinmask << 16);

    //PD[15..0] =  1/0 (Open drain / Controlled driver)
    if ((opts & PN_CTRLDRIVER_MASK) == PN_OPENDRAIN)
	regPD |=  pinmask;
    else
	regPD &= ~pinmask;

    //Save registers
    reg->RXTX   = regRXTX;
    reg->OE     = regOE;
    reg->FUNC   = regFUNC;
    reg->ANALOG = regANALOG;
    reg->PULL   = regPULL;
    reg->PD     = regPD;
    reg->PWR    = regPWR;
    reg->GFEN   = regGFEN;

    return MCR_OK;
}



/* Turn port */
void TurnPORT(PORTBLOCK port, u8 turn)
{
    if (port > PORT_F)
	return;

    if (turn)
	ClkEnable(get_port_clknum(port));
    else
	ClkDisable(get_port_clknum(port));
}


//Indicates that value has zero value (char '0' or 0)
#define IsValOFF(v)		((v) == 0x00 || (v) == 0x30)

/* */
s32 PORTSetOption(PORTBLOCK port, u32 oid, const void *ival)
{
	PORTOPT *opt = (PORTOPT*)ival;
	return PORTSetOptionE(port, (PINMASK)opt->mask, oid, &opt->value);
}



/* */
s32 PORTSetOptionE(PORTBLOCK port, PINMASK pins, u32 oid, const void *ival)
{
	_port *reg;
	u8 val;

	if (port >= PORT_COUNT)
		 return -1;

	reg = get_port(port);
	val = *((u8*)ival);

	//Notes
	//FUNC and PWR: 2 bits for value
	//ANALOG = 1/0 (digital/analog)
	//OE = 1/0 (output/input)
	//PULL[31..16] - pull up;       [15..0] - pull down
	//PD  [31..16] - Shmit trigger; [15..0] - Open drain/Controlled driver

	switch(oid)
	{
		case PORT_OID_RXTX:
		{
			if (IsValOFF(val))
				reg->RXTX &= ~pins;
			else
				reg->RXTX |= pins;
			return 2;
		}

		case PORT_OID_TURN:
		{
			if (IsValOFF(val))
				ClkDisable(get_port_clknum(port));
			else
				ClkEnable(get_port_clknum(port));
			return 1;
		}


		case PORT_OID_OUTPUT:
		{
			if (IsValOFF(val))
				reg->OE &= ~pins;
			else
				reg->OE |= pins;
			return 2;
		}	


		case PORT_OID_DIGITAL:
		{
			if (IsValOFF(val))
				reg->ANALOG &= ~pins;
			else
				reg->ANALOG |= pins;
			return 2;
		}


		case PORT_OID_PULLUP:
		{
			if (IsValOFF(val))
				reg->PULL &= ~((u32)pins << 16);
			else
				reg->PULL |= ((u32)pins << 16);
			return 2;
		}


		case PORT_OID_PULLDOWN:
		{
			if (IsValOFF(val))
				reg->PULL &= ~pins;
			else
				reg->PULL |= pins;
			return 2;
		}


		case PORT_OID_SHMIT:
		{
			if (IsValOFF(val))
				reg->PD &= ~((u32)pins << 16);
			else
				reg->PD |= ((u32)pins << 16);
			return 2;
		}


		case PORT_OID_OPENDRAIN:
		{
			if (IsValOFF(val))
				reg->PD &= ~pins;
			else
				reg->PD |= pins;
			return 2;
		}


		case PORT_OID_INFILTER:
		{
			if (IsValOFF(val))
				reg->GFEN &= ~pins;
			else
				reg->GFEN |= pins;
			return 2;
		}


		case PORT_OID_FUNC:
		{
			u32 shift = 0;

			if (val & PN_ANALOG)
				reg->ANALOG &= ~pins;
			else
				reg->ANALOG |= pins;

			val &= 0x03;
			while(shift < 32)
			{
				if (pins & 1)
					reg->FUNC = (reg->FUNC & ~(0x03 << shift)) | (val << shift);

				pins >>= 1;
				shift += 2;
			}

			return 4;
		}


		case PORT_OID_PWR:
		{
			u32 shift = 0;

			val = (val >> 4) & 0x03;

			if (val)
				reg->OE |= pins;
			else
				reg->OE &= ~pins;

			while(shift < 32)
			{
				if(pins & 1)
					reg->PWR = (reg->PWR & ~(0x03 << shift)) | (val << shift);

				pins >>= 1;
				shift += 2;
			}

			return 4;
		}


		//Unknown option: return error
		default: return -1;
	}
}



/* */
s32 PORTGetOption(PORTBLOCK port, u32 oid, void *oval)
{
	_port *reg;

	if (port >= PORT_COUNT)
		 return -1;

	reg = get_port(port);

	//Notes
	//FUNC and PWR: 2 bits for value
	//ANALOG = 1/0 (digital/analog)
	//PULL[31..16] - pull up;       [15..0] - pull down
	//PD  [31..16] - Shmit trigger; [15..0] - Open drain/Controlled driver

	switch(oid)
	{
		case PORT_OID_RXTX:	 *((PINVALUE*)oval) = (PINVALUE)reg->RXTX;         return 2;

		case PORT_OID_TURN:	 *((u8*)oval) = IsClkEnable(get_port_clknum(port));	return 1;

		case PORT_OID_OUTPUT:    *((PINVALUE*)oval) = (PINVALUE)reg->OE;           return 2;

		case PORT_OID_DIGITAL:   *((PINVALUE*)oval) = (PINVALUE)reg->ANALOG;       return 2;

		case PORT_OID_PULLUP:    *((PINVALUE*)oval) = (PINVALUE)(reg->PULL >> 16); return 2;

		case PORT_OID_PULLDOWN:  *((PINVALUE*)oval) = (PINVALUE)reg->PULL;         return 2;

		case PORT_OID_SHMIT:     *((PINVALUE*)oval) = (PINVALUE)(reg->PD >> 16);   return 2;

		case PORT_OID_OPENDRAIN: *((PINVALUE*)oval) = (PINVALUE)reg->PD;           return 2;

		case PORT_OID_INFILTER:  *((PINVALUE*)oval) = (PINVALUE)reg->GFEN;         return 2;

		case PORT_OID_FUNC:	 *((u32*)oval) = (u32)reg->FUNC; return 4;

		case PORT_OID_PWR:	 *((u32*)oval) = (u32)reg->PWR;	 return 4;

		//Unknown option: return error
		default: return -1;
	}
}



/* */
s32 PORTSetPull(PORTBLOCK port, PINMASK pins, u8 up, u8 down)
{
        _port *reg;

	if (port > PORT_F)
	    return -1;

	reg = get_port(port);

	//PULL [31..16] = 1/0 (pull up / no pull up)
	if (up)
                reg->PULL |= ((u32)pins << 16);
        else
                reg->PULL &= ~((u32)pins << 16);

	//PULL [15..0] = 1/0 (pull down / no pull down)
	if (down)
                reg->PULL |= pins;
        else
                reg->PULL &= ~pins;

		return 0;
}



/* Read data from port */
s32 PORTRead(PORTBLOCK port, u8 *buffer, s32 count)
{
	_port *reg = get_port(port);
	u8 *end;
	u16 val;

	//MCU has 16-bit port: count must aligned by 2
	count &= ~1;
	end = &buffer[count];

	while(buffer != end)
	{
		//Read port
		val = reg->RXTX;

		//Copy port data to buffer
		*buffer++ = (u8)val;
		*buffer++ = (u8)(val >> 8);
	}

	return count;
}



/* Write data to port */
s32 PORTWrite(PORTBLOCK port, const u8 *buffer, s32 count)
{
	_port *reg = get_port(port);
	const u8 *end;

	//MCU has 16-bit port: count must aligned by 2
	count &= ~1;
	end = &buffer[count];

	while(buffer != end)
	{
		//Write port
		reg->RXTX = (buffer[1] << 8) | buffer[0];
		buffer += 2;
	}

	return count;
}

/*===========================================================================
 * External bus functions
 *===========================================================================*/

/* */
MCRESULT InitializeEBus(EBusConfig *cfg)
{
		//Enable clock
        ClkEnable(CLK_EXTBUS);

		//Select RAM by default
		EBusSelectMemory(EXTBUS_RAM);

        return MCR_OK;
}



/* */
void EBusSetClock(u32 ws)
{
        u16 tmp;		

		if (ws < 1)
			ws = 1;
		else if (ws > 15)
			ws = 15;

        //CONTROL: WAIT_STATE[15..12], values at [0..15]
        tmp = EXTBUS->CONTROL;
        tmp = (tmp & 0x0FFF) | ((ws & 0x0F) << 12);
        EXTBUS->CONTROL = tmp;
}



/* */
void EBusSetClockEx(u32 addr, u32 wsSetup, u32 wsActive, u32 wsHold)
{
        #if (TARGET_MCU == MCU_1986BE1x ||\
            (TARGET_MCU == MCU_1986BE9x && TARGET_MCUM == MCUM_1986BE94x))

        u32 ramcyc;

        #if (TARGET_MCU == MCU_1986BE9x && TARGET_MCUM == MCUM_1986BE94x)

        //Only 1986BE94x
        //EXT_BUS_MODE[9]: 1/0 - enable/disable tuning of memory region
        u32 tmp = EXTBUS->CONTROL;
        tmp |= PN9;
        EXTBUS->CONTROL = tmp;

        #endif

		//Check values
		if (wsHold   > 7)   wsHold   = 7;
		if (wsSetup  > 7)   wsSetup  = 7;
		if (wsActive > 127) wsActive = 127;

		//RAM_CyclesX:
		//WS_HOLD[13..11], WS_SETUP[10..8], WS_ACTIVE[7..1], ENABLE[0] = 1
        ramcyc = (wsHold << 11) | (wsSetup << 8) | (wsActive << 1) | 1;

        if (addr >= 0x10000000U && addr <= 0x1FFFFFFFU)
                EXTBUS->RAM_Cycles1 = ramcyc;
        else if (addr >= 0x50000000U && addr <= 0x5FFFFFFFU)
                EXTBUS->RAM_Cycles2 = ramcyc;
        else if (addr >= 0x60000000U && addr <= 0x6FFFFFFFU)
                EXTBUS->RAM_Cycles3 = ramcyc;
        else if (addr >= 0x70000000U && addr <= 0xDFFFFFFFU)
                EXTBUS->RAM_Cycles4 = ramcyc;
		else
				EBusSetClock(wsSetup + wsActive + wsHold);

        /* TARGET_MCU == MCU_1986BE1x || TARGET_MCUM == MCUM_1986BE94x  */
        #else

		EBusSetClock(wsSetup + wsActive + wsHold);

        #endif /* TARGET_MCU != MC_1986BE1x */
}



/* */
void EBusSetTime(u32 ws_ns)
{
        u32 hclkMHz = (u32)(GetHCLK() / 1000000);

		EBusSetClock(CalcClocks(ws_ns, hclkMHz));
}



/* */
void EBusSetTimeEx(u32 addr, u32 wsSetup_ns, u32 wsActive_ns, u32 wsHold_ns)
{
        u32 hclkMHz = (u32)(GetHCLK() / 1000000);

        EBusSetClockEx(addr,
                       CalcClocks(wsSetup_ns, hclkMHz),
                       CalcClocks(wsActive_ns, hclkMHz),
                       CalcClocks(wsHold_ns, hclkMHz));
}



/* */
void EBusSelectMemory(u32 selmem)
{
        u16 tmp;

        //CONTROL: NAND[2], RAM[1], ROM[0]
        tmp = EXTBUS->CONTROL;
        tmp = (tmp & 0xFFF8) | (1 << (selmem & 0x03));
        EXTBUS->CONTROL = tmp;
}


/*===========================================================================
 * Synchronization functions: currently not implemented
 *===========================================================================*/
/*
#define SEM_FREE                0 //Indicating that semaphore is free
#define SEM_LOCK                1 //Indicating that semaphore is locked
#define SEM_FAIL                2 //Trying lock was fail

#define IsSemLock(sem)       (sem == SEM_LOCK)
#define SetSemLock(sem, val) sem = val
#define SemLock(sem)         while(IsSemLock(sem)); SetSemLock(sem, SEM_LOCK)
#define SemUnlock(sem)       SetSemLock(sem, SEM_FREE)
*/

/* */
//u32 TryLock(u32 *sem);

/* */
//u32 TryUnlock(u32 *sem);

/* */
//void Lock(u32 *sem);

/* */
//void Unlock(u32 *sem);

/*
u32 TryLock(u32 *sem)
{
        if (__ldrex(sem) == (u32)SEM_FREE)
        {
                if (__strex(SEM_LOCK, sem) == (s32)0)
                        return (u32)SEM_LOCK;
                else
                        return (u32)SEM_FAIL;
        }
        else
                return (u32)SEM_FAIL;
}



u32 TryUnlock(u32 *sem)
{
       if (__strex((u32)SEM_FREE, sem) == (s32)0)
                return (u32)SEM_FREE;
       else
                return (u32)SEM_FAIL;
}


void Lock(u32 *sem)
{
        while(TryLock(sem) == SEM_FAIL);
}

void Unlock(u32 *sem)
{
        while(TryUnlock(sem) == SEM_FAIL);
} */

/*===========================================================================
 * End of file 1986BE9x_CPU.c
 *===========================================================================*/

