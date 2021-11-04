#ifndef TASK31_H
#define TASK31_H

#include "LCD.h"

void start(void);//начало
void task(void);//задание
void Beep(void);
u_int mas_to_int(short *mas, size_t size);
u_int power(u_int num, short ex);

#endif