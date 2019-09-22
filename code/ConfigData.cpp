/**
   @file ConfigData.cpp

   @brief  Manage the process of storing/retrieving data from EEPROM

           All of the data members of an LED, Flash, Pattern, or Event object
           are preserved in EEPROM, except for the Number member. The Number
           member is used to calculate the address in EEPROM where the object
           is stored, so its value can be inferred from the EEPROM address.

           A one-byte checksum is calculated for every object stored in the
           EEPROM. The checksum is calculated by adding all of the bytes of
           the class's data elements, excluding the Number member, modulo 256.
           The initial value of the checksum is set so that if all of the
           bytes of the EEPROM are 0xFF or if they are all 0x00 then the
           checksum will be seen as invalid.

   @author K. Joseph Hass
   @date Created: 2019-02-20T08:28:10-0500
   @date Last modified: 2019-09-18T09:49:06-0400

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
#include <EEPROM.h>
#include "Firefly.h"
#include "ConfigData.h"
#include "ConfigMsg.h"
#include "RealTimeClock.h"
#include "TempSensor.h"
#include "Arduino.h"


//
// The initial value of the checksum is chosen so that the sum of erased
// EEPROM cells can not yield a checksum that is also the value of an
// erased cell. Assume that an erased cell could be 0x00 or 0xFF
//
#define CHKSUM_INIT 0xA5

namespace {
//
// The number of bytes reserved in EEPROM for each configuration data
// structure. The EEPROM in the ATmega chips has a page size of 4 bytes
// so all allocations are in multiples of 4 bytes.
//
const uint8_t PatternSize = 32;  //!< # of EEPROM bytes for a pattern
const uint8_t FlashSize = 16;    //!< # of EEPROM bytes for a flash
const uint8_t LEDSize = 4;       //!< # of EEPROM bytes for an LED
const uint8_t EventSize = 4;     //!< # of EEPROM bytes for an event
const uint8_t RandomSize = 4;    //!< # of EEPROM bytes for random set
}
//
// Define and initialize static member data of the ConfigMem
//
#if ARDUINO_IS_UNO == 1
uint8_t  ConfigMem::MaxChannel  = 6;
#else
#error "Arduino model is undefined"
uint8_t  ConfigMem::MaxChannel  = 0;
#endif
uint8_t  ConfigMem::MaxEvent       = 0;
uint8_t  ConfigMem::MaxFlash       = 0;
uint8_t  ConfigMem::MaxLED         = 0;
uint8_t  ConfigMem::MaxPattern     = 0;
uint8_t  ConfigMem::MaxPatternSet  = 0;
uint16_t ConfigMem::PatternBase    = 0;
uint16_t ConfigMem::FlashBase      = 0;
uint16_t ConfigMem::LEDBase        = 0;
uint16_t ConfigMem::EventBase      = 0;
uint16_t ConfigMem::RandomBase     = 0;


#if ARDUINO_IS_UNO == 1
const int  MAX_PWM = 255;
#else
#error "Arduino model is undefined"
#endif

/**
   @fn    WriteByte
   @brief Writes one byte to EEPROM, updating checksum and address

   @param address_p pointer to EEPROM address
   @param byte value to be stored
   @param checksum_p pointer to the checksum
*/
static void WriteByte(uint16_t * address_p, uint8_t byte, uint8_t * checksum_p)
{
  EEPROM.update(*address_p, byte);
  (*checksum_p) += byte;
  (*address_p)++;
}
/**
   @fn    Write2Byte
   @brief Writes two bytes to EEPROM, updating checksum and address

          The 16-bit value is written low-byte first.

   @param address_p pointer to EEPROM address
   @param word 16-bit value to be stored
   @param checksum_p pointer to the checksum
*/
static void Write2Byte(uint16_t * address_p, uint16_t word,
                       uint8_t * checksum_p)
{
  uint8_t byte = (uint8_t) (word & 0xFF);
  EEPROM.update(*address_p, byte);
  (*checksum_p) += byte;
  (*address_p)++;

  byte = (uint8_t) ((word >> 8) & 0xFF);
  EEPROM.update(*address_p, byte);
  (*checksum_p) += byte;
  (*address_p)++;
}
/**
   @fn    ReadByte
   @brief Reads one byte from EEPROM, updating checksum and address

   @param address_p pointer to EEPROM address
   @param checksum_p pointer to the checksum
   @return byte read from EEPROM
*/
static uint8_t ReadByte(uint16_t * address_p, uint8_t * checksum_p)
{
  uint8_t byte;
  byte = EEPROM.read(*address_p);
  (*checksum_p) += byte;
  (*address_p)++;
  return byte;
}
/**
   @fn    Read2Byte
   @brief Read two bytes from EEPROM, updating checksum and address

          The low byte is read first and then the high byte of the 16-bit
          word is read from the higher address.

   @param address_p pointer to EEPROM address
   @param checksum_p pointer to the checksum
   @return word read from EEPROM
*/
static uint16_t Read2Byte(uint16_t * address_p, uint8_t * checksum_p)
{
  uint8_t byte = EEPROM.read(*address_p);
  (*checksum_p) += byte;
  (*address_p)++;
  uint16_t word = byte;

  byte = EEPROM.read(*address_p);
  (*checksum_p) += byte;
  (*address_p)++;
  word += (byte << 8);
  return word;
}

