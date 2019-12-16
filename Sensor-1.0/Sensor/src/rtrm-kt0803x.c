/*===========================================================================
 *
 *  Sensor controller
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2019
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File rtrm-kt0803x.c. Monolithic Digital Stereo FM Transmitter KT0803x.
 *                       I2C interface.
 *                       Vdd = 1.6-3.6V.
 *                       Range = 70-108Mhz.
 *                       Freq step: 50KHz.
 *  Notes
 *   1. Discovered random read bug at KT0803L. It's recommended read
 *      registers from 0x00 to to N register, otherwise possible read
 *      0x00 register value instead N register value.
 *
 *===========================================================================*/

#include "sensor.h"

//Debug messages (comment to disable)
#define PrintMsg(...)       std_printf(__VA_ARGS__)


//KT0803L registers
//CHSEL[11..0]: frequency at 50kHz units (50kHz per LSB)
#define KT0803_CHSEL_LSB       50

//Register 0x00
//bits[7..0]: CHSEL[8..1]

//Register 0x01
//bits[7..6]: RFGAIN[1..0] - bits 1 and 0 of RFGAIN[3..0]
//bits[5..3]: PGA[2..0] - PGA Gain control
//            7   12dB
//            6   8dB
//            5   4dB
//            4   0dB
//            3 -12dB
//            2  -8dB
//            1  -4dB
//            0   0dB
//bits[2..0]: CHSEL[11..9]

//Register 0x02
//bit[7]: CHSEL[0]  - LSB of CHSEL
//bit[6]: RFGAIN[3] - MSB of RFGAIN
//bit[3]: MUTE - Software Mute
//        0 - MUTE Disabled
//        1 - MUTE Enabled
//bit[2]: PLTADJ - Pilot Tone Amplitude Adjustment
//        0 - Amplitude low
//        1 - Amplitude high
//bit[0]: PHTCNST -  Pre-emphasis Time-Constant Set
//        0 - 75us, USA
//        1 - 50us, Europe
#define KT0803_PHTCNST_USA        (0 << 0)
#define KT0803_PHTCNST_EUROPE     (1 << 0)

//Register 0x04
//bit[7]: ALC_EN
//        0 - Disable ALC
//        1 - Enable ALC
//bit[6]: MONO
//        0 - Stereo
//        1 - Mono
//bits[5..4]: PGA_LSB[1..0]
//bits[1..0]: BASS[1..0] Bass Boost Control
//            0 - Disabled
//            1 - 5dB
//            2 - 11dB
//            3 - 17dB
#define KT0803_ALC_ENABLE (1 << 7)
#define KT0803_STEREO     (0 << 6)
#define KT0803_MONO       (1 << 6)

//Register 0x0B
//bit[7]: Standby - Chip Standby Control Bit
//        0 - Normal operation (work mode)
//        1 - Standby
//bit[5]: PDPA - Power Down Power Amplifier
//        0 - Power on
//        1 - Power down
//bit[2]: AUTO_PADN - Automatic Power Down Power Amplifier When Silence is Detected
//        0 - Disable
//        1 - Enable
#define KT0803_CHIP_WORKMODE      (0 << 7)
#define KT0803_CHIP_STANDBY       (1 << 7)
#define KT0803_PDPA_ON            (0 << 5)
#define KT0803_PDPA_DOWN          (1 << 5)
#define KT0803_AUTO_PADN_OFF      (0 << 2)
#define KT0803_AUTO_PADN_ON       (1 << 2)

//Register 0x0C
//bits[7..4]: ALC_DECAY_TIME[3:0] - ALC Decay Time Selection
//             [0..7]  => [25..200]us, step 25us
//             [8..15] => [50..400]ms, step 50ms
//bits[3..0]: ALC_ATTACK_TIME[3:0] - ALC Attack Time Selection
//             [0..7]  => [25..200]us, step 25us
//             [8..15] => [50..400]ms, step 50ms

//Register 0x0E
//bit[1]: PA_BIAS - PA Bias Current Enhancement.
//        0 - Disable
//        1 - Enable
#define KT0803_PA_BIAS_ENABLE       (1 << 1)

//Register 0x0F (read-only)
//bit[4]: PW_OK  - Power OK Indicator
//bit[2]: SLNCID - 1 when silence is detected
#define KT0803_FLAG_PW_OK           (1 << 4)
#define KT0803_FLAG_SLNCID          (1 << 2)

