/**
 * @file ConfigData.cpp
 *
 * @brief  Manage the process of storing/retrieving data from EEPROM
 *
 *         All of the data members of an LED, Flash, Pattern, or Event object
 *         are preserved in EEPROM, except for the Number member. The Number
 *         member is used to calculate the address in EEPROM where the object
 *         is stored, so its value can be inferred from the EEPROM address.
 *
 *         A one-byte checksum is calculated for every object stored in the
 *         EEPROM. The checksum is calculated by adding all of the bytes of
 *         the class's data elements, excluding the Number member, modulo 256.
 *         The initial value of the checksum is set so that if all of the
 *         bytes of the EEPROM are 0xFF or if they are all 0x00 then the
 *         checksum will be seen as invalid.
 *
 * @author K. Joseph Hass
 * @date Created: 2019-02-20T08:28:10-0500
 * @date Last modified: 2019-03-05T11:13:16-0500
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
#include <EEPROM.h>
#include "ConfigData.h"

#define CHKSUM_INIT 0xA5
//
// The number of bytes reserved in EEPROM for each configuration data
// structure. The EEPROM in the ATmega chips has a page size of 4 bytes
// so all allocations are in multiples of 4 bytes.
// 
namespace {
    const uint8_t PatternSize = 32;  //!< # of EEPROM bytes for a pattern
    const uint8_t FlashSize = 16;    //!< # of EEPROM bytes for a flash
    const uint8_t LEDSize = 4;       //!< # of EEPROM bytes for an LED
    const uint8_t EventSize = 4;     //!< # of EEPROM bytes for an event
}
//
// Define and initialize static member data of the ConfigMem
//
uint8_t  ConfigMem::MaxChannel  = 0;
uint8_t  ConfigMem::MaxEvent    = 0;
uint8_t  ConfigMem::MaxFlash    = 0;
uint8_t  ConfigMem::MaxLED      = 0;
uint8_t  ConfigMem::MaxPattern  = 0;
uint16_t ConfigMem::PatternBase = 0;
uint16_t ConfigMem::FlashBase   = 0;
uint16_t ConfigMem::LEDBase     = 0;
uint16_t ConfigMem::EventBase   = 0;

/**
 * @fn    WriteByte
 * @brief Writes one byte to EEPROM, updating checksum and address
 *
 * @param address_p pointer to EEPROM address
 * @param byte value to be stored
 * @param checksum_p pointer to the checksum
 */
static void WriteByte(uint16_t *address_p, uint8_t byte, uint8_t *checksum_p) {
  EEPROM.update(*address_p, byte);
  (*checksum_p) += byte;
  (*address_p)++;
}
/**
 * @fn    Write2Byte
 * @brief Writes two bytes to EEPROM, updating checksum and address
 *
 *        The 16-bit value is written low-byte first.
 *
 * @param address_p pointer to EEPROM address
 * @param word 16-bit value to be stored
 * @param checksum_p pointer to the checksum
 */
static void Write2Byte(uint16_t *address_p, uint16_t word, uint8_t *checksum_p) {
  uint8_t byte = (uint8_t)(word & 0xFF);
  EEPROM.update(*address_p, byte);
  (*checksum_p) += byte;
  (*address_p)++;

  byte = (uint8_t)((word >> 8) & 0xFF);
  EEPROM.update(*address_p, byte);
  (*checksum_p) += byte;
  (*address_p)++;
}
/**
 * @fn    ReadByte
 * @brief Reads one byte from EEPROM, updating checksum and address
 *
 * @param address_p pointer to EEPROM address
 * @param checksum_p pointer to the checksum
 * @return byte read from EEPROM
 */
static uint8_t ReadByte(uint16_t *address_p, uint8_t *checksum_p) {
  uint8_t byte;
  byte = EEPROM.read(*address_p);
  (*checksum_p) += byte;
  (*address_p)++;
  return byte;
}
/**
 * @fn    Read2Byte
 * @brief Read two bytes from EEPROM, updating checksum and address
 *
 *        The low byte is read first and then the high byte of the 16-bit
 *        word is read from the higher address.
 *
 * @param address_p pointer to EEPROM address
 * @param checksum_p pointer to the checksum
 * @return word read from EEPROM
 */