/**
  @fn    MSDelay
  @brief Waits for specified milliseconds

         The wait time is actually some number of milliseconds from the
         <b>last time</b> that MSDelay() was called. If called with a
         parameter of zero the function just waits for the millisecond timer
         to increment. The intent of this function is to provide more precise
         delays than the built-in delay() function.

  @param time milliseconds to wait
*/
void MSDelay(uint16_t time)
{
  static unsigned long StartTime;
  if (time == 0) {
    StartTime = millis();
    while (millis() == StartTime) {};
  } else {
    while ((millis() - StartTime) < time) {};
  }
  StartTime = millis();
}

/**
   @fn      ConfigMem::Init
   @brief   Get EEPROM size, calculate EEPROM parameters

            This function must be called before any of the static class data
            members are used. It reads the size of the EEPROM on the Arduino
            processor and sets reasonable limits for the number of unique
            LEDs, Flashes, Patterns and Events that can be configured. Based
            on these limits, the starting address is calculated for the regions
            of EEPROM that store the different kinds of objects.

*/
void ConfigMem::Init(void)
{
  if (EEPROM.length() == 1024) {
    MaxPattern = 16;
    MaxFlash = 16;
    MaxLED = 16;
    MaxEvent = 16;
    MaxPatternSet = 16;
  } else {
    MaxPattern = 8;
    MaxFlash = 8;
    MaxLED = 8;
    MaxEvent = 8;
    MaxPatternSet = 8;
  }
  PatternBase = 0x000;
  //
  // For UNO
  //   MaxPattern=16     PatternSize=32 => FlashBase = 0x200
  //   MaxFlash=16       FlashSize=16   => LEDBase = 0x300
  //   MaxLED=16         LEDSize=4      => EventBase = 0x340
  //   MaxEvent=16       EventSize=4    => RandomBase = 0x380
  //   MaxPatternSet=16  RandomSize=4   => NextUnused = 0x3C0
  //
  FlashBase = PatternBase + (MaxPattern * PatternSize);
  LEDBase = FlashBase + (MaxFlash * FlashSize);
  EventBase = LEDBase + (MaxLED * LEDSize);
  RandomBase = EventBase + (MaxEvent * EventSize);
}

/**
   @fn      ConfigMem::Display
   @brief   Print the Capacity Response Message

*/
void ConfigMem::Display(void)
{
  String Msg = "c";
  Msg += "," + String(RealTimeClock::DateTime());
  Msg += "," + String(TempSensor::Temperature());
  Msg += "," + String(MaxChannel);
  Msg += "," + String(MaxLED);
  Msg += "," + String(MaxFlash);
  Msg += "," + String(MaxEvent);
  Msg += "," + String(MaxPattern);
  Msg += "," + String(MaxPatternSet);
  Serial.println(Msg);
}

