/*===========================================================================
 *
 *  MCU library
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2014
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File 1986BE9x_CAN.c: CAN functions.
 *
 *===========================================================================*/

#include "../../inc/CAN.h"

#define GetCAN(__x)      ((_can*)(BASE_CAN1 + (u32)(0x00008000 * (__x & 1))))


typedef struct tag_CANClkParams
{
        //Input params
        u32 speed;

        //Output params
        u32 ospeed;
        u32 brg;
        float divider;
} CANClkParams;


//Configuration CAN1
CANConfig cfgCAN1;

//Configuration CAN2
CANConfig cfgCAN2;



static MCRESULT TuneCANClkParams(CANClkParams *p)
{
        u32 hclk, brg;
        u32 canClk;
        u8 isFound = 0;

        if (!p)
                return MCR_SETUPCLK_FAIL;

        hclk = GetHCLK();
        brg = 0;
        while(brg < 8)
        {
                canClk = hclk / (1 << brg);

                brg++;
        }


        return isFound ? MCR_OK : MCR_SETUPCLK_FAIL;
}


#define CANx_PORTRXTX_CONFIG    PN_NOPULLUP, PN_NOPULLDOWN, PN_NOSHMIT,\
                                PN_CTRLDRIVER, PN_NOINFILTER

#define CANRxTx(port,func,prx,ptx) portRxTx = port; pinFunc = func;\
                                   pinRx = prx; pinTx = ptx;

static MCRESULT CANMapPortPins(CANConfig *cfg)
{
        PORTBLOCK portRxTx;
        PINVALUE pinRx, pinTx;
        u8 pinFunc;
        u8 isRxTxDefined = 1;

        switch(cfg->can)
        {
                //Select mapping at 1986BE9x
                #if TARGET_MCU == MCU_1986BE9x
                case CAN_N1:
                {
                        switch(cfg->mapRxTx)
                        {
                                case CAN1_MAP_RXTX_C9C8:
                                {
                                        CANRxTx(PORT_C, PN_BASIC, PN9, PN8);
                                        break;
                                }

                                case CAN1_MAP_RXTX_A7A6:
                                {
                                        CANRxTx(PORT_A, PN_ALTER, PN7, PN6);
                                        break;
                                }

                                case CAN1_MAP_RXTX_B3B2:
                                {
                                        CANRxTx(PORT_B, PN_REMAP, PN3, PN2);
                                        break;
                                }

                                case CAN1_MAP_RXTX_D14D13:
                                {
                                        CANRxTx(PORT_D, PN_REMAP, PN14, PN13);
                                        break;
                                }

                                case CAN1_MAP_RXTX_E0E1:
                                {
                                        CANRxTx(PORT_E, PN_REMAP, PN0, PN1);
                                        break;
                                }

                                default:
                                {
                                        isRxTxDefined = 0;
                                        break;
                                }
                        } //switch(cfg->mapRxTx)


                        break;
                } //case CAN_N1


                case CAN_N2:
                {
                        switch(cfg->mapRxTx)
                        {
                                case CAN2_MAP_RXTX_D15D9:
                                {
                                        CANRxTx(PORT_D, PN_BASIC, PN15, PN9);
                                        break;
                                }

                                case CAN2_MAP_RXTX_E6E7:
                                {
                                        CANRxTx(PORT_E, PN_ALTER, PN6, PN7);
                                        break;
                                }

                                case CAN2_MAP_RXTX_C14C15:
                                {
                                        CANRxTx(PORT_C, PN_REMAP, PN14, PN15);
                                        break;
                                }

                                case CAN2_MAP_RXTX_F2F3:
                                {
                                        CANRxTx(PORT_F, PN_REMAP, PN2, PN3);
                                        break;
                                }

                                default:
                                {
                                        isRxTxDefined = 0;
                                        break;
                                }
                        } //switch(cfg->mapRxTx)

                        break;
                } // case CAN_N2
                //End selection

                //Select mapping at 1986BE1x
                #elif TARGET_MCU == MCU_1986BE1x

                case CAN_N1:
                {
                        switch(cfg->mapRxTx)
                        {
                                case CAN1_MAP_RXTX_E11E12:
                                {
                                        CANRxTx(PORT_E, PN_BASIC, PN11, PN12);
                                        break;
                                }

                                case CAN1_MAP_RXTX_C9C10:
                                {
                                        CANRxTx(PORT_C, PN_REMAP, PN9, PN10);
                                        break;
                                }

                                default:
                                {
                                        isRxTxDefined = 0;
                                        break;
                                }
                        } //switch(cfg->mapRxTx)


                        break;
                } //case CAN_N1


                case CAN_N2:
                {
                        switch(cfg->mapRxTx)
                        {
                                case CAN2_MAP_RXTX_E13E14:
                                {
                                        CANRxTx(PORT_E, PN_BASIC, PN13, PN14);
                                        break;
                                }

                                case CAN2_MAP_RXTX_C11C12:
                                {
                                        CANRxTx(PORT_C, PN_REMAP, PN11, PN12);
                                        break;
                                }

                                default:
                                {
                                        isRxTxDefined = 0;
                                        break;
                                }
                        } //switch(cfg->mapRxTx)

                        break;
                } // case CAN_N2
                //End selection

                #endif

                default:
                        return MCR_INVALID_MAPPING;

        } //switch


        if (isRxTxDefined)
        {
                //Initialize port pins
                //RXD PIN
                InitializePORTEx(portRxTx, pinRx, 0, pinFunc, PN_INPUT,
                                CANx_PORTRXTX_CONFIG);

                //TXD PIN
                InitializePORTEx(portRxTx, pinTx, 0, pinFunc, PN_PWR_FASTEST,
                                CANx_PORTRXTX_CONFIG);
        }

        return isRxTxDefined ? MCR_OK : MCR_INVALID_MAPPING;
}

