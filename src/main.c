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
    float linhas[5]; linhas[0] = 3.2; linhas[1] = 4.3; linhas[2] = 5.6;
    float colunas[5]; colunas[0] = 8.1; colunas[1] = 6.9; colunas[2] = 5.8;
    
    SetDutyCyclePWM1(colunas[coluna], Period);
    SetDutyCyclePWM2(linhas[linha], Period);
}

int *matrix_scan(){
    int linha, coluna, position[3];
    LCD_Clear();
    LCD_String_xy(1, 4, "SCANNING");
    LCD_String_xy(2, 5, "MATRIX");
    for(coluna = 0; coluna < 3; coluna++){
        for(linha = 0; linha < 3; linha++){
            braco_rb(linha, coluna);
            __delay_ms(300);
            position[0] = get_color();
            if(position[0] != NO_COLOR){
                braco_rb(1,1);
                position[1] = linha;
                position[2] = coluna;
                return position;
            }
        }
    }
    braco_rb(1,1);
    position[0] = NO_COLOR;
    return position;
}

void print_color(int *matrix_led){
    char display[16];
    LCD_Clear();
    switch(matrix_led[0]){
        case RED:
            sprintf(display, "LINE:%d  COLUMN:%d", matrix_led[1], matrix_led[2]);
            LCD_String_xy(1, 0, display);
            LCD_String_xy(2, 3, "COLOR: RED");
            break;
        case GREEN:
            sprintf(display, "LINE:%d  COLUMN:%d", matrix_led[1], matrix_led[2]);
            LCD_String_xy(1, 0, display);
            LCD_String_xy(2, 2, "COLOR: GREEN");
            break;
        case BLUE:
            sprintf(display, "LINE:%d  COLUMN:%d", matrix_led[1], matrix_led[2]);
            LCD_String_xy(1, 0, display);
            LCD_String_xy(2, 2, "COLOR: BLUE");
            break;
        default:
            LCD_String_xy(1, 4, "NO COLOR");
            LCD_String_xy(2, 4, "DETECTED");
            break;
    }
}

int main(){
    /* OSCILADOR INTERNO 4MHz*/
    OSCCON = 0x62; 
    
    /*LCD*/
    LCD_Init();
    
    /*PWM*/
    PWM1_Init();  
    PWM2_Init();
    Period = setPeriodTo(50);   
    
    /*SENSOR DE COR*/
    init_tcs3200();
    
    config_int0();
    
    braco_rb(1,1);
    LCD_String_xy(1, 4, "WAITING");
    LCD_String_xy(2, 4, "REQUEST");
    
    while(1){
        if(interrupt_flag & INT0_FLAG){
            interrupt_flag = interrupt_flag & !INT0_FLAG;
            print_color(matrix_scan());
        }
    }
    
    return (EXIT_SUCCESS);
}