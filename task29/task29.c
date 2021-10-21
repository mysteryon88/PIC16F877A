#include "task29.h"

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

//звонок
void Beep(void){ 
	byte tmp_TRISB=TRISB, tmp_PORTB=PORTB, i;
	TRISB3=0; // clrbit (TRISB,3);
	i=1000000; 
	while(i--){
 		RB3=1;
		Delay(140); //0,125 мс
 		RB3=0;
		Delay(140);
	}
 	PORTB=tmp_PORTB;
 	TRISB=tmp_TRISB;
}


void task(void){
	short numbers[] = {0, 0, 0, 0, 0, 0, 0, 0}; 
	short odd[] = {0, 0, 0, 0, 0, 0, 0, 0};
	short count_even;
	byte decision = 0x00;
	short i = 0;
	while (1){
		decision = 0x00;
		for (i = 0; i < N; i++ ){
			do {
				PORTB |= 0xF0;
				if (!(PORTB & (1 << 4))){
					numbers[i] = (numbers[i] == 9) ? 0 : ++numbers[i];
					show(numbers[i], i, 0);
				}	
				PORTB |= 0xF0;
				if (!(PORTB & (1 << 7))){
					Set_Coord_LCD(0, 2*(1+i));
					decision = 1;
				}		
			} while (!decision);
			decision = 0x00;
		}
		for (i = 0; i < N; i++){
			(numbers[i] % 2) ? odd[i] = numbers[i] : ++count_even;
			show(odd[i], i, 1);
		}	
		short p = 10;
		if(count_even == 8) while(p--) Beep(); 
		do {
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 7))) break;	
		} while (1);
		Clr_LCD();
		for(i = 0; i < N; i++) numbers[i] = 0;
		count_even = 0;
	}
}

void show(short num, short i, short j){
	char num_lcd[1] = " ";
	sprintf(num_lcd, "%d", num); 
	Set_Coord_LCD(j, 2*i);
	Show_String_LCD(num_lcd);
}