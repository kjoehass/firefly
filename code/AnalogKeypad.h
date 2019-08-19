/**
   @file AnalogKeypad.h

   @brief  Provides interface to a multiplexed keypad connected to an analog
           input pin.

   @author K. Joseph Hass
   @date Created: 2019-04-23T11:45:43-0400
   @date Last modified: 2019-08-19T16:05:30-0400

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
#ifndef __ANALOGKEYPAD_H
#define __ANALOGKEYPAD_H

#include "Arduino.h"

/**
   @class  AnalogKeypad
   @brief  Support for a keypad connected to an analog input

           When a key is pressed the analog input voltage changes, and this
           takes time. We need to make sure that the voltage has remained
           stable and relatively constant for a reasonable length of time
           before declaring that a key was pressed. There is just one method
           and it is static, so it is not necessary to construct an object.
*/
class AnalogKeypad {
  public:
    /**
       @fn     PressedKey
       @brief  Returns a character if a key is pressed, else 0

               The analog voltage is read and converted to the corresponding
               key character. We read the voltage just once, so this function
               must be called at least MinTimesPressed times before a valid key
               press will be recognized. However, the function <b>does not</b>
               block or add any fixed delay.

       @return ASCII character for pressed key or 0
    */
    static char PressedKey(void);
  private:
    enum {
      //
      // Analog input used for the keypad
      //
      KeypadPin = A0,
      //
      // Number of times to read the same key value before believing it.
      // This number may be increased if false presses are reported.
      //
      MinTimesPressed = 100
    };
    /**
       @var   LastKey
       @brief Keeps track of the key pressed previously
    */
    static char LastKey;
    /**
       @var   TimesPressed
       @brief Counts how many times the same key has been pressed
    */
    static int TimesPressed;
};

#endif
