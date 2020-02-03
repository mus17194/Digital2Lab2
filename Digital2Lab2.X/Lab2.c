/*
 * File:   Lab2.c
 * Author: Vincenzo Musella
 * 17194
 * Interrupciones y Librerias
 * Created on January 29, 2020, 3:24 PM
 */

// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include "ADC.h"
//FRECUENCIA PARA TRABAJAR
#define _XTAL_FREQ 8000000  

// Configuración de los puertos
void config(void);
void TMR0_v(void);
// VARIABLES
    int bandera = 1;
    int bandera2 = 1;
    int valoradc = 0x00;
    
void main(void) {
    
    config();
    ADC();
    TMR0_v();
    
    while (1) {
        if (PORTBbits.RB0 == 1) {
           bandera = 1;
        }
        
        if (PORTBbits.RB1 == 1) {
            bandera2 = 1;
        }
        ADCON0bits.GO = 1;
        while(ADCON0bits.GO == 1) {
        }
        PORTC = valoradc;
        
        if(valoradc > PORTA) {
            PORTDbits.RD2=1;
        }
        if(valoradc < PORTA) {
            PORTDbits.RD2=0;
        }
        
        __delay_ms(100);
    }
    return;
}

void TMR0_v(void) {
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS2 = 0;
    OPTION_REGbits.PS1 = 0;
    OPTION_REGbits.PS0 = 1;
    
    INTCONbits.T0IF = 0;
    
    TMR0 = 0xFF;
    
}

void __interrupt() interB (void) {
    
    if (INTCONbits.RBIF == 1) {    //REVISAR BANDERA
        //di();
        if (PORTBbits.RB1 == 0 && bandera2 == 1) {
           PORTA = PORTA - 1;
           bandera2 = 0;
           }
        if (PORTBbits.RB0 == 0 && bandera == 1) {
           PORTA++;
           bandera = 0;
           }        
        INTCONbits.RBIF = 0;           //APAGAR BANDERA
       // ei();
    }
    if (PIR1bits.ADIF == 1) {
        valoradc = ADRESH;
        PIR1bits.ADIF = 0;
       
    }
    
    if (INTCONbits.T0IF == 1) {
        switch(PORTDbits.RD0) {
            case 0:
                PORTDbits.RD0 = 1;
                PORTDbits.RD1 = 0;
                break;
            case 1:
                PORTDbits.RD0 = 0;
                PORTDbits.RD1 = 1;
                break;
        }
        INTCONbits.T0IF = 0;
    }
}


void config(void) {
    // PUERTO C COMO SALIDA PARA LOS DISPLAY
    TRISC = 0x00;
    PORTC = 0x00;
    // PUERTO B COMO ENTRADA PARA LOS BOTONES
    PORTB = 0x00;
    TRISB = 0b11111111;
    ANSELH = 0x00;
    //ACTIVAR INTERRUPCIONES EN PORTB RB0 y RB1
    INTCONbits.GIE = 1;
    INTCONbits.RBIE = 1;
    INTCONbits.PEIE = 1;
    IOCB = 0b00000011;      
    // ACTIVAR RESISTENCIAS INTERNAS PULL-UPP
    WPUB = 0b11111111;
    OPTION_REGbits.nRBPU = 0;
    // PUERTO A COMO SALIDA
    PORTA = 0x00;
    TRISA = 0x00;
    // PUERTO D COMO SALIDA
    PORTD = 0x00;
    TRISD = 0x00;
    // PUERTO E COMO SALIDA
    PORTE = 0x0;
    // ENTRADA ANALÓGICA
    ANSEL = 0x00;
    TRISE = 0x0;
    INTCONbits.RBIF = 0;
    
    
    
    
    
    
}
