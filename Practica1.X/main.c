/* 
 * File:   main.c
 * Author: ivan_
 *
 * Created on 26 de septiembre de 2024, 05:41 PM
 */

#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTRC oscillator; port I/O function on both RA6/OSC2/CLKO pin and RA7/OSC1/CLKI pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is digital I/O, MCLR internally tied to VDD)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (RB3/PGM pin has digital I/O function, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off)
#pragma config CCPMX = RB2      // CCP1 Pin Selection bit (CCP1 function on RB2)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)


#include <stdio.h>
#include <stdlib.h>
//#include <pic16f818.h>
#include <xc.h>
#include "Teclado.h"
#define _XTAL_FREQ 4000000

#define BUZZER PORTAbits.RA7

/*
 * 
 */
void main() {
    TRISB=0xF0;
    TRISA= 0x00;
    char tecla;
    int valor1 = 0, valor2 = 0;
    char operacion = 0;
    char estado = 0;
    
    loop:
        tecla = lector();
        if (tecla != 0) {
            if (tecla >= '0' && tecla <= '9') {
                int valorTecla = (int)(tecla - '0');
                BUZZER = 1;
                __delay_ms(100);
                BUZZER = 0;

                if (estado == 0) {
                    valor1 = (valor1 * 10 + valorTecla);
                    actualizarPantalla((valor1/1000)%10,(valor1/100)%10,(valor1/10)%10,valor1%10);
                    
                } else if (estado == 2) {
                    valor2 = (valor2 * 10 + valorTecla);
                    actualizarPantalla((valor1/100)%10,(valor1/10)%10,valor1%10);
                }
            } else if (tecla == 'A' || tecla == 'B' || tecla == 'C' || tecla == 'D') {
                operacion = tecla;
                estado = 2;
                actualizarPantalla(0,0,0);
            } else if (tecla == '*') {
                // Borrar el último dígito ingresado o reiniciar la operación
                if (estado == 0 && valor1 > 0) {
                    valor1 /= 10;
                } else if (estado == 2 && valor2 > 0) {
                    valor2 /= 10;
                } else {
                    // Si no hay valores para borrar, reiniciar toda la operación
                    valor1 = 0;
                    valor2 = 0;
                    operacion = 0;
                    estado = 0;
                }
            } else if (tecla == '#') {
                int resultado = 0;
                if (operacion == 'A') {
                    resultado = valor1 + valor2;
                } else if (operacion == 'B') {
                    resultado = valor1 * valor2;
                }
                valor1 = resultado;
                actualizarPantalla((valor1/100)%10,(valor1/10)%10,valor1%10);
                valor2 = 0;
                operacion = 0;
                estado = 0;
            }
        }
    goto loop;
}

