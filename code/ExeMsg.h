/**
  @file ExeMsg.h

  @brief All commands that somehow flash the LEDs are handled here.

  @author Matt Turconi, K. Joseph Hass
  @date Created: 2019-08-19T19:08:30-0400
  @date Last modified: 2019-09-04T09:41:37-0400

  @copyright Copyright (C) 2019 Kenneth Joseph Hass

  @copyright This program is free software: you can redistribute it and/or
  modify it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or (at your
  option) any later version.

  @copyright This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FiTNESS FOR A PARTICULAR PURPOSE. See the GNU General
  Public License for more details.

*/
#ifndef __exemsg__
#define __exemsg__

/**
  @fn    exec_led
  @brief Turns on an LED to specified brightness

  @param dat pointer to command string
*/
void exec_led(char *dat);

/**
  @fn    exec_flash
  @brief Repeatedly executes the specified flash.

  Most of the work in this function is retrieving the flash data structure
  for the flash number specified in the command string.

  @param dat pointer to command string
*/
void exec_flash(char *dat);

/**
  @fn    exec_pattern
  @brief Repeatedly executes the specified pattern.

  Most of the work in this function is retrieving the pattern data structure
  for the pattern number specified in the command string.

  @param dat pointer to command string
*/
void exec_pattern(char *dat);

/**
  @fn    exec_random_pat
  @brief Executes the specified random pattern set.

  Most of the work in this function is retrieving the pattern set data
  structure for the pattern set number specified in the command string.

  @param dat pointer to command string
*/
void exec_random_pat(char *dat);

/**
  @fn    exec_event_msg
  @brief Handles the execute event message.

  This is a placeholder for a function that is not yet defined.

  @param dat pointer to command string
*/
void exec_event_msg(char *dat);

#endif
