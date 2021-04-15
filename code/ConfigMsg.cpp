/**
  @file ConfigMsg.cpp

  @brief  Processes configuration and display messages from the host

  @author Matt Turconi, K. Joseph Hass
  @date Created: 2019-09-04T12:24:37-0400
  @date Last modified: 2019-09-04T13:02:22-0400

  @copyright Copyright (C) 2019 Matt Turconi, Kenneth Joseph Hass

  @copyright This program is free software: you can redistribute it and/or
  modify it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or (at your
  option) any later version.

  @copyright This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
  Public License for more details.
*/
#include "Arduino.h"
#include "Firefly.h"
#include "ConfigData.h"
#include "ConfigMsg.h"
#include "RealTimeClock.h"
#include "TempSensor.h"

/**
  @var   toks
  @brief Array of tokens extracted from a message from the host PC
*/
int toks[20];

/**
  @fn    display_LEDs
  @brief Sends LED info back to the host PC

         A separate line for each properly configured LED is sent over the
         serial communications interface.
*/
void display_LEDs() {
  int i;
  for (i = 1; i <= ConfigMem::MaxLED; i++) {
    if (ld.isDefined(i)) {
      ld.Get(i);
      ld.Display();
    }
  }
}

/**
  @fn    display_Flashes
  @brief Sends Flash info back to the host PC

         A separate line for each properly configured Flash is sent over the
         serial communications interface.
*/
void display_Flashes() {
  int i;
  for (i = 1; i <= ConfigMem::MaxFlash; i++) {
    if (fl.isDefined(i)) {
      fl.Get(i);
      fl.Display();
    }
  }
}

/**
  @fn    display_Patterns
  @brief Sends Pattern info back to the host PC

         A separate line for each properly configured Pattern is sent over
         the serial communications interface.
*/
void display_Patterns() {
  int i;
  for (i = 1; i <= ConfigMem::MaxPattern; i++) {
    if (pt.isDefined(i)) {
      pt.Get(i);
      pt.Display();
    }
  }
}

/**
  @fn    display_Random
  @brief Sends Random Pattern Set info back to the host PC

         A separate line for each properly configured Random Pattern Set is
         sent over the serial communications interface.
*/
void display_Random() {
  for (int i = 1; i <= ConfigMem::MaxPatternSet; i++) {
    if (rd.isDefined(i)) {
      rd.Get(i);
      rd.Display();
    }
  }
}

/**
  @fn    config_LED
  @brief Parses message from host and configures an LED

         The input string should have exactly 4 tokens: the letter L,
         the LED number, the PWM channel, and the maximum brightness.

  @param dat string containing message from host
*/
void config_LED(char *dat) {
  Serial.print(dat); // echo
  int size = tokenize(dat);
  if ((size == 4) && \
      (toks[1] <= ConfigMem::MaxLED) && \
      (toks[2] <= ConfigMem::MaxChannel) && \
      (toks[3] < 101) && \
      (toks[3] >= 1)) {
    ld.Number = toks[1];
    ld.Channel = toks[2];
    ld.MaxBrightness = toks[3];
    if (ld.Save()) {
      Serial.println("LED Configured");
    } else {
      Serial.println("LED Save Error");
    }
  } else {
    Serial.println("Invalid Input. LED Not Configured");
  }
}

/**
  @fn    config_Flash
  @brief Parses message from host and configures a Flash

         The input string should have exactly 7 tokens: the letter F,
         the flash number, the LED number, the up duration, the on duration,
         the down duration, and the interpulse interval.

  @param dat string containing message from host
*/
void config_Flash(char *dat) {
  Serial.print(dat); // echo
  int size = tokenize(dat);
  if (size == 7) {
    fl.Number = toks[1];
    fl.LED = toks[2];
    fl.UpDuration = toks[3];
    fl.OnDuration = toks[4];
    fl.DownDuration = toks[5];
    if ((fl.UpDuration % 10 != 0) || (fl.DownDuration % 10 != 0)) {
      Serial.println("Up and Down Duration must be multiples of ten.\nFlash not configured.");
      return;
    }
    fl.InterpulseInterval = toks[6];
    if (fl.Save()) {
      Serial.println("Flash Configured");
    } else {
      Serial.println("Flash Configuration Error");
    }
  } else {
    Serial.println("Flash Configuration Error");
  }
}

