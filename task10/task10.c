#include "task10.h"

#define TRUE 1

void start(void)
{
	Clr_LCD();
	byte decision = 0x00;
	if (eeprom_decision())
	{ // output from eeprom or task
		char str_to_lcd[32] = " ";
		loadstr(str_to_lcd);
		Clr_LCD();
		Show_String_LCD(str_to_lcd);
		do
		{
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 7)))
				decision = 1;
		} while (!decision);
	}
	task();
}

void task(void)
{
	Clr_LCD();
	char str_to_eeprom[32] = " ";
	byte ind = 0x00, ind_ = 0x00;
	memset(str2, ' ', 15);
	strcat(str1, "    "); // 12+4=16
	Show_String_LCD(str1);
	Set_Coord_LCD(0, 0);
	while (TRUE)
	{
		PORTB |= 0xF0;
		if (!(PORTB & (1 << 4)))
		{
			ind == 15 ? ind = 0 : ind++;
			Set_Coord_LCD(0, ind);
		}
		PORTB |= 0xF0;
		if (!(PORTB & (1 << 5)))
		{
			ind == 0 ? ind = 15 : ind--;
			Set_Coord_LCD(0, ind);
		}
		PORTB |= 0xF0;
		if (!(PORTB & (1 << 6)))
		{
			str2[ind_] = str1[ind];
			ind_ == 14 ? ind_ = 0 : ind_++;
			Set_Coord_LCD(1, 0);
			Show_String_LCD(str2);
			Set_Coord_LCD(0, ind);
		}
		PORTB |= 0xF0;
		if (!(PORTB & (1 << 7)))
		{
			memset(str_to_eeprom, '\0', 32);
			strcpy(str_to_eeprom, str1);
			strcat(str_to_eeprom, str2);
			savestr(str_to_eeprom);
			Clr_LCD();
			Set_Coord_LCD(0, 0);
			Show_String_LCD("SAVED RB4->NEXT");
			while (TRUE)
			{
				PORTB |= 0xF0;
				if (!(PORTB & (1 << 4)))
					break;
			}
			Set_Coord_LCD(0, 0);
			Show_String_LCD(str1);
			Set_Coord_LCD(1, 0);
			memset(str2, ' ', 15);
			Show_String_LCD(str2);
			ind = 0x00;
			ind_ = 0x00;
			Set_Coord_LCD(0, ind);
		}
	}
}
