#include "ConfigData.h"
#include "ConfigMsg.h"
#include "Arduino.h"

#define DEBUG 1
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
  if (ld.isDefined(LED_num) && (getChannel(ld.Channel) != -1)) {
    ld.Get(LED_num);
    //Configure the output pin on the board
    int illum_lvl = toks[2];
    // TODO map virtual channel to physical pin correctly
    pinMode(getChannel(ld.Channel), OUTPUT);

    //Set the illum level of the output pin
    illum_lvl = (int)(ld.MaxBrightness * ((float)(illum_lvl / 100.0)));
    analogWrite(getChannel(ld.Channel), illum_lvl);
  } else {
    Serial.println("Invalid LED Number");
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
void flash_led() {
  int offDir = fl.InterpulseInterval - (fl.UpDuration + fl.DownDuration + fl.OnDuration);
  int i, extra;
  int initBrightness = 0;
  
#ifdef DEBUG
  Serial.print("    offDir "); Serial.println(offDir);
#endif


  pinMode(getChannel(ld.Channel), OUTPUT);

  long level = 0;

  //Executing flash
  //Hold the ram delay at a constant 10ms assuming that the delays
  //are multiples of 10
  int rampDelay = 10;
#ifdef DEBUG
  Serial.print("    rampDelay "); Serial.println(rampDelay);
#endif
  //Ramp up
  //Calculate total steps
  int totSteps = fl.UpDuration / 10;
  int stepNum;
  int val;
  //Loop through each step
  for (stepNum = 1; stepNum <= totSteps; stepNum++) {
    val = (ld.MaxBrightness * 255 + 50) / 100;
    level = (long(val) * long(stepNum) + long(totSteps / 2)) / long(totSteps);
    analogWrite(getChannel(ld.Channel), level);
#ifdef DEBUG
    Serial.print("      level "); Serial.println(level);
    Serial.print("      stepNum "); Serial.println(stepNum);
#endif
    delay(rampDelay);
  }

  //Delay for the led On time
#ifdef DEBUG
  Serial.print("  OnDuration "); Serial.println(fl.OnDuration);
#endif
  delay(fl.OnDuration);

  //Start the loop for the ramp down time
#ifdef DEBUG
  Serial.print("    rampDelay "); Serial.println(rampDelay);
#endif

  for (stepNum = totSteps; totSteps >= 0; stepNum--) {
    val = (ld.MaxBrightness * 255 + 50) / 100;
    level = (long(val) * long(stepNum) + long(totSteps / 2)) / long(totSteps);    analogWrite(getChannel(ld.Channel), level);
#ifdef DEBUG
    Serial.print("      level "); Serial.println(level);
    Serial.print("      stepNum "); Serial.println(stepNum);
#endif
    delay(rampDelay);
  }
  analogWrite(getChannel(ld.Channel), 0);
#ifdef DEBUG
  Serial.print("  offDir "); Serial.println(offDir);
#endif
  delay(offDir);
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

  if(getChannel(ld.Channel) == -1){
    Serial.println("LED Channel Error");
    return;
  }
  
  char num[2];
  itoa(fl.Number, num, 10);
  Serial.print("Executing flash number ");
  Serial.print(num);
  Serial.print("\n");

  while (true) {
    flash_led();
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
  int offDur = pt.FlashPatternInterval;
  //Display the pattern message
  disp_pattern();
  pt.Display();
  int index = 0, FListLen;
  FListLen = CalcFlashListLen();
#ifdef DEBUG
  Serial.print("Flash list length "); Serial.println(FListLen);
#endif
  //Engage the pattern loop
  while (true) {
#ifdef DEBUG
    Serial.print("  Flash index "); Serial.println(index);
#endif
    fl.Get(pt.FlashList[index]);
#ifdef DEBUG
    Serial.print("  Flash number "); Serial.println(fl.Number);
#endif
    // TODO handle FlashPatternInterval
    offDur -= fl.InterpulseInterval;
#ifdef DEBUG
    Serial.print("  offDur "); Serial.println(offDur);
#endif
    ld.Get(fl.LED);
    if(getChannel(ld.Channel) == -1){
      Serial.println("LED Channel Error");
      return;
    }
#ifdef DEBUG
    Serial.print("  LED number "); Serial.println(ld.Number);
#endif
    flash_led();
    index = (index + 1) % FListLen;
    // TODO insert FlashPatternInterval delay
    if (index == 0) {
#ifdef DEBUG
    Serial.print("  Delay for "); Serial.println(offDur);
#endif
      delay(offDur);
      offDur = pt.FlashPatternInterval;
    }
  }
}

/*
   Parameters:
   maxDir: the max diration for a random flash

   The function randomizes numbers between 1 and maxDir
   to create a random flash to be executed.
*/
void gen_rand_flash(int maxDir = 10000) {
  fl.Number = 0;
  fl.LED = 1;
  fl.UpDuration = random(1, maxDir);
  fl.OnDuration = random(1, maxDir);
  fl.DownDuration = random(1, maxDir);
  int tot = fl.UpDuration + fl.OnDuration + fl.DownDuration;
  fl.InterpulseInterval = random(tot, tot + maxDir);
}

/*

*/
void exec_random_pat(char* dat) {
  Serial.println("Generating Random Flashes");
  while (true) {
    gen_rand_flash();
    if(ld.isDefined(fl.LED)){
      ld.Get(fl.LED);
      fl.Display();
      flash_led();
    }else{
      Serial.println("Invalid default LED. Please configure LED 1");
    }
    
  }
}

void exec_event_msg(char* dat) {
  return;
}
