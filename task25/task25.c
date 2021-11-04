#include "task25.h"

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

//звонок
void Beep(void){ 
	byte tmp_TRISB=TRISB, tmp_PORTB=PORTB, i;
	TRISB3=0; 
	i=100000; 
	while(i--){
 		RB3=1;
		Delay(130); //0,125 мс
 		RB3=0;
		Delay(10);
	}
 	PORTB=tmp_PORTB;
 	TRISB=tmp_TRISB;
}

void Overflow(void){ 
	Set_Coord_LCD(1, 4);
	Show_String_LCD("OVERFLOW");
}

void task(void){
	short num[N];
	unsigned short i = 0x00, j = 0x00;
	char up_str[N], str_to_eeprom[32] = " ";
	byte decision = 0x00;
	short duration = 0x00;
	short c_asc = 0x00, c_desc = 0x00, c_not = 0x00;
	while (TRUE){
		for(i = 0; i < N; i++){
			num[i] = -1;
			up_str[i] = ' ';	
		}
		Clr_LCD();
		i = 0x00;
		Set_Coord_LCD(0, 4);
		Show_String_LCD("0");
		do {
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 4))){
				num[i]++;
				(num[i] < 10) ? up_str[i] = 48 + num[i] : num[i] = -1; //overflow
				Set_Coord_LCD(0, 4);
				Show_String_LCD(up_str);
				Set_Coord_LCD(0, 5+i);
			}	
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 5))) {
				if (num[i] != -1){
					if (i < (N-1)) i++;
					else Overflow();
				}
				for (j = 0; j < 4; j++) Delay(50000);
			}	
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 7))) decision = 1;	//exit	
		} while (!decision);
		
		c_asc = 0x00, c_desc = 0x00, c_not = 0x00;
	
		for (j = 0; j < i; j++)
			(num[j]<num[j+1]) ? c_asc++ : (num[j]>num[j+1]) ? c_desc++ : c_not++;
		
		strcpy(str_to_eeprom, "    "); //4
		strcat(str_to_eeprom, up_str); //7
		strcat(str_to_eeprom, "     ");//5
		
		if(c_asc == i) {
			duration = 3;
			while(duration--) Beep();
			Set_Coord_LCD(1, 0);
			Show_String_LCD(ASCENDING);
			strcat(str_to_eeprom, ASCENDING);
		}
		else if (c_desc == i){
			Set_Coord_LCD(1, 0);
			Show_String_LCD(DESCENDING);
			strcat(str_to_eeprom, DESCENDING);
			for (j = 0; j < 2; j++){
				duration = 4;
				while(duration--) Beep();
				Delay(999999);
			}
		}
		else {
			Set_Coord_LCD(1, 0);
			Show_String_LCD(NOT_ORDERED);
			strcat(str_to_eeprom, NOT_ORDERED);
			duration = 15;
			while(duration--) Beep();
		}
		do {
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 7))) break;
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 6))){
				Clr_LCD();
				savestr(str_to_eeprom);	
				Show_String_LCD(SAVE);
			}
		} while (TRUE);
		decision = 0x00;
	}
}
