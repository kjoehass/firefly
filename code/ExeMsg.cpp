#include "Firefly.h"
#include "ConfigData.h"
#include "ConfigMsg.h"
#include "Arduino.h"

/*
   Paramaters:
   dat: configuration string that contains the LED number,
   and the illumination level for the test.

   Turns on the specified LED saved in EPROM to the specified
   illumination level
*/
void exec_led(char *dat) {
  // TODO check for valid number of tokens
  int i = tokenize(dat);
  Serial.print("Executing LED ");
  //Used to load the correct led from eprom
  int LED_num = toks[1];
  Serial.println(LED_num);
  //Load led code into static led
  bool ret;
  if (ld.isDefined(LED_num)) {
    ld.Get(LED_num);
    //Configure the output pin on the board
    // TODO verify level is from 0 to 100 KJH
    int illum_lvl = toks[2];
    // TODO map virtual channel to physical pin correctly
    pinMode(Channel2Pin[ld.Channel], OUTPUT);  // KJH

    //Set the illum level of the output pin
    illum_lvl = (int)(ld.MaxBrightness * ((float)(illum_lvl / 100.0)));
    analogWrite(Channel2Pin[ld.Channel], illum_lvl);  // KJH
  } else {
    Serial.println("ERROR");
    return;
  }
}

/*
   Parameters:
   interval: The number of intervals used to ramp the
   led brightness up and down. Default value is 40

   This function servers as a helper function to the exec_flash function.
   This funciton executes what ever flash is currently loaded into the global
   fl Flash object.
   Current iteration uses 40 intervals to increase and decrease the ramp
   up and ramp down lighting.
*/
void flash_led(int interval = 40) {
  int offDir = fl.InterpulseInterval - (fl.UpDuration + fl.DownDuration + fl.OnDuration);
  int i, extra;
  int initBrightness = 0;
  int rampStep = ld.MaxBrightness / interval;

#ifdef DEBUG
  Serial.print("    offDir "); Serial.println(offDir);
  Serial.print("    rampStep "); Serial.println(rampStep);
#endif


  pinMode(Channel2Pin[ld.Channel], OUTPUT);   // KJH take this out
  //Ensures that the interval ramping is even by adding the remainder to the
  //brightness before we beging ramping up
  extra = ld.MaxBrightness % interval;
#ifdef DEBUG
  Serial.print("    rampStep "); Serial.println(rampStep);
#endif


  int level = 0;

  //Executing flash
  //Calculate the ramp up delay in between level increases
  // FIXME FIXME FIXME TODO KJH
  // The error is TOO LARGE for short durations
  //
  int rampDelay = fl.UpDuration / interval;
#ifdef DEBUG
  Serial.print("    rampDelay "); Serial.println(rampDelay);
#endif
  //Ramp up
  for (i = 1; i <= interval; i++) {
    level += rampStep;
    if (extra > 0) {
      level += 1;
      extra --;
    }
    analogWrite(Channel2Pin[ld.Channel], level);  // KJH
#ifdef DEBUG
    Serial.print("      level "); Serial.println(level);
#endif

    delay(rampDelay);
  }

  //Delay for the led On time
#ifdef DEBUG
  Serial.print("  OnDuration "); Serial.println(fl.OnDuration);
#endif
  delay(fl.OnDuration);

  //Start the loop for the ramp down time
  extra = ld.MaxBrightness % interval;
  // FIXME FIXME FIXME TODO KJH
  // The error is TOO LARGE for short durations
  //
  rampDelay = fl.DownDuration / interval;
#ifdef DEBUG
  Serial.print("    rampDelay "); Serial.println(rampDelay);
#endif
  level = ld.MaxBrightness;
  for (i = 1; i <= interval; i++) {
    level -= rampStep;
    if (extra > 0) {
      level -= 1;
      extra --;
    }
    if (i == interval) {
      level = 0;
    }
    analogWrite(Channel2Pin[ld.Channel], level);  // KJH
#ifdef DEBUG
    Serial.print("      level "); Serial.println(level);
#endif
    delay(rampDelay);
  }
  delay(offDir);
#ifdef DEBUG
  Serial.print("  offDir "); Serial.println(offDir);
#endif

}

