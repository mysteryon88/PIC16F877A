/*
29)  Вводятся 8  десятичных цифр. Вывести нечетные цифры во 
второй строке. Если их нет, то один длинный звонок.
*/
#include "LCD.h"
#include "task29.h"

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
