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
 *  File sensor.h. Sensor definitions and functons prototypes.
 *
 *===========================================================================*/

#ifndef __SENSOR_H
#define __SENSOR_H

#include <bastypes.h>
#include <I2C.h>
#include <IWire.h>
#include <CRC.h>
#include <string.h>
#include <screen.h>
#include <controls.h>

/*===========================================================================
 * SENSOR DEFINITIONS
 *===========================================================================*/

//Maximum count of sensors
#define SENSOR_MAXCOUNT          14

//Sensor types (SensorConfig::type)
#define SENSOR_TYPE_BH1750       1 //BH1750:  ambient light,   Vdd 2.4-3.6V (!)
#define SENSOR_TYPE_BMP280       2 //BMP280:  pressure/temp.,  Vdd 1.7-3.6V (!)
#define SENSOR_TYPE_DHT12        3 //DHT12:   temp./humidity,  Vdd 2.7-5.5V
#define SENSOR_TYPE_DS18B20      4 //DS18B20: temperature,     Vdd 3.0-5.5V
#define SENSOR_TYPE_HMC5883      5 //HMC5883: 3-ax. compass,   Vdd 2.2-3.6V (!)
#define SENSOR_TYPE_INA219       6 //INA219:  voltage/current, Vdd 3.0-5.5V
#define SENSOR_TYPE_LM75A        7 //LM75A:   temperature,     Vdd 2.8-5.5V
#define SENSOR_TYPE_MPU6000      8 //MPU60X0: 3-ax. gyro/accel,Vdd 2.4-3.4V (!)
#define SENSOR_TYPE_PCF8591      9 //PCF8591: AD/DA converter, Vdd 2.5-6.0V
#define SENSOR_TYPE_KT0803      10 //KT0803:  FM radio tran.,  Vdd 1.6-3.6V (!)
#define SENSOR_TYPE_SI470X      11 //SI470x:  FM radio rec.,   Vdd 3.3V (!)

//Define other type of sensor
//#define SENSOR_TYPE_x

#define SENSOR_TYPE_NONE         0 //no sensor
#define SENSOR_TYPE_UNDEF      255 //no sensor


//Profiles (SensorConfig::profile)
//Sensor BMP280 profiles
#define BMP280_PROFILE_HANDHELD_LOWPOWER     0
#define BMP280_PROFILE_HANDHELD_DYNAMIC      1
#define BMP280_PROFILE_WEATHER_MONITORING    2
#define BMP280_PROFILE_ELEVATOR              3
#define BMP280_PROFILE_DROP_DETECTION        4
#define BMP280_PROFILE_INDOOR_NAVIGATION     5

//Sensor MPU60X0 profiles
#define MPU6000_PROFILE_250dps               (0 << 0) //gyro
#define MPU6000_PROFILE_500dps               (1 << 0)
#define MPU6000_PROFILE_1000dps              (2 << 0)
#define MPU6000_PROFILE_2000dps              (3 << 0)
#define MPU6000_PROFILE_2g                   (0 << 2) //acc
#define MPU6000_PROFILE_4g                   (1 << 2)
#define MPU6000_PROFILE_8g                   (2 << 2)
#define MPU6000_PROFILE_16g                  (3 << 2)

//Sensor address maximum length, bytes.
//Must 8 bytes: 1-Wire sensors has 64-bit address
#define SENSOR_ADDRESS_MAXLENGTH    8

//Sensor name maximum length (including zero char), bytes
#define SENSOR_NAME_MAXLENGTH       18

/* Sensor configuration */
typedef struct tag_SensorConfig
{
    //Sensor type (see SENSOR_TYPE_x)
    u8 type;

    //Sensor profile (see x_PROFILE_y)
    u8 profile;

    //I2C/1-Wire address
    //I2C:    addr[0] - address [0..127]; addr[1..7] - unused
    //1-Wire: addr[0] - fam.code; addr[1..6] - S/N; addr[7] - ROM CRC
    u8 addr[SENSOR_ADDRESS_MAXLENGTH];

    //Sensor name, readable string
    char name[SENSOR_NAME_MAXLENGTH];

} SensorConfig;


/* Kalman filter control structure (float) */
typedef struct tag_KalmanF
{
    //Noise
    float Q;

    //Measuring error
    float R;

    //Dynamics of the system
    float F;

    //(1)
    float H;

    //
    float X0;

    //
    float P0;

    //
    float covar;

    //Filter output
    float output;
} KalmanF;


