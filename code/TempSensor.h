/**
   @file TempSensor.h

   @brief Support for an LM75 temperature sensor

   @author K. Joseph Hass
   @date Created: 2019-03-07T16:22:40-0500
   @date Last modified: 2019-04-14T09:47:22-0400

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
#ifndef __TEMPSENSOR_H
#define __TEMPSENSOR_H

#include "stdint.h"
#include <Wire.h>

/**
   @class TempSensor
   @brief Provides a static function to return the temperature.

*/
class TempSensor {
  public:
    static int8_t Temperature(void);
};

#endif
