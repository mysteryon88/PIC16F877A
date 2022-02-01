/*
Author: unknown
*/
#ifndef _LCD_H_
#define	_LCD_H_

#include "stack.h"
#include <pic16f877a.h>
#include <xc.h>


#define PORT_LCD PORTB
#define TRIS_LCD TRISB
#define RS RB1
#define E RB2

#define _LENGTH_STING_ 16

#define DELAY_1600us 1600
#define DELAY_40us 40
#define DELAY_5us 5

#define DELAY_5ms 5
#define DELAY_50ms 50

byte CURRENT_POS;

#define push_LCD() {                                \
    push_stack(TRIS_LCD);                           \
    push_stack(PORT_LCD);                           \
}
#define pop_LCD() {                                 \
    pop_stack(&PORT_LCD);                           \
    pop_stack(&TRIS_LCD);                           \
}

#define pulse_E() {                                 \
    E = 1;                                          \
    __delay_us(DELAY_5us);                          \
    E = 0;                                          \
    __delay_us(DELAY_5us);                          \
}

#define SEND_LCD(mode, num) {                               \
                                                            \
    PORT_LCD = 0;                                           \
    TRIS_LCD = 0;                                           \
                                                            \
    RS = (mode);                                            \
    E = 0;                                                  \
                                                            \
    do {                                                    \
                                                            \
        while(check_buttons())                              \
            __delay_ms(DELAY_5ms);                          \
                                                            \
        __delay_ms(DELAY_5ms);                              \
                                                            \
        while(check_buttons())                              \
            __delay_ms(DELAY_5ms);                          \
                                                            \
        __delay_ms(DELAY_5ms);                              \
                                                            \
    } while(check_buttons());                               \
                                                            \
    PORT_LCD = (PORT_LCD & 0x0F) + ((num) & 0xF0);          \
    pulse_E();                                              \
                                                            \
    PORT_LCD = (PORT_LCD & 0x0F) + ((num) << 4 & 0xF0);     \
    pulse_E();                                              \
                                                            \
}


void send_command_LCD(byte num);
void send_byte_LCD(byte num);
void init_LCD(void);
void show_string_LCD(const char *str);
void set_coord_LCD(byte i, byte j);
byte check_buttons(void);


#endif