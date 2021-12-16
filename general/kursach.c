/*
The library for I2C was written by my teacher Sergey Ivanovich Kovalev, who died in 2021
*/
#include "LCD.h"

#define EEPROM "Use EEPROM?     B1 YES     B2 NO"

void Delay(unsigned short tmp){
    while(tmp--);
	return;
}

byte Check_buttons(void){
    byte tmp, tmp_PORTB = PORTB, tmp_TRISB = TRISB;
    TRISB &= 0x0F;
    PORTB |= 0xF0;
    tmp = PORTB^0xFF;
    PORTB = tmp_PORTB;
    TRISB = tmp_TRISB;
    return (tmp >> 4);
}byte eeprom_decision(void) {
    byte decision = 0, answer = 0;
    Show_String_LCD(EEPROM);
    do {
        PORTB |= 0xF0;
        {
            if (!(PORTB & (1 << 4))) {
                answer = 1;
                decision = 1;
            }
            if (!(PORTB & (1 << 5))) decision = 1;

        }
    } while (!decision);
    return answer;
}

//------------------------------------
void Init_I2C() {
    Slave_ADR_RW_I2C = 0xA0; //0b10100000
    TRISC = 0x9B; //0b10011011
    RC4 = 0;
    TRISC3 = 0;
    TRISC4 = 0;
}

void LOW_SCL_I2C(void) {
    clrbit(PORTC, SCL);
    Delay(5);
}

void HIGH_SCL_I2C(void) {
    setbit(PORTC, SCL);
    Delay(5);
}

void LOW_SDA_I2C(void) {
    clrbit(PORTC, SDA);
    clrbit(TRISC, SDA);
    Delay(5);
}

void HIGH_SDA_I2C(void) {
    setbit(TRISC, SDA);
    Delay(5);
}

void CLOCK_PULSE_I2C(void) {
    HIGH_SCL_I2C();
    LOW_SCL_I2C();
}

void ACK_I2C(void) {
    LOW_SDA_I2C();
    CLOCK_PULSE_I2C();
}

void NACK_I2C(void) {
    HIGH_SDA_I2C();
    CLOCK_PULSE_I2C();
}
//formation of a negative edge on the SDA bus, and then on the SCL bus
void START_I2C(void) {
    HIGH_SDA_I2C();
    HIGH_SCL_I2C();
    LOW_SDA_I2C();
    LOW_SCL_I2C();
}
//formation of a positive edge on SCL, and then on the SDA bus
void STOP_I2C(void) {
    LOW_SDA_I2C();
    LOW_SCL_I2C();
    HIGH_SCL_I2C();
    HIGH_SDA_I2C();
    LOW_SCL_I2C();
}

byte IN_BYTE_I2C(void) {
    byte t = 0, tmp = 8;
    HIGH_SDA_I2C();
    while (tmp--) {
        t += t;
        HIGH_SCL_I2C();
        if (testbit(PORTC, SDA))
            t++;
        LOW_SCL_I2C();
    }
    return (t);
}

byte IN_BYTE_ACK_I2C(void) {
    byte t;
    t = IN_BYTE_I2C();
    ACK_I2C();
    return (t);
}

byte IN_BYTE_NACK_STOP_I2C(void) {
    byte t;
    t = IN_BYTE_I2C();
    NACK_I2C();
    STOP_I2C();
    return (t);
}

void Check_ACK_I2C(void) {
    HIGH_SCL_I2C();
    if (testbit(PORTC, SDA))
        Ch_ACK = 1;
    else
        Ch_ACK = 0;
    LOW_SCL_I2C();
}

void OUT_BYTE_I2C(byte t) {
    byte tmp = 8;
    while (tmp--) {
        if (t & 0x80) //10000000
            HIGH_SDA_I2C();
        else
            LOW_SDA_I2C();
        CLOCK_PULSE_I2C();
        t += t;
    }
    HIGH_SDA_I2C();
    Check_ACK_I2C();
}

void Init_WRITE_I2C(unsigned int Adr_begin) {
    Adr_I2C = Adr_begin;
rep:
    START_I2C();
    Slave_ADR_RW_I2C &= 0xFE; //0b11111110
    OUT_BYTE_I2C(Slave_ADR_RW_I2C);
    if (Ch_ACK) {
        STOP_I2C();
        goto rep;
    }
    OUT_BYTE_I2C(Adr_begin >> 8);
    if (Ch_ACK) {
        STOP_I2C();
        goto rep;
    }
    OUT_BYTE_I2C(Adr_begin);
    if (Ch_ACK) {
        STOP_I2C();
        goto rep;
    }
}

void Init_READ_I2C(unsigned int Adr_begin) {
    Init_WRITE_I2C(Adr_begin);
    START_I2C();
    Slave_ADR_RW_I2C |= 1;
    OUT_BYTE_I2C(Slave_ADR_RW_I2C);
}

void OUT_BYTE_PAGE_I2C(byte tmp) {
    OUT_BYTE_I2C(tmp);
    Adr_I2C++;
    if ((cache_size_I2C - 1) & Adr_I2C)
        return;
    STOP_I2C();
    Init_WRITE_I2C(Adr_I2C);
}

//---------------------------
//saving to eeprom
void savestr(char * mySTRING) {
    Init_WRITE_I2C(0);
	
    while (*mySTRING) OUT_BYTE_PAGE_I2C(*mySTRING++);
 
    STOP_I2C();
}
//reading from eeprom
void loadstr(char * mySTRING) {
    Init_READ_I2C(0);
	
//	while (*mySTRING = IN_BYTE_ACK_I2C()) mySTRING++;

	int i = 0;
   	for (i = 0; i < 31; i++)
    	mySTRING[i] = IN_BYTE_ACK_I2C();
	
    IN_BYTE_NACK_STOP_I2C();
}
