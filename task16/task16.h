#ifndef TASK16_H
#define TASK16_H

#include "LCD.h"

#define TRUE 1
#define N 16
#define STR_TO_EEPROM 32
#define SAVE       "     SAVED      "
#define ASYMMETRIC "   ASYMMETRIC   "
#define SYMMETRIC  "   SYMMETRIC    "


void start(void); //начало
void task(void);  //задание
byte mass_cmp(short* num_one, short* num_two, short count_one, short count_two);
void make_string(char* str_to_eeprom, char* str_one, char* str_two);
void init(short* num_one, short* num_two, char* str_one, char* str_two); 
void Beep(void);
#endif