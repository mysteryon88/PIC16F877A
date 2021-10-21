#include "task26.h"

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

//звонок
void Beep(void){ 
	byte tmp_TRISB=TRISB, tmp_PORTB=PORTB, i;
	TRISB3=0; // clrbit (TRISB,3);
	i=100000; 
	while(i--){
 		RB3=1;
		Delay(110); //0,125 мс
 		RB3=0;
		Delay(110);
	}
 	PORTB=tmp_PORTB;
 	TRISB=tmp_TRISB;
}

void Show_LCD(short num, short j){
	char num_lcd[5] = " ";
	sprintf(num_lcd, "%d", num); 
	Set_Coord_LCD(j, 6);
	Show_String_LCD(num_lcd);
}

void task(void){
	short num = 0x00, sum = 0x00;
	byte decision = 0x00;
	while (TRUE){
		decision = 0x00;
		do {
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 4))){
				num++;
				Show_LCD(num, 0);
			}	
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 5))){
				num +=10;
				Show_LCD(num, 0);
			}	
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 6))){
				num+=100;
				Show_LCD(num, 0);
			}	
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 7))) decision = 1;		
		} while (!decision);

		//calculate the amount
		while (num!=0){
      		sum += num % 10;
			num /= 10;
    	}

		Show_LCD(sum, 1);
		int p = 10;
		
		if(!(sum % 10)) while(p--) Beep();
		do {

			PORTB |= 0xF0;
			if (!(PORTB & (1 << 7))) break;
		} while (TRUE);
		num = 0;
		sum = 0;
		Clr_LCD();
	}
}

