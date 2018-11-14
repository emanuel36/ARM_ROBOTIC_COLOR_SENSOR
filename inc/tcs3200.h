#ifndef TCS3200_H
#define	TCS3200_H

#include "interrupt.h"
#include "lcd16x2.h"

#define red   0
#define blue  1
#define clear 2
#define green 3

#define S0  PORTDbits.RD0
#define S1  PORTDbits.RD1
#define S2  PORTDbits.RD2
#define S3  PORTDbits.RD3
#define OE  PORTDbits.RD4

void init_tcs3200(){
    TRISD = TRISD & 0xE0;
    S0 = 1;
    S1 = 0;
    OE = 1;
    config_tmr0();
    config_tmr1();
}

void select_photodiode(int diode){
    switch(diode){
        case red:
            S2 = 0;
            S3 = 0;
            break;
            
        case blue:
            S2 = 0;
            S3 = 1;
            break;
            
        case clear:
            S2 = 1;
            S3 = 0;
            break;
            
        case green:
            S2 = 1;
            S3 = 1;
            break;
    }
}

int get_freq(){
    T0CONbits.TMR0ON = 0;
    TMR0 = 0x00;
    OE = 0;
    T0CONbits.TMR0ON = 1;
    TMR1 = 0xD8F0;
    T1CONbits.TMR1ON = 1;
    while(interrupt_flag & 8);
    T1CONbits.TMR1ON = 0;
    T0CONbits.TMR0ON = 0;
    interrupt_flag = interrupt_flag | 8;
    OE = 1;
    return TMR0;
}

void print_color(){
    int freq_red, freq_blue, freq_green;
    char line_1[16], line_2[16];
    
    select_photodiode(red);
    __delay_ms(1);
    freq_red = get_freq()/10;
    
    select_photodiode(green);
    __delay_ms(1);
    freq_green = get_freq()/10;
    
    select_photodiode(blue);
    __delay_ms(1);
    freq_blue = get_freq()/10;
    
    sprintf(line_1, "R:%dkHz G:%dkHz", freq_red, freq_green);
    sprintf(line_2, "B:%dkHz", freq_blue);
    
    LCD_Clear();
    LCD_String_xy(1, 0, line_1);
    LCD_String_xy(2, 0, line_2);
}

#endif	/* TCS3200_H */