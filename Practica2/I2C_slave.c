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