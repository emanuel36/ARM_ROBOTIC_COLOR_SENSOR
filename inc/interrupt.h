#ifndef INTERRUPT_H
#define	INTERRUPT_H
    
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <pic18f4550.h>
#include "../inc/lcd16x2.h"
#include "configuration.h"

void config_tmr0();
void config_int0();
void config_int1();
void config_int2();

void config_tmr0(){
    T0CONbits.PSA = 1;
    T0CONbits.T0SE = 0;
    T0CONbits.T0CS = 1;
    T0CONbits.T08BIT = 0;
    TMR0 = 0x00;
    T0CONbits.TMR0ON = 1;
    
    INTCONbits.TMR0IE = 0;
}

void config_tmr1(){
    T1CONbits.RD16 = 0;
    T1CONbits.T1CKPS = 0;
    T1CONbits.T1OSCEN = 0;
    T1CONbits.TMR1CS = 0;
    T1CONbits.TMR1ON = 0;
    
    PIE1bits.TMR1IE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
}

void config_int0(){
    INTCONbits.PEIE_GIEL = 0;
    TRISBbits.RB0 = 1;
    INTCON2bits.INTEDG0 = 1;
    INTCONbits.INT0IF = 0;
    INTCONbits.INT0IE = 1;
    INTCONbits.GIE_GIEH = 1;
}

void config_int1(){
    INTCONbits.GIE_GIEH = 1;
    INTCONbits.PEIE_GIEL = 0;
    TRISBbits.RB1 = 1;
    INTCON3bits.INT1IE = 1;
    INTCON2bits.INTEDG2 = 0;
    INTCON3bits.INT2IF = 0;
}

void config_int2(){
    INTCONbits.GIE_GIEH = 1;
    INTCONbits.PEIE_GIEL = 0;
    TRISBbits.RB2 = 1;
    INTCON3bits.INT2E = 1;
    INTCON2bits.INTEDG2 = 0;
    INTCON3bits.INT2IF = 0;
}

void interrupt ISR(){
    if(INTCONbits.INT0IF){      //Bot�o 1 pressionado
        INTCONbits.INT0IF = 0;
        interrupt_flag = interrupt_flag ^ 1;
    }
    
    if(INTCON3bits.INT1IF){     //Bot�o 2 pressionado    
        INTCON3bits.INT1IF = 0;
        interrupt_flag = interrupt_flag ^ 2;
    }
    
    if(INTCON3bits.INT2IF){     //Bot�o 3 pressionado
        INTCON3bits.INT2F = 0; 
        interrupt_flag = interrupt_flag ^ 4;
    } 
    
    if(PIR1bits.TMR1IF){
        PIR1bits.TMR1IF = 0;
        interrupt_flag = interrupt_flag ^ 8;
    }
}

#endif	/* INTERRUPT_H */