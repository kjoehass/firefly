/**
  @file ExeMsg.cpp

  @brief All commands that somehow flash the LEDs are handled here.

  @author Matt Turconi, K. Joseph Hass
  @date Created: 2019-08-19T19:08:30-0400
  @date Last modified: 2019-09-24T14:07:37-0400

  @copyright Copyright (C) 2019 Matt Turconi and Kenneth Joseph Hass

  @copyright This program is free software: you can redistribute it and/or
  modify it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or (at your
  option) any later version.

  @copyright This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FiTNESS FOR A PARTICULAR PURPOSE. See the GNU General
  Public License for more details.

*/
#include "Firefly.h"
#include "ConfigData.h"
#include "ConfigMsg.h"
#include "Arduino.h"

/**
  @fn    exec_led
  @brief Turns on an LED to specified brightness
   
         If the specified brightness token is missing, the brightness
         defaults to 100% (of max brightness).

  @param dat pointer to command string
*/
void exec_led(char *dat) {
  tokenize(dat);

  int LED_num = toks[1];
  //
  // Get the LED configuration from EEPROM and light it up
  //
  if (ld.isDefined(LED_num)) {
    Serial.print("Executing LED ");
    Serial.println(LED_num);
    ld.Get(LED_num);
    if (toks[2] == 0) {
      ld.setLevel(100);
    } else {
      ld.setLevel(toks[2]);
    }
  } else {
    Serial.println("ERROR");
  }
}

/**
  @fn    exec_flash
  @brief Repeatedly executes the specified flash.

  Most of the work in this function is retrieving the flash data structure
  for the flash number specified in the command string.

  @param dat pointer to command string
*/
void exec_flash(char *dat) {
  tokenize(dat);

  //Loads the correct flash from eeprom
  int flash_num = toks[1];
  if (fl.isDefined(flash_num) == false) {
    Serial.println("Invalid Flash Number");
    return;
  }
  fl.Get(flash_num);

  Serial.print("Executing flash ");
  Serial.println(flash_num);

  //Execute the flash repeatedly
  while (true) {
    fl.Execute();
  }
}

/**
  @fn    exec_pattern
  @brief Repeatedly executes the specified pattern.

  Most of the work in this function is retrieving the pattern data structure
  for the pattern number specified in the command string.

  @param dat pointer to command string
*/
void exec_pattern(char* dat) {
  tokenize(dat);

  //Load the pattern from EEPROM
  int pat_num = toks[1];
  if (pt.isDefined(pat_num) == false) {
    Serial.print("Invalid Pattern Number");
    return;
  }
  pt.Get(pat_num);
  while (true) {
    pt.Execute();
  }
}

/**
  @fn    exec_random_pat
  @brief Executes the specified random pattern set.

  Most of the work in this function is retrieving the pattern set data
  structure for the pattern set number specified in the command string.

  @param dat pointer to command string
*/
void exec_random_pat(char* dat) {
  tokenize(dat);

  //Load the pattern set from EEPROM
  int set_num = toks[1];
  if (!rd.isDefined(set_num)) {
    Serial.print("Invalid Random Pattern Set Number");
    return;
  }
  rd.Get(set_num);
  rd.Display();
  // Note that the Execute() function for a pattern set runs continuously
  // so it need not be repeated here.
  rd.Execute();
}

/**
  @fn    exec_event_msg
  @brief Handles the execute event message.

  This is a placeholder for a function that is not yet defined.

  @param dat pointer to command string
*/
void exec_event_msg(char* dat) {
  return;
}
