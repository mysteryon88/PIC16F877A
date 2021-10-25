#ifndef KURSACH_H
#define KURSACH_H

#include <xc.h> 		
#include <stdio.h>  //для sprintf()
#include <string.h> //для strcat() и strcpy()

#define byte unsigned char
#define u_int unsigned int

#pragma config FOSC = HS //биты выбора осцилятора HS-осцилятор
#pragma config WDTE = OFF // Бит включения сторожевого таймера (WDT отключен)
#pragma config PWRTE = OFF //Бит включения таймера включения (PWRT выключен)
#pragma config BOREN = OFF // Бит разрешения сброса пониженного напряжения (BOR отключен)
#pragma config LVP = OFF //Бит включения последовательного программирования низкого напряжения (однополярный) (RB3 - это цифровой ввод / вывод, для программирования необходимо использовать HV на MCLR)
#pragma config CPD = OFF //Бит защиты кода памяти EEPROM данных (защита кода EEPROM данных отключена)
#pragma config WRT = OFF //Биты разрешения записи в память программ (защита от записи отключена; вся программная память может быть записана управлением EECON)
#pragma config CP = OFF //Бит защиты кода флэш-памяти программ (защита кода выключена)
#define _F877
#define DIP_40
#define SCL 3
#define SDA 4
#define cache_size_I2C 0x40 // 0b01000000

#define testbit(var, bit) ((var) & (1 <<(bit)))
#define setbit(var, bit) ((var) |= (1 << (bit)))
#define clrbit(var, bit) ((var) &= ~(1 << (bit)))

byte Check_buttons(void); 
void Delay(unsigned short tmp); 
void Beep(void);
//выбор eeprom в задании
byte eeprom_decision(void); 

void Init_I2C(void);
byte IN_BYTE_I2C(void);
byte IN_BYTE_ACK_I2C(void);
byte IN_BYTE_NACK_STOP_I2C(void);
void OUT_BYTE_I2C(byte);
void ACK_I2C(void);
void NACK_I2C(void);
void START_I2C(void);
void STOP_I2C(void);
void LOW_SCL_I2C(void);
void HIGH_SCL_I2C(void);
void LOW_SDA_I2C(void);
void HIGH_SDA_I2C(void);
void CLOCK_PULSE_I2C(void);
void Init_WRITE_I2C(u_int);
void Init_READ_I2C(u_int);
void Check_ACK_I2C(void);
void OUT_BYTE_PAGE_I2C(byte);
//-------------
void savestr(char * mySTRING);
void loadstr(char * mySTRING);
//-------------

//для LCD
byte Current_ind = 0;
//для I2C
byte tmp_buffer_I2C;
byte Slave_ADR_RW_I2C;
byte tmp_I2C;
byte Ch_ACK;
u_int Adr_I2C;

#endif


