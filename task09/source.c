/*
Author: unknown

9)Вводится число, которое расценивается как десятичное значение ASCII – кода (в интервале 30..50). На второй строке отображается символ с таким кодом. Можно вводить несколько чисел подряд, при этом все, что ранее выведено, сдвигается вправо.
*/
#include "lcd.h"
#include "stdlib.h"

#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = OFF
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define testbit(var, bit) ((var) & (1 << (bit)))
#define setbit(var, bit) ((var) |= (1 << (bit)))
#define clrbit(var, bit) ((var) &= ~(1 << (bit)))

void init_I2C(void);
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
void Beep(void);

#define SCL 3
#define SDA 4

byte Ch_ACK;

#define ClockValue 1
#define SET_HIGH_SPEED_I2C SSPADD = ClockValue
#define cache_size_I2C 0x40

void Delay(u_int);

byte tmp_buffer_I2C;
byte Slave_ADR_RW_I2C;
byte tmp_I2C;

u_int Adr_I2C;

byte Current_ind;
byte Key_Press;

#define SIZEOF_BUF 15
#define COUNT_DIG 2

#define MSG_1 "Number: "
#define MSG_2 "Not valid!"
#define EMPTY_STR "               "

u_int read_number(char *buf, u_int lengthNum);
void shr_buf(char sym, char *buf_1);

void main(void)
{

    u_int i;

    init_LCD();

    u_int size_1;

    char buf_1[COUNT_DIG + 1];
    char buf_2[SIZEOF_BUF] = {'\0'};

    Slave_ADR_RW_I2C = 0xA0;

    TRISC = 0x9B;

    init_I2C();

    __delay_ms(100);

    send_command_LCD(0x01);
    show_string_LCD("EEPROM(S1/S4)");

    TRISB &= 0x0f;
    PORTB |= 0xf0;

    while (1)
    {

        if (RB4 == 0)
        {

            Init_READ_I2C(0);
            for (i = 0; (buf_2[i] = IN_BYTE_ACK_I2C()) != '\0'; ++i)
                ;
            IN_BYTE_NACK_STOP_I2C();

            break;
        }
        else if (RB7 == 0)
        {

            break;
        }
    }

    __delay_ms(200);
    while (check_buttons())
        ;

    send_command_LCD(0x01);

    set_coord_LCD(1, 0);
    show_string_LCD(buf_2);
    set_coord_LCD(0, 0);

    while (1)
    {

        set_coord_LCD(0, 0);
        show_string_LCD(EMPTY_STR);
        set_coord_LCD(0, 0);

        while (1)
        {

            show_string_LCD(MSG_1);
            size_1 = read_number(&buf_1[0], COUNT_DIG);
            size_1 = atoi(buf_1);

            if (size_1 < 30 || size_1 > 50)
            {

                set_coord_LCD(0, 0);
                show_string_LCD(EMPTY_STR);
                set_coord_LCD(0, 0);
                show_string_LCD(MSG_2);

                __delay_ms(2000);

                set_coord_LCD(0, 0);
                show_string_LCD(EMPTY_STR);
                set_coord_LCD(0, 0);
            }
            else
                break;
        }

        __delay_ms(50);
        while (check_buttons())
            ;

        shr_buf(size_1, buf_2);

        set_coord_LCD(1, 0);
        show_string_LCD(buf_2);
        set_coord_LCD(0, 0);

        Init_WRITE_I2C(0);

        for (i = 0; buf_2[i] != '\0'; ++i)
            OUT_BYTE_PAGE_I2C(buf_2[i]);

        OUT_BYTE_PAGE_I2C('\0');

        STOP_I2C();

        set_coord_LCD(0, 0);
    }

    return;
}

void shr_buf(char sym, char *buf_1)
{

    u_int i;

    for (i = SIZEOF_BUF - 2; i > 0; --i)
        buf_1[i] = buf_1[i - 1];

    buf_1[0] = sym;
}

