/*
29)  �������� 8  ���������� ����. ������� �������� ����� �� 
������ ������. ���� �� ���, �� ���� ������� ������.
*/
#include "LCD.h"
#include "task29.h"

void main(){
	PORTB = 0xF0;
    TRISB = 0xF0;
	Init_I2C();
    Init_LCD();
	start();
	return;	
}
