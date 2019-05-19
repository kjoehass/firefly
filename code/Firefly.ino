#include "Firefly.h"
#include "ConfigData.h"
#include "ConfigMsg.h"
#include "ExeMsg.h"
#include "RealTimeClock.h"
#include "TempSensor.h"
#include "AnalogKeypad.h"

LED ld;
Flash fl;
Pattern pt;
RandPatternSet rd;

#if ARDUINO_IS_UNO == 1
const int Channel2Pin[] = {0, 3, 5, 6, 9, 10, 11}; // KJH
#else
#error "Arduino model is undefined"
const int Channel2Pin[] = {0, 3, 5, 6, 9, 10, 11}; // KJH
#endif


const int BUFSIZE = 100;
char data[BUFSIZE];

/*
   TODO:
   Get ready for demo
   Error handling
   Fix pattern duration
   Finish comments
*/


/*
   Parameters:
   data: A configuration string message

   Return:
   retuns a 0 on success and -1 on failure

   This Function processes what type of confiuration message was sent.
   i.e. is this message an LED, Flash, or Pattern configuration message.
   Once determined it will call the approprate configuration function.
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

/*
   Parameters:
   dat: A display configuration message

   Return:
   returns 0 on success and -1 for an error condition

   This function will display the correct list of stored
   LEDs, Flashes or Patterns depending on the configuration message
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

/*
   Parameters:
   dat: An ecexution configuration message

   Return:
   return 0 on success and -1 for an error condition

   This function receives an execution configuration message
   and uses that message to call the correct type of configuration
   message.
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

String str;
void setup() {
  // TODO call Wire.begin once
  Wire.begin();
  Serial.begin(9600);
  Serial.println("Running ConfigMem::Init");
  Serial.setTimeout(1000); // wait 1 seconds for input
  ConfigMem::Init();
  randomSeed(analogRead(0));
  RealTimeClock::Set(2019, 4, 8, 16, 33, 02);
  // TODO Configure all pins here...intelligently
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
}

// TODO delete i?
//int i = 0;
void loop() {
  if (Serial.available()) {
    str = Serial.readString();
    str.toCharArray(data, BUFSIZE);
    process_config_msg(data);
  }
  //
  // No serial input, look for a key press
  //
  char Key = AnalogKeypad::PressedKey();
  if (Key == '*') {
    data[0] = 'X';
    data[1] = 'P';
    data[2] = ',';
  } else if ((data[0] == 'X') && (Key >= '0') && (Key <= '9')) {
    data[3] = Key;
    data[4] = 0;
    Serial.println(data);
    process_config_msg(data);
  }
}
