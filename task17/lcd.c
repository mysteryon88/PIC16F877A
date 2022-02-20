/*
Author: unknown
*/
#include "lcd.h"


void send_command_LCD(byte num) {
        
    SEND_LCD(0, num);
    
    if(num == 1 || num == 2) {
        __delay_us(DELAY_1600us);
        CURRENT_POS = 0;
    }
    else
        __delay_us(DELAY_40us);
    
    return;
}
void send_byte_LCD(byte num) {
            
    SEND_LCD(1, num);
    
    __delay_us(DELAY_40us);
    
    if(!(++CURRENT_POS % _LENGTH_STING_))
        set_coord_LCD(CURRENT_POS / _LENGTH_STING_, 0);
        
    return;
}
void init_LCD(void) {

    push_LCD();
    
    __delay_ms(20);
    
    send_command_LCD(0x30);
    
    PORT_LCD = 0x28;
    pulse_E();
    __delay_us(DELAY_40us);
    
    send_command_LCD(0x28);
    send_command_LCD(0x08);
    send_command_LCD(0x01);
    send_command_LCD(0x06);
    send_command_LCD(0x0E);
    
    
    CURRENT_POS = 0;
    
    pop_LCD();
    
}
void show_string_LCD(const char *str) {
    
    push_LCD();
    
    while(*str)
        send_byte_LCD(*(str++));
    
    pop_LCD();
}
void set_coord_LCD(byte i, byte j) {
    
    push_LCD();
    
    CURRENT_POS = i * _LENGTH_STING_ + j;
    send_command_LCD(0x80 + (i) * 0x40 + (j));
    
    pop_LCD();
}
byte check_buttons(void) {
    push_stack(_TRIS_BUTTON_);
    push_stack(_PORT_BUTTON_);

    _TRIS_BUTTON_ &= 0x0f;
    _PORT_BUTTON_ |= 0xf0;

    byte tmp = (_PORT_BUTTON_ & 0xf0) != 0xf0;

    pop_stack(_PORT_BUTTON_);
    pop_stack(_TRIS_BUTTON_);
    
    return tmp;
}
