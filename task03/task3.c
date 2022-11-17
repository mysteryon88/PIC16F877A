#include "task3.h"

void interrupt timer0(void)
{
	if (time == 40)
	{
		Clr_LCD();
		if (all)
		{
			Set_Coord_LCD(ind / 16, ind % 16);
			Show_String_LCD(ticker);
		}
		else
		{
			if (up_down)
				Set_Coord_LCD(1, ind % 16);
			else
				Set_Coord_LCD(ind / 16, ind % 16);
			Show_String_LCD_(ticker);
		}
		if (direction)
		{
			if (!all)
				ind = (ind + 1) % 16;
			if (all)
				ind = (ind + 1) % 32;
		}
		else
		{
			if (!all)
				(ind == 0) ? ind = 15 : ind--;
			if (all)
				(ind == 0) ? ind = 31 : ind--;
		}
		time = 0;
	}
	time++;
	T0IF = 0x00;
}

void start(void)
{
	byte decision = 0x00;

	if (eeprom_decision())
	{ //����� �� eeprom ��� ���� �������
		char str[6] = " ";
		loadstr_(str);
		if (str[1] == '-')
			ind = str[0] - 48;
		else
			ind = 10 * (str[0] - 48) + str[1] - 48;
		up_down = str[2] - 48;
		all = str[3] - 48;
		direction = str[4] - 48;
		/*
		Clr_LCD();
		Show_String_LCD(str);
		do {
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 7))) decision = 1;
		} while (!decision);
		*/
	}
	Clr_LCD();
	OPTION_REG = 0x07; // TMR0 and prescaler
	INTCON = 0xA0;	   //���������� �� ������������ TMR0
	task();
}

void task(void)
{
	short num = 0x00;
	byte decision = 0x00;
	int jj = 0x00;

	while (TRUE)
	{
		decision = 0x00;

		PORTB |= 0xF0;
		if (!(PORTB & (1 << 4)))
		{
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 4)))
				direction = direction ? 0x00 : 0x01;
		}

		PORTB |= 0xF0;
		if (!(PORTB & (1 << 5)))
		{
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 5)))
			{
				if (up_down)
				{
					up_down = 0x00;
					ind = 0;
				}
				else
				{
					up_down = 0x01;
					ind = 16;
				}
				all = 0x00;
			}
		}
		PORTB |= 0xF0;
		if (!(PORTB & (1 << 6)))
		{
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 6)))
				all = 0x01;
		}
		PORTB |= 0xF0;
		if (!(PORTB & (1 << 7)))
		{
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 7)))
				save_ind();
		}
	}
}

void Send_Byte_LCD_(byte tmp)
{
	while (Check_buttons())
		Delay(1000);
	PORTB = (PORTB & 0x0F) + (tmp & 0xF0);
	Pulse(10);
	PORTB = (PORTB & 0x0F) + (tmp << 4);
	Pulse(10);
	Current_ind++;
	if (Current_ind == 16)
		Set_Coord_LCD(up_down, 0);
	if (Current_ind == 32)
		Set_Coord_LCD(up_down, 0);
}

void Show_String_LCD_(const char *mySTRING)
{
	while (*mySTRING)
	{
		Send_Byte_LCD_(*(mySTRING++));
	};
}

void save_ind(void)
{
	char ind_str[6] = " ";
	sprintf(ind_str, "%d", ind); //��������� ������ 1-2 �������
	if (ind < 10)
		ind_str[1] = '-';
	ind_str[2] = 48 + up_down;	 //����� ������
	ind_str[3] = 48 + all;		 //��� �� ������
	ind_str[4] = 48 + direction; //�����������
	ind_str[5] = '\0';			 //�����������
	savestr(ind_str);
}

void loadstr_(char *mySTRING)
{
	Init_READ_I2C(0);

	int i = 0;
	for (i = 0; i < 5; i++)
		mySTRING[i] = IN_BYTE_ACK_I2C();

	IN_BYTE_NACK_STOP_I2C();
}
