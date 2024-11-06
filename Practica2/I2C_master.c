// I2C_master.c
#include "I2C_master.h"

#define _XTAL_FREQ 4000000 // Reemplaza este valor por tu frecuencia de oscilador
#define I2C_DELAY __delay_us(10) // Ajusta el retardo según sea necesario

void I2C_Master_Init(void) {
    ADCON1 = 0x06;          // Configurar pines como digitales
    I2C_SCL = 0;            // Asegurar que PORT está en 0
    I2C_SDA = 0;
    I2C_SCL_TRIS = 1;       // SCL como entrada (línea en alto por pull-up)
    I2C_SDA_TRIS = 1;       // SDA como entrada (línea en alto por pull-up)
}

void I2C_Master_Start(void) {
    I2C_SDA_TRIS = 1;       // SDA en alto
    I2C_SCL_TRIS = 1;       // SCL en alto
    I2C_DELAY;
    I2C_SDA_TRIS = 0;       // SDA en bajo
    I2C_SDA = 0;
    I2C_DELAY;
    I2C_SCL_TRIS = 0;       // SCL en bajo
    I2C_SCL = 0;
    I2C_DELAY;
}

void I2C_Master_Stop(void) {
    I2C_SDA_TRIS = 0;       // SDA en bajo
    I2C_SDA = 0;
    I2C_DELAY;
    I2C_SCL_TRIS = 1;       // SCL en alto
    I2C_DELAY;
    I2C_SDA_TRIS = 1;       // SDA en alto
    I2C_DELAY;
}

unsigned char I2C_Master_Write(unsigned char data) {
    unsigned char ack;

    for(unsigned char i = 0; i < 8; i++) {
        if (data & 0x80) {
            I2C_SDA_TRIS = 1; // Liberar SDA (alto)
        } else {
            I2C_SDA_TRIS = 0; // Forzar SDA a bajo
            I2C_SDA = 0;
        }
        I2C_DELAY;
        I2C_SCL_TRIS = 1;    // SCL en alto
        I2C_DELAY;
        I2C_SCL_TRIS = 0;    // SCL en bajo
        I2C_SCL = 0;
        I2C_DELAY;
        data <<= 1;
    }

    // Lectura del ACK
    I2C_SDA_TRIS = 1;       // Liberar SDA para ACK
    I2C_DELAY;
    I2C_SCL_TRIS = 1;       // SCL en alto
    I2C_DELAY;
    ack = I2C_SDA;          // Leer ACK del esclavo
    I2C_SCL_TRIS = 0;       // SCL en bajo
    I2C_SCL = 0;
    I2C_DELAY;

    return ack;
}