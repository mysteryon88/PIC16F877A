#ifndef TASK29_H
#define TASK29_H

#include "LCD.h"

#define N 8
#define SAVE "     SAVED"

void start(void); //������
void task(void);  //�������
void Beep(void);
void show(short num, short i, short j);
void save(short *num, short *odd);
#endif