static uint16_t Read2Byte(uint16_t *address_p, uint8_t *checksum_p) {
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
 * @fn      ConfigMem::Init
 * @brief   Get EEPROM size, calculate EEPROM parameters
 *
 *          This function must be called before any of the static class data
 *          members are used. It reads the size of the EEPROM on the Arduino
 *          processor and sets reasonable limits for the number of unique
 *          LEDs, Flashes, Patterns and Events that can be configured. Based
 *          on these limits, the starting address is calculated for the regions
 *          of EEPROM that store the different kinds of objects.
 *
 * @todo    Set MaxChannel based on type of Arduino used
 * @todo    Do proper checks for different Arduino models
 */
void ConfigMem::Init(void) {
    MaxChannel = 6;
    if (EEPROM.length() == 1024) {
      MaxPattern = 16;
      MaxFlash = 16;
      MaxLED = 16;
      MaxEvent = 16;
    } else {
      MaxPattern = 8;
      MaxFlash = 8;
      MaxLED = 8;
      MaxEvent = 8;
    }
    PatternBase = 0x000;
    FlashBase   = PatternBase + (MaxPattern * PatternSize);
    LEDBase     = FlashBase + (MaxFlash * FlashSize);
    EventBase   = LEDBase + (MaxLED * LEDSize);
}

/**
 * @fn      LED::Save
 * @brief   Saves an LED object to EEPROM
 * 
 *          Returns true if LED configuration was stored in EEPROM.
 *          Returns false, and does not change EEPROM, if any of these is true:
 *              - Number is 0 or > ConfigMem::MaxLED
 *              - Channel is 0 or > ConfigMem::MaxChannel
 *              - Brightness is 0 or > 100
 *          
 * @return boolean true or false
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
 * @fn      LED::Get
 * @brief   Loads an LED object's data members from EEPROM
 * 
 *          The lednum parameter must be greater than zero and less than or
 *          equal to ConfigMem::MaxLED. If the lednum is invalid, or if the
 *          stored checksum is incorrect, then the LED data members are set
 *          to zero.
 *
 * @param   lednum the LED number to fetch
 *
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
 * @fn      LED::isDefined
 * @brief   Checks to see if a given LED number has been configured
 * 
 *          If the LED number parameter is out of range then return false.
 *          Read the EEPROM to see if a valid checksum has been stored.
 *          If so then return true, else return false.
 *
 * @param   lednum the LED number to check
 * @return  boolean true or false
 *
 */
bool LED::isDefined(uint8_t lednum)
{
    if ((lednum > 0) && (lednum <= ConfigMem::MaxLED)) {
        uint16_t MyAddress = ConfigMem::LEDBase + ((lednum - 1) * LEDSize);
        uint8_t Checksum = CHKSUM_INIT;
        ReadByte(&MyAddress, &Checksum); // Channel
        ReadByte(&MyAddress, &Checksum); // MaxBrightness
        // Finally, get the stored checksum
        Checksum ^= EEPROM.read(MyAddress);
        if (Checksum == 0) {
          return true;
        }
    }
    return false;
}

/**
 * @fn      Flash::Save
 * @brief   Saves a Flash object to EEPROM
 * 
 *          Returns true if Flash configuration was stored in EEPROM.
 *          Returns false, and does not change EEPROM, if any of these is true:
 *              - Number is 0 or > ConfigMem::MaxFlash
 *              - LED is  > ConfigMem::MaxLED
 *              - Any duration is > 32767
 *              - InterpulseInterval is 0 or > 32767
 *          
 * @return boolean true or false
 * 
 */
bool Flash::Save(void)
{
    // Don't touch the EEPROM if values are invalid
    if ((Number == 0) || (Number > ConfigMem::MaxFlash) ||
        (LED > ConfigMem::MaxLED) ||
        (UpDuration > 32767) ||
        (OnDuration > 32767) ||
        (DownDuration > 32767) ||
        (InterpulseInterval == 0) || (InterpulseInterval > 32767)) {
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
 * @fn      Flash::Get
 * @brief   Retrieves a Flash object from EEPROM
 *
 *          The flashnum parameter must be greater than zero and less than or
 *          equal to ConfigMem::MaxFlash. If the flashnum is invalid, or if
 *          the stored checksum is incorrect, then the LED data members are
 *          set to zero.
 *
 * @param   flashnum the Flash number to fetch
 *
 */
void Flash::Get(uint8_t flashnum)
{
    uint8_t Checksum = CHKSUM_INIT;
    if ((flashnum > 0) && (flashnum <= ConfigMem::MaxFlash)) {
      uint16_t MyAddress = ConfigMem::FlashBase + ((flashnum - 1) * FlashSize);
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
 * @fn      Flash::isDefined
 * @brief   Checks to see if a given Flash number has been configured
 * 
 *          If the Flash number parameter is out of range then return false.
 *          If the stored checksum is incorrect return false. Otherwise
 *          return true.
 *
 * @param   flashnum the Flash number to check
 * @return  boolean true or false
 *
 */
bool Flash::isDefined(uint8_t flashnum)
{
    if ((flashnum > 0) && (flashnum <= ConfigMem::MaxFlash)) {
        uint16_t MyAddress =
            ConfigMem::FlashBase + ((flashnum - 1) * FlashSize);
        uint8_t Checksum = CHKSUM_INIT;
        // Read config data, updating checksum along the way
        ReadByte(&MyAddress, &Checksum);      // LED number
        for (int i = 0; i < 4; i++) {
            Read2Byte(&MyAddress, &Checksum); // timing values
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
 * @fn      Pattern::Save
 * @brief   Saves an Pattern object to EEPROM
 * 
 *          Returns true if Pattern configuration was stored in EEPROM.
 *          Returns false, and does not change EEPROM, if any of these is true:
 *              - Number is 0 or > ConfigMem::MaxPattern
 *              - FlashPatternInterval is 0 or > 32767
 *              - Any FlashList element is > ConfigMem::MaxFlash
 *          
 * @return boolean true or false
 *
 */
bool Pattern::Save(void)
{
    // Don't touch the EEPROM if values are invalid
    if ((Number == 0) || (Number > ConfigMem::MaxPattern) ||
        (FlashPatternInterval == 0) || (FlashPatternInterval > 32767)) {
        return false;
    } else {
        for (int i = 0; i < 16; i++) {
            if (FlashList[i] > ConfigMem::MaxFlash) {
              return false;
            }
        }
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
 * @fn      Pattern::Get
 * @brief   Retrieves an Pattern object from EEPROM
 *
 *          The pattnum parameter must be greater than zero and less than or
 *          equal to ConfigMem::MaxPattern. If this is not true, or if the
 *          stored checksum is invalid, then the data members are set to zero.
 * 
 * @param   pattnum the Pattern number to fetch
 *
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
 * @fn      Pattern::isDefined
 * @brief   Checks to see if a given Pattern number has been configured
 * 
 *          Read the EEPROM to see if a valid configuration has been
 *          stored. If so then return true, else return false.
 *
 * @param   pattnum the Pattern number to check
 * @return  boolean true or false
 *
 */
bool Pattern::isDefined(uint8_t pattnum)
{
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
