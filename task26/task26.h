#ifndef TASK26_H
#define TASK26_H

#include "LCD.h"

#define TRUE 1
#define SAVE "     SAVED"

void start(void); //������
void task(void);  //�������
void Beep(void);
void Show_LCD(short num, short j);
void make_string(char *str_to_eeprom, char *num_, char *sum_);
#endif