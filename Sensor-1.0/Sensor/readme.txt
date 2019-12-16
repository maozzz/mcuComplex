Sensor-1.0 - I2C/1-Wire sensor library

The files in ../lib, Sensor directories (folders) and any 
subdirectories (sub-folders) thereof are part of GPL Sensor-1.0

Sensor-1.0 is free software; you can redistribute it and/or
modify it under the terms of version 2 of the GNU General Public
License as published by the Free Software Foundation.

Sensor-1.0 is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty
of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program so you can know your rights and responsibilities.
It should be in a file named LICENSE.

Supported 1-Wire sensors:
 - DS18B20;

Supported I2C sensors: 
 - PCF8591;
 - BH1750;
 - BMP280;
 - DHT12;
 - INA219;
 - LM75A;
 - MPU6050;

Also: 
 - FM transmitter 76-108MHz KT0803;

1. Requirements
Hardware:
- Milandr 1986BE91T/1986BE1T evaluation board;
- LCDs MT-12864 or PC2004 (PC1602);
- Pull up resistors ~4.7kOhm (see datasheet to specified sensor).

Compilator:
- arm-none-eabi-gcc


2. Connect hardware components
At MCU 1986BE91T used this I2C lines:
X33.2#11-> PC1, I2C_SDA
X33.2#10-> PC0, I2C_SCK

X33.1#25-> PA9, 1Wire_data (software implementation)

2.2 Graphic LCD MT-12864 and Milandr 1986BE91T eval.board
This LCD already installed at 1986BE91T eval.board
MT-12864      1986BE91T eval board
Vss=GND
Vdd=+3.3V
DB0           PA0
DB1           PA1
...           ...
DB7           PA7
E1            PE4
E2            PE5
RES=+3.3V
RW            PC2
A0            PE11
E             PC7

2.3 Char LCDs PC2004 (PC1602) and Milandr 1986BE91T eval.board
PC2004        1986BE91T eval.board
Vss=GND
Vdd=+5V
Vo=+1V  (contrast, +1V recommended)
RS            PE11
RW            PC2
E             PC7
D0            PA0
D1            PA1
...           ...
D7            PA7
A=+5V
K=GND


3. Build sensors drivers for MCU 1986BE91T
and LCD MT-12864:
$ make MCU=1986BE9x MCUM=1986BE91T LCD=mt12864

and LCD PC2004:
$ make MCU=1986BE9x MCUM=1986BE91T LCD=pc2004

and LCD PC1602:
$ make MCU=1986BE9x MCUM=1986BE91T LCD=pc1602

For MCU 1986BE1T
and LCD MT-12864:
$ make MCU=1986BE1x MCUM-1986BE1T LCD=mt12864


4. Build usage sample of MPU-6050 (position sensor, no LCD need):
$ make -f Makefile-mpu6000

5. Build usage sample of KT0803 (FM tramsitter 76-108MHz, no LCD need):
$ make -f Makefile-kt0803

6. Output files
Output files *.hex and *.bin at folder "output/bin":
Sensor-<MCU>-<LCD>-gcc.hex
MPU6000-<MCU>-<LCD>-gcc.hex
KT0803-<MCU>-<LCD>-gcc.hex

7. Program MCU with any accessible programmator.

8. Turn MCU power and use "UP"/"DOWN" buttons to scroll screen.