/* Vector 3D (float) */
typedef struct Vector3DF
{
    float x;
    float y;
    float z;
} Vector3DF;

/*===========================================================================
 * Control structures
 *===========================================================================*/

//Sensor flags (SensorCtrl::flags)
//Indicates that sensor is configured.
#define SENSOR_FLAG_CONFIGURED      0x01

//Value not available
#define SENSOR_VALUE_NA             0xFFFFFFFFU

//Interval between read/configure sensors, msec
#define SENSOR_READ_INTERVAL        2

//Polling interval, msec.
//DS18B20 with 12-bit resolution has convertation time 750ms:
//set 760ms pause by default.
#ifndef SENSOR_POLL_INTERVAL
#define SENSOR_POLL_INTERVAL       760
#endif

//Count of scan bus retries and retry interval at msec
//"ScanIWireBus", "ScanI2CBus", "AutoconfigureSensors" functions
#define SENSOR_SCANRETRY_COUNT      3
#define SENSOR_SCANRETRY_INTERVAL   100

//Sensor list states
#define SENSOR_STATE_START          0 //START->PAUSE
#define SENSOR_STATE_PAUSE          3 //PAUSE->READ_DATA (SEN#1,SEN#2..SEN#N)
#define SENSOR_STATE_READ_DATA      4 //READ_DATA->PAUSE


//Length of SensorCtrl::encValue[] array
#define SENSOR_ENCVALUE_MAXCOUNT     7

typedef struct tag_SensorCtrl SensorCtrl;

/* Sensor control structure (runtime state) */
typedef struct tag_SensorCtrl
{
    //Pointer to sensor configuration
    const SensorConfig *cfg;

    //Sensor flags (see SENSOR_FLAG_x)
    u8 flags;

    //Error flag counter
    u16 errCount;

    //Code of last error
    s16 lastError;

    //Sensor encoded values
    u32 encValue[SENSOR_ENCVALUE_MAXCOUNT];          

    //Pointers to functions that performs actions specific for each sensor
    //They are called at "UpdateSensorsState" function for each sensor at list.

    //Configure sensor. Called when SENSOR_FLAG_CONFIGURED is not set
    //Return: 0 - success, flag "SENSOR_FLAG_CONFIGURED" sets
    //at field "SensorCtrl::flag"
    s32 (*pfConfigure)(SensorCtrl *sen);

    //Write data to sensor (restart calculation in some cases).
    //Return: 0 - success
    s32 (*pfWriteData)(SensorCtrl *sen);

    //Pointer to function: read data from sensor.
    //Called when SENSOR_FLAG_CONFIGURED is set at field "SensorCtrl::flag".
    //Return: 0 - success
    s32 (*pfReadData)(SensorCtrl *sen);

    //Convert encoded value to readable string.
    //Return: "dest" pointer
    char *(*pfToString)(SensorCtrl *sen, char *dest);
} SensorCtrl;


//Pointer to BMP280 sensor
extern SensorCtrl *pBMP280Sensor;

//Pointer to screen
extern ScreenParams *pscreen;
//Textbox control structure
extern TextBox textBox;

extern u32 blinkCounter;
extern u32 mainSpeed;


/*===========================================================================
 * Top level functions
 *===========================================================================*/

//Decrement this variable to 0 with interval 1ms.
extern volatile u32 senWaitTimeCounter;

/* Wait specified count of milliseconds */
__STATIC_INLINE void WaitIntervalMs(u32 msec)
{
    senWaitTimeCounter = msec;
    __NOP();
    while(senWaitTimeCounter > 0) __NOP();
}


/* Initialize sensors */
void InitializeSensors(const SensorConfig *scfgPtr, u32 pollInterval);

/* Convert sensor address at hexadecimal format to address */
s32 ParseSensorAddress(const char *src, u8 *addr);

/* Convert sensor type to readable string */
const char *SensorTypeToString(u8 type);

/* Get pointer to sensor control structure using specified index */
SensorCtrl *GetSensor(u32 index);

/* Get count of sensors */
u32 GetSensorCount();

/* Get poll interval */
u32 GetPollInterval();

/* Set poll interval */
void SetPollInterval(u32 pollInterval);

