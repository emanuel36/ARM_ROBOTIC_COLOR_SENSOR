#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pic18f4550.h>
#include "../inc/configuration.h"
#include "../inc/lcd16x2.h"
#include "../inc/interrupt.h"
#include "../inc/pwm.h"
#include "../inc/tcs3200.h"

int Period;

void braco_rb(unsigned int linha, unsigned int coluna){
    float linhas[5]; linhas[0] = 3.8; linhas[1] = 4.9; linhas[2] = 5.8;
    float colunas[5]; colunas[0] = 8.0; colunas[1] = 6.8; colunas[2] = 5.7;
    
    SetDutyCyclePWM1(colunas[coluna], Period);
    SetDutyCyclePWM2(linhas[linha], Period);
}

int main(){
    OSCCON = 0x62; //Internal Oscilator - 4 MHz
    char display_1[16];
    char display_2[16];
    
    PWM1_Init();  
    PWM2_Init();
    braco_rb(1,1);
    Period = setPeriodTo(50);   
    
    init_tcs3200();
    LCD_Init();
    
    //config_int0();
    //config_int1();
    
//    while(1){
//        LCD_Clear();
//        sprintf(display_1, "S1: %.1f S2: %.1f", DutyPWM1, DutyPWM2);
//        LCD_String_xy(1,0,display_1);
//        SetDutyCyclePWM1(DutyPWM1, Period);
//        SetDutyCyclePWM2(DutyPWM2, Period);
//        __delay_ms(200);
//    }
    
    
    int linha, coluna;
    while(1){
        for(coluna = 0; coluna < 3; coluna++){
            for(linha = 0; linha < 3; linha++){
                braco_rb(linha, coluna);
                __delay_ms(100);
                get_color();
                __delay_ms(500);
            }
            braco_rb(linha, coluna);
        } 
    }
    return (EXIT_SUCCESS);
}