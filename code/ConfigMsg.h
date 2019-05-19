#ifndef __configmsg__
#define __configmsg__

#include "ConfigData.h"


extern int toks[20];


void display_LEDs();
void display_Flashes();
void display_Patterns();
void display_Random();
void disp_pattern();
void config_LED(char *dat);
void config_Flash(char *dat);
void config_Pattern(char *dat);
void config_Random(char *dat);
int tokenize(char *dat);

#endif