/* Get free item at sensor list */
SensorConfig *GetFreeSensorCfg(SensorConfig *dcfg);

/* Get sensor with specified address */
SensorConfig *GetSensorCfgByAddr(SensorConfig *dcfg, const u8 *addr);

/* I2C bus: decode error code */
const char *I2CDecodeError(SensorCtrl *sen);

/* 1-Wire bus: decode error code */
const char *IWireDecodeError(SensorCtrl *sen);

/* 1-Wire bus: read 64-bit ROM.
   Note: must be only one device on 1-Wire bus */
s32 IWireReadROM(u8 *dest);

/* Write register with specified address:
   writes register address + register data */
s32 I2CWriteRegister(I2CBLOCK i2c, u8 devaddr, u8 regaddr, const u8 *src, s32 n);

/* Read register with specified address:
   writes register address than read register */
s32 I2CReadRegister(I2CBLOCK i2c, u8 devaddr, u8 regaddr, u8 *dest, s32 n);

/* Scan 1-Wire bus to detect devices */
s32 ScanIWireBus(char *dest, u8 isHtmlFormat);

/* Scan I2C bus to detect devices */
s32 ScanI2CBus(char *dest, u8 isHtmlFormat);

/* Set N/A values */
void SetNAValues(SensorCtrl *sen);

/* Scan 1-Wire and I2C bus and generate configuration */
s32 AutoconfigureSensors(SensorConfig *destCfg, s32 maxcount);

/* Print data from all sensors to LCD */
void PrintSensorsData(u32 isRefresh);

/* Update sensors state.
   Cyclic configure/read sensors with specified poll interval.
   Call once at main cycle. */
void UpdateSensorsState();

/*===========================================================================
 * KALMAN FILTER
 *===========================================================================*/

/* Initialize Kalman filter (float) */
void InitializeKalmanF(KalmanF *kf, float q, float  r, float  f, float  h);

/* Apply Kalman filter (float) */
float ApplyKalmanF(KalmanF *kf, float inval);

/*===========================================================================
 * PCF8591: 4 channel ADC, 1 channel DAC, I2C interface
 *===========================================================================*/

//Constant I2C address: 1001xxx = 0x48
//NOTE! Conflict with temperature sensor LM75A when A2,A1,A0 equal
#define PCF8591_CADDR           0x48

//Variable address mask (3 low bits: A2,A1,A0)
#define PCF8591_VADDR_MASK      0x07

/* PCF8591: initialize control structure */
void InitializePCF8591(SensorCtrl *sen, const SensorConfig *cfg);

/* PCF8591: decode AIN0 */
float PCF8591DecodeAIN0(SensorCtrl *sen);

/* PCF8591: decode AIN1 */
float PCF8591DecodeAIN1(SensorCtrl *sen);

/* PCF8591: decode AIN2 */
float PCF8591DecodeAIN2(SensorCtrl *sen);

/* PCF8591: decode AIN3 */
float PCF8591DecodeAIN3(SensorCtrl *sen);

/* PCF8591 sensor: convert encoded value to readable string */
char *PCF8591ToString(SensorCtrl *sen, char *dest);

/* PCF8591: configure */
s32 PCF8591Configure(SensorCtrl *sen);

/* PCF8591: write sample to DAC */
s32 PCF8591WriteData(SensorCtrl *sen);

/* PCF8591: read analog inputs */
s32 PCF8591ReadData(SensorCtrl *sen);


/*===========================================================================
 * KT0803x: FM Radio Transmitter 76-108Mhz, I2C interface
 *===========================================================================*/

//I2C address: 0111110b
//Note! Possible conflict with PCF8574A
#define KT0803_CADDR0        0x3E

//Transmitter defaults
#define KT0803_DEFAULT_FREQ     89000 //89MHz
#define KT0803_DEFAULT_RFGAIN   14    //[0..15]
#define KT0803_DEFAULT_PGA      1     //[0..7], 1: -4dB
#define KT0803_DEFAULT_PHTCNST  1     //[0,1]=>[USA,Europe]

//Minimum/maximum frequency, kHz
#define KT0803_MINFREQ_KHZ   76000
#define KT0803_MAXFREQ_KHZ   108000

/* KT0803: initialize control structure */
void InitializeKT0803(SensorCtrl *sen, const SensorConfig *cfg);

