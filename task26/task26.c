#include "task26.h"

void start(void)
{
	byte decision = 0x00;
	Clr_LCD();
	if (eeprom_decision())
	{
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
		Clr_LCD();
	}
	Clr_LCD();
	task();
}

void Beep(void)
{
	byte tmp_TRISB = TRISB, tmp_PORTB = PORTB, i;
	TRISB3 = 0; // clrbit (TRISB,3);
	i = 100000;
	while (i--)
	{
		RB3 = 1;
		Delay(110); // 0,125 ��
		RB3 = 0;
		Delay(110);
	}
	PORTB = tmp_PORTB;
	TRISB = tmp_TRISB;
}

void Show_LCD(short num, short j)
{
	char num_lcd[5] = " ";
	sprintf(num_lcd, "%d", num);
	Set_Coord_LCD(j, 6);
	Show_String_LCD(num_lcd);
}

void task(void)
{
	short num = 0x00, sum = 0x00;
	byte str_to_eeprom[32] = " ", num_[5] = " ", sum_[4] = " ";
	byte decision = 0x00;
	while (TRUE)
	{
		memset(num_, '\0', 5);
		memset(sum_, '\0', 4);
		Show_LCD(num, 0);
		decision = 0x00;
		do
		{
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 4)))
			{
				num++;
				Show_LCD(num, 0);
			}
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 5)))
			{
				num += 10;
				Show_LCD(num, 0);
			}
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 6)))
			{
				num += 100;
				Show_LCD(num, 0);
			}
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 7)))
				decision = 1;
		} while (!decision);

		sprintf(num_, "%d", num);
		// calculate the amount
		while (num != 0)
		{
			sum += num % 10;
			num /= 10;
		}

		sprintf(sum_, "%d", sum);
		Show_LCD(sum, 1);

		int p = 10;
		if (!(sum % 10))
			while (p--)
				Beep();

		do
		{
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 7)))
				break;
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 6)))
			{
				make_string(str_to_eeprom, num_, sum_);
				Clr_LCD();
				savestr(str_to_eeprom);
				Show_String_LCD(SAVE);
			}
		} while (TRUE);
		memset(str_to_eeprom, '\0', 32);
		num = 0x00;
		sum = 0x00;
		Clr_LCD();
	}
}

void make_string(char *str_to_eeprom, char *num_, char *sum_)
{
	short i = 0x00, j = 0x00;
	strcpy(str_to_eeprom, "      "); // 6
	strcat(str_to_eeprom, num_);
	while (num_[i])
		i++;
	for (j = 10; j > i; j--)
		strcat(str_to_eeprom, " ");
	strcat(str_to_eeprom, "      "); // 6
	strcat(str_to_eeprom, sum_);
	i = 0x00;
	while (sum_[i])
		i++;
	for (j = 10; j > i; j--)
		strcat(str_to_eeprom, " ");
}
