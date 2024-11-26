#include <xc.h>
#include "pantalla.h"
#include "I2C_slave.h"
#define _XTAL_FREQ 4000000

#pragma config FOSC = INTOSCIO
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config MCLRE = OFF
#pragma config BOREN = OFF
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config CP = OFF

volatile unsigned char received_data = 0;
volatile unsigned char data_ready = 0;

void __interrupt() I2C_Slave_Receive(void) {
    if (PIR1bits.SSPIF) {
        if (!SSPSTATbits.R_nW) {
            if (!SSPSTATbits.D_nA) {
                volatile unsigned char dummy = SSPBUF;
                SSPCONbits.CKP = 1;
            } else {
                received_data = SSPBUF;
                data_ready = 1;
                SSPCONbits.CKP = 1;
            }
        }
        PIR1bits.SSPIF = 0;
    }
}

void main(void) {
    ADCON1 = 0x06;
    TRISA = 0x00;
    PORTA = 0x00;

    I2C_Slave_Init(0x50);

    unsigned char last_data = 0;

    while (1) {
        if (data_ready) {
            last_data = received_data; // Actualizar último dato recibido
            data_ready = 0;            // Reiniciar indicador
        }

        actualizarPantalla((int)last_data); // Mostrar siempre el último valor
    }
}
