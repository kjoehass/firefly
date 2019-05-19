#include "Arduino.h"
#include "Firefly.h"
#include "ConfigData.h"
#include "ConfigMsg.h"
#include "RealTimeClock.h"
#include "TempSensor.h"

int toks[20];

//enum Channel{  // KJH
//  channel1 = 3,
//  channel2 = 5,
//  channel3,
//  channel4 = 9,
//  channel5,
//  channel6,
//};

/*
   Param:
   Receives an integer num which corresponds to the selected channel

   Return:
   Retuns an integer that refers to the phsical channel
*/
//int getChannel(int num){    // KJH
//  if(num == 1){return channel1;}
//  else if(num == 2){return channel2;}
//  else if(num == 3){return channel3;}
//  else if(num == 4){return channel4;}
//  else if(num == 5){return channel5;}
//  else{return channel6;}
//}


/*
   Displays all the saved LEDs in the current EPROM
   Outputs an orginized list to the Arduino Serial terminal
*/
void display_LEDs() {
  int i;
  Serial.println("Saved LEDs");
  for (i = 1; i <= ConfigMem::MaxLED; i++) {
    if (ld.isDefined(i)) {
      ld.Get(i);
      ld.Display();   // KJH
    }
  }
}

/*
   Displays all the saved Flashes in the current EPROM
   Outputs an orginized list to the Arduino Serial terminal
*/
void display_Flashes() {
  int i;
  //Serial.println("Saved Flashes");
  for (i = 1; i <= ConfigMem::MaxFlash; i++) {
    if (fl.isDefined(i)) {
      fl.Get(i);
      fl.Display();   // KJH
    }
  }
}

/*
   Displays all the saved Patterns in the current EPROM
   Outputs an orginized list to the Arduino Serial terminal
*/
void display_Patterns() {
  int i;
  //Serial.println("Saved Patterns");
  for (i = 1; i <= ConfigMem::MaxPattern; i++) {
    if (pt.isDefined(i)) {
      pt.Get(i);
      pt.Display();   // KJH
    }
  }
}

void display_Random() {
  for (int i = 1; i <= ConfigMem::MaxPatternSet; i++) {
    if (rd.isDefined(i)) {
      rd.Get(i);
      rd.Display();
    }
  }
}

/*
   Helper function for the display_Patterns function.
   It's main function is to convert the flash list into
   a nicely formatted string.
*/
void disp_pattern() {
  String Msg = "p,";
  Msg += String(RealTimeClock::DateTime());
  Msg += ",";
  Msg += String(TempSensor::Temperature());
  Msg += ",";
  Msg += String(pt.Number, DEC);
  Serial.println(Msg);
}

/*
   Param:
   dat - configuration message conatining an identifier number,
   a channel number, and the max brightness
*/
void config_LED(char *dat) {
  Serial.println(dat);
  int size = tokenize(dat);
  if (size >= 4) {
    if ((toks[1] < ConfigMem::MaxLED) && (toks[2] < ConfigMem::MaxChannel) && \
        (toks[3] < 101) && (toks[3] >= 1)) {
      ld.Number = toks[1];
      ld.Channel = toks[2];  // KJH
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
}

void config_Flash(char *dat) {
  Serial.print(dat);
  int size = tokenize(dat);
  if (size >= 7) {
    fl.Number = toks[1];
    fl.LED = toks[2];
    fl.UpDuration = toks[3];
    fl.OnDuration = toks[4];
    fl.DownDuration = toks[5];
    fl.InterpulseInterval = toks[6];
    if (fl.Save()) {
      Serial.println("Flash Configured");
    } else {
      Serial.println("Flash Configuration Error");
    }
  } else {
    Serial.println("Flash Congiguration Error");
  }
}

void config_Pattern(char *dat) {
  Serial.println(dat);
  int size = tokenize(dat);
  if (size >= 4) {
    pt.Number = toks[1];
    pt.FlashPatternInterval = toks[2];
    int i;
    for (i = 3; i < size; i++) {
      pt.FlashList[i - 3] = toks[i];
    }
    for ( ; i < 19; i++) {      // KJH
      pt.FlashList[i - 3] = 0;  // KJH
    }                           // KJH
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
  @fn     config_Random
  @brief  Configures a random pattern set

          The input string should have at least 3 tokens: the letter R,
          the pattern set number, and at least one pattern to add to
          the set. Up to 16 patterns can be added to the set, from 1 to
          16. The set is stored as a single 16-bit integer, with bit N-1
          set to 1 to indicate that pattern N is in the set.

  @param  dat pointer to input character string
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

/*
   Parameters:
   delim: If dat contains one of these, split it at this character
   dat: a char string to be split up wherever there is a delim parameter

   Return:
   returns the number of tokens split

   Note:
   The split tokens are stored in a global 2D char array that needs to be allocated enough space
*/
int tokenize(char *dat) {
  char* ptr = NULL;
  int index = 1;
  strtok(dat, ",");
  while (1) {
    ptr = strtok(NULL, ",");
    if (ptr == NULL) {
      break;
    }
    toks[index] = atoi(ptr);
    index ++;
  }
  return index;
}