#undef CANRxTx


//Initialize CAN
MCRESULT InitializeCAN(CANConfig *cfg)
{
        _can *reg;
        CANConfig *destCfg;
        CANClkParams p;

        if (!cfg)
               return MCR_INVALID_CFG;

        switch(cfg->can)
        {
                case CAN_N1: destCfg = &cfgCAN1; break;
                case CAN_N2: destCfg = &cfgCAN2; break;
                default: return MCR_INVALID_PORT;
        }

        //Check speed
        if (cfg->speed > CAN_MAX_SPEED)
                cfg->speed = CAN_MAX_SPEED;

        //Tune clock parameters
        p.speed = cfg->speed;
        if (TuneCANClkParams(&p) != MCR_OK)
                return MCR_SETUPSPEED_FAIL;

        //Map control Rx/Tx pins
        if (CANMapPortPins(cfg) != MCR_OK)
                return MCR_INVALID_MAPPING;

        //Copy configuration
        *destCfg = *cfg;

        //Get registers
        //reg = GetCAN(cfg->can);

        //Enable block clock
        switch(cfg->can)
        {
                //Enable CAN1 clock
                case CAN_N1: ClkEnable(CLK_CAN1); break;

                //Enable CAN2 clock
                case CAN_N2: ClkEnable(CLK_CAN2); break;
        } //switch

        //Turn OFF CAN
        TurnCAN(cfg->can, 0);

        //Configure registers here
        //........................

        if (cfg->received)
        {
                //Set IMSC

                switch(cfg->can)
                {
                        case CAN_N1: IRQEnable(IRQ_CAN1); break;
                        case CAN_N2: IRQEnable(IRQ_CAN2); break;
                }
        }
        else
        {
                switch(cfg->can)
                {
                        case CAN_N1: IRQDisable(IRQ_CAN1); break;
                        case CAN_N2: IRQDisable(IRQ_CAN2); break;
                }
        }

        return MCR_OK;
}



//Turn CAN
void TurnCAN(CANBLOCK can, u8 turn)
{
        //_can *reg = GetCAN(can);

      /*  if (turn)
        {}
        else
        {}*/
}



//Turn CAN transmitter
void TurnCANTx(CANBLOCK can, u8 turn)
{
        //_can *reg = GetCAN(can);

        /*if (turn)
        {}
        else
        {}*/
}



//Read data from input fifo
u32 CANRead(CANBLOCK can, canrx_t *buffer, u32 count)
{
        //_can *reg = GetCAN(can);
        u32 i = 0;

        while(i < count)
        {
                buffer[i] = 0;
                i++;
        }

        return i;
}



//Write data to output fifo
u32 CANWrite(CANBLOCK can, cantx_t *buffer, u32 count)
{
        //_can *reg = GetCAN(can);
        u32 i = 0;

        while(i < count)
        {
                i++;
        }

        return i;
}



void CANx_IRQHandler(_can *reg, CANConfig *cfg)
{
        //Check interrupt bits
        //Reset interrupt bits
        //Read from fifo
        //Call handler
}


u32 can1IRQCounter = 0;
u32 can2IRQCounter = 0;

void CAN1_IRQHandler(void)
{
        can1IRQCounter++;
        CANx_IRQHandler(CAN1, &cfgCAN1);
}


void CAN2_IRQHandler(void)
{
        can2IRQCounter++;
        CANx_IRQHandler(CAN2, &cfgCAN2);
}

/*===========================================================================
 * End of file 1986BE9x_CAN.c
 *===========================================================================*/

