#include "task11.h"

#define TRUE 1

void start(void){
	byte decision = 0x00;
	
	if (eeprom_decision()) {//output from eeprom or task
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
}

void Show_LCD(short num, char *num_){
	char num_lcd[4] = " ";
	sprintf(num_, "%x", num); //output in hexadecimal
	
	Set_Coord_LCD(0, 6);
	Show_String_LCD(num_);
}

void task(void){
	Clr_LCD();
	short num = 0x00;
	byte decision = 0x00;
	char str_to_eeprom[32] = " ";
	char num_[4] = " ";
	byte FLAG = 0x00; //block RB7
	short i = 0x00;
	u_int v = 0x8000; //32768 
	char bin[16] = " ";
	while (TRUE){
		decision = 0x00;
		Show_LCD(num, num_); // output 0
		for(i = 0; i < 15; i++) bin[i] = ' ';
		for(i = 0; i < 4; i++) num_[i] = ' ';
		do {
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 4))){
				num++;
				Show_LCD(num, num_);
				FLAG = 1;
			}	
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 5))){
				num +=0xF;
				Show_LCD(num, num_);
				FLAG = 1;
			}	
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 6))){
				num+=0xFF;
				Show_LCD(num, num_);
				FLAG = 1;
			}	
			PORTB |= 0xF0;
			if (num >= 0xFFF) decision = 1;  //overflow
			if(FLAG) if (!(PORTB & (1 << 7))) decision = 1;	
		} while (!decision);

		for(i = 0; i <= 15; i++){
			if(num >= v){
				bin[i]='1';
				num -=v;
            }
            else bin[i]='0';
			v >>= 1; // v = v/2
		}
		Set_Coord_LCD(1, 0);
		Show_String_LCD(bin); //output binary
		//waiting for a button
		do {
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 7))){ 
				strcpy(str_to_eeprom, "      ");//6
				strcat(str_to_eeprom, num_);
				i = 0;
				while(num_[i]) i++;	
				for (short j = 10; j > i; j--)
					strcat(str_to_eeprom, " ");
				
				strcat(str_to_eeprom, bin);
				savestr(str_to_eeprom);	
				break;	
			}
		} while (1);
		//clear
		num = 0x00;
		v = 0x8000; //32768
		Clr_LCD();
		FLAG = 0x00; 
	}
}

