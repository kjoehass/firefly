#ifndef __exemsg__
#define __exemsg__

#include "ConfigMsg.h"
#include "ConfigData.h"

void exec_led(char *dat);
void flash_led(int interval);
void exec_flash(char *dat);
int CalcFlashListLen();
void exec_pattern(char *dat);
void gen_rand_flash(int maxDir);
void exec_random_pat(char *dat);
void exec_event_msg(char *dat);

#endif
