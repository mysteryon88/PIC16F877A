#include "task27.h"

void start(void){	
	Clr_LCD();
	byte decision = 0x00;
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
	short tr[] = {0, 0, 0, 0, 0, 0, 0, 0};
	byte decision = 0x00, triples = 0x00;
	short i = 0x00;
	while (1){
		show(numbers[0], 0, 0);
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
		for (i = 0; i < N - 3; i++){
			//поиск тройки цифр
			if(numbers[i] + numbers[i+1] + numbers[i+2] == 20){
				tr[i] = numbers[i];
				tr[i+1] = numbers[i+1];
				tr[i+2] = numbers[i+2];
				show(tr[i], i, 1);
				show(tr[i+1], i+1, 1);
				show(tr[i+2], i+2, 1);
				short p = 10;
				while(p--) Beep();
				triples = 0x01;
				break;
			}
			else triples = 0x00;
		}	
		
		if (!triples){
			Set_Coord_LCD(1,0);
			Show_String_LCD(NOT);
		}
		do {
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 7))) break;	
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 6))) {
				if (triples)
					save(numbers, tr);
				else	
					save_not(numbers);
				Clr_LCD();
				Show_String_LCD(SAVE);
			}
		} while (1);
		Clr_LCD();
		for(i = 0; i < N; i++) {
			numbers[i] = 0;
			tr[i] = 0;
		}		
	}
}

void show(short num, short i, short j){
	char num_lcd[1] = " ";
	sprintf(num_lcd, "%d", num); 
	Set_Coord_LCD(j, 2*i);
	Show_String_LCD(num_lcd);
}

void save(short *num, short *tr){
	char to_eeprom[32] = " ";
	memset(to_eeprom, ' ', 32);
	short i;
	for(i = 0; i < 8; i++){
		to_eeprom[i*2] = 48 + num[i];
		if (tr[i])
			to_eeprom[i*2+16] = 48 + tr[i];
	}
	savestr(to_eeprom);
}

void save_not(short *num){
	char to_eeprom[32] = " ";
	char not[] = "   NO TRIPLES   ";
	memset(to_eeprom, ' ', 32);
	short i;
	for(i = 0; i < 8; i++)
		to_eeprom[i*2] = 48 + num[i];
	for(i = 0; i < 16; i++)
		to_eeprom[i + 16] = not[i];
	savestr(to_eeprom);
}
