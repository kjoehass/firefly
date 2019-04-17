/**
 * @file RealTimeClock.h
 *
 * @brief Provides real-time clock functions 
 *
 * @author K. Joseph Hass
 * @date Created: 2019-03-07T13:57:19-0500
 * @date Last modified: 2019-04-17T12:16:34-0400
 *
 * @copyright Copyright (C) 2019 Kenneth Joseph Hass
 *
 * @copyright This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * @copyright This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 *
 */
#ifndef __REALTIMECLOCK_H
#define __REALTIMECLOCK_H

#include "stdint.h"
#include "Wire.h"

/**
 * @def   FAKE_RTC
 * @brief if defined, means that there is no actual RTC module
 *
 *        RTC functions will act as stubs, returning fake data
 */
#define FAKE_RTC

/**
 * @class  RealTimeClock
 * @brief  Provides ISO 8601 date/time strings
 *
 *         Reports year, month, day, hour, minutes, and seconds using
 *         ISO 8601 format: YYYY-MM-DDTHH:MM:SSZ
 *         All times are UTC (Zulu) rather than local timezone
 */
class RealTimeClock {
  public:
    static void Set(uint16_t year,      //!< 4-digit year 
                    uint8_t month,      //!< 1=Jan, 12=Dec
                    uint8_t day,        //!< 1 to 31
                    uint8_t hour,       //!< 0 to 23
                    uint8_t minutes,    //!< 0 to 59
                    uint8_t seconds);   //!< 0 to 59
    static char *DateTime(void);
  private:
    static char timestring[21];
};

#endif
