/**
 * @file RealTimeClock.cpp
 *
 * @brief Provides date/time functions
 *
 * @author K. Joseph Hass
 * @date Created: 2019-03-07T15:47:07-0500
 * @date Last modified: 2019-03-17T17:03:26-0400
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

const uint8_t RTC_ADDRESS = 0x6F;
const uint8_t RTCWKDAY    = 0x03;

const uint8_t OSCRUN      = (1 << 5);
const uint8_t PWRFAIL     = (1 << 4);
const uint8_t VBATEN      = (1 << 3);

//char RealTimeClock::timestring[30] = "2019-04-01T00:00:00Z";
static char RealTimeClock::timestring[30];

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
  Wire.beginTransmission(RTC_ADDRESS);
  Wire.write(RTCWKDAY);
  Wire.write(0x07);
  Wire.endTransmission();
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
  uint8_t StringPointer = 0;
  //
  // Year, 4 digits
  //
  RealTimeClock::timestring[StringPointer++] = '2';
  
  Wire.beginTransmission(RTC_ADDRESS);
  Wire.write(RTCWKDAY);
  Wire.endTransmission();
  Wire.requestFrom(RTC_ADDRESS, 1);
  {
    Weekday = Wire.read();
  }
  return RealTimeClock::timestring;
}
