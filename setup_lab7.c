/* 
 * File:   setup.c
 * Author: Luis Pedro Garrido Jurado
 *
 * Created on 28 maart 2022, 16:32
 */

#include <xc.h>

void setup(void){
    ANSEL = 0;
    ANSELH = 0;                 // I/O digitales
    
    OSCCONbits.IRCF = 0b100;    // 1MHz
    OSCCONbits.SCS = 1;         // Oscilador interno
    
    TRISA = 0;                  // PORTA como salida
    PORTA = 0;                  // Limpiamos PORTA
    TRISC = 0;                  // PORTC como salida
    PORTC = 0;                  // Limpiamos PORTC
    TRISD = 0;                  // PORTC como salida
    PORTD = 0;                  // Limpiamos PORTC
    TRISB = 0xFF;               // RB0 como entrada (configurada en decimal)

    OPTION_REGbits.nRBPU = 0;   // Habilitamos resistencias de pull-up del PORTB
    WPUB = 0x03;                // Habilitamos resistencia de pull-up de RB0
    
    OPTION_REGbits.T0CS = 0;    // TMR0 como temporizador
    OPTION_REGbits.PSA = 0;     // prescaler a TMR0
    OPTION_REGbits.PS = 0b111;  // prescaler (111) = 1:256
    TMR0 = 254;
    
    INTCONbits.GIE = 1;         // Habilitamos interrupciones globales
    INTCONbits.RBIE = 1;        // Habilitamos interrupciones del PORTB
    IOCB = 0x03;                // Habilitamos interrupción por cambio de estado para RB0 y RB1
    INTCONbits.RBIF = 0;        // Limpiamos bandera de interrupción
    INTCONbits.T0IE = 1;        // Habilitamos interrupciones del TMR0
    INTCONbits.T0IF = 0;        // Limpiamos bandera de interrupción
    
}

