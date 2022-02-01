/*
Author: unknown
*/
#ifndef _STACK_H_
#define	_STACK_H_

typedef unsigned int u_int;
typedef unsigned char byte;

#define _XTAL_FREQ (20000000)


#define _PORT_BUTTON_ (PORTB)
#define _TRIS_BUTTON_ (TRISB)


#define _SIZE_STACK_ 32

byte STACK_BUF[_SIZE_STACK_];
byte CURRENT_ELEMENT = 0;

#define push_stack(val) {                                       \
    STACK_BUF[CURRENT_ELEMENT] = (val);                         \
    CURRENT_ELEMENT = (CURRENT_ELEMENT + 1) % _SIZE_STACK_;     \
}

#define clear_stack() {                                         \
    CURRENT_ELEMENT = 0;                                        \
}

#define pop_stack(val) {                                        \
    if(!CURRENT_ELEMENT) ++CURRENT_ELEMENT;                     \
    *(val) = STACK_BUF[--CURRENT_ELEMENT];                      \
}



#endif