/* KT0803: wait power flag (PW_OK=1) */
s32 KT0803WaitPowerFlag(SensorCtrl *sen);

/* KT0803: turn to work/standby mode */
s32 KT0803Turn(SensorCtrl *sen, u8 on);

/* KT0803: tune transmitter with specified frequency, RF gain, audio gain.
   Return: 0 - success, otherwise error. */
s32 KT0803Tune(SensorCtrl *sen,
               s32 fkHz,
               u8 rfgainCode,
               u8 pgaCode,
               u8 phtCnst);

/* KT0803: read status - current frequency, RF gain, audio gain
   and other parameters.
   Return: 0 - success, otherwise error. */
s32 KT0803GetStatus(SensorCtrl *sen,
                    s32 *fkHz,
                    u8 *rfgainCode,
                    u8 *pgaCode,
                    u8 *phtCnst);

/* KT0803: convert encoded value to readable string.
   This function as part of sensor interface (see SensorCtrl::pfToString).
   Return: "dest" pointer. */
char *KT0803ToString(SensorCtrl *sen, char *dest);

/* KT0803: tune transmitter with defaults (see KT0803_DEFAULT_x).
   This function as part of sensor interface (see SensorCtrl::pfConfigure).
   Return: 0 - success, otherwise error. */
s32 KT0803Configure(SensorCtrl *sen);

/* KT0803: write data (no actions).
   This function as part of sensor interface (see SensorCtrl::pfWriteData).
   Return: 0 - success, otherwise error. */
s32 KT0803WriteData(SensorCtrl *sen);

/* KT0803: read data (get status).
   This function as part of sensor interface (see SensorCtrl::pfReadData).
   Return: 0 - success, otherwise error. */
s32 KT0803ReadData(SensorCtrl *sen);

/*===========================================================================
 * Si470x: FM Radio Tuner 76-108MHz, I2C interface 
 *
 * AN230 "Si4700/01/02/03 PROGRAMMING GUIDE",
 * Table 23. Summary of Seek Settings
 * 1. Default
 * SEEKTH=0x19, SKSNR=0x00 (disabled), SKCNT=0x00 (disabled)
 * Compatible with Firmware 14
 *
 * 2. Recommended
 * SEEKTH=0x19, SKSNR=0x04 (Good SNR), SKCNT=0x08 (Fewer imp)
 * Relative to Firmware 14
 *
 * 3. More Stations
 * SEEKTH=0x0C, SKSNR=0x04 (Good SNR), SKCNT=0x08 (Fewer imp)
 * Reduced SEEKTH identifies valid
 * stations in low RSSI environments
 *
 * 4. Good Quality Stations Only
 * SEEKTH=0x0C, SKSNR=0x0F (Excel SNR), SKCNT=0x0F (Feweset imp)
 * Identifies only high quality stations
 *
 * 5. Most Stations
 * SEEKTH=0x00, SKSNR=0x04 (Good SNR), SKCNT=0x0F (Feweset imp)
 * Seek algorithm relies solely on AFC rail, SNR and FM impulse;
 * Most valid stations identified; Potential for slightly longer seek time
 *===========================================================================*/

//I2C address: 0010000b
#define SI470X_CADDR0           0x10

//Receiver defaults
#define SI470X_DEFAULT_FREQ     100600 //100600kHz
#define SI470X_DEFAULT_RSSITH   0x0C   //[0..127]
#define SI470X_DEFAULT_SKSNR    0x04   //[0..15]
#define SI470X_DEFAULT_SKCNT    0x08   //[0..15]
#define SI470X_DEFAULT_VOLUME   12     //0 - Mute, [1..15]=>[-28..0]dBFS

//Minimum/maximum frequency, kHz
#define SI470X_MINFREQ_KHZ      76000
#define SI470X_MAXFREQ_KHZ      108000

//Minimum/maximum volume: 0 - Mute, [1..15]=>[-28..0]dBFS
#define SI470X_MINVOLUME        1
#define SI470X_MAXVOLUME        15

//Si470x has 16 16-bit registers
//Write: addr 0x02..0x0F, 0x00..0x01.
//Read:  addr 0x0A..0x0F, 0x00..0x09.
//High 8 bits is read/write first.
#define SI470X_REGSCOUNT        16

