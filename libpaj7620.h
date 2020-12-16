/*
   Copyright (c) 2015 seeed technology inc.
   Website    : www.seeed.cc
   Author     : Wuruibin
   Modified Time: June 2015

   2017 - Modified by MarcFinns to encapsulate in class without global variables
   2020 - Modified by Aaron S. Crandall <crandall@gonzaga.edu>

   Description: This demo can recognize 9 gestures and output the result, including move up, move down, move left, move right,
  				move forward, move backward, circle-clockwise, circle-counter clockwise, and wave.

   The MIT License (MIT)

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.
*/

/*
  PAJ7620U2 Sensor data sheet for reference found here:
    https://datasheetspdf.com/pdf-file/1309990/PixArt/PAJ7620U2/1
*/

#ifndef __PAJ7620_H__
#define __PAJ7620_H__

#include <Wire.h>

#if defined(__AVR__) || defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_ESP32)
#define PROGMEM_COMPATIBLE
#include <avr/pgmspace.h>
#endif

/* 
  Direction definitions 
  - Used as return value from gesture read
 */
enum {
  GES_NONE,
  GES_LEFT,
  GES_RIGHT,
  GES_UP,
  GES_DOWN,
  GES_FORWARD,
  GES_BACKWARD,
  GES_CLOCKWISE,
  GES_CNTRCLOCKWISE,
  GES_WAVE
};

/*
  bank_e used to for selecting PAJ7620 memory bank to read/write from
 */
typedef enum {
  BANK0 = 0,
  BANK1,
} bank_e;



/*
  Notice: When you want to recognize the Forward/Backward gestures, your gestures' reaction time must less than GES_ENTRY_TIME(0.8s).
        You also can adjust the reaction time according to the actual circumstance.
*/
#define GES_ENTRY_TIME		0 // 150// 200 // 800				// When you want to recognize the Forward/Backward gestures, your gestures' reaction time must less than GES_ENTRY_TIME(0.8s). 
#define GES_QUIT_TIME		200 //300 // 1000

// DEVICE'S I2C ID - defined by manufacturer
#define PAJ7620_I2C_BUS_ADDR  0x73

// Helper macro to bit shift defines
#define BIT(x)  1 << x

// REGISTER DESCRIPTION
#define PAJ7620_VAL(val, maskbit)    ( val << maskbit )

// Base address for accessing registers
#define PAJ7620_ADDR_BASE                 0x00

// REGISTER BANK SELECT
#define PAJ7620_REGISTER_BANK_SEL         (PAJ7620_ADDR_BASE + 0xEF)  // W

// REGISTER BANK 0
#define PAJ7620_ADDR_SUSPEND_CMD          (PAJ7620_ADDR_BASE + 0x3)   // W
#define PAJ7620_ADDR_GES_PS_DET_MASK_0    (PAJ7620_ADDR_BASE + 0x41)  // RW
#define PAJ7620_ADDR_GES_PS_DET_MASK_1    (PAJ7620_ADDR_BASE + 0x42)  // RW
#define PAJ7620_ADDR_GES_PS_DET_FLAG_0    (PAJ7620_ADDR_BASE + 0x43)  // R
#define PAJ7620_ADDR_GES_PS_DET_FLAG_1    (PAJ7620_ADDR_BASE + 0x44)  // R
#define PAJ7620_ADDR_STATE_INDICATOR      (PAJ7620_ADDR_BASE + 0x45)  // R
#define PAJ7620_ADDR_PS_HIGH_THRESHOLD    (PAJ7620_ADDR_BASE + 0x69)  // RW
#define PAJ7620_ADDR_PS_LOW_THRESHOLD     (PAJ7620_ADDR_BASE + 0x6A)  // RW
#define PAJ7620_ADDR_PS_APPROACH_STATE    (PAJ7620_ADDR_BASE + 0x6B)  // R
#define PAJ7620_ADDR_PS_RAW_DATA          (PAJ7620_ADDR_BASE + 0x6C)  // R
#define PAJ7620_ADDR_WAVE_COUNT           (PAJ7620_ADDR_BASE + 0xB7)  // R

