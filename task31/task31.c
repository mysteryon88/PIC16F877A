#include "task31.h"

#define TRUE 1
#define N 6
#define M 9

void start(void){
	byte decision = 0x00;
	
	if (eeprom_decision()) {//output from eeprom or task
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
	short num_one[N], num_two[N];
	byte str_one[N], str_two[N], result_str[M];
	u_int int_num_two = 0x00;
	unsigned long int_num_one = 0x00;
	size_t i = 0x00, j = 0x00;
	float result = 0;
	byte ONE = 0x01, decision = 0x00, FLAG_RB7 = 0x00;
	while (TRUE){
		Clr_LCD();
		for (i = 0x00; i < N; i++){
			str_one[i] = ' ';
			str_two[i] = ' ';
			num_one[i] = -1;
            num_two[i] = -1;
		}
		for (i = 0x00; i < M; i++) result_str[8] = ' ';
		i = 0x00;
		do {
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 4))){
				if(ONE){
					num_one[i]++;
					(num_one[i] < 10) ? str_one[i] = 48 + num_one[i] : num_one[i] = -1; //overflow
					Set_Coord_LCD(0, 0);
					Show_String_LCD(str_one);
					Set_Coord_LCD(0, 0+i+1);
				}
				else {
					FLAG_RB7 = 0x01; // разблокирурем кнопку конца ввода
					num_two[i]++;
					(num_two[i] < 10) ? str_two[i] = 48 + num_two[i] : num_two[i] = -1; //overflow
					Set_Coord_LCD(0, 9);
					Show_String_LCD(str_two);
					Set_Coord_LCD(0, 9+i+1);
				}
			}	
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 5))){
				i++;
				//переходим на второе число
				if (i == N) ONE = i = 0x00;
				for (j = 0; j < 4; j++) Delay(50000);
			}	
			if(FLAG_RB7){
				PORTB |= 0xF0;
				if (!(PORTB & (1 << 7))) decision = 1;	
			}
		} while (!decision);
		int_num_one = mas_to_int(num_one, N);
		int_num_two = mas_to_int(num_two, i+1);
		result = int_num_one / int_num_two;
		sprintf(result_str, "%.4f", int_num_one / (float)int_num_two); //output 
	//	sprintf(result_str, "%d", int_num_one);
		Set_Coord_LCD(1, 0);
		Show_String_LCD(result_str);
		do{
			PORTB |= 0xF0;
			if (!(PORTB & (1 << 7))) break;
		} while (TRUE);
		decision = 0x00;
		FLAG_RB7 = 0x00;
		ONE = 0x01;
	}
}

u_int mas_to_int(short *mas, size_t size){
	size_t size_tmp = size;
	u_int num = 0;
	for (size_t i = size, n = 0; i > 0; --i, n++)
		num += mas[n] * power(10,i-1);
	
    return num;
}

u_int power(u_int num, short ex){
	u_int num_tmp = num;
	if(ex == 0) return 1;
	for(size_t i = 1; i < ex; i++) num_tmp*=num;
	return num_tmp;
}
