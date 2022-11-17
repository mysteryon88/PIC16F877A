#ifndef TASK3_H
#define TASK3_H

#include "LCD.h"

#define TRUE 1

void start(void); //������
void task(void);  //�������
void Beep(void);
void Show_String_LCD_(const char *mySTRING);
void Send_Byte_LCD_(byte tmp);
void interrupt timer0(void);
void save_ind(void);
void loadstr_(char *mySTRING);

char ticker[] = "Sell garage";
// down - 1 up - 0
byte up_down = 0x01;
// 1 - two string
// 0 - one string
byte all = 0x00;
// 1 - right 0 - left
byte direction = 0x01;
short ind = 0x00;
unsigned short time = 0x00;

#endif