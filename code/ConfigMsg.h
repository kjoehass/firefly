#ifndef __configmsg__
#define __configmsg__

#include "ConfigData.h"


static LED ld;
static Flash fl;
static Pattern pt;
extern int toks[20];

int getChannel(int num);
void display_LEDs();
void display_Flashes();
void display_Patterns();
void disp_pattern();
void config_LED(char *dat);
void config_Flash(char *dat);
void config_Pattern(char *dat);
int tokenize(char *dat);

#endif