/**
   @fn      LED::Save
   @brief   Saves an LED object to EEPROM

            Returns true if LED configuration was stored in EEPROM.
            Returns false, and does not change EEPROM, if any of these is true:
                - Number is 0 or > ConfigMem::MaxLED
                - Channel is 0 or > ConfigMem::MaxChannel
                - Brightness is 0 or > 100

   @return boolean true or false
*/
bool LED::Save(void)
{
  // Don't touch the EEPROM if values are invalid
  if ((Number == 0) || (Number > ConfigMem::MaxLED) ||
      (Channel == 0) || (Channel > ConfigMem::MaxChannel) ||
      (MaxBrightness == 0) || (MaxBrightness > 100)) {
    return false;
  } else {
    uint16_t MyAddress = ConfigMem::LEDBase + ((Number - 1) * LEDSize);
    uint8_t Checksum = CHKSUM_INIT;
    WriteByte(&MyAddress, Channel, &Checksum);
    WriteByte(&MyAddress, MaxBrightness, &Checksum);
    EEPROM.update(MyAddress, Checksum);
    return true;
  }
}

/**
   @fn      LED::Get
   @brief   Loads an LED object's data members from EEPROM

            The lednum parameter must be greater than zero and less than or
            equal to ConfigMem::MaxLED. If the lednum is invalid, or if the
            stored checksum is incorrect, then the LED data members are set
            to zero.

   @param   lednum the LED number to fetch

*/
void LED::Get(uint8_t lednum)
{
  uint8_t Checksum = CHKSUM_INIT;
  if ((lednum > 0) && (lednum <= ConfigMem::MaxLED)) {
    uint16_t MyAddress = ConfigMem::LEDBase + ((lednum - 1) * LEDSize);
    Number = lednum;
    // Get rest of config data, updating checksum along the way
    Channel = ReadByte(&MyAddress, &Checksum);
    MaxBrightness = ReadByte(&MyAddress, &Checksum);
    // Finally, get the stored checksum
    Checksum ^= EEPROM.read(MyAddress);
  }
  //
  // Make sure that Number and Channel are in correct range
  // Verify that checksum is zero
  //
  if ((lednum == 0) || (lednum > ConfigMem::MaxLED) ||
      (Checksum != 0)) {
    Number = 0;
    Channel = 0;
    MaxBrightness = 0;
  }
}
/**
   @fn      LED::isDefined
   @brief   Checks to see if a given LED number has been configured

            If the LED number parameter is out of range then return false.
            Read the EEPROM to see if a valid checksum has been stored.
            If so then return true, else return false.

   @param   lednum the LED number to check
   @return  boolean true or false

*/
bool LED::isDefined(uint8_t lednum)
{
  if ((lednum > 0) && (lednum <= ConfigMem::MaxLED)) {
    uint16_t MyAddress = ConfigMem::LEDBase + ((lednum - 1) * LEDSize);
    uint8_t Checksum = CHKSUM_INIT;
    ReadByte(&MyAddress, &Checksum);        // Channel
    ReadByte(&MyAddress, &Checksum);        // MaxBrightness
    // Finally, get the stored checksum
    Checksum ^= EEPROM.read(MyAddress);
    if (Checksum == 0) {
      return true;
    }
  }
  return false;
}

/**
   @fn      LED::Display
   @brief   Displays a single LED to the serial terminal

*/
void LED::Display()
{
  String Msg = "";
#if HUMAN_READABLE_MSG == 1
  Msg += "Number: " + String(Number) + "   ";
  Msg += "Channel: " + String(Channel) + "   ";
  Msg += "MaxBrightness: " + String(MaxBrightness);
#else
  Msg += "l," + String(Number);
  Msg += "," + String(Channel);
  Msg += "," + String(MaxBrightness);
#endif
  Serial.println(Msg);
}
/**
  @fn    LED::setLevel
  @brief Sets the level of the LED's illumination

         The desired level can be from 0 to 100%, and the configured
         maximum brightness of an LED can be from 0 to 100%. Conceptually,
         these two values can be multiplied, and their product divided by 10000
         to get a fraction between 0 and 1. The fraction is then multiplied by
         the maximum PWM value to get the desired PWM value. Since we are
         working with integer arithmetic, all of the multiplications are done
         first, then half the value of the remaining bit is added for rounding,
         then the result is divided by 10000.

         Some Arduino channels do not fully extinguish for a PWM value of 0
         so we brute-force change them to digital channels with a low output.

  @param level integer 0 to 100, desired illumination level
*/
void LED::setLevel(uint8_t level)
{

  uint32_t PWM_Value = level * MaxBrightness;
  PWM_Value *= MAX_PWM;
  PWM_Value = PWM_Value + 5000;
  PWM_Value = PWM_Value / 10000;
  if (PWM_Value > MAX_PWM) {
    PWM_Value = MAX_PWM;
  }
  if (PWM_Value == 0) {
    digitalWrite(Channel2Pin[ld.Channel], LOW);
  } else {
    analogWrite(Channel2Pin[ld.Channel], PWM_Value);
  }
}

