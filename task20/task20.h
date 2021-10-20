#ifndef TASK20_H
#define TASK20_H

#include "LCD.h"
#define RIGHT "    RIGHT BUTTON "
#define LEFT " LEFT BUTTON     "
#define AVEREGE "    AVEREGE      "

#define IN_TO_SEC 19600

//кол-во переполнений tmr0
unsigned long TMR0_count = 0;

void interrupt timer0(void); //переполнение tmr0
void start(void);//начало
void task(void);//задание

#endif