/*
   Parameters:
   dat: execution message that contains the flash to be executed

   This function is primarily used to load the correct flash number from
   EPROM into the global flash object. This function then calls flash_led
   which does most of the heavy lifting.
*/
void exec_flash(char *dat) {
  tokenize(dat);

  //Loads the correct flash from eprom
  int flash = toks[1];
  if (fl.isDefined(flash) == false) {
    Serial.println("Invalid Flash Number");
    return;
  }

  fl.Get(flash);

  //Loads the correct LED so we can get max brightness
  int led = fl.LED;
  ld.Get(led);

  char num[2];
  itoa(fl.Number, num, 10);
  Serial.print("Executing flash number ");
  Serial.print(num);
  Serial.print("\n");

  while (true) {
    //flash_led();
    // KJH
    fl.Execute();
  }
}

/*
   Return:
   returns an integer that represents the lenght of the
   flash pattern list of a whatever pattern is loaded into
   the global pattern object pt.
*/
int CalcFlashListLen() {
  int len = 0, index = 0;
  // TODO delete either len or index
  // TODO limit index to 15
  while (pt.FlashList[index] != 0) {
    len ++;
    index ++;
  }
  return len;
}

/*
   Parameters:
   dat: execution message that contains the flash pattern to be executed

   Loads the correct flash pattern from the execution message. It then uses
   the loaded flash pattern to get the pattern's flash list. The function
   then loops through the flashh pattern list and loads and executes each
   flash in the list one after the other.
*/
void exec_pattern(char* dat) {

  tokenize(dat);

  //Load the pattern from EPROM
  int pat = toks[1];
  if (pt.isDefined(pat) == false) {
    Serial.print("Invalid Pattern Number");
    return;
  }
  pt.Get(pat);
  while (true) {
   pt.Execute();
  }
// KJH 
// KJH   int offDur = pt.FlashPatternInterval;
// KJH   /*  KJH moved inside infinite loop
// KJH     //Display the pattern message
// KJH     disp_pattern();
// KJH   */
// KJH   //pt.Display();        // KJH
// KJH   int index = 0, FListLen;
// KJH   FListLen = CalcFlashListLen();
// KJH #ifdef DEBUG
// KJH   Serial.print("Flash list length "); Serial.println(FListLen);
// KJH #endif
// KJH   //Engage the pattern loop
// KJH   while (true) {
// KJH     //Display the pattern message KJH
// KJH     if (index == 0) {
// KJH       disp_pattern();
// KJH     }
// KJH #ifdef DEBUG
// KJH     Serial.print("  Flash index "); Serial.println(index);
// KJH #endif
// KJH     fl.Get(pt.FlashList[index]);
// KJH #ifdef DEBUG
// KJH     Serial.print("  Flash number "); Serial.println(fl.Number);
// KJH #endif
// KJH     // TODO handle FlashPatternInterval
// KJH     offDur -= fl.InterpulseInterval;
// KJH #ifdef DEBUG
// KJH     Serial.print("  offDur "); Serial.println(offDur);
// KJH #endif
// KJH     ld.Get(fl.LED);
// KJH #ifdef DEBUG
// KJH     Serial.print("  LED number "); Serial.println(ld.Number);
// KJH #endif
// KJH     flash_led();
// KJH     index = (index + 1) % FListLen;
// KJH     // TODO insert FlashPatternInterval delay
// KJH     if (index == 0) {
// KJH #ifdef DEBUG
// KJH       Serial.print("  Delay for "); Serial.println(offDur);
// KJH #endif
// KJH       delay(offDur);
// KJH       offDur = pt.FlashPatternInterval;
// KJH     }
// KJH   }
}

///*
//   Parameters:
//   maxDir: the max diration for a random flash
//
//   The function randomizes numbers between 1 and maxDir
//   to create a random flash to be executed.
//*/
//void gen_rand_flash(int maxDir = 10000) {
//  fl.Number = 0;
//  fl.LED = 1;
//  fl.UpDuration = random(1, maxDir);
//  fl.OnDuration = random(1, maxDir);
//  fl.DownDuration = random(1, maxDir);
//  int tot = fl.UpDuration + fl.OnDuration + fl.DownDuration;
//  fl.InterpulseInterval = random(tot, tot + maxDir);
//}

/*

*/
void exec_random_pat(char* dat) {
  Serial.println("Generating Random Patterns");
  tokenize(dat);

  //Load the pattern set from EPROM
  int set = toks[1];
  if (!rd.isDefined(set)) {
    Serial.print("Invalid Random Pattern Set Number");
    return;
  }
  rd.Get(set);
  rd.Display();
  rd.Execute();
}

void exec_event_msg(char* dat) {
  return;
}
