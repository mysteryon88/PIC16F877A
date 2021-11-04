#ifndef TASK14_H
#define TASK14_H

#include "LCD.h"

#define TRUE 1
#define N 11
#define SAVE "     SAVED"

void start(void);//начало
void task(void);//задание
void Beep(void);
void Show_LCD(short num);
void swap(char* a, char* b);
void reverse(char* first, char* last);
void make_string(char* str_to_eeprom, char* num_, char* ternary);
#endif