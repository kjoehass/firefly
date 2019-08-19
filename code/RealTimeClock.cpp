/**
    @file RealTimeClock.cpp

   @brief Provides date/time functions

   @author K. Joseph Hass
   @date Created: 2019-03-07T15:47:07-0500
   @date Last modified: 2019-08-19T15:46:38-0400

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

#include "RealTimeClock.h"
#include "Arduino.h"

//
// MCP79410 RTC chip constants.
// All address pins must be wired to '1'
//
const uint8_t RTC_ADDRESS = 0x6F;       //!< I2C address for RTC chip
const uint8_t RTC_SECONDS = 0x00;       //!< Internal address for RTCSEC register

//
// Control bits in RTC time registers
//
const uint8_t OSCRUN = (1 << 5);        //!< Oscillator running? in RTCWKDAY
const uint8_t PWRFAIL = (1 << 4);       //!< Primary power lost? in RTCWKDAY
const uint8_t VBATEN = (1 << 3);        //!< Enable battery backup in RTCWKDAY
const uint8_t LEAPYR = (1 << 5);        //!< Indicate year is leap year in RTCMTH
const uint8_t STRTOSC = (1 << 7);       //!< Start oscillator, in RTCSEC

/**
   @fn     BCD2char
   @brief  Convert the bottom 4 bits of a BCD value to an ASCII character

   @param  BCDval an 8-bit BCD integer
   @return a single ASCII digit character
*/
static char BCD2char(uint8_t BCDval)
{
  return ('0' + (BCDval & 0xF));
}

/**
   @fn     bin2BCD
   @brief  Convert an 8-bit integer to BCD

   @param  value an 8-bit unsigned integer, 0 to 99
   @return an 8-bit BCD value
*/
static uint8_t bin2BCD(uint8_t value)
{
  return (value + 6 * (value / 10));
}

//
// Redundant(?) declaration of the static timestring
//
char RealTimeClock::timestring[21] = "2000-04-01T12:59:59Z";

/**
   @fn    RealTimeClock::Set
   @brief Sets the current date and time

   @param year    current year, e.g. 2019
   @param month   current month, 1 to 12
   @param day     current day, 1 to 31
   @param hour    current hour, 0 to 23
   @param minutes current minute, 0 to 59
   @param seconds current seconds, 0 to 59
*/
void RealTimeClock::Set(uint16_t year,  //!< 4-digit year
                        uint8_t month,  //!< 1=Jan, 12=Dec
                        uint8_t day,    //!< 1 to 31
                        uint8_t hour,   //!< 0 to 23
                        uint8_t minutes,        //!< 0 to 59
                        uint8_t seconds)        //!< 0 to 59
{
#ifndef FAKE_RTC
  Wire.beginTransmission(RTC_ADDRESS);
  Wire.write(RTC_SECONDS);      // first address is seconds register
  Wire.write(bin2BCD(seconds)); // also stops the oscillator, STRTOSC = 0
  Wire.write(bin2BCD(minutes));
  Wire.write(bin2BCD(hour));    // also sets to 24-hr mode
  Wire.write(VBATEN);           // enable battery in RTCWKDAY
  Wire.write(bin2BCD(day));
  //
  // If year is divisible by 4, is a leap year
  // Check if 2 LSBs are zero
  //
  if ((year & 0xFC) == year) {
    Wire.write(bin2BCD(month) | LEAPYR);
  } else {
    Wire.write(bin2BCD(month));
  }
  Wire.write(bin2BCD(year - 2000));   // year is just 2 digits
  Wire.endTransmission();

  //
  // Now we can start the clock running
  //
  Wire.beginTransmission(RTC_ADDRESS);
  Wire.write(RTC_SECONDS);                // set address to RTCSEC register
  Wire.write(bin2BCD(seconds) | STRTOSC); // start the clock
  Wire.endTransmission();
#else
  sprintf(timestring, "%4d-%02d-%02dT%02d:%02d:%02dZ",
          year, month, day, hour, minutes, seconds);
#endif
  return;
}

/**
   @fn    RealTimeClock::DateTime
   @brief Returns a string with the current date and time in ISO 8601 format

   @return a date/time string
*/
char *RealTimeClock::DateTime(void)
{
#ifndef FAKE_RTC
  uint8_t BCDvalue;
  strcpy(RealTimeClock::timestring, "2000-00-00T00:00:00Z");
  //
  // Write the beginning address into the RTC address register
  //
  Wire.beginTransmission(RTC_ADDRESS);
  Wire.write(RTC_SECONDS);
  if (Wire.endTransmission() == 0) {
    //
    // Request 7 fields: sec, min, hr, wkday, date, mth, yr
    //
    Wire.requestFrom(RTC_ADDRESS, (uint8_t) 7);
    //
    // Seconds, 2 digits, bit 7 is ST (start oscillator)
    // RealTimeClock::timestring[19] is 'Z'
    // RealTimeClock::timestring[16] is ':'
    //
    BCDvalue = Wire.read();
    RealTimeClock::timestring[18] = BCD2char(BCDvalue);
    RealTimeClock::timestring[17] = BCD2char((BCDvalue & ~STRTOSC) >> 4);
    //
    // Minutes, 2 digits
    // RealTimeClock::timestring[13] is ':'
    //
    BCDvalue = Wire.read();
    RealTimeClock::timestring[15] = BCD2char(BCDvalue);
    RealTimeClock::timestring[14] = BCD2char(BCDvalue >> 4);
    //
    // Hour, 2 digits, 24-hour format
    // RealTimeClock::timestring[10] is 'T'
    //
    BCDvalue = Wire.read();
    RealTimeClock::timestring[12] = BCD2char(BCDvalue);
    RealTimeClock::timestring[11] = BCD2char(BCDvalue >> 4);
    //
    // Day-of-week, discarded
    //
    BCDvalue = Wire.read();
    //
    // Date, 2 digits
    // RealTimeClock::timestring[7] is '-'
    //
    BCDvalue = Wire.read();
    RealTimeClock::timestring[9] = BCD2char(BCDvalue);
    RealTimeClock::timestring[8] = BCD2char(BCDvalue >> 4);
    //
    // Month, 2 digits, bit 5 is leap year
    // RealTimeClock::timestring[4] is '-'
    //
    BCDvalue = Wire.read();
    RealTimeClock::timestring[6] = BCD2char(BCDvalue);
    RealTimeClock::timestring[5] = BCD2char((BCDvalue & ~LEAPYR) >> 4);
    //
    // Year, 4 digits, 8 bits represent bottom two digits
    //
    BCDvalue = Wire.read();
    RealTimeClock::timestring[3] = BCD2char(BCDvalue);
    RealTimeClock::timestring[2] = BCD2char(BCDvalue >> 4);
    RealTimeClock::timestring[1] = '0';
    RealTimeClock::timestring[0] = '2';
  }
#endif
  return RealTimeClock::timestring;
}