/**
   @fn      Flash::Save
   @brief   Saves a Flash object to EEPROM

            Returns true if Flash configuration was stored in EEPROM.
            Returns false, and does not change EEPROM, if any of these is true:
                - Number is 0 or > ConfigMem::MaxFlash
                - LED is  > ConfigMem::MaxLED
                - Any duration is > 32767
                - InterpulseInterval is 0 or > 32767
                - InterpulseInterval is < sum of up, on ,down durations

   @return boolean true or false

*/
bool Flash::Save(void)
{
  // Don't touch the EEPROM if values are invalid
  if ((Number == 0) || (Number > ConfigMem::MaxFlash) ||
      (LED > ConfigMem::MaxLED) ||
      (UpDuration > 32767) ||
      (OnDuration > 32767) ||
      (DownDuration > 32767) ||
      (InterpulseInterval == 0) || (InterpulseInterval > 32767) ||
      (InterpulseInterval < (UpDuration + OnDuration + DownDuration))) {
    return false;
  } else {
    uint16_t MyAddress = ConfigMem::FlashBase + ((Number - 1) * FlashSize);
    uint8_t Checksum = CHKSUM_INIT;
    WriteByte(&MyAddress, LED, &Checksum);
    Write2Byte(&MyAddress, UpDuration, &Checksum);
    Write2Byte(&MyAddress, OnDuration, &Checksum);
    Write2Byte(&MyAddress, DownDuration, &Checksum);
    Write2Byte(&MyAddress, InterpulseInterval, &Checksum);
    EEPROM.update(MyAddress, Checksum);
    return true;
  }
}

/**
   @fn      Flash::Get
   @brief   Retrieves a Flash object from EEPROM

            The flashnum parameter must be greater than zero and less than or
            equal to ConfigMem::MaxFlash. If the flashnum is invalid, or if
            the stored checksum is incorrect, then the LED data members are
            set to zero.

   @param   flashnum the Flash number to fetch

*/
void Flash::Get(uint8_t flashnum)
{
  uint8_t Checksum = CHKSUM_INIT;
  if ((flashnum > 0) && (flashnum <= ConfigMem::MaxFlash)) {
    uint16_t MyAddress =
      ConfigMem::FlashBase + ((flashnum - 1) * FlashSize);
    Number = flashnum;
    // Get rest of config data, updating checksum along the way
    LED = ReadByte(&MyAddress, &Checksum);
    UpDuration = Read2Byte(&MyAddress, &Checksum);
    OnDuration = Read2Byte(&MyAddress, &Checksum);
    DownDuration = Read2Byte(&MyAddress, &Checksum);
    InterpulseInterval = Read2Byte(&MyAddress, &Checksum);
    // Finally, get the stored checksum
    Checksum ^= EEPROM.read(MyAddress);
  }
  //
  // Make sure that flashnum is in the correct range, and checksum is zero
  // If not, clear the data members.
  //
  if ((flashnum == 0) || (flashnum > ConfigMem::MaxFlash) ||
      (Checksum != 0)) {
    Number = 0;
    LED = 0;
    UpDuration = 0;
    OnDuration = 0;
    DownDuration = 0;
    InterpulseInterval = 0;
  }
}

/**
   @fn      Flash::isDefined
   @brief   Checks to see if a given Flash number has been configured

            If the Flash number parameter is out of range then return false.
            If the stored checksum is incorrect return false. Otherwise
            return true.

   @param   flashnum the Flash number to check
   @return  boolean true or false

*/
bool Flash::isDefined(uint8_t flashnum)
{
  if ((flashnum > 0) && (flashnum <= ConfigMem::MaxFlash)) {
    uint16_t MyAddress =
      ConfigMem::FlashBase + ((flashnum - 1) * FlashSize);
    uint8_t Checksum = CHKSUM_INIT;
    // Read config data, updating checksum along the way
    ReadByte(&MyAddress, &Checksum);        // LED number
    for (int i = 0; i < 4; i++) {
      Read2Byte(&MyAddress, &Checksum);   // timing values
    }
    // Finally, get the stored checksum
    Checksum ^= EEPROM.read(MyAddress);
    // If checksum is zero, success!
    if (Checksum == 0) {
      return true;
    }
  }
  // Invalid
  return false;
}