//Register 0x10
//bit[0]: PGAMOD - PGA Mode Selection
//        0 - 4dB step
//        1 - 1dB step with Reg04.PGA_LSB[1..0] used
#define KT0803_PGAMOD_STEP_4dB      (0 << 0)
#define KT0803_PGAMOD_STEP_1dB      (1 << 0)

//Register 0x12
//bit[7]: SLNCDIS - Silence Detection Disable
//        0 - Enable
//        1 - Disable
//bits[6..4]: SLNCTHL - Silence Detection Low Threshold
//            0 -  0.25mv
//            1 -  0.50mv
//            2 -  1.00mv
//            3 -  2.00mv
//            4 -  4.00mv
//            5 -  8.00mv
//            6 - 16.00mv
//            7 - 32.00mv
//bits[3..1]: SLNCTHH - Silence Detection High Threshold
//            0 -  0.5mv
//            1 -  1.0mv
//            2 -  2.0mv
//            3 -  4.0mv
//            4 -  8.0mv
//            5 - 16.0mv
//            6 - 32.0mv
//            7 - 64.0mv
//bits[0]: SW_MOD - Switching Channel Mode Selection
//         0 - MUTE when changing channel
//         1 - Power amplifier OFF when changing channel
#define KT0803_SLNCDIS_DISABLE    (1 << 7)
#define KT0803_SW_MOD_MUTE        (0 << 0)
#define KT0803_SW_MOD_PAOFF       (1 << 0)

//Register 0x13
//bit[7]: RFGAIN[2] - bit 2 of RFGAIN[3..0]
//bit[2]: PA_CTRL - Power amplifier structure selection
//        0 - Internal power supply, KT0803 compatible
//        1 - External power supply via external inductor
//        Note: When an external inductor is used, this bit
//              must be set to 1 immediately after the Reg0F.PW_OK=1
//              Otherwise, the device may be destroyed!
#define KT0803_PA_CTRL_INTERNAL     (0 << 2)
#define KT0803_PA_CTRL_EXTERNAL     (1 << 2)

//Register 0x14
//bits[7..5]: SLNCTIME[2..0] - Silence Detection Low Level and High Level Duration Time
//            0 - 50ms  (16s if SLNCTIME[3]=1)
//            1 - 100ms (24s if SLNCTIME[3]=1)
//            2 - 200ms (32s if SLNCTIME[3]=1)
//            3 - 400ms (40s if SLNCTIME[3]=1)
//            4 - 1s    (48s if SLNCTIME[3]=1)
//            5 - 2s    (56s if SLNCTIME[3]=1)
//            6 - 4s    (60s if SLNCTIME[3]=1)
//            7 - 8s    (64s if SLNCTIME[3]=1)
//bits[4..2]: SLNCCNTHIGH[2..0] - Silence Detection High Level Counter Threshold
//            0 - 15
//            1 - 31
//            2 - 63
//            3 - 127
//            4 - 255
//            5 - 511
//            6 - 1023
//            7 - 2047
//bit[0]: SLNCTIME[3] - Silence Detection Long Duration Time Enable
//        0 - Short duration time enable
//        1 - Long duration time enable


//Register 0x15
//bit[7..5]: ALCCMPGAIN[2..0] - ALC Compressed Gain Setting
//           100 = 06 (  6dB)
//           101 = 03 (  3dB)
//           110 = 00 (  0dB)
//           111 = 1D ( -3dB)
//           000 = 1A ( -6dB)
//           001 = 17 ( -9dB)
//           010 = 14 (-12dB)
//           011 = 11 (-15dB)


//Register 0x16
//bits[2..0]: SLNCCNTLOW[2..0] - Silence Low Counter
//            [0..7], Val = 2^SLNCCNTLOW

//Register 0x17
//bit[6]: FDEV - Frequency Deviation
//        0 - 75kHz deviation
//        1 - 112.5kHz deviation
//bit[5]: AU_ENHANCE - Audio Frequency Response Enable
//        0 - Disable
//        1 - Enable
//bit[3]: XTAL_SEL - Crystal Oscillator Selection
//        0 - 32.768kHz crystal
//        1 - 7.6MHz crystal
#define KT0803_FDEV_75kHz          (0 << 6)
#define KT0803_FDEV_112p5kHz       (1 << 6)
#define KT0803_AU_ENHANCE_ENABLE   (1 << 5)
#define KT0803_XTAL_SEL_32768Hz    (0 << 3)
#define KT0803_XTAL_SEL_7600kHz    (1 << 3)

