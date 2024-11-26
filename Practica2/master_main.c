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


//***************************************************************************************************************************************
// Declaración de funciones
void PWM_Init(void);
void Timer0_Init(void);
//***************************************************************************************************************************************

void main(void) {

//***************************************************************************************************************************************
    unsigned int pulses = 0;       // Pulsos contados en RA4
    unsigned int frequency = 0;  // Frecuencia calculada (en Hz)
    unsigned int rpm = 0;        // Revoluciones por minuto (RPM)
//***************************************************************************************************************************************
 
//***************************************************************************************************************************************    
    // Configuración de los pines
    ADCON1bits.PCFG=7;
    TRISAbits.TRISA0 = 1; // RA0 como entrada (Botón incrementar)
    TRISAbits.TRISA1 = 1; // RA1 como entrada (Botón decrementar)
    TRISAbits.TRISA4 = 1; // RA4 como entrada (Señal de pulsos cuadrados)
    TRISBbits.TRISB3 = 0; // RB3 como salida (PWM)
//***************************************************************************************************************************************

//***************************************************************************************************************************************    
    // Inicialización del PWM, Timer0 e I2C
    I2C_Master_Init();
    PWM_Init();
    Timer0_Init();
//***************************************************************************************************************************************

    while (1) {
        // Medir la frecuencia de la señal en RA4
        __delay_ms(100);  // Tiempo de medición ajustado a 100 ms
        //GIE = 0;          // Deshabilitar interrupciones
        //pulses = TMR0;    // Leer el número de pulsos
        //TMR0 = 0;         // Reiniciar Timer0
        //GIE = 1;          // Habilitar interrupciones
        // Calcular la frecuencia en Hz
        //frequency = pulses * 10; // Frecuencia en Hz (100 ms -> *10 para escalar)
        // Calcular las RPM
        //rpm = frequency * 60; // RPM = Frecuencia * 60 (un pulso por revolución)
        //rpm = (CCPR1L*255)/255;
//***************************************************************************************************************************************
        // Incrementar o decrementar el duty_cycle con botones
        if (PORTAbits.RA0 == 1 && CCPR1L<250) { // Botón incrementar
            CCPR1L+=10;
            __delay_ms(50);
        }
        if (PORTAbits.RA1 == 1 && CCPR1L>0) { // Botón decrementar
            // Retardo para debounce
            CCPR1L-=10;
            __delay_ms(50);
        }
//***************************************************************************************************************************************
        // Enviar las RPM como un solo valor por I2C
        I2C_Master_Send((unsigned char)CCPR1L);
    }
}

// Inicialización del módulo PWM
void PWM_Init(void) {
    T2CON=0b00000110;
    CCP1CON=0b00111111;
    PR2=255;
    CCPR1L = 100;
}

// Inicialización de Timer0 para contar pulsos en RA4
void Timer0_Init(void) {
    OPTION_REGbits.T0CS = 1; // Fuente de reloj de Timer0 es RA4/T0CKI
    OPTION_REGbits.T0SE = 0; // Incrementa en flanco de subida
    OPTION_REGbits.PSA = 1;  // No usar prescaler para Timer0
    TMR0 = 0;                // Reiniciar Timer0
}
