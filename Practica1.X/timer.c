/* 
 * File:   timer.c
 * Author: ivan_
 *
 * Created on 26 de septiembre de 2024, 05:45 PM
 */

//  Declaracion de las bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <pic16f818.h>
#include <xc.h>


/*
 * 
 */
void main() {
    OSCCONbits.IRCF0 = 0;       //Frecuencia del oscilador local 31 kHz
    OSCCONbits.IOFS = 1;        //Frecuencia estable
    OPTION_REG= 0b00100010;     //Sin restricciones pull-up, sin flancos
    TMR0 = 0x00;                //Limpiar el registro TMR0
    INTCONbits.TMR0IF = 0;      //Limpiar bandera de interrupcion del TMR0
    //Configuracion de los puertos
    
    TRISB = 0x00;            //Puerto B de salida
    PORTB = 0x00;   //Puerto B limpio
    
loop:
    
    PORTBbits.RB0=1;                //Se enciende 
    __delay_ms(20000);                    //Retardo en segundos
    PORTBbits.RB0 = 0;
    goto loop;
}

