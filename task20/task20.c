#include "task20.h"

void interrupt timer0(void)
{
	TMR0_count++; //13.1 мс
    T0IF=0x00;
}

void start(void){
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
		task();
	}
	else task();
}

void task(void){
	byte decision = 0x00;
	byte count_sum = 0x01;
	byte num[8] = " ";
	char str_to_eeprom[32] = " ";
	unsigned long time = TMR0;
	float time_double = 0x00, sum = 0x00;
	
	while(1){	
		Clr_LCD();
		TMR0_count = 0x00;
		TMR0 = 0x00;
		if(time % 2 - 1){ //первый рабочий рандом)
  			Show_String_LCD(LEFT);
			do {
				PORTB |= 0xF0;
				//ждем нажатия кнопки 
				if (!(PORTB & (1 << 4))) decision = 1; 
			} while (!decision);
		}
		else{
  			Show_String_LCD(RIGHT);
			do {
				PORTB |= 0xF0;
				if (!(PORTB & (1 << 5))) decision = 1;	
			} while (!decision);
		}
		decision = 0x00;

		time = TMR0 + TMR0_count * 256;//для рандома
		
		//вывести на экран число 
		time_double = time/(float)IN_TO_SEC;
		
		sum += time_double; //складываем время
		sprintf(num, "%f", sum / count_sum);

		Set_Coord_LCD(1, 2);
 	    Show_String_LCD(num);
		Show_String_LCD(" Secs");
		
		do {
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 7))){
				decision = 1;
				count_sum++;
			}
			if (!(PORTB & (1 << 6))){
				//сохранение
				
				strcpy(str_to_eeprom, AVEREGE);
				strcat(str_to_eeprom, num);
				strcat(str_to_eeprom, " Secs");
				savestr(str_to_eeprom);	

				Clr_LCD();
				Show_String_LCD("  SAVED AVEREGE    ");
	
	 		    Show_String_LCD(num);
				Show_String_LCD(" Secs");
			}		
		} while (!decision);
		decision = 0x00;
	}
}
