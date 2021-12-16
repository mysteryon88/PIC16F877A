/*
The library for LCD was written by my teacher Sergey Ivanovich Kovalev, who died in 2021
*/
#ifndef LCD_H
#define LCD_H

#include "kursach.h"

void Pulse(unsigned short x);
void Init_LCD(void);
void Set_Coord_LCD(byte i, byte j);
void Send_Byte_LCD(byte tmp);
void Show_String_LCD(const char * mySTRING);
void Clr_LCD(void);

#endif