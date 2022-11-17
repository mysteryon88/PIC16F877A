#ifndef TASK20_H
#define TASK20_H

#include "LCD.h"

#define RIGHT "    RIGHT BUTTON "
#define LEFT " LEFT BUTTON     "
#define AVEREGE "    AVEREGE      "

#define IN_TO_SEC 19600

//���-�� ������������ tmr0
unsigned long TMR0_count = 0;

void interrupt timer0(void); //������������ tmr0
void start(void);            //������
void task(void);             //�������

#endif