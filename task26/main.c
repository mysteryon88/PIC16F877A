/*
26)  Ввод десятичного числа. Во второй строке – сумма введенных цифр.
 Если сумма оканчивается на 0  - один длинный звонок.
*/
#include "LCD.h"
#include "task26.h"

void main(){
	PORTB = 0xF0;
    TRISB = 0xF0;
    TRISD = 0x00;
    PORTD = 0x00;
	Init_I2C();
    Init_LCD();
    Clr_LCD();
//	start();
	task();
	return;	
}