/**
   @fn      Flash::Display
   @brief   Displays a single Flash to the serial terminal

*/
void Flash::Display()
{
  String Msg = "";
#if HUMAN_READABLE_MSG == 1
  Msg += "Number: " + String(Number) + "   ";
  Msg += "LED: " + String(LED) + "   ";
  Msg += "UpDur: " + String(UpDuration) + "   ";
  Msg += "OnDur: " + String(OnDuration) + "   ";
  Msg += "DnDur: " + String(DownDuration) + "   ";
  Msg += "Interval: " + String(InterpulseInterval);
#else
  Msg += "f," + String(Number);
  Msg += "," + String(LED);
  Msg += "," + String(UpDuration);
  Msg += "," + String(OnDuration);
  Msg += "," + String(DownDuration);
  Msg += "," + String(InterpulseInterval);
#endif
  Serial.println(Msg);
}

/**
   @fn      Flash::GetInterpulseInterval
   @brief   Gets the InterpulseInterval of a Flash from EEPROM

   @param   flashnum the Flash number to fetch
   @return  the InterpulseInterval

*/
uint16_t Flash::GetInterpulseInterval(uint8_t flashnum)
{
  uint8_t Checksum = CHKSUM_INIT;
  uint16_t MyAddress = ConfigMem::FlashBase + ((flashnum - 1) * FlashSize);
  MyAddress += 7;             // Offset to Interpulse Interval
  return (Read2Byte(&MyAddress, &Checksum));
}

/**
  @fn    Flash::Execute
  @brief Executes the flash

         Uses 10ms timesteps, all durations should be multiples of 10ms
*/
void Flash::Execute()
{
  const int TimePerStep = 10; //<! milliseconds per timestep
  int Steps, StepRound;

  ld.Get(LED);

  Steps = UpDuration / TimePerStep;
  StepRound = Steps / 2;

  MSDelay(0);

  for (int step = 0; step < Steps; step++) {
    ld.setLevel((step * 100 + StepRound) / Steps);
    MSDelay(10);
  }

  ld.setLevel(100);
  MSDelay(OnDuration);

  Steps = DownDuration / TimePerStep;
  StepRound = Steps / 2;

  for (int step = Steps; step > 0; step--) {
    ld.setLevel(((step - 1) * 100 + StepRound) / Steps);
    MSDelay(10);
  }

  ld.setLevel(0);
  MSDelay(InterpulseInterval - UpDuration - OnDuration - DownDuration);
}
/**
   @fn      Pattern::Save
   @brief   Saves an Pattern object to EEPROM

            Returns true if Pattern configuration was stored in EEPROM.
            Returns false, and does not change EEPROM, if any of these is true:
                - Number is 0 or > ConfigMem::MaxPattern
                - FlashPatternInterval is 0 or > 32767
                - Any FlashList element is not defined
                - The sum of the InterFlashIntervals is > FlashPatternInterval

   @return boolean true or false

*/
bool Pattern::Save(void)
{
  //
  // Calculate total time used by flashes, look for non-zero flash numbers
  // that have not been defined. A flash number of zero is just ignored.
  //
  uint32_t TotalFlashTime = 0;
  int UndefinedFlash = 0;
  for (int i = 0; i < 16; i++) {
    if (Flash::isDefined(FlashList[i])) {
      TotalFlashTime += Flash::GetInterpulseInterval(FlashList[i]);
    } else {
      if (FlashList[i] != 0) {
        UndefinedFlash++;
      }
    }
  }

  // Don't touch the EEPROM if values are invalid
  if ((Number == 0) || (Number > ConfigMem::MaxPattern) ||
      (FlashPatternInterval == 0) || (FlashPatternInterval > 32767) ||
      (FlashPatternInterval < TotalFlashTime) || (UndefinedFlash > 0)) {
    return false;
  } else {
    uint16_t MyAddress = ConfigMem::PatternBase + ((Number - 1)
                         * PatternSize);
    uint8_t Checksum = CHKSUM_INIT;
    // Write configuration data
    Write2Byte(&MyAddress, FlashPatternInterval, &Checksum);
    for (int i = 0; i < 16; i++) {
      WriteByte(&MyAddress, FlashList[i], &Checksum);
    }
    // Write checksum
    EEPROM.update(MyAddress, Checksum);
    return true;
  }
}