//Register 0x1E
//bit[6]: DCLK - Multiple Reference Clock Selection Enable
//        0 - Select clock through SW1/SW2 pins
//        1 - Select clock through REF_CLK[3..0]
//bit[5]: XTALD - Crystal Oscillator Disable Control
//        0 - Enable crystal oscillator
//        1 - Disable crystal oscillator
//bits[3..0]: REF_CLK[3..0] - Reference Clock Selection
//            0 - 32.768kHz
//            1 -  6.5MHz
//            2 -  7.6MHz
//            3 - 12.0MHz
//            4 - 13.0MHz
//            5 - 15.2MHz
//            6 - 19.2MHz
//            7 - 24.0MHz
//            8 - 26.0MHz
//            9..15 - Reserved
#define KT0803_DCLK_SW1SW2      (0 << 6)
#define KT0803_DCLK_REFCLK      (1 << 6)
#define KT0803_XTALD_DISABLE    (1 << 5)
#define KT0803_REFCLK_32768Hz   (0 << 0)
#define KT0803_REFCLK_6p5MHz    (1 << 0)
#define KT0803_REFCLK_7p6MHz    (2 << 0)
#define KT0803_REFCLK_12MHz     (3 << 0)
#define KT0803_REFCLK_13MHz     (4 << 0)
#define KT0803_REFCLK_15p2MHz   (5 << 0)
#define KT0803_REFCLK_19p2MHz   (6 << 0)
#define KT0803_REFCLK_24MHz     (7 << 0)
#define KT0803_REFCLK_26MHz     (8 << 0)
#define KT0803_REFCLK_MASK      (15 << 0) //mask

//SW1 SW2 Chip Mode IOVDD     Clock Source
//0   0   Power Off 1.6-3.6V  N/A
//0   1   Power On  1.6-3.6V  12MHz
//1   0   Power On  1.6-3.6V  32.768KHz
//1   1   Power On  1.6-3.6V  7.6MHz


//Register 0x26
//bits[7..5]: ALCHOLD[2..0] - ALC Hold Time Selection
//            0 - 50ms
//            1 - 100ms
//            2 - 150ms
//            3 - 200ms
//            4 - 1s
//            5 - 5s
//            6 - 10s
//            7 - 15s
//bits[3..1]: ALCHIGHTH[2:0] - ALC High Threshold Selection
//            0 - 0.6
//            1 - 0.5
//            2 - 0.4
//            3 - 0.3
//            4 - 0.2
//            5 - 0.1
//            6 - 0.05
//            7 - 0.01

//Register 0x27
//bits[3..0]: ALCLOWTH[3:0] - ALC Low Threshold
//            0  - 0.25
//            1  - 0.2
//            2  - 0.15
//            3  - 0.1
//            4  - 0.05
//            5  - 0.03
//            6  - 0.02
//            7  - 0.01
//            8  - 0.005
//            9  - 0.001
//            10 - 0.0005
//            11 - 0.0001
//            12..15 - Reserved


//RFGAIN[3..0] => [0..15]; 15 by default
//RFGAIN  dBuV
//0        95.5
//1        96.5
//2        97.5
//3        98.2
//4        98.9
//5       100.0
//6       101.5
//7       102.8
//When Reg0E.PA_BIAS=0
//8       105.1
//9       105.6
//10      106.2
//11      106.5
//12      107.0
//13      107.4
//14      107.7
//15      108.0
//When Reg0E.PA_BIAS=1
//8       107.2
//9       108.0
//10      108.7
//11      109.5
//12      110.3
//13      111.0
//14      111.7
//15      112.5


/* KT0803: initialize control structure */
void InitializeKT0803(SensorCtrl *sen, const SensorConfig *cfg)
{
    memset(sen, 0, sizeof(SensorCtrl));

    //Save pointer to configuration
    sen->cfg = cfg;

    //Set functions
    sen->pfConfigure = &KT0803Configure;
    sen->pfWriteData = &KT0803WriteData;
    sen->pfReadData  = &KT0803ReadData;
    sen->pfToString  = &KT0803ToString;
}