/* Si470x: initialize control structure */
void InitializeSi470x(SensorCtrl *sen, const SensorConfig *cfg);

/* Si470x: read specified count of registers.
   regs: pointer to array of length SI470X_REGSCOUNT 16-bit integers
   Return: count of read bytes. */
s32 Si470xReadRegisters(SensorCtrl *sen, u16 *regs, s32 regcount);

/* Si470x: write specified count of registers.
   regs: pointer to array of length SI470X_REGSCOUNT 16-bit integers
   Return: count of write bytes */
s32 Si470xWriteRegisters(SensorCtrl *sen, const u16 *regs, s32 regcount);

/* Si470x: turn ON/OFF receiver.
   Return: 0 - success, otherwise error. */
s32 Si470xTurn(SensorCtrl *sen, u8 on);

/* Si470x: Tune receiver.
   Return: 0 - success, otherwise error. */
s32 Si470xTune(SensorCtrl *sen,
               s32 fkHz,
               u8 rssithCode,
               u8 volumeCode);

/* Si470x: Start seek at specified direction.
   Return: 0 - success, otherwise error. */
s32 Si470xSeek(SensorCtrl *sen, s32 up);

/* Si470x: Increase/decrease volume.
   Return: 0 - success, otherwise error. */
s32 Si470xVolume(SensorCtrl *sen, s32 step);

/* Si470x: Read status - current frequency, RSSI, volume.
   Return: 0 - success, otherwise error. */
s32 Si470xGetStatus(SensorCtrl *sen,
                    s32 *fkHz,
                    u8 *rssiCode,
                    u8 *volumeCode);

/* Si470x: convert encoded value to readable string.
   This function as part of sensor interface (see SensorCtrl::pfToString).
   Return: "dest" pointer. */
char *Si470xToString(SensorCtrl *sen, char *dest);

/* Si470x: tune receiver with defaults (see SI470X_DEFAULT_x).
   This function as part of sensor interface (see SensorCtrl::pfConfigure).
   Return: 0 - success, otherwise error. */
s32 Si470xConfigure(SensorCtrl *sen);

/* Si470x: write data.
   This function as part of sensor interface (see SensorCtrl::pfWriteData).
   Return: 0 - success, otherwise error. */
s32 Si470xWriteData(SensorCtrl *sen);

/* Si470x: read data (get status).
   This function as part of sensor interface (see SensorCtrl::pfReadData).
   Return: 0 - success, otherwise error. */
s32 Si470xReadData(SensorCtrl *sen);

/* Si4703 only. RDS finctions */
/* Si4703 only: get text of GT=0A (group type) */
const char *Si4703RDSGetText0A(SensorCtrl *sen);

/* Si4703 only: get text of GT=0B  */
const char *Si4703RDSGetText0B(SensorCtrl *sen);

/* Si4703 only: get text of GT=2A  */
const char *Si4703RDSGetText2A(SensorCtrl *sen);

/* Si4703 only: get text of GT=10A  */
const char *Si4703RDSGetText10A(SensorCtrl *sen);

/* Si4703 only: get PTy (program type) description */
const char *Si4703RDSGetTextPTy(SensorCtrl *sen);

/*===========================================================================
 * BH1750: ambient light sensor, I2C interface
 *===========================================================================*/

//I2C address, ADDR=0: 0100011b
#define BH1750_CADDR0          0x23
//I2C address, ADDR=1: 1011100b
//NOTE! Conflict with DHT12 temperature sensor
#define BH1750_CADDR1          0x5C

/* BH1750: initialize control structure */
void InitializeBH1750(SensorCtrl *sen, const SensorConfig *cfg);

/* BH1750 sensor: decode code to floating point number */
float BH1750DecodeData(SensorCtrl *sen);

/* BH1750 sensor: convert encoded value to readable string */
char *BH1750ToString(SensorCtrl *sen, char *dest);

/* BH1750 sensor: configure */
s32 BH1750Configure(SensorCtrl *sen);

/* BH1750 sensor: restart convertation */
s32 BH1750WriteData(SensorCtrl *sen);

/* BH1750 sensor: read data */
s32 BH1750ReadData(SensorCtrl *sen);


/*===========================================================================
 * BMP280: pressure/temperature sensor, I2C interface
 *===========================================================================*/