/**
   @fn      Pattern::Get
   @brief   Retrieves an Pattern object from EEPROM

            The pattnum parameter must be greater than zero and less than or
            equal to ConfigMem::MaxPattern. If this is not true, or if the
            stored checksum is invalid, then the data members are set to zero.

   @param   pattnum the Pattern number to fetch

*/
void Pattern::Get(uint8_t pattnum)
{
  uint8_t Checksum = CHKSUM_INIT;
  if ((pattnum > 0) && (pattnum <= ConfigMem::MaxPattern)) {
    uint16_t MyAddress =
      ConfigMem::PatternBase + ((pattnum - 1) * PatternSize);
    Number = pattnum;
    // Get rest of config data, updating checksum along the way
    FlashPatternInterval = Read2Byte(&MyAddress, &Checksum);
    for (int i = 0; i < 16; i++) {
      FlashList[i] = ReadByte(&MyAddress, &Checksum);
    }
    // Finally, get the stored checksum
    Checksum ^= EEPROM.read(MyAddress);
  }
  // Make sure that checksum is zero
  // If not, zero all of the class data members
  if (Checksum != 0) {
    Number = 0;
    FlashPatternInterval = 0;
    for (int i = 0; i < 16; i++) {
      FlashList[i] = 0;
    }
  }
}
/**
   @fn      Pattern::isDefined
   @brief   Checks to see if a given Pattern number has been configured

            Read the EEPROM to see if a valid configuration has been
            stored. If so then return true, else return false.

   @param   pattnum the Pattern number to check
   @return  boolean true or false

*/
bool Pattern::isDefined(uint8_t pattnum) {
  if ((pattnum > 0) && (pattnum <= ConfigMem::MaxPattern)) {
    uint16_t MyAddress = ConfigMem::PatternBase + ((pattnum - 1)
                         * PatternSize);
    uint8_t Checksum = CHKSUM_INIT;
    // Get config data, updating checksum along the way
    Read2Byte(&MyAddress, &Checksum);
    for (int i = 0; i < 16; i++) {
      ReadByte(&MyAddress, &Checksum);
    }
    // Finally, get the stored checksum
    Checksum ^= EEPROM.read(MyAddress);
    // If checksum is zero, success!
    if (Checksum == 0) {
      return true;
    }
  }
  // Invalid pattnum
  return false;
}

/**
   @fn      Pattern::Display
   @brief   Displays a single Pattern to the serial terminal

*/
void Pattern::Display(void)
{
  String Msg = "";
#if HUMAN_READABLE_MSG == 1
  Msg += "Number: " + String(Number) + "   ";
  Msg += "FlashPatternInterval: " + String(FlashPatternInterval) + "   ";
  Msg += "FlashList:";
  int i = 0;
  while ((i < 16) && (FlashList[i] > 0)) {
    Msg += " " + String(FlashList[i]);
    i++;
  }
#else
  Msg += "p," + String(Number);
  Msg += "," + String(FlashPatternInterval);
  int i = 0;
  while ((i < 16) && (FlashList[i] > 0)) {
    Msg += "," + String(FlashList[i++]);
  }
#endif
  Serial.println(Msg);
}

/**
  @fn    Pattern::Execute
  @brief Executes the pattern

  Before beginning each pattern we send a message to the host with the
  current time, temperature, and pattern number.
*/
void Pattern::Execute()
{
  uint16_t TimeLeft = FlashPatternInterval;

  String Msg = "p,";
  Msg += String(RealTimeClock::DateTime());
  Msg += ",";
  Msg += String(TempSensor::Temperature());
  Msg += ",";
  Msg += String(Number, DEC);
  Serial.println(Msg);

  for (int i = 0; i < 16; i++) {
    if (FlashList[i] != 0) {
      fl.Get(FlashList[i]);
      TimeLeft -= fl.InterpulseInterval;
      fl.Execute();
    }
  }

  MSDelay(TimeLeft);
}