// REGISTER BANK 1
#define PAJ7620_ADDR_PS_GAIN              (PAJ7620_ADDR_BASE + 0x44)  // RW
#define PAJ7620_ADDR_IDLE_S1_STEP_0       (PAJ7620_ADDR_BASE + 0x67)  // RW
#define PAJ7620_ADDR_IDLE_S1_STEP_1       (PAJ7620_ADDR_BASE + 0x68)  // RW
#define PAJ7620_ADDR_IDLE_S2_STEP_0       (PAJ7620_ADDR_BASE + 0x69)  // RW
#define PAJ7620_ADDR_IDLE_S2_STEP_1       (PAJ7620_ADDR_BASE + 0x6A)  // RW
#define PAJ7620_ADDR_OP_TO_S1_STEP_0      (PAJ7620_ADDR_BASE + 0x6B)  // RW
#define PAJ7620_ADDR_OP_TO_S1_STEP_1      (PAJ7620_ADDR_BASE + 0x6C)  // RW
#define PAJ7620_ADDR_OP_TO_S2_STEP_0      (PAJ7620_ADDR_BASE + 0x6D)  // RW
#define PAJ7620_ADDR_OP_TO_S2_STEP_1      (PAJ7620_ADDR_BASE + 0x6E)  // RW
#define PAJ7620_ADDR_OPERATION_ENABLE     (PAJ7620_ADDR_BASE + 0x72)  // RW

// PAJ7620_REGISTER_BANK_SEL
#define PAJ7620_BANK0   PAJ7620_VAL(0,0)
#define PAJ7620_BANK1   PAJ7620_VAL(1,0)

// PAJ7620_ADDR_SUSPEND_CMD
#define PAJ7620_I2C_WAKEUP  PAJ7620_VAL(1,0)
#define PAJ7620_I2C_SUSPEND PAJ7620_VAL(0,0)

// PAJ7620_ADDR_OPERATION_ENABLE
#define PAJ7620_ENABLE    PAJ7620_VAL(1,0)
#define PAJ7620_DISABLE   PAJ7620_VAL(0,0)

// Return values from gesture I2C memory reads
#define GES_RIGHT_FLAG            PAJ7620_VAL(1,0)
#define GES_LEFT_FLAG             PAJ7620_VAL(1,1)
#define GES_UP_FLAG               PAJ7620_VAL(1,2)
#define GES_DOWN_FLAG             PAJ7620_VAL(1,3)
#define GES_FORWARD_FLAG          PAJ7620_VAL(1,4)
#define GES_BACKWARD_FLAG         PAJ7620_VAL(1,5)
#define GES_CLOCKWISE_FLAG        PAJ7620_VAL(1,6)
#define GES_COUNT_CLOCKWISE_FLAG  PAJ7620_VAL(1,7)
#define GES_WAVE_FLAG             PAJ7620_VAL(1,0)  // Read from 0x44


#define INIT_REG_ARRAY_SIZE (sizeof(initRegisterArray)/sizeof(initRegisterArray[0]))

