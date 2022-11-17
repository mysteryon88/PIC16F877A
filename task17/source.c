/*
Author: unknown

17) Вводятся число в верхней строке. Во второй строке надо вывести сообщение - простое это число или нет. (Число <999. Использовать список простых чисел до 31 и проверять их как делители введенного числа.)
*/

#include "lcd.h"

#include <time.h>
#include <stdlib.h>

//Конфиги
#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = OFF
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

//Макроопределения для проверки значения бита, его установки в 1 и сбрасывания в 0
#define testbit(var, bit) ((var) & (1 << (bit)))
#define setbit(var, bit) ((var) |= (1 << (bit)))
#define clrbit(var, bit) ((var) &= ~(1 << (bit)))

//Функции для общения с экраном по стандарту I2C

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

//Номер пина частоты и данных в PORTC
#define SCL 3
#define SDA 4

//Необходимые переменные и макроопределения для работы функций общения с экраном
byte Ch_ACK;

#define ClockValue 1
#define SET_HIGH_SPEED_I2C SSPADD = ClockValue
#define cache_size_I2C 0x40

//Функция заержки
void Delay(u_int);

//Снова необходимые переменные
byte tmp_buffer_I2C;
byte Slave_ADR_RW_I2C;
byte tmp_I2C;

u_int Adr_I2C;

byte Current_ind;
byte Key_Press;

//Размер буфера
#define SIZE_OF_BUF 4

//Ответ является ли число простым
const char *msg[] = {"YES", "NO"};

//Функции чтения числа и проверки его на простоту
u_int read_number(char *buf, u_int lengthNum);
int is_primeNumber(int num);

void main(void)
{

    //Инициализация экрана
    init_LCD();

    //Адрес экрана
    Slave_ADR_RW_I2C = 0xA0;

    //Устанавливаем PORTC для общения по I2C
    TRISC = 0x9B;

    //Инициализация I2C
    init_I2C();

    //Немного ждем
    __delay_ms(100);

    u_int i;
    char buf[SIZE_OF_BUF];

    //Выводим сообщение

    show_string_LCD("EEPROM(S1/S4)");

    TRISB &= 0x0f;
    PORTB |= 0xf0;

    //Решаем читать старое значение или забыть про него
    // RB 4 - прочитать старый результат
    // RB 7 - проигнорировать

    while (1)
    {

        if (RB4 == 0)
        {

            Init_READ_I2C(0);

            for (i = 0; (buf[i] = IN_BYTE_ACK_I2C()) != '\0'; ++i)
                ;

            show_string_LCD(buf);

            IN_BYTE_NACK_STOP_I2C();

            __delay_ms(5000);

            break;
        }
        else if (RB7 == 0)
        {

            __delay_ms(200);
            while (check_buttons())
                ;

            break;
        }
    }

    //Главный цикл
    while (1)
    {

        //Очищаем экран
        send_command_LCD(0x01);

        //Вводим число в буфер
        show_string_LCD("Number: ");
        unsigned int size = read_number(&buf[0], 3);

        //Переводим его из буфера в int
        int n = atoi(&buf[0]);

        //Провряем на простоту
        if (is_primeNumber(n))
            n = 0;
        else
            n = 1;

        //Записываем результат в EEPROM

        Init_WRITE_I2C(0);

        for (i = 0; msg[n][i] != '\0'; ++i)
        {

            OUT_BYTE_PAGE_I2C(msg[n][i]);
        }

        OUT_BYTE_PAGE_I2C('\0');

        STOP_I2C();

        //Выводим на экран

        set_coord_LCD(1, 0);

        show_string_LCD(&msg[n][0]);

        //Ждём 5 секунд и едем по новой

        __delay_ms(5000);
    }

    return;
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

            pop_stack(CURRENT_POS);

            set_coord_LCD(
                CURRENT_POS / _LENGTH_STING_,
                CURRENT_POS % _LENGTH_STING_);
        }

        PORTB |= 0xf0;
    }

    pop_stack(PORTB);
    pop_stack(TRISB);

    return (buf[0] == '\0') ? posBuf : ++posBuf;
}

int is_primeNumber(int num)
{

    int mas[11] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31};

    if (num == 1)
        return 0;

    for (int i = 0; i < 11; ++i)
        if (!(num % mas[i]) && num != mas[i])
            return 0;

    return 1;
}