//I2C address, SDO=0: 1110110b
#define BMP280_CADDR0           0x76
//I2C address, SDO=1: 1110111b
#define BMP280_CADDR1           0x77

/* BMP280: initialize control structure */
void InitializeBMP280(SensorCtrl *sen, const SensorConfig *cfg);

/* BMP280 sensor: reset leveling */
void BMP280ResetLeveling(SensorCtrl *sen);

/* BMP280 sensor: do barometric leveling */
float BMP280DoBarometricLeveling(SensorCtrl *sen);

/* BMP280 sensor: decode pressure and temperature.
   Returns: pressure at Pa, temperature at DegC */
float BMP280DecodePressure(SensorCtrl *sen, float *Tout);

/* BMP280 sensor: decode temperature code to DegC */
float BMP280DecodeTemperature(SensorCtrl *sen);

/* BMP280 sensor: convert encoded value to readable string */
char *BMP280ToString(SensorCtrl *sen, char *dest);

/* BMP280 sensor: configure */
s32 BMP280Configure(SensorCtrl *sen);

/* BMP280 sensor: restart convertation */
s32 BMP280WriteData(SensorCtrl *sen);

/* BMP280 sensor: read data */
s32 BMP280ReadData(SensorCtrl *sen);


/*===========================================================================
 * DHT12: temperature/humidity sensor, I2C interface
 *===========================================================================*/

//One constant address: 1011100b=0x5C
//NOTE! Conflict with BH1750 light sensor when BH1750.ADDR=1
#define DHT12_CADDR0          0x5C

/* DHT12: initialize control structure */
void InitializeDHT12(SensorCtrl *sen, const SensorConfig *cfg);

/* DHT12 sensor: decode temperature to floating point number */
float DHT12DecodeTemperature(SensorCtrl *sen);

/* DHT12 sensor: convert encoded value to readable string */
char *DHT12ToString(SensorCtrl *sen, char *dest);

/* DHT12 sensor: configure */
s32 DHT12Configure(SensorCtrl *sen);

/* DHT12 sensor: restart temperature convertation */
s32 DHT12WriteData(SensorCtrl *sen);

/* DHT12 sensor: read data using I2C bus */
s32 DHT12ReadData(SensorCtrl *sen);


/*===========================================================================
 * DS18B20: temperature sensor, 1-wire interface
 *===========================================================================*/

//64-bit address

/* DS18B20: initialize control structure */
void InitializeDS18B20(SensorCtrl *sen, const SensorConfig *cfg);

/* DS18B20 sensor: decode temperature to floating point number */
float DS18B20DecodeData(SensorCtrl *sen);

/* DS18B20 sensor: convert encoded value to readable string */
char *DS18B20ToString(SensorCtrl *sen, char *dest);

/* DS18B20 sensor: configure */
s32 DS18B20Configure(SensorCtrl *sen);

/* DS18B20 sensor: restart convertation */
s32 DS18B20WriteData(SensorCtrl *sen);

/* DS18B20 sensor: read data */
s32 DS18B20ReadData(SensorCtrl *sen);

/*===========================================================================
 * HMC5883: 3-axis digital compass, I2C interface
 *===========================================================================*/

//One constant address: 0x1E
#define HMC5883_CADDR0  0x1E

/* HMC5883: initialize control structure */
void InitializeHMC5883(SensorCtrl *sen, const SensorConfig *cfg);

/* HMC5883 sensor: decode temperature to floating point number */
float HMC5883DecodeData(SensorCtrl *sen);

/* HMC5883 sensor: convert encoded value to readable string */
char *HMC5883ToString(SensorCtrl *sen, char *dest);

/* HMC5883 sensor: configure */
s32 HMC5883Configure(SensorCtrl *sen);

/* HMC5883 sensor: restart temperature convertation */
s32 HMC5883WriteData(SensorCtrl *sen);

/* HMC5883 sensor: read data using I2C bus */
s32 HMC5883ReadData(SensorCtrl *sen);

/*===========================================================================
 * INA219: voltage/current monitor, I2C interface
 *===========================================================================*/

//Up to 16 INA219 on bus: A1,A0 may connected to SDA,SCL;
//but here assumed that A1,A0 connected to Vss or GND
#define INA219_CADDR00      0x40 //[A1,A0] => [GND,GND]
#define INA219_CADDR01      0x41 //[A1,A0] => [GND,Vdd]
#define INA219_CADDR10      0x44 //[A1,A0] => [Vdd,GND]
#define INA219_CADDR11      0x45 //[A1,A0] => [Vdd,Vdd]

