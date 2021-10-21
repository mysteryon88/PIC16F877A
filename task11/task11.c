#include "task11.h"

#define TRUE 1

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
	sprintf(num_lcd, "%x", num); 
	Set_Coord_LCD(0, 6);
	Show_String_LCD(num_lcd);
}

void task(void){
	short num = 0x00;
	byte decision = 0x00;
	short i = 0x00;
	u_int v = 0x8000; //32768 
	char bin[15] = " ";
	while (TRUE){
		decision = 0x00;
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
		
		for(i = 1; i <= 16; i++){
			if(num >= v){
				bin[i-1]='1';
				num -=v;
            }
            else bin[i-1]='0';
			v >>= 1;
		}
		Set_Coord_LCD(1, 0);
		Show_String_LCD(bin);
		do {
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 7))) break;	
		} while (1);
		for(i = 0; i < 15; i++) bin[i] = ' ';
		num = -1;
		v = 0x8000; //32768
		Clr_LCD();
	}
}

