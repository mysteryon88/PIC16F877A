#include "task14.h"

void start(void){
	byte decision = 0x00;
	Clr_LCD();
	if (eeprom_decision()) {//вывод из eeprom или само задание
		char str_to_lcd[32] = " ";
		loadstr(str_to_lcd);
		Clr_LCD();
		Show_String_LCD(str_to_lcd);
		do {
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 7))) decision = 1;
		} while (!decision);
	}
	Clr_LCD();
	task();
}

void Show_LCD(short num){
	char num_lcd[6] = " ";
	sprintf(num_lcd, "%o", num); 
	Set_Coord_LCD(0, 6);
	Show_String_LCD(num_lcd);
}

void task(void){
	u_int num = 0x00;
	byte decision = 0x00, FLAG_RB6 = 0x00;
	short i = 0x00, j = 0x00;
	byte ternary[N] = " ";
	byte str_to_eeprom[32] = " ", num_[6] = " ";
	while (TRUE){
		Show_LCD(num);
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
		
		if(num == 0)
			ternary[3] = '0';
		
		i = 0x00;
		sprintf(num_, "%o", num); 

		while (num > 0){
        	ternary[i] = 48 + num%3;
        	num /= 3;
       		i++;
		}
		
		reverse(ternary, &ternary[N-1]);

		Set_Coord_LCD(1, 0);
		Show_String_LCD(ternary);

    	decision = 0x00;
		do {
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 7))) decision = 1;	
			PORTB |= 0xF0;
			//сохранение
			if (!FLAG_RB6 && !(PORTB & (1 << 6))){
				FLAG_RB6 = 0x01;
				make_string(str_to_eeprom, num_, ternary);
				Clr_LCD();
				savestr(str_to_eeprom);	
				Show_String_LCD(SAVE);
			}
		} while (!decision);
		memset(str_to_eeprom, '\0', 32);
		decision = 0x00;
		num = 0x00;
		Clr_LCD();
		FLAG_RB6 = 0x00;
	}
}

void swap(char* a, char* b){
    char c = *a;
    *a = *b;
    *b = c;
}

void reverse(char* first, char* last){
    while((first != last) && (first != --last))
        swap(first++, last);
}

void make_string(char* str_to_eeprom, char* num_, char* ternary){
	short i = 0x00, j = 0x00;
	strcpy(str_to_eeprom, "      ");//6
	strcat(str_to_eeprom, num_);
	while(num_[i]) i++;	
	for (j = 10; j > i; j--)
		strcat(str_to_eeprom, " ");
	strcat(str_to_eeprom, ternary);	   
	strcat(str_to_eeprom, "      ");//6
}