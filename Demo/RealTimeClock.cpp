/**
 * @file RealTimeClock.cpp
 *
 * @brief Provides date/time functions
 *
 * @author K. Joseph Hass
 * @date Created: 2019-03-07T15:47:07-0500
 * @date Last modified: 2019-03-07T16:16:07-0500
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

#include "RealTimeClock.h"

char RealTimeClock::timestring[30] = "2019-04-01T00:00:00Z";

/**
 * @fn    RealTimeClock::Set
 * @brief Sets the current date and time
 *
 * @param year    current year, e.g. 2019
 * @param month   current month, 1 to 12
 * @param day     current day, 1 to 31
 * @param hour    current hour, 0 to 23
 * @param minute  current minute, 0 to 59
 * @param seconds current seconds, 0 to 59
 */
void RealTimeClock::Set(uint16_t year,     //!< 4-digit year 
                        uint8_t month,    //!< 1=Jan, 12=Dec
                        uint8_t day,      //!< 1 to 31
                        uint8_t hour,     //!< 0 to 23
                        uint8_t minutes,  //!< 0 to 59
                        uint8_t seconds)  //!< 0 to 59
{
  return;
}

/**
 * @fn    RealTimeClock::DateTime
 * @brief Returns a string with the current date and time in ISO 8601 format
 *
 * @return a date/time string
 */
char* RealTimeClock::DateTime(void)
{
  return RealTimeClock::timestring;
}
