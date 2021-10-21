#include "task14.h"

#define TRUE 1
#define N 10

void start(void){
	byte decision = 0x00;
	/*
	if (eeprom_decision()) {//вывод из eeprom или само задание
		char str_to_lcd[32] = " ";
		loadstr(str_to_lcd);
		Clr_LCD();
		Show_String_LCD(str_to_lcd);
		do {
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 7))) decision = 1;
		} while (!decision);
		task();
	}
	else task();
	*/
	task();
}

void Show_LCD(short num){
	char num_lcd[6] = " ";
	sprintf(num_lcd, "%o", num); 
	Set_Coord_LCD(0, 6);
	Show_String_LCD(num_lcd);
}

void task(void){
	short num = 0x00;
	byte decision = 0x00;
	short i = 0x00;
	char ternary[N] = " ";
	while (TRUE){
		decision = 0x00;
		for(i = 0; i < N; i++) ternary[i] = ' ';
		do {
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 4))){
				num++;
				Show_LCD(num);
			}	
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 5))){
				num +=10;
				Show_LCD(num);
			}	
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 6))){
				num+=100;
				Show_LCD(num);
			}	
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 7))) decision = 1;		
		} while (!decision);
		i = 0x00;
		
		while (num > 0){
        	ternary[i] = 48 + num%3;
        	num /= 3;
       		i++;
		}

		Set_Coord_LCD(1, 1);
		for (i = N-1; i >= 0; i--)
       		Send_Byte_LCD(ternary[i]);
    
		do {
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 7))) break;	
		} while (1);
		num = 0x00;
		Clr_LCD();
	}
}