/* INA219: initialize control structure */
void InitializeINA219(SensorCtrl *sen, const SensorConfig *cfg);

/* INA219 sensor: decode Vshunt to mV */
float INA219DecodeVshunt(SensorCtrl *sen);

/* INA219 sensor: decode Vbus to volts */
float INA219DecodeVbus(SensorCtrl *sen);

/* INA219 sensor: decode current code to A */
float INA219DecodeCurrent(SensorCtrl *sen);

/* INA219 sensor: decode power code to W */
float INA219DecodePower(SensorCtrl *sen);

/* INA219 sensor: convert encoded value to readable string */
char *INA219ToString(SensorCtrl *sen, char *dest);

/* INA219 sensor: configure */
s32 INA219Configure(SensorCtrl *sen);

/* INA219 sensor: restart convertation */
s32 INA219WriteData(SensorCtrl *sen);

/* INA219 sensor: read data */
s32 INA219ReadData(SensorCtrl *sen);


/*===========================================================================
 * LM75A/STLM75: temperature sensor, I2C interface
 *===========================================================================*/

//Constant part of I2C address: 1001xxx = 0x48
//NOTE! Conflict with PCF8591 ADC/DAC when A2,A1,A0 equal
#define LM75A_CADDR          0x48
//Variable address mask (3 low bits: A2,A1,A0)
#define LM75A_VADDR_MASK     0x07

/* LM75A: initialize control structure */
void InitializeLM75A(SensorCtrl *sen, const SensorConfig *cfg);

/* LM75A sensor: decode temperature to floating point number */
float LM75ADecodeData(SensorCtrl *sen);

/* LM75A sensor: convert encoded value to readable string */
char *LM75AToString(SensorCtrl *sen, char *dest);

/* LM75A sensor: configure */
s32 LM75AConfigure(SensorCtrl *sen);

/* LM75A sensor: restart temperature convertation */
s32 LM75AWriteData(SensorCtrl *sen);

/* LM75A sensor: read data using I2C bus */
s32 LM75AReadData(SensorCtrl *sen);

/*===========================================================================
 * MPU60X0: 3-axis accel/gyro sensor, I2C interface
 *===========================================================================*/

//I2C address, ADO=0: 1101000
#define MPU6000_CADDR0          0x68
//I2C address, ADO=1: 1101001
#define MPU6000_CADDR1          0x69

//Current accelerator/gyroscope scale
extern float mpu6000AccelScale;
extern float mpu6000GyroScale;

//Current accelerator/gyroscope offsets [0,1,2] => [Xoff,Yoff,Zoff]
extern s16 mpu6000AccelOffset[3];
extern s16 mpu6000GyroOffset[3];

/* MPU60X0: initialize control structure */
void InitializeMPU6000(SensorCtrl *sen, const SensorConfig *cfg);

/* Get roll/pitch (rotate about X/Y axis).
   Output: dest[0] - roll; dest[1] - pitch */
void MPU6000GetRollPitch(SensorCtrl *sen, float *dest);

/* MPU60X0 sensor: get gyroscope offsets */
void MPU6000GetGyroOffsets(SensorCtrl *sen, s16 *dest);

/* MPU60X0 sensor: get gyroscope offsets */
void MPU6000GetAccelOffsets(SensorCtrl *sen, s16 *dest);

/* MPU60X0 sensor: reset device */
s32 MPU6000Reset(SensorCtrl *sen);

/* MPU60X0 sensor: decode data */
float MPU6000DecodeTemperature(SensorCtrl *sen);

/* MPU60X0 sensor: convert encoded value to readable string */
char *MPU6000ToString(SensorCtrl *sen, char *dest);

/* MPU60X0 sensor: configure */
s32 MPU6000Configure(SensorCtrl *sen);

/* MPU60X0 sensor: restart convertation */
s32 MPU6000WriteData(SensorCtrl *sen);

/* MPU60X0 sensor: read data */
s32 MPU6000ReadData(SensorCtrl *sen);

#endif /* __SENSOR_H */

/*===========================================================================
 * End of file sensor.h
 *===========================================================================*/
