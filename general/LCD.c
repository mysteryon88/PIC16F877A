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
    Delay(1000);
    TRISB = 0;
    PORTB = 0x30;
    Pulse(300);
    PORTB = 0x20;
    Pulse(100);
    Send_Command_LCD(0x28);
    Send_Command_LCD(0x0D); 
    Send_Command_LCD(0x06);
    Send_Command_LCD(0x02);
    Current_ind = 0;
    PORTB = 0xF0;
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
	Send_Command_LCD(0x01);//команда отчистки дисплея
	/*
    for (byte i = 0; i < 32; i++) {
        Set_Coord_LCD(i / 16, i % 16);
        Send_Byte_LCD(' ');
    }
	*/
}