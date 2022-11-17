#ifndef TASK27_H
#define TASK27_H

#include "LCD.h"

#define N 8
#define SAVE "     SAVED      "
#define NOT "   NO TRIPLES   "

void start(void); //������
void task(void);  //�������
void Beep(void);
void show(short num, short i, short j);
void save(short *num, short *odd);
void save_not(short *num);
#endif