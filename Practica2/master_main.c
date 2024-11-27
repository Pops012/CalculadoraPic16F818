#include <xc.h>
#include "I2C_master.h"

#define _XTAL_FREQ 4000000  // Frecuencia del oscilador (4 MHz)

// Configuración de bits de configuración del microcontrolador
#pragma config FOSC = INTOSCIO   // Oscilador interno, función I/O en RA6 y RA7
#pragma config WDTE = OFF        // Watchdog Timer deshabilitado
#pragma config PWRTE = OFF       // Power-up Timer deshabilitado
#pragma config MCLRE = OFF       // MCLR pin configurado como I/O
#pragma config BOREN = OFF       // Brown-out Reset deshabilitado
#pragma config LVP = OFF         // Programación en Bajo Voltaje deshabilitada
#pragma config CPD = OFF         // Protección de código en EEPROM deshabilitada
#pragma config WRT = OFF         // Protección de escritura en memoria Flash deshabilitada
#pragma config CCPMX = RB3       // CCP1 asignado al pin RB3
#pragma config CP = OFF          // Protección de código en memoria Flash deshabilitada

#define PULSOS_POR_REVOLUCION 1  // Un pulso por revolución

// Declaración de funciones
void PWM_Init(void);
void Timer0_Init(void);

// Variables globales
unsigned int pulses = 0;       // Pulsos contados en RA4
unsigned int frequency = 0;    // Frecuencia calculada (en Hz)
unsigned int rpm = 0;          // Revoluciones por minuto (RPM)

// Rutina de interrupción
void __interrupt() ISR(void) {
    if (INTCONbits.RBIF) { // Verificar interrupt-on-change en PORTB
        if (PORTBbits.RB5 == 1 && CCPR1L < 250) { // Botón incrementar (activo bajo)
            CCPR1L += 10;
            I2C_Master_Send(CCPR1L);
        }
        if (PORTBbits.RB6 == 1 && CCPR1L > 0) { // Botón decrementar (activo bajo)
            CCPR1L -= 10;
            I2C_Master_Send(CCPR1L);
        }
        INTCONbits.RBIF = 0; // Limpiar bandera de interrupción
        __delay_ms(50);    // Debounce de los botones
    }
}

void main(void) {
    // Configuración de los pines
    ADCON1bits.PCFG = 7;
    TRISAbits.TRISA4 = 1;  // RA4 como entrada (señal de pulsos)
    TRISBbits.TRISB3 = 0;  // RB3 como salida (PWM)
    TRISBbits.TRISB5 = 1;  // RB5 como entrada (botón incrementar)
    TRISBbits.TRISB6 = 1;  // RB6 como entrada (botón decrementar)

    // Habilitar resistencias pull-up en PORTB
    OPTION_REGbits.nRBPU = 0; // Habilitar pull-ups

    // Configuración de interrupciones
    INTCONbits.RBIE = 1;  // Habilitar interrupción por cambio en PORTB
    INTCONbits.RBIF = 0;  // Limpiar bandera de interrupción
    INTCONbits.GIE = 1;   // Habilitar interrupciones globales

    // Inicialización del PWM, Timer0 e I2C
    I2C_Master_Init();
    PWM_Init();
    Timer0_Init();

    CCPR1L = 100;
    I2C_Master_Send(CCPR1L);
    while (1) {
    }
}

// Inicialización del módulo PWM
void PWM_Init(void) {
    T2CON = 0b00000110;
    CCP1CON = 0b00111111;
    PR2 = 255;
    CCPR1L = 100;
}

// Inicialización de Timer0 para contar pulsos en RA4
void Timer0_Init(void) {
    OPTION_REGbits.T0CS = 1; // Fuente de reloj de Timer0 es RA4/T0CKI
    OPTION_REGbits.T0SE = 0; // Incrementa en flanco de subida
    OPTION_REGbits.PSA = 1;  // No usar prescaler para Timer0
    TMR0 = 0;                // Reiniciar Timer0
}