/* KT0803: write 8-bit register.
   According KT0803L datasheet, p 3.1:
   "A write operation requires an 8-bit register address following
    the device address word and acknowledgment. Upon receipt of this
    address, the KT0803L will again respond with a "0" and then clock
    in the 8-bit register data. Following receipt of the 8-bit register
    data, the KT0803L will output a "0" and the addressing device, such
    as a microcontroller, MUST terminate the write sequence with a stop
    condition." */
static s32 KT0803WriteRegister(SensorCtrl *sen, u8 regaddr, u8 regval)
{
    s32 r;

    if ((r = I2CWriteRegister(I2C_N1, KT0803_CADDR0, regaddr, &regval, 1)) == 1)
    {
        WaitIntervalMs(100);
    }

    return r;
}



/* KT0803: wait power flag (PW_OK=1) */
s32 KT0803WaitPowerFlag(SensorCtrl *sen)
{
    //Count of retries and retry interval, msec
    #define RETRY_COUNT         15
    #define RETRY_INTERVAL      200
    //Recommended! Read registers from 0x00 to 0x0F
    #define KT0803x_REGCOUNT    (0x0F + 1)
    s32 r;
    u32 retrycnt;
    u8 regs[KT0803x_REGCOUNT];

    retrycnt = RETRY_COUNT;
    while (retrycnt--)
    {
        if ((r = I2CReadRegister(I2C_N1, KT0803_CADDR0,
                                 0x00, regs, KT0803x_REGCOUNT)) == KT0803x_REGCOUNT)
        {
            //Reg0F.PW_OK=1 - power OK, return 0 (success)
            if (regs[0x0F] & KT0803_FLAG_PW_OK)
                return 0;
            else                            
                r = I2C_RESULT_CRCERROR;           
        }        

        WaitIntervalMs(RETRY_INTERVAL);
    }

    return r;
    #undef KT0803x_REGCOUNT
    #undef RETRY_COUNT
    #undef RETRY_INTERVAL
}



/* KT0803: turn to work/standby mode */
s32 KT0803Turn(SensorCtrl *sen, u8 on)
{
    s32 r;
    u8 reg0B;

    //Standby[7]: 0/1 - work/standby mode
    if (on)
        reg0B = KT0803_CHIP_WORKMODE | KT0803_PDPA_ON | KT0803_AUTO_PADN_OFF;
    else
        reg0B = KT0803_CHIP_STANDBY  | KT0803_PDPA_ON | KT0803_AUTO_PADN_OFF;

    if ((r = KT0803WriteRegister(sen, 0x0B, reg0B)) == 1)
    {
        WaitIntervalMs(500);
        return 0;
    }
    else
        return r;
}



/* KT0803: Write configuration */
s32 KT0803Tune(SensorCtrl *sen,
                     s32 fkHz,
                     u8 rfgainCode,
                     u8 pgaCode,
                     u8 phtCnst)
{
    s32 r = I2C_RESULT_CRCERROR;
    u8 regs[4];

    //Get frequency code
    fkHz /= KT0803_CHSEL_LSB;

    //Reg00
    //bits[7..0]: CHSEL[8..1]
    regs[0] = ((fkHz >> 1) & 0xFF);

    //Reg01
    //bits[7..6]: RFGAIN[1..0]
    //bits[5..3]: PGA[2..0]
    //bits[2..0]: CHSEL[11..9]
    regs[1] = ((rfgainCode & 0x03) << 6) |
              ((pgaCode & 0x07) << 3) |
              ((fkHz >> 9) & 0x07);

    //Reg02
    //bit[7]: CHSEL[0]
    //bit[6]: RFGAIN[3]
    //bit[0]: PHTCNST, 0/1 -> 75/50us -> USA/Europe
    regs[2] = ((fkHz << 7) & 0x80) |
              ((rfgainCode & 0x08) << 3) |
              (phtCnst & KT0803_PHTCNST_EUROPE);

    //Reg13
    //bit[7]: RFGAIN[2]
    //bit[2]: PA_CTRL 0/1 - internal/external inductor
    regs[3] = ((rfgainCode & 0x04) << 5) |
              KT0803_PA_CTRL_INTERNAL;

    //Write registers with new values.
    //NOTE! Write each register separately.
    if ((r = KT0803WriteRegister(sen, 0x00, regs[0])) == 1 &&
        (r = KT0803WriteRegister(sen, 0x01, regs[1])) == 1 &&
        (r = KT0803WriteRegister(sen, 0x02, regs[2])) == 1 &&
        (r = KT0803WriteRegister(sen, 0x13, regs[3])) == 1)
            return 0;

    return r;
}



