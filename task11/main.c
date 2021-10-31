/*
11)¬водитс€ шестнадцатеричное число в верхней строчке. ѕри нажатии на кнопку-окончание ввода и в нижней строке по€вл€етс€ двоичное число, равное верхнему.
*/
#include "LCD.h"
#include "task11.h"

void main(){
	PORTB = 0xF0;
    TRISB = 0xF0;
    TRISD = 0x00;
    PORTD = 0x00;
	Init_I2C();
    Init_LCD();
    Clr_LCD();
	start();
//	task();
	return;	
}
