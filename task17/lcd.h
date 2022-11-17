/*
Author: unknown
*/
#ifndef _LCD_H_
#define _LCD_H_

#include "stack.h"
#include <pic16f877a.h>
#include <xc.h>

//Обозначаем на каком порту экран и ножки RS и E
#define PORT_LCD PORTB
#define TRIS_LCD TRISB
#define RS RB1
#define E RB2

//Длина одной строки экрана
#define _LENGTH_STING_ 16

//Необходимые задержки
#define DELAY_1600us 1600
#define DELAY_40us 40
#define DELAY_5us 5

#define DELAY_5ms 5
#define DELAY_50ms 50

//Текущая позиция на экране
byte CURRENT_POS;

//Запоминание в стэк порта к которому подключен экран
#define push_LCD()            \
    {                         \
        push_stack(TRIS_LCD); \
        push_stack(PORT_LCD); \
    }
//Вытаскивание из стэка порта к которому подключен экран
#define pop_LCD()            \
    {                        \
        pop_stack(PORT_LCD); \
        pop_stack(TRIS_LCD); \
    }
//Пульс на ножку E
#define pulse_E()              \
    {                          \
        E = 1;                 \
        __delay_us(DELAY_5us); \
        E = 0;                 \
        __delay_us(DELAY_5us); \
    }
//Отправка  данных на экран (команда/данные, само значение)
#define SEND_LCD(mode, num)                                 \
    {                                                       \
                                                            \
        PORT_LCD = 0;                                       \
        TRIS_LCD = 0;                                       \
                                                            \
        RS = (mode);                                        \
        E = 0;                                              \
                                                            \
        do                                                  \
        {                                                   \
                                                            \
            while (check_buttons())                         \
                __delay_ms(DELAY_5ms);                      \
                                                            \
            __delay_ms(DELAY_5ms);                          \
                                                            \
        } while (check_buttons());                          \
                                                            \
        PORT_LCD = (PORT_LCD & 0x0F) + ((num)&0xF0);        \
        pulse_E();                                          \
                                                            \
        PORT_LCD = (PORT_LCD & 0x0F) + ((num) << 4 & 0xF0); \
        pulse_E();                                          \
    }

//Отправка команды на экран (например отчистить экран)
void send_command_LCD(byte num);
//Отправка символа на экран
void send_byte_LCD(byte num);
//Инициализация экрана (выбор способоа общения (4bit, 8bit), выбор формы курсора и т.д.)
void init_LCD(void);
//Отправка строки на экран
void show_string_LCD(const char *str);
//Установка координат (0,0 - первая строка; 1,0 - вторая строка)
void set_coord_LCD(byte i, byte j);
//Проверка нажаты ли сейчас кнопки
byte check_buttons(void);

#endif