/*
10) В верхней строке выводится фраза (любая - придумать самим). Кнопками курсор подводится под нужную букву, которая при ее выборе опускается на нижнюю строку и сдвигается вплотную к левому краю (к ранее выбранным буквам), где формируется слово из выбранных букв.
*/
#include "LCD.h"
#include "task10.h"

void main()
{
	PORTB = 0xF0;
	TRISB = 0xF0;
	Init_I2C();
	Init_LCD();
	start();
	//	task();
	return;
}