/**
   @fn      RandPatternSet::Save
   @brief   Saves a Random Pattern Set object to EEPROM

            Returns true if Random Pattern Set was stored in EEPROM.
            Returns false, and does not change EEPROM, if any of these is true:
                - Number is 0 or > ConfigMem::MaxRandPatternSet
                - Any selected pattern is undefined

   @return boolean true or false

*/
bool RandPatternSet::Save(void)
{
  // Don't touch the EEPROM if values are invalid
  if ((Number == 0) || (Number > ConfigMem::MaxPatternSet)) {
    return false;
  }
  for (int i = 1; i <= ConfigMem::MaxPattern; i++) {
    if (((PatternSet & (1 << (i - 1))) != 0) && (! Pattern::isDefined(i))) {
      return false;
    }
  }

  uint16_t MyAddress = ConfigMem::RandomBase +
                       ((Number - 1) * RandomSize);

  uint8_t Checksum = CHKSUM_INIT;
  // Write configuration data
  Write2Byte(&MyAddress, PatternSet, &Checksum);
  // Write checksum
  EEPROM.update(MyAddress, Checksum);
  return true;
}

/**
   @fn      RandPatternSet::Get
   @brief   Retrieves a RandPatternSet object from EEPROM

            The setnum parameter must be greater than zero and less than or
            equal to ConfigMem::MaxRandPatternSet. If this is not true, or if
            the stored checksum is invalid, then the data members are set to
            zero.

   @param   setnum the RandPatternSet number to fetch

*/
void RandPatternSet::Get(uint8_t setnum)
{
  uint8_t Checksum = CHKSUM_INIT;
  if ((setnum > 0) && (setnum <= ConfigMem::MaxPatternSet)) {
    uint16_t MyAddress = ConfigMem::RandomBase +
                         ((setnum - 1) * RandomSize);
    Number = setnum;
    // Get config data, updating checksum along the way
    PatternSet = Read2Byte(&MyAddress, &Checksum);
    // Finally, get the stored checksum
    Checksum ^= EEPROM.read(MyAddress);
  }
  // Make sure that checksum is zero
  // If not, zero all of the class data members
  if (Checksum != 0) {
    Number = 0;
    PatternSet = 0;
  }
}
/**
   @fn      RandPatternSet::isDefined
   @brief   Checks to see if a given RandPatternSet number has been configured

            Read the EEPROM to see if a valid configuration has been
            stored. If so then return true, else return false.

   @param   setnum the RandPatternSet number to check
   @return  boolean true or false

*/
bool RandPatternSet::isDefined(uint8_t setnum) {
  if ((setnum > 0) && (setnum <= ConfigMem::MaxPatternSet)) {
    uint16_t MyAddress = ConfigMem::RandomBase +
                         ((setnum - 1) * RandomSize);
    uint8_t Checksum = CHKSUM_INIT;
    // Get config data, updating checksum along the way
    Read2Byte(&MyAddress, &Checksum);
    // Finally, get the stored checksum
    Checksum ^= EEPROM.read(MyAddress);
    // If checksum is zero, success!
    if (Checksum == 0) {
      return true;
    }
  }
  // Invalid setnum
  return false;
}

/**
   @fn      RandPatternSet::Display
   @brief   Displays a single RandPatternSet to the serial terminal

*/
void RandPatternSet::Display(void)
{
  String Msg = "";
#if HUMAN_READABLE_MSG == 1
  Msg += "Number: " + String(Number) + "   ";
  Msg += "PatternSet:";
#else
  Msg += "r," + String(Number);
#endif
  for (int i = 0; i < ConfigMem::MaxPattern; i++) {
    if ((PatternSet & (1 << i)) != 0) {
#if HUMAN_READABLE_MSG == 1
      Msg += " " + String(i + 1);
#else
      Msg += "," + String(i + 1);
#endif
    }
  }
  Serial.println(Msg);
}

/**
  @fn    RandPatternSet::Execute
  @brief Executes the set of random patterns

*/
void RandPatternSet::Execute(void)
{
  while (true) {
    int patnum = random(1, 17); // max value is exclusive
    if ((rd.PatternSet & (1 << (patnum - 1))) != 0) {
      pt.Get(patnum);
      pt.Execute();
    }
  }
}
