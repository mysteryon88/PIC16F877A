#ifndef TASK31_H
#define TASK31_H

#include "LCD.h"

void start(void); //������
void task(void);  //�������
void Beep(void);
unsigned long int mas_to_int(short *mas, size_t size);
unsigned long int power(u_int num, short ex);
#endif