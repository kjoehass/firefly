/**
 * @file ConfigData.h
 *
 * @brief Define constants and classes for the firefly simulator
 *
 *        These variables and classes are used to store and retrieve
 *        configuration information in nonvolatile memory. Configuration
 *        data for all LEDs, Flashes, Patterns, and Events is store in
 *        the EEPROM memory available on the ATmega processor on an Arduino
 *        board.
 *
 *        The size of the available EEPROM determines how many of each object
 *        can be stored. The ConfigMem::Init function is provided to determine
 *        these capacity parameters and make them available as static data
 *        members of the ConfigMem class.
 *
 * @author K. Joseph Hass
 * @date Created: 2019-02-10T13:26:37-0500
 * @date Last modified: 2019-04-14T10:59:56-0400
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
#ifndef __CONFIGDATA_H
#define __CONFIGDATA_H

#include "stdint.h"

/**
 * @class ConfigMem
 * @brief Non-volatile storage for configuration data
 *
 *        The configuration data is stored in the small EEPROM that is
 *        on the Arduino processor chip. It is not necessary to declare an
 *        instance of this class. All of the class members are static and
 *        should be accessed using the ConfigMem:: namespace.
 */
class ConfigMem {
  public:
    static void Init(void);

    static uint8_t MaxChannel;   //!< Physical PWM pins available
    static uint8_t MaxEvent;     //!< Max number of events in EEPROM
    static uint8_t MaxFlash;     //!< Max number of flashes in EEPROM
    static uint8_t MaxLED;       //!< Max number of LEDs in EEPROM
    static uint8_t MaxPattern;   //!< Max number of patterns in EEPROM

    static uint16_t PatternBase; //!< Base address of Patterns in EEPROM
    static uint16_t FlashBase;   //!< Base address of Flashes in EEPROM
    static uint16_t LEDBase;     //!< Base address of LEDs in EEPROM
    static uint16_t EventBase;   //!< Base address of Events in EEPROM
};

/**
 * @class   LED
 * @brief   An LED is defined by its `max brightness` and `channel`
 *
 */
class LED {
  public:
    uint8_t Number = 0;         //!< From 1 to MaxLED
    uint8_t Channel = 0;        //!< Physical connection, 1 to MaxChannel
    uint8_t MaxBrightness = 0;  //!< Maximum percent current, 1 to 100

    bool Save(void);
    void Get(uint8_t lednum);
    static bool isDefined(uint8_t lednum);
    void Display();
};

/**
 * @class   Flash
 * @brief   Define a `flash` by its timing and which LED is used
 *
 *          The LED member can be set to zero, and the Flash can then be used
 *          as a pure delay element in the FlashList of a Pattern. All timing 
 *          parameters are in milliseconds with a minimum value of 0 and a
 *          maximum value of 32767 ms, except that the InterpulseInterval must
 *          be greater than zero.
 */
class Flash {
  public:
    uint8_t Number = 0;         //!< From 1 to MaxFlash
    uint8_t LED = 0;            //!< Which LED to use, 0 to MaxLED
    uint16_t UpDuration = 0;    //!< Time from dark to full illumination
    uint16_t OnDuration = 0;    //!< Time flash stays at full illumination
    uint16_t DownDuration = 0;  //!< Time from full illumination to dark
    uint16_t InterpulseInterval = 0;    //!< Total duration of the flash

    bool Save(void);
    void Get(uint8_t flashnum);
    static uint16_t GetInterpulseInterval(uint8_t flashnum);
    static bool isDefined(uint8_t flashnum);
    void Display();
};
/**
 * @class   Pattern
 * @brief   Define a `pattern` by a list of flashes that will occur and by the
 *          total duration of the pattern
 *
 *          The FlashPatternInterval is specified in milliseconds, must be
 *          greater than 0, and has a maximum value of 32767 ms. A FlashList
 *          array of 16 flashes is specified for each pattern. One or all of
 *          these may be a real Flash, with a Flash Number that is an integer
 *          from 1 to MaxFlash. If fewer than 16 real flashes are used in a
 *          pattern then the unused elements in * FlashList <b>must</b> have
 *          a value of 0; FlashList values of zero are simply ignored.
 */
class Pattern {
  public:
    uint8_t Number = 0;         //!< From 1 to MaxPattern
    uint16_t FlashPatternInterval = 0;  //!< Total duration of pattern
    uint8_t FlashList[16];      //!< Array of 16 Flash numbers

    bool Save(void);
    void Get(uint8_t pattnum);
    static bool isDefined(uint8_t pattnum);
    void Display();
};
#endif                          /* __CONFIGDATA_H */