/* KT0803: Read configuration */
s32 KT0803GetStatus(SensorCtrl *sen,
                            s32 *fkHz,
                            u8 *rfgainCode,
                            u8 *pgaCode,
                            u8 *phtCnst)
{
    //Recommended! Read registers from 0x00 to 0x14
    #define KT0803x_REGCOUNT        (0x13+1)
    s32 r;
    u32 rfgain;
    u8 regs[KT0803x_REGCOUNT];

    if ((r = I2CReadRegister(I2C_N1, KT0803_CADDR0,
                             0x00, regs, KT0803x_REGCOUNT)) == KT0803x_REGCOUNT)
    {
        //-------------------------------
        //Reg00.bits[7..0]: CHSEL[8..1]
        //Reg01.bits[2..0]: CHSEL[11..9]
        //Reg02.bit[7]:     CHSEL[0]
        *fkHz = (regs[0] << 1) |
                ((regs[1] & 0x07) << 9) |
                (regs[2] >> 7);

        //Get frequency at kHz
        *fkHz *= KT0803_CHSEL_LSB;

        //-------------------------------
        //Reg01.bits[7..6]: RFGAIN[1..0]
        //Reg02.bit[6]:     RFGAIN[3]
        //Reg13.bit[7]:     RFGAIN[2]
        rfgain = ((regs[0x01] >> 6) & 0x03) |
                 ((regs[0x02] & 0x40) >> 3) |
                 ((regs[0x13] & 0x80) >> 5);

        //Reg0E.bit[1]: PA_BIAS
        if (rfgain > 7 && (regs[0x0E] & KT0803_PA_BIAS_ENABLE))
            rfgain += 8; //PA_BIAS=1

        *rfgainCode = (u8)rfgain;

        //-------------------------------
        //Reg01.bits[5..3]: PGA[2..0]
        *pgaCode = (regs[1] >> 3) & 0x07;

        //-------------------------------
        //Reg02.bit[0]: PHTCNST -> 0/1 -> 75/50 -> USA/Europe
        *phtCnst = (regs[2] & KT0803_PHTCNST_EUROPE);

        return 0;
    }
    else
    {
        *fkHz       = 0;
        *rfgainCode = 0;
        *pgaCode    = 0;
        *phtCnst    = 0;
        return r;
    }

    #undef KT0803x_REGCOUNT
}



/* KT0803: convert encoded value to readable string */
char *KT0803ToString(SensorCtrl *sen, char *dest)
{
    if (sen->encValue[0] == SENSOR_VALUE_NA)
    {
        std_sprintf(dest, "%s", I2CDecodeError(sen));
    }
    else
    {        
        std_sprintf(dest,
                   "%ikHz\n"
                   "RFG %i, PGA %i, PHT %ius",
                   sen->encValue[0], sen->encValue[1], sen->encValue[2],
                   (sen->encValue[3] == 0) ? 75 : 50 //75/50 -> USA/Europe
                    );
    }

    return dest;
}



/* KT0803: configure with defaults */
s32 KT0803Configure(SensorCtrl *sen)
{
    s32 r;

    if ((r = KT0803WaitPowerFlag(sen)) == 0 &&
        (r = KT0803Turn(sen, 1)) == 0 &&
        (r = KT0803Tune(sen, KT0803_DEFAULT_FREQ,
                             KT0803_DEFAULT_RFGAIN,
                             KT0803_DEFAULT_PGA,
                             KT0803_DEFAULT_PHTCNST)) == 0)
    {
        //Success
        return 0;
    }
    else
    {
        //Fail
        return r;
    }
}



/* KT0803: write data */
s32 KT0803WriteData(SensorCtrl *sen)
{
    //No actions, always return 0 - success
    return 0;
}



/* KT0803: read data */
s32 KT0803ReadData(SensorCtrl *sen)
{
    s32 r;
    s32 fkHz;
    u8 rfgain, pga, pht;

    if ((r = KT0803GetStatus(sen, &fkHz, &rfgain, &pga, &pht)) == 0)
    {
        sen->encValue[0] = fkHz;
        sen->encValue[1] = rfgain;
        sen->encValue[2] = pga;
        sen->encValue[3] = pht;
        return 0;
    }
    else
        return r;
}

/*===========================================================================
 * End of file rtrm-kt0803x.c
 *===========================================================================*/
