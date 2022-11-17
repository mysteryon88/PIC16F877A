/*
13)Вводится восьмеричное число в верхней строчке. При нажатии на кнопку-окончание ввода и в нижней строке появляется 10-ичное число, равное верхнему.
*/
#include "LCD.h"
#include "task13.h"

void main()
{
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
