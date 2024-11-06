#include <xc.h>
#include "I2C_slave.h"

#define _XTAL_FREQ 4000000 // Frecuencia del oscilador

void __interrupt() I2C_Slave_Receive(void) {
    if (PIR1bits.SSPIF) {
        PORTAbits.RA1 ^= 1; // Alternar LED en RA1 para indicar entrada a la interrupción

        if (!SSPSTATbits.R_nW) { // Maestro escribe
            if (!SSPSTATbits.D_nA) { // Último byte fue una dirección
                volatile unsigned char dummy = SSPBUF; // Lectura dummy
                SSPCONbits.CKP = 1; // Liberar reloj SCL
            } else { // Último byte fue dato
                unsigned char data = SSPBUF; // Leer dato recibido
                PORTAbits.RA0 ^= 1; // Alternar estado del LED en RA0
                SSPCONbits.CKP = 1; // Liberar reloj SCL
            }
        }
        PIR1bits.SSPIF = 0; // Limpiar bandera de interrupción
    }
}

void main(void) {
    TRISAbits.TRISA0 = 0; // RA0 como salida (LED para recepción de datos)
    PORTAbits.RA0 = 0;    // Apagar LED
    TRISAbits.TRISA1 = 0; // RA1 como salida (LED para entrada a interrupción)
    PORTAbits.RA1 = 0;    // Apagar LED

    ADCON1 = 0x06; // Configurar entradas analógicas como digitales

    I2C_Slave_Init(0x50); // Inicializar esclavo con dirección 0x50

    while(1) {
        // Esperar interrupciones
    }
}