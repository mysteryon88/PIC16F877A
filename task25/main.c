/*
25)¬вод дес€тичного числа. ≈сли цифры в нем упор€дочены по возрастанию Ц короткий звонок, по убыванию Ц два звонка, если другое Ц один длинный звонок.
*/
#include "LCD.h"
#include "task25.h"

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
