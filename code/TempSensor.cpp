/**
   @file TempSensor.cpp

   @brief Support for an LM75 temperature sensor

   @author K. Joseph Hass
   @date Created: 2019-03-07T16:21:42-0500
   @date Last modified: 2019-04-17T12:17:25-0400

   @copyright Copyright (C) 2019 Kenneth Joseph Hass

   @copyright This program is free software: you can redistribute it and/or
   modify it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or (at your
   option) any later version.

   @copyright This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
   Public License for more details.

*/
#include "TempSensor.h"
#include <Wire.h>

/**
   @var   LM75_ADDR
   @brief The I2C address of the LM75 sensor.

          This is a 7-bit address. The entire 8-bit address is defined as

          |Bit 7|Bit 6|Bit 5|Bit 4|Bit 3|Bit 2|Bit 1|Bit 0|
          |:----|:----|:----|:----|:----|:----|:----|:----|
          |1    |0    |0    |1    |A2   |A1   |A0   |R/W  |

          where A2, A1, and A0 are pins on the LM75 that may be connected
          to the supply voltage or to ground. Therefore, there are 8 possible
          7-bit addresses for an LM75 from 0x48 to 0x4F.
*/
const uint8_t LM75_ADDR = 0x4F;

const int8_t MSB = 1 << 7;

/**
   @fn    TempSensor::Temperature
   @brief Reads an LM75 sensor for current temperature

          Two registers in the LM75 are read to determine the temperature.
          The first byte returned is an unsigned integer with units of degrees
          Celsius. The MSB of the second byte has a value of 0.5 degree. If
          the MSB of the second byte is 1, we round the integer temperature
          up.

   @return integer degrees Celsius
*/
int8_t TempSensor::Temperature(void)
{
  int8_t TempC, HalfDegree;
  TempC = 0;
  
  Wire.requestFrom(LM75_ADDR, (uint8_t) 2);
  while (Wire.available()) {
    TempC = (int8_t) Wire.read();
    HalfDegree = (int8_t) Wire.read();
    if (HalfDegree & MSB) {
      TempC++;
    }
  }
  return TempC;
}
