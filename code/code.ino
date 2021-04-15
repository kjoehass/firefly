/**
  @file code.ino

  @brief Top-level functions for the firefly simulator 

  @author Matt Turconi, K. Joseph Hass
  @date Created: 2019-09-04T13:50:19-0400
  @date Last modified: 2019-09-04T13:51:36-0400

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
#include "Firefly.h"
#include "ConfigData.h"
#include "ConfigMsg.h"
#include "ExeMsg.h"
#include "RealTimeClock.h"
#include "TempSensor.h"
#include "AnalogKeypad.h"

/**
  @var   ld
  @brief A global variable that holds the "current" LED definition
*/
LED ld;
/**
  @var   fl
  @brief A global variable that holds the "current" flash definition
*/
Flash fl;
/**
  @var   pt
  @brief A global variable that holds the "current" pattern definition
*/
Pattern pt;
/**
  @var   rd
  @brief A global variable that holds the "current" random pattern set
         definition
*/
RandPatternSet rd;

/**
  @var   Channel2Pin
  @brief Global array that maps PWM 'channel' to Arduino 'pin'

  Note that 0 is an invalid channel number and maps to pin 0.

*/
#if ARDUINO_IS_UNO == 1
const int Channel2Pin[] = {0, 3, 5, 6, 9, 10, 11};
#else
#error "Arduino model is undefined"
#endif

/**
  @var   data
  @brief Buffer for incoming command string
*/
const int BUFSIZE = 100;
char data[BUFSIZE];

/**
  @fn    process_config_msg
  @brief First-level parser for messages from host computer

  The first character of a received message is examined. If the message
  is a configuration message then the appropriate function to configure
  an LED, Flash, Pattern, or Random Pattern Set is called immediately.
  A configuration dump message is also handled directly. 'Execute'
  'Display' message are passed to second level parsing.

  @param  dat String received from host
  @return 0 for success, -1 if first character unrecognized
*/
int process_config_msg(char *dat) {
  if (dat[0] == 'L') {
    config_LED(dat);
  } else if (dat[0] == 'F') {
    config_Flash(dat);
  } else if (dat[0] == 'P') {
    config_Pattern(dat);
  } else if (dat[0] == 'R') {
    config_Random(dat);
  } else if (dat[0] == 'T') {
    config_Time(dat);
  } else if (dat[0] == 'C') {
    ConfigMem::Display();
  } else if (dat[0] == 'X') {
    process_exec_msg(dat);
  } else if (dat[0] == 'D') {
    process_disp_msg(dat);
  } else {
    Serial.println("Configuration Message Error");
    return -1;
  }
  return 0;
}

/**
  @fn    process_disp_msg
  @brief Second-level parser for 'Display' messages from host computer

  The second character of a received message is examined. If the message
  is a valid display command then the appropriate function to display
  an LED, Flash, Pattern, or Random Pattern Set is called.

  @param  dat String received from host
  @return 0 for success, -1 if second character unrecognized
*/
int process_disp_msg(char *dat) {
  if (dat[1] == 'L') {
    display_LEDs();
  } else if (dat[1] == 'F') {
    display_Flashes();
  } else if (dat[1] == 'P') {
    display_Patterns();
  } else if (dat[1] == 'R') {
    display_Random();
  } else {
    Serial.println("Display Message Error");
    return -1;
  }
  return 0;
}

/**
  @fn    process_exec_msg
  @brief Second-level parser for 'Execute' messages from host computer

  The second character of a received message is examined. If the message
  is a valid execute command then the appropriate function to execute
  an LED, Flash, Pattern, Random Pattern Set, or Event is called.

  @param  dat String received from host
  @return 0 for success, -1 if second character unrecognized
*/
int process_exec_msg(char *dat) {
  if (dat[1] == 'L') {
    exec_led(dat);
  }
  else if (dat[1] == 'F') {
    exec_flash(dat);
  }
  else if (dat[1] == 'P') {
    exec_pattern(dat);
  }
  else if (dat[1] == 'R') {
    exec_random_pat(dat);
  }
  else if (dat[1] == 'e') {
    exec_event_msg(dat);
  }
  else {
    Serial.println("Execute Message Error");
    return -1;
  }
  return 0;
}

/**
  @fn    setup
  @brief Standard Arduino setup function

  Initialize all of the hardware.
*/
void setup() {
  //
  // Initialize I2C for Real-Time Clock and Temperature
  //
  Wire.begin();
  //
  // Initialize serial port comm to host PC
  //
  Serial.begin(9600);
  Serial.println("Running");
  Serial.setTimeout(1000);
  //
  // Initialize EEPROM storage parameters
  //
  ConfigMem::Init();
  //
  // Seed the random number generator
  //
  randomSeed(analogRead(0));
  //
  // Configure all PWM pins as outputs
  //
  for (int i = 1; i < (sizeof Channel2Pin / sizeof * Channel2Pin); i++) {
    pinMode(Channel2Pin[i], OUTPUT);
  }
}

/**
  @var   str
  @brief Raw String object from serial interface
*/
String str;

/**
  @fn    loop
  @brief Standard Arduino loop function, runs forever

  Looks for input from either the serial port of the keypad.
*/
void loop() {
  //
  // If something came in on the serial interface, handle it
  //
  if (Serial.available()) {
    str = Serial.readString();
    str.toCharArray(data, BUFSIZE);
    process_config_msg(data);
  }
  //
  // No serial input, look for a key press
  //
  // Pressing the `*` followed by a digit causes a fake "execute pattern"
  // message to be created. Pressing `#` followed by a digit causes a fake
  // "execute random [pattern set]" message to be created. Note that only
  // a single digit is allowed, from 1 to 9.
  //
  char Key = AnalogKeypad::PressedKey();
  if (Key == '*') {
    data[0] = 'X';
    data[1] = 'P';
    data[2] = ',';
  } else if (Key == '#') {
    data[0] = 'X';
    data[1] = 'R';
    data[2] = ',';
  } else if ((data[0] == 'X') && (Key >= '0') && (Key <= '9')) {
    data[3] = Key;
    data[4] = 0;
    Serial.println(data);
    process_config_msg(data);
  }
}