void init_I2C(void)
{

    RC4 = 0;
    TRISC3 = 0;
    TRISC4 = 0;
}
byte IN_BYTE_I2C(void)
{

    byte t = 0;
    byte tmp = 8;

    HIGH_SDA_I2C();

    while (tmp--)
    {

        t += t;

        HIGH_SCL_I2C();

        if (testbit(PORTC, SDA))
            t++;

        LOW_SCL_I2C();
    }

    return t;
}
byte IN_BYTE_ACK_I2C(void)
{
    byte t;
    t = IN_BYTE_I2C();
    ACK_I2C();
    return t;
}
byte IN_BYTE_NACK_STOP_I2C(void)
{
    byte t;
    t = IN_BYTE_I2C();
    NACK_I2C();
    STOP_I2C();
    return t;
}
void OUT_BYTE_I2C(byte t)
{

    byte tmp;
    tmp = 8;

    while (tmp--)
    {

        if (t & 0x80)
            HIGH_SDA_I2C();
        else
            LOW_SDA_I2C();

        CLOCK_PULSE_I2C();

        t += t;
    }

    HIGH_SDA_I2C();
    Check_ACK_I2C();
}
void ACK_I2C(void)
{
    LOW_SDA_I2C();
    CLOCK_PULSE_I2C();
}
void NACK_I2C(void)
{
    HIGH_SDA_I2C();
    CLOCK_PULSE_I2C();
}
void START_I2C(void)
{
    HIGH_SDA_I2C();
    HIGH_SCL_I2C();
    LOW_SDA_I2C();
    LOW_SCL_I2C();
}
void STOP_I2C(void)
{
    LOW_SDA_I2C();
    LOW_SCL_I2C();
    HIGH_SCL_I2C();
    HIGH_SDA_I2C();
    LOW_SCL_I2C();
}
void LOW_SCL_I2C(void)
{
    clrbit(PORTC, SCL);
    Delay(5);
}
void HIGH_SCL_I2C(void)
{
    setbit(PORTC, SCL);
    Delay(5);
}
void LOW_SDA_I2C(void)
{
    clrbit(PORTC, SDA);
    clrbit(TRISC, SDA);
    Delay(5);
}
void HIGH_SDA_I2C(void)
{
    setbit(TRISC, SDA);
    Delay(5);
}
void CLOCK_PULSE_I2C(void)
{
    HIGH_SCL_I2C();
    LOW_SCL_I2C();
}
void Init_WRITE_I2C(u_int Adr_begin)
{

    Adr_I2C = Adr_begin;

rep:
    START_I2C();
    Slave_ADR_RW_I2C &= 0xFE;
    OUT_BYTE_I2C(Slave_ADR_RW_I2C);

    if (Ch_ACK)
    {
        STOP_I2C();
        goto rep;
    }

    OUT_BYTE_I2C(Adr_begin >> 8);

    if (Ch_ACK)
    {
        STOP_I2C();
        goto rep;
    }

    OUT_BYTE_I2C(Adr_begin);

    if (Ch_ACK)
    {
        STOP_I2C();

        goto rep;
    }
}
void Init_READ_I2C(u_int Adr_begin)
{
    Init_WRITE_I2C(Adr_begin);
    START_I2C();
    Slave_ADR_RW_I2C |= 1;
    OUT_BYTE_I2C(Slave_ADR_RW_I2C);
}
void Check_ACK_I2C(void)
{

    HIGH_SCL_I2C();

    if (testbit(PORTC, SDA))
        Ch_ACK = 1;
    else
        Ch_ACK = 0;

    LOW_SCL_I2C();
}
void OUT_BYTE_PAGE_I2C(byte tmp)
{

    OUT_BYTE_I2C(tmp);

    Adr_I2C++;

    if ((cache_size_I2C - 1) & Adr_I2C)
        return;

    STOP_I2C();

    Init_WRITE_I2C(Adr_I2C);

    return;
}

void Delay(u_int tmp)
{
    while (tmp--)
        ;
}

u_int read_number(char *buf, u_int lengthNum)
{

    byte posBuf = 0;
    byte flag = 1;

    push_stack(TRISB);
    push_stack(PORTB);

    TRISB &= 0x0f;
    PORTB |= 0xf0;

    --lengthNum;

    for (buf[0] = '\0'; flag;)
    {

        if (!RB4)
        {

            flag = 0;
        }
        else if (!RB5)
        {

            if (posBuf > 0)
            {

                send_byte_LCD(' ');

                CURRENT_POS -= 2;

                buf[posBuf--] = '\0';

                set_coord_LCD(
                    CURRENT_POS / _LENGTH_STING_,
                    CURRENT_POS % _LENGTH_STING_);
            }
        }
        else if (!RB6)
        {

            if (buf[posBuf] != '\0' && posBuf < lengthNum && buf[0] != '0')
            {
                ++CURRENT_POS;
                buf[++posBuf] = '\0';

                set_coord_LCD(
                    CURRENT_POS / _LENGTH_STING_,
                    CURRENT_POS % _LENGTH_STING_);
            }
        }
        else if (!RB7)
        {

            if (buf[posBuf])
                buf[posBuf] = ((buf[posBuf] + 1) - 48) % 10 + 48;
            else
            {
                buf[posBuf] = '0';
                buf[posBuf + 1] = '\0';
            }

            push_stack(CURRENT_POS);

            CURRENT_POS -= posBuf;

            set_coord_LCD(
                CURRENT_POS / _LENGTH_STING_,
                CURRENT_POS % _LENGTH_STING_);

            show_string_LCD(buf);

            pop_stack(&CURRENT_POS);

            set_coord_LCD(
                CURRENT_POS / _LENGTH_STING_,
                CURRENT_POS % _LENGTH_STING_);
        }

        PORTB |= 0xf0;
    }

    pop_stack(&PORTB);
    pop_stack(&TRISB);

    return (buf[0] == '\0') ? posBuf : ++posBuf;
}