/*
	Initializing Gesture I2C mode register addresses and values
    NOTE: Has not been tested for SPI bus image mode
  Changed to JayCar-Electronics PROGMEM approach from their fork
    https://github.com/Jaycar-Electronics
    Saves about 21% of SRAM on an Arduino Uno - Around 440 bytes
*/
#ifdef PROGMEM_COMPATIBLE
const unsigned short initRegisterArray[] PROGMEM = {
#else
const unsigned short initRegisterArray[] = {
#endif
		0xEF00,
		0x3229,
		0x3301,
		0x3400,
		0x3501,
		0x3600,
		0x3707,
		0x3817,
		0x3906,
		0x3A12,
		0x3F00,
		0x4002,
		0x41FF,
		0x4201,
		0x462D,
		0x470F,
		0x483C,
		0x4900,
		0x4A1E,
		0x4B00,
		0x4C20,
		0x4D00,
		0x4E1A,
		0x4F14,
		0x5000,
		0x5110,
		0x5200,
		0x5C02,
		0x5D00,
		0x5E10,
		0x5F3F,
		0x6027,
		0x6128,
		0x6200,
		0x6303,
		0x64F7,
		0x6503,
		0x66D9,
		0x6703,
		0x6801,
		0x69C8,
		0x6A40,
		0x6D04,
		0x6E00,
		0x6F00,
		0x7080,
		0x7100,
		0x7200,
		0x7300,
		0x74F0,
		0x7500,
		0x8042,
		0x8144,
		0x8204,
		0x8320,
		0x8420,
		0x8500,
		0x8610,
		0x8700,
		0x8805,
		0x8918,
		0x8A10,
		0x8B01,
		0x8C37,
		0x8D00,
		0x8EF0,
		0x8F81,
		0x9006,
		0x9106,
		0x921E,
		0x930D,
		0x940A,
		0x950A,
		0x960C,
		0x9705,
		0x980A,
		0x9941,
		0x9A14,
		0x9B0A,
		0x9C3F,
		0x9D33,
		0x9EAE,
		0x9FF9,
		0xA048,
		0xA113,
		0xA210,
		0xA308,
		0xA430,
		0xA519,
		0xA610,
		0xA708,
		0xA824,
		0xA904,
		0xAA1E,
		0xAB1E,
		0xCC19,
		0xCD0B,
		0xCE13,
		0xCF64,
		0xD021,
		0xD10F,
		0xD288,
		0xE001,
		0xE104,
		0xE241,
		0xE3D6,
		0xE400,
		0xE50C,
		0xE60A,
		0xE700,
		0xE800,
		0xE900,
		0xEE07,
		0xEF01,
		0x001E,
		0x011E,
		0x020F,
		0x0310,
		0x0402,
		0x0500,
		0x06B0,
		0x0704,
		0x080D,
		0x090E,
		0x0A9C,
		0x0B04,
		0x0C05,
		0x0D0F,
		0x0E02,
		0x0F12,
		0x1002,
		0x1102,
		0x1200,
		0x1301,
		0x1405,
		0x1507,
		0x1605,
		0x1707,
		0x1801,
		0x1904,
		0x1A05,
		0x1B0C,
		0x1C2A,
		0x1D01,
		0x1E00,
		0x2100,
		0x2200,
		0x2300,
		0x2501,
		0x2600,
		0x2739,
		0x287F,
		0x2908,
		0x3003,
		0x3100,
		0x321A,
		0x331A,
		0x3407,
		0x3507,
		0x3601,
		0x37FF,
		0x3836,
		0x3907,
		0x3A00,
		0x3EFF,
		0x3F00,
		0x4077,
		0x4140,
		0x4200,
		0x4330,
		0x44A0,
		0x455C,
		0x4600,
		0x4700,
		0x4858,
		0x4A1E,
		0x4B1E,
		0x4C00,
		0x4D00,
		0x4EA0,
		0x4F80,
		0x5000,
		0x5100,
		0x5200,
		0x5300,
		0x5400,
		0x5780,
		0x5910,
		0x5A08,
		0x5B94,
		0x5CE8,
		0x5D08,
		0x5E3D,
		0x5F99,
		0x6045,
		0x6140,
		0x632D,
		0x6402,
		0x6596,
		0x6600,
		0x6797,
		0x6801,
		0x69CD,
		0x6A01,
		0x6BB0,
		0x6C04,
		0x6D2C,
		0x6E01,
		0x6F32,
		0x7100,
		0x7201,
		0x7335,
		0x7400,
		0x7533,
		0x7631,
		0x7701,
		0x7C84,
		0x7D03,
		0x7E01};


/*
  PAJ7620U Device API class
 */
class PAJ7620U
{
  public:
    uint8_t begin();
    int readGesture();
    void cancelGesture();
    int getWaveCount();

  private:
    uint8_t writeRegister(uint8_t addr, uint8_t cmd);
    uint8_t readRegister(uint8_t addr, uint8_t qty, uint8_t data[]);
    void selectRegisterBank(bank_e bank);
    bool isPAJ7620UDevice();
    void initializeDeviceSettings();
};

#endif