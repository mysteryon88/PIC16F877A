/*
26)  ���� ����������� �����. �� ������ ������ � ����� ��������� ����. ���� ����� ������������ �� 0  - ���� ������� ������.
*/
#include "LCD.h"
#include "task26.h"

void main(){
	PORTB = 0xF0;
    TRISB = 0xF0;
	Init_I2C();
    Init_LCD();
	start();
	return;	
}
