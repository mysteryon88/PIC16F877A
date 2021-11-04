#ifndef TASK25_H
#define TASK25_H

#include "LCD.h"

#define TRUE 1

#define ASCENDING "   ASCENDING    " 
#define DESCENDING "   DESCENDING   "
#define NOT_ORDERED "  NOT ORDERED   "
#define SAVE "     SAVED"
#define N 7

void start(void);//начало
void task(void);//задание
void Beep(void);
void Overflow(void);
#endif