/*
The library for I2C was written by my teacher Sergey Ivanovich Kovalev, who died in 2021
*/
#ifndef KURSACH_H
#define KURSACH_H

#include <xc.h>
#include <stdio.h>  //��� sprintf()
#include <string.h> //��� strcat() � strcpy()

#define byte unsigned char
#define u_int unsigned int

#pragma config FOSC = HS   //���� ������ ���������� HS-���������
#pragma config WDTE = OFF  // ��� ��������� ����������� ������� (WDT ��������)
#pragma config PWRTE = OFF //��� ��������� ������� ��������� (PWRT ��������)
#pragma config BOREN = OFF // ��� ���������� ������ ����������� ���������� (BOR ��������)
#pragma config LVP = OFF   //��� ��������� ����������������� ���������������� ������� ���������� (������������) (RB3 - ��� �������� ���� / �����, ��� ���������������� ���������� ������������ HV �� MCLR)
#pragma config CPD = OFF   //��� ������ ���� ������ EEPROM ������ (������ ���� EEPROM ������ ���������)
#pragma config WRT = OFF   //���� ���������� ������ � ������ �������� (������ �� ������ ���������; ��� ����������� ������ ����� ���� �������� ����������� EECON)
#pragma config CP = OFF    //��� ������ ���� ����-������ �������� (������ ���� ���������)
#define _F877
#define DIP_40
#define SCL 3
#define SDA 4
#define cache_size_I2C 0x40 // 0b01000000

#define testbit(var, bit) ((var) & (1 << (bit)))
#define setbit(var, bit) ((var) |= (1 << (bit)))
#define clrbit(var, bit) ((var) &= ~(1 << (bit)))

byte Check_buttons(void);
void Delay(unsigned short tmp);
void Beep(void);
//����� eeprom � �������
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
void savestr(char *mySTRING);
void loadstr(char *mySTRING);
//-------------

//��� LCD
byte Current_ind = 0;
//��� I2C
byte tmp_buffer_I2C;
byte Slave_ADR_RW_I2C;
byte tmp_I2C;
byte Ch_ACK;
u_int Adr_I2C;

#endif
