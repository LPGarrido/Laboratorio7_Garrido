/* 
 * File:   main.c
 * Author: Luis Pedro Garrido Jurado
 *
 * Incremento de PORTC al presionar RB0
 * 
 * Created on 22 maart 2022, 20:37
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
#include <stdint.h>
#include "setup_lab7_h.h"

/*------------------------------------------------------------------------------
 * CONSTANTES 
 ------------------------------------------------------------------------------*/
//#define _XTAL_FREQ 4000000
// Asignamos un alias a los pines
#define B1 PORTBbits.RB0     
#define B2 PORTBbits.RB1     
#define EN_D0 PORTDbits.RD0  
#define EN_D1 PORTDbits.RD1
#define EN_D2 PORTDbits.RD2

/*------------------------------------------------------------------------------
 * VARIABLES 
 ------------------------------------------------------------------------------*/
uint8_t conteo = 0;             // LAB sirve para el conteo cada 100ms
uint8_t banderas = 0;           // banderas para el display
uint8_t valores[3]={0,0,0};     // unidades, decenas y centenas del valor
uint8_t display[3]={0,0,0};     // valor a colocar en los displays
uint8_t TABLA[16]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};
/*------------------------------------------------------------------------------
 * PROTOTIPO DE FUNCIONES 
 ------------------------------------------------------------------------------*/
void RESET_TMR0(uint8_t TMR_VAR);   // resetear tmr0
void obtener_valor(uint8_t VALOR);  // obtener uni,dec,cen del valor
void set_display(uint8_t VALORES0, uint8_t VALORES1, uint8_t VALORES2);     // Seleccionar valor para el display
void mostrar_valor(uint8_t DISPLAY0, uint8_t DISPLAY1, uint8_t DISPLAY2);   // Mostrar valor en el display

/*------------------------------------------------------------------------------
 * INTERRUPCIONES 
 ------------------------------------------------------------------------------*/
void __interrupt() isr (void){
    if(INTCONbits.RBIF){            // Fue interrupción del PORTB
        if(!B1)                     // Verificamos si fue RB0 quien generó la interrupción
            PORTA++;                // Incrementar PORTA 
        else if (!B2)               // Si es RB2
            PORTA--;                // Decrementar PORTA
        INTCONbits.RBIF = 0;        // Limpiamos bandera de interrupción
    }
    else if (INTCONbits.T0IF){
        RESET_TMR0(254);            // Reseteo cada 2 ms
        //conteo++;                 // LAB
        mostrar_valor(display[0],display[1],display[2]);    //Mostrar valor cada 2ms
    }
    return;
}

/*------------------------------------------------------------------------------
 * CICLO PRINCIPAL 
 ------------------------------------------------------------------------------*/
void main(void) {
    setup();                        // Llamamos a la función de configuraciones
    while(1){
        obtener_valor(PORTA);       // Valor para el PORTA
        set_display(valores[0],valores[1],valores[2]);  // seteo del display
    }
    return;
}

/*------------------------------------------------------------------------------
 * CONFIGURACION 
 ------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * FUNCIONES 
 ------------------------------------------------------------------------------*/

void RESET_TMR0(uint8_t TMR_VAR){
    TMR0 = TMR_VAR;                 // TMR0 = valor
    INTCONbits.T0IF = 0;            // Limpiamos bandera de interrupción
    return;
}

void obtener_valor(uint8_t VALOR){
    valores[2] = VALOR/100;                             // centenas
    valores[1] = (VALOR-valores[2]*100)/10;             // decenas
    valores[0] = VALOR-valores[2]*100-valores[1]*10;    // unidades
}

void set_display(uint8_t VALORES0, uint8_t VALORES1, uint8_t VALORES2){
    display[0] = TABLA[VALORES2];       // valor del display para cen
    display[1] = TABLA[VALORES1];       // valor del display para dec
    display[2] = TABLA[VALORES0];       // valor del display para uni
    return;
}

void mostrar_valor(uint8_t DISPLAY0, uint8_t DISPLAY1, uint8_t DISPLAY2){
    // limpiar enables
    EN_D0 = 0;
    EN_D1 = 0;
    EN_D2 = 0;
    // segun la bandera mostrar display
    switch (banderas)
     {
          case 0:
            PORTC = DISPLAY0;
            EN_D0 = 1;
            banderas = 1;
            return;
          case 1:
            PORTC = DISPLAY1;
            EN_D1 = 1;
            banderas = 2;
            return;
          case 2:
            PORTC = DISPLAY2;
            EN_D2 = 1;
            banderas = 0;
            return;
          default:
            PORTC = 0b10000000;
            EN_D0 = 1;
            banderas = 0;
            return;
     }  
}