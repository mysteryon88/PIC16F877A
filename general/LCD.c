/*
The library for LCD was written by my teacher Sergey Ivanovich Kovalev, who died in 2021
*/
#include "LCD.h"

void Pulse(unsigned short x){ 
	RB2=1; 
	Delay(x); 
	RB2=0;
	Delay(x);
}

void Send_Command_LCD(byte tmp) {
    while (Check_buttons())
		Delay(1000);
    RB1 = 0;
    PORTB = (PORTB & 0x0F)+ (tmp & 0xF0);
    Pulse(100);
    PORTB = (PORTB & 0x0F)+ (tmp << 4);
    Pulse(100);
    RB1 = 1;
    Delay(1000);
}

void Init_LCD(void) {
  	Delay(200*20);
    TRISB=0;   
    PORTB=0x30;   //0011000
    Pulse(20*20);
    Pulse(20*20);
    Pulse(20*20);
 
    PORTB=0x20;  
    Pulse(20*20);
 
    Send_Command_LCD  (0x28);
    Send_Command_LCD  (0x0E);
    Send_Command_LCD  (0x06);
    Send_Command_LCD  (0x02);
}

void Set_Coord_LCD(byte i, byte j) {
    if (i == 0) {
        Current_ind = j;
        Send_Command_LCD(0x80 + j);
    } else {
        Current_ind = 16 + j;
        Send_Command_LCD(0xC0 + j);
    };
    return;
}

void Send_Byte_LCD(byte tmp) {
    while (Check_buttons()) 
    	Delay(1000);
    PORTB = (PORTB & 0x0F)+ (tmp & 0xF0);
    Pulse(10);
    PORTB = (PORTB & 0x0F)+ (tmp << 4);
    Pulse(10);
    Current_ind++;
    if (Current_ind == 16)Set_Coord_LCD(1, 0);
    if (Current_ind == 32)Set_Coord_LCD(0, 0);
}

void Show_String_LCD(const char * mySTRING) {
    while (*mySTRING) {
        Send_Byte_LCD(*(mySTRING++));
    };
}

void Clr_LCD(void) {
	Set_Coord_LCD(0,0);
    Show_String_LCD("                ");
	Show_String_LCD("                ");
    Set_Coord_LCD(0,0);
}
