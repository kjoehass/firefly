#ifndef __configmsg__
#define __configmsg__

/**
  @var   toks
  @brief Array of tokens extracted from a message from the host PC
*/
extern int toks[20];

/**
  @fn    display_LEDs
  @brief Sends LED info back to the host PC
*/
void display_LEDs();
/**
  @fn    display_Flashes
  @brief Sends Flash info back to the host PC
*/
void display_Flashes();
/**
  @fn    display_Patterns
  @brief Sends Pattern info back to the host PC
*/
void display_Patterns();
/**
  @fn    display_Random
  @brief Sends Random Pattern Set info back to the host PC
*/
void display_Random();

/**
  @fn    config_LED
  @brief Parses message from host and configures an LED

  @param dat string containing message from host
*/
void config_LED(char *dat);
/**
  @fn    config_Flash
  @brief Parses message from host and configures a Flash

  @param dat string containing message from host
*/
void config_Flash(char *dat);
/**
  @fn    config_Pattern
  @brief Parses message from host and configures a Pattern

  @param dat string containing message from host
*/
void config_Pattern(char *dat);
/**
  @fn    config_Random
  @brief Parses message from host and configures a random pattern set

  @param dat pointer to input character string
*/
void config_Random(char *dat);
/**
  @fn     config_Time
  @brief  Sets the real-time clock on the simulator

  @param  dat pointer to input character string
*/
void config_Time(char *dat);
/**
  @fn    tokenize
  @brief Extract tokens from the message sent by the host

  @param  dat pointer to input character string
  @return number of tokens found
*/
int tokenize(char *dat);

#endif
