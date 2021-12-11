#include "task15.h"

void start(void){
	byte decision = 0x00;
	Init_I2C();
    Init_LCD();
	Clr_LCD();
	if (eeprom_decision()) {//вывод из eeprom или само задание
		char str_to_lcd[STR_TO_EEPROM] = " ";
		loadstr(str_to_lcd);
		Clr_LCD();
		Show_String_LCD(str_to_lcd);
		do {
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 7))) decision = 1;
		} while (!decision);
	}
	task();
}

void task(void){
	byte str_one[N], str_two[N-1];
	byte ONE = 0x01, decision = 0x00, FLAG_RB7 = 0x00, SAVED = 0x01;
	char str_to_eeprom[STR_TO_EEPROM];
	short num_one[N], num_two[N-1];
	int p = 10;
	short i = 0x00, j = 0x00, count_one = 0x00;
	while (TRUE){
		Clr_LCD();
		init(num_one, num_two, str_one, str_two);
		do {
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 4))){
				if(ONE){
					num_one[i]++;
					(num_one[i] < 10) ? str_one[i] = 48 + num_one[i] : num_one[i] = -1; //overflow
					Set_Coord_LCD(0, 0);
					Show_String_LCD(str_one);
					Set_Coord_LCD(0, i+1);
				}
				else {
					FLAG_RB7 = 0x01; // разблокирурем кнопку конца ввода
					num_two[i]++;
					(num_two[i] < 10) ? str_two[i] = 48 + num_two[i] : num_two[i] = -1; //overflow
					Set_Coord_LCD(1, 0);
					Show_String_LCD(str_two);
					Set_Coord_LCD(1, i+1);
				}
			}	
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 5))){
				//защита пропуска цифры
				if(num_one[i] != -1 && ONE) i++;
				if(num_two[i] != -1 && !ONE) i++;
				//переходим на второе число если переполнился массив
				if (i == N - 1) ONE = i = 0x00;
				for (j = 0; j < 4; j++) Delay(50000);
			}	
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 6))){
				count_one = i;
				ONE = i = 0x00; // переходим на второе число
				Set_Coord_LCD(1, 0);
			}	
			if(FLAG_RB7){
				PORTB |= 0xF0;
				if (!(PORTB & (1 << 7))) decision = 1;	
			}	
		} while (!decision);
		Clr_LCD();

		//сравнение массивов 
		if(mass_cmp(num_one, num_two, count_one, i)) {
			p = 10;
			while(p--) Beep();
			Show_String_LCD(CAN); //можно получить
		}	
		else
			Show_String_LCD(CANT); //нельзя получить
		Show_String_LCD(SAVE_);
    	decision = 0x00;
		do {
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 7))) decision = 1;	
			//сохранение
			PORTB |= 0xF0;
			if (SAVED && !(PORTB & (1 << 6))){
				SAVED = 0x00; //сохраняем один раз
 				make_string(str_to_eeprom, str_one, str_two);
				savestr(str_to_eeprom);	
				Clr_LCD();
				Show_String_LCD(SAVE);
				Show_String_LCD(NEXT);
			}
		} while (!decision);
		decision = 0x00;
		FLAG_RB7 = 0x00;
		count_one = 0x00;
		SAVED = 0x01; 
		ONE = 0x01;
		i = 0x00;
		Clr_LCD();
	}
}

void make_string(char* str_to_eeprom, char* str_one, char* str_two){
	strcpy(str_to_eeprom, str_one);
	strcat(str_to_eeprom, str_two);	   
}

byte mass_cmp(short* num_one, short* num_two, short count_one, short count_two){
	//сравнение длин, если не совпадают то нам не сравнить
	int counter_one[] = {0,0,0,0,0,0,0,0,0,0};
	int counter_two[] = {0,0,0,0,0,0,0,0,0,0};
	int num[] = {0,1,2,3,4,5,6,7,8,9};
	if(count_one == count_two) {
		for (int i = 0; i < 10; i++) 
        	for (int j = 0; j < count_one; j++){
            	if(num[i] == num_one[j]) counter_one[i]++;
            	if(num[i] == num_two[j]) counter_two[i]++;
        	}

    	for (int i = 0; i < 10; i++)  
			if(counter_one[i] != counter_two[i]) return 0;	
	}
	else return 0;
	return 1;
}

void init(short* num_one, short* num_two, char* str_one, char* str_two){
	byte i;
	for (i = 0x00; i < N; i++){
		str_one[i] = ' ';
		num_one[i] = -1;
	}
	for (i = 0x00; i < N-1; i++){
		str_two[i] = ' ';
        num_two[i] = -1;
	}
}

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