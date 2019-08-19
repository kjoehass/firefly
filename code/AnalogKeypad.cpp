/**
   @file AnalogKeypad.cpp

   @brief  Provides interface to a multiplexed keypad connected to an analog
           input pin.

   @author K. Joseph Hass
   @date Created: 2019-04-23T11:52:05-0400
   @date Last modified: 2019-08-19T16:03:46-0400

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
#include "AnalogKeypad.h"

namespace {
//
// These are the typical raw values read from the ADC when a key is pressed.
// They depend on the actual resistor values used with the keypad.
//
enum {
  RAWHASH = 0,
  RAW0 = 131,
  RAW9 = 178,
  RAW8 = 270,
  RAW6 = 348,
  RAW5 = 409,
  RAW3 = 464,
  RAW2 = 505,
  RAWSTAR = 549,
  RAW7 = 591,
  RAW4 = 637,
  RAW1 = 673,
  RAWNONE = 853
};

//
// Calculate the midpoint values between any two raw key readings. These are
// the decision points for translating an analog value to a specific key.
//
const int KeyThresh[] = { (RAWNONE + RAW1) / 2,
                          (RAW1 + RAW4) / 2,
                          (RAW4 + RAW7) / 2,
                          (RAW7 + RAWSTAR) / 2,
                          (RAWSTAR + RAW2) / 2,
                          (RAW2 + RAW3) / 2,
                          (RAW3 + RAW5) / 2,
                          (RAW5 + RAW6) / 2,
                          (RAW6 + RAW8) / 2,
                          (RAW8 + RAW9) / 2,
                          (RAW9 + RAW0) / 2,
                          (RAW0 + RAWHASH) / 2,
                          -1
                        };

//
// Element 0 is a dummy value, means that no key was pressed.
//
const char KeyValue[] = { ' ', '1', '4', '7', '*', '2', '3', '5', '6', '8',
                          '9', '0', '#'
                        };

}

//
// Initialize static data members
//
char AnalogKeypad::LastKey = 0;
int AnalogKeypad::TimesPressed = 0;

/**
   @fn     AnalogKeypad::PressedKey
   @brief  Returns a character if a key is pressed, else 0

           The analog voltage is read and converted to the corresponding
           key character. We read the voltage just once, so this function
           must be called at least MinTimesPressed times before a valid key
           press will be recognized. However, the function <b>does not</b>
           block or add any fixed delay.

   @return ASCII character for pressed key or 0
*/
char AnalogKeypad::PressedKey(void)
{
  //
  // Read the raw analog value
  //
  int sensorValue = analogRead(AnalogKeypad::KeypadPin);
  //
  // Convert analog reading to key index
  //
  int ThisKey = 0;
  while (sensorValue < KeyThresh[ThisKey]) {
    ThisKey++;
  }
  //
  // If a key was pressed and it was the same as last time
  //   If we're still counting how many times it was pressed
  //     Increment the count
  //     If we hit the limit, return the character for the key
  // else clear the record of the last key pressed and the count
  //
  // It is very important that this logic does not allow the same
  // key value to be returned repeatedly without first being released.
  // Once we hit the count limit for a particular key then that
  // same key will be ignored until it is released.
  //
  if ((LastKey == ThisKey) && (ThisKey > 0)) {
    if (TimesPressed < AnalogKeypad::MinTimesPressed) {
      TimesPressed++;
      if (TimesPressed == AnalogKeypad::MinTimesPressed) {
        return KeyValue[ThisKey];
      }
    }
  } else {
    LastKey = ThisKey;
    TimesPressed = 0;
  }
  return 0;
}