/**
  @fn    config_Pattern
  @brief Parses message from host and configures a Pattern

         The input string should have at least 4 tokens: the letter P,
         the pattern number, the flash pattern interval, and 1 to 16 flash
         numbers. The maximum number of tokens is 19.

  @param dat string containing message from host
*/
void config_Pattern(char *dat) {
  Serial.print(dat);
  int size = tokenize(dat);
  if (size >= 4) {
    pt.Number = toks[1];
    pt.FlashPatternInterval = toks[2];
    //
    // Put specified flash numbers into FlashList, fill rest of array
    // with zeros
    //
    int i;
    for (i = 3; i < size; i++) {
      pt.FlashList[i - 3] = toks[i];
    }
    for ( ; i < 19; i++) {
      pt.FlashList[i - 3] = 0;
    }
    if (pt.Save()) {
      Serial.println("Pattern Configured");
    } else {
      Serial.println("Pattern Configuration Error");
    }
  } else {
    Serial.println("Pattern Configuration Error");
  }
}

/**
  @fn    config_Random
  @brief Parses message from host and configures a random pattern set

         The input string should have at least 3 tokens: the letter R,
         the pattern set number, and at least one pattern to add to
         the set. Up to 16 patterns can be added to the set, from 1 to
         16. The set is stored as a single 16-bit integer, with bit N-1
         set to 1 to indicate that pattern N is in the set.

  @param dat pointer to input character string
*/
void config_Random(char *dat)
{
  Serial.print(dat);
  rd.PatternSet = 0;
  int size = tokenize(dat);
  if ((size >= 3) && (size <= 18)) {
    rd.Number = toks[1];
    for (int i = 2; i < size; i++) {
      rd.PatternSet |= (1 << (toks[i] - 1));
    }
    if (rd.Save()) {
      Serial.println("Random Pattern Set Configured");
    } else {
      Serial.println("Random Pattern Set Configuration Error");
    }
  } else {
    Serial.println("Random Pattern Set Configuration Error");
  }
}

/**
  @fn     config_Time
  @brief  Sets the real-time clock on the simulator

          The input string should have at exactly 7 tokens: the letter T,
          the year, the month, the date, the hour, the minute, and seconds.
          The year is a 4-digit number; all other numeric fields are 2-digit
          integers.

  @param  dat pointer to input character string
*/
void config_Time(char *dat)
{
  Serial.print(dat);
  int size = tokenize(dat);
  if (size == 7) {
    RealTimeClock::Set(toks[1], toks[2], toks[3], toks[4], toks[5], toks[6]);
    Serial.println("Real-Time Clock Configured");
  } else {
    Serial.println("Time Configuration Error");
  }
}

/**
  @fn    tokenize
  @brief Extract tokens from the message sent by the host

         The tokens are converted to integers and loaded into a global array
         'toks'. Unused entries in the 'toks' array are set to zero.

  @param  dat pointer to input character string
  @return number of tokens found
*/
int tokenize(char *dat) {

  int index;
  for (index = 0; index < (sizeof toks / sizeof * toks); index++) {
    toks[index] = 0;
  }

  char* ptr = NULL;
  strtok(dat, ",");
  index = 1;
  while (1) {
    ptr = strtok(NULL, ",");
    if (ptr == NULL) {
      break;
    }
    if (index >= (sizeof toks / sizeof * toks)) {
      Serial.println("Error: Message has too many tokens");
      return 0;
    }
    toks[index] = atoi(ptr);
    index ++;
  }
  return index;
}
