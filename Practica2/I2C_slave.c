// I2C_slave.c
#include "I2C_slave.h"

#define _XTAL_FREQ 4000000 // Frecuencia del oscilador

void I2C_Slave_Init(unsigned char address) {
    SSPADD = address << 1; // Dirección del esclavo
    SSPCON = 0b00110110; // Modo esclavo 7 bits, habilitar MSSP
    SSPSTAT = 0x80; // Slew rate control deshabilitado
    TRISBbits.TRISB1 = 1; // SDA como entrada
    TRISBbits.TRISB4 = 1; // SCL como entrada
    PIR1bits.SSPIF = 0;   // Limpiar bandera de interrupción
    PIE1bits.SSPIE = 1;   // Habilitar interrupción MSSP
    INTCONbits.PEIE = 1;  // Habilitar interrupciones periféricas
    INTCONbits.GIE = 1;   // Habilitar interrupciones globales
}

void __interrupt() I2C_Slave_Receive(void) {
    if (PIR1bits.SSPIF) {
        if (!SSPSTATbits.R_nW) {
            if (!SSPSTATbits.D_nA) {
                volatile unsigned char dummy = SSPBUF;
                SSPCONbits.CKP = 1;
            } else {
                unsigned char received_byte = SSPBUF;
                if (byte_count == 0) {
                    received_int = received_byte << 8;
                    byte_count++;
                } else {
                    received_int |= received_byte;
                    byte_count = 0;
                    data_ready = 1;
                }
                SSPCONbits.CKP = 1;
            }
        }
        PIR1bits.SSPIF = 0;
    }
}