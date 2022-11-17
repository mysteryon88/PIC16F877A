#include "task31.h"
#include <math.h>

#define TRUE 1
#define N 7
#define M 10
#define RES "RES - "

//���������� � �����, ��������� ���������
void start(void)
{
	Init_I2C();
	Init_LCD();
	Clr_LCD();

	byte decision = 0x00;

	if (eeprom_decision())
	{ // output from eeprom or task
		char str_to_lcd[32] = " ";
		loadstr(str_to_lcd);
		Show_String_LCD(str_to_lcd);
		do
		{
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 7)))
				decision = 1;
		} while (!decision);
	}
	byte cls[32];
	memset(cls, ' ', 32);
	savestr(cls);
	task();
}

void task(void)
{
	short num_one[N], num_two[N];							 //������� ���� �� ������� ��������� �����
	byte str_one[N], str_two[N], result_str[M], res_drop[4]; //������� ��� ������ �� �������
	byte str_eeprom[32] = " ";								 //������ ���������� � ������
	byte ZEL = 0;
	size_t i = 0x00, j = 0x00; // ��������
	//�������� �� ���� �������/������� �����, �������� �� ����� �� �����, ���� ������������ ������
	byte ONE = 0x01, decision = 0x00, FLAG_RB7 = 0x00;
	long double res = 0x00; //��������� ������� �� ��������� � result_str
	while (TRUE)
	{
		Clr_LCD();
		//�������������/��������
		for (i = 0x00; i < N; i++)
		{
			str_one[i] = '\0';
			str_two[i] = '\0';
			num_one[i] = -1;
			num_two[i] = -1;
		}
		for (i = 0x00; i < M; i++)
			result_str[i] = '\0';
		for (i = 0x00; i < 32; i++)
			str_eeprom[i] = '\0';
		i = 0x00;
		do
		{
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 4)))
			{ //������ �� RB4 ������ ������ +1
				if (ONE)
				{ //����� ����� ��������
					num_one[i]++;

					(num_one[i] < 10) ? str_one[i] = 48 + num_one[i] : num_one[i] = -1; // overflow
					Set_Coord_LCD(0, 0);
					Show_String_LCD(str_one);	 //�������
					Set_Coord_LCD(0, 0 + i + 1); //������������ ������ ������
				}
				else
				{
					FLAG_RB7 = 0x01; // ������������� ������ ����� �����
					num_two[i]++;
					(num_two[i] < 10) ? str_two[i] = 48 + num_two[i] : num_two[i] = -1; // overflow
					Set_Coord_LCD(0, 9);
					Show_String_LCD(str_two);
					Set_Coord_LCD(0, 9 + i + 1);
				}
			}
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 5)))
			{ //��������� �� ��������� �����
				//������ �� ������, ����� �� �������� ����
				if (num_one[i] != -1 && ONE)
					i++;
				if (num_two[i] != -1 && !ONE)
					i++;
				//	i++;
				//��������� �� ������ ����� ��� ���������� i = 6
				if (i == N - 1)
					ONE = i = 0x00;
				for (j = 0; j < 4; j++)
					Delay(50000);
			}
			if (FLAG_RB7)
			{ // end of enter
				PORTB |= 0xF0;
				if (!(PORTB & (1 << 7)))
					decision = 1;
			}
		} while (!decision);

		//	if(mas_to_int(num_two, i+1) != 1){
		//����� ������ ����� �� ������
		res = (long double)mas_to_int(num_one, N - 1) / (long double)mas_to_int(num_two, i + 1);
		//	}
		//	else res = mas_to_int(num_one, N);

		//���������� ��� � ������
		unsigned long res_int = 0;

		res_int = res;

		sprintf(result_str, "%llu", res_int);

		//	sprintf(result_str, "%f", res);

		i = 10;

		//	if (floorl(res) == res)	{
		if ((res - res_int) == 0)
			while (i--)
				Beep();

		i = 0x00;

		while (result_str[i])
			i++;

		result_str[i] = '.';
		res -= res_int; //�������� ������� �����
		i++;
		for (j = 0; j < 3; j++)
		{
			res *= 10;
			res_int = res;	//�������� ����� �����
			res -= res_int; //�������� ������� �����
			result_str[i++] = 48 + res_int;
		}

		Set_Coord_LCD(1, 0);
		Show_String_LCD(RES);
		Show_String_LCD(result_str);

		//�������� ����� �� �����, ���� ������ �� ������

		do
		{
			PORTB |= 0xF0;
			//��������������
			if (!(PORTB & (1 << 7)))
			{
				strcpy(str_eeprom, str_one); // 6
				strcat(str_eeprom, "    ");	 // 4
				strcat(str_eeprom, str_two);
				i = 0x00;
				while (str_two[i])
					i++;
				for (j = 6; j > i; j--)
					strcat(str_eeprom, " ");
				strcat(str_eeprom, RES);

				strcat(str_eeprom, result_str);
				i = 0x00;
				//	while(result_str[i]) i++;
				//	for (j = 16; j > i; j--)
				//		strcat(str_eeprom, " ");
				savestr(str_eeprom);
				break;
			}

		} while (TRUE);
		decision = 0x00;
		FLAG_RB7 = 0x00;
		ONE = 0x01;
		ZEL = 0x00;
	}
}
//��������� �� ������� � ����� �����
unsigned long int mas_to_int(short *mas, size_t size)
{
	size_t size_tmp = size;
	unsigned long int num = 0;
	for (size_t i = size, n = 0; i > 0; --i, n++)
		num += *(mas + n) * power(10, i - 1);
	return num;
}
//������� ��� 10
unsigned long int power(u_int num, short ex)
{
	unsigned long num_tmp = num;
	if (ex == 0)
		return 1;
	for (size_t i = 1; i < ex; i++)
		num_tmp *= num;
	return num_tmp;
}

//������
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