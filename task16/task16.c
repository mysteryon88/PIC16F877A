#include "task16.h"

void start(void)
{
	byte decision = 0x00;
	Clr_LCD();
	if (eeprom_decision())
	{ //����� �� eeprom ��� ���� �������
		char str_to_lcd[STR_TO_EEPROM] = " ";
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
	byte str_one[N], str_two[N - 1];
	byte ONE = 0x01, decision = 0x00, FLAG_RB7 = 0x00, SAVED = 0x01;
	char str_to_eeprom[STR_TO_EEPROM];
	short num_one[N], num_two[N - 1];
	int p = 10;
	short i = 0x00, j = 0x00, count_one = 0x00;
	while (TRUE)
	{
		Clr_LCD();
		init(num_one, num_two, str_one, str_two);
		do
		{
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 4)))
			{
				if (ONE)
				{
					num_one[i]++;
					(num_one[i] < 10) ? str_one[i] = 48 + num_one[i] : num_one[i] = -1; // overflow
					Set_Coord_LCD(0, 0);
					Show_String_LCD(str_one);
					Set_Coord_LCD(0, i + 1);
				}
				else
				{
					FLAG_RB7 = 0x01; // ������������� ������ ����� �����
					num_two[i]++;
					(num_two[i] < 10) ? str_two[i] = 48 + num_two[i] : num_two[i] = -1; // overflow
					Set_Coord_LCD(1, 0);
					Show_String_LCD(str_two);
					Set_Coord_LCD(1, i + 1);
				}
			}
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 5)))
			{
				//������ �������� �����
				if (num_one[i] != -1 && ONE)
					i++;
				if (num_two[i] != -1 && !ONE)
					i++;
				//��������� �� ������ ����� ���� ������������ ������
				if (i == N - 1)
					ONE = i = 0x00;
				for (j = 0; j < 4; j++)
					Delay(50000);
			}
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 6)))
			{
				count_one = i;
				ONE = i = 0x00; // ��������� �� ������ �����
				Set_Coord_LCD(1, 0);
			}
			if (FLAG_RB7)
			{
				PORTB |= 0xF0;
				if (!(PORTB & (1 << 7)))
					decision = 1;
			}
		} while (!decision);
		Clr_LCD();

		//��������� ��������
		if (mass_cmp(num_one, num_two, count_one, i))
		{

			Show_String_LCD(SYMMETRIC); //����� ��������
			while (p--)
				Beep();
		}
		else
			Show_String_LCD(ASYMMETRIC); //������ ��������

		decision = 0x00;
		do
		{
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 7)))
				decision = 1;
			//����������
			PORTB |= 0xF0;
			if (SAVED && !(PORTB & (1 << 6)))
			{
				SAVED = 0x00; //��������� ���� ���
				make_string(str_to_eeprom, str_one, str_two);
				savestr(str_to_eeprom);
				Clr_LCD();
				Show_String_LCD(SAVE);
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

void make_string(char *str_to_eeprom, char *str_one, char *str_two)
{
	strcpy(str_to_eeprom, str_one);
	strcat(str_to_eeprom, str_two);
}

byte mass_cmp(short *num_one, short *num_two, short count_one, short count_two)
{
	//��������� ����, ���� �� ��������� �� ��� �� ��������
	if (count_one == count_two)
	{
		for (size_t i = 0, j = count_one; i < count_one; i++, j--)
			if (*(num_one + i) != *(num_two + j))
				return 0;
	}
	else
		return 0;
	return 1;
}

void init(short *num_one, short *num_two, char *str_one, char *str_two)
{
	byte i;
	for (i = 0x00; i < N; i++)
	{
		str_one[i] = ' ';
		num_one[i] = -1;
	}
	for (i = 0x00; i < N - 1; i++)
	{
		str_two[i] = ' ';
		num_two[i] = -1;
	}
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
