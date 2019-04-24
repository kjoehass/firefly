/**
 * @file Firefly.h
 *
 * @brief  Global configuration options for the firefly simulator
 *
 * @author Joe Hass and Matt Turconi
 * @date Created: 2019-04-14T10:51:46-0400
 * @date Last modified: 2019-04-24T15:20:20-0400
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
#ifndef _FIREFLY_H
#    define _FIREFLY_H

//
// The particular model of Arduino in use will determine
//   - The number of PWM channels available
//
#    define ARDUINO_IS_UNO  1
#    define ARDUINO_IS_MEGA 0

//
// Response messages can be send in human-readable or machine-readable form.
// Human-readable messages print meaningful identifiers for all of the fields
// in a message, but the machine-readable messages have a single, lower-case
// character as the first character and then all fields are numbers separated
// by commas (i.e. a csv format).
//
#    define HUMAN_READABLE_MSGS = 1

#endif
