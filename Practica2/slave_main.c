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

void main(void) {
    ADCON1 = 0x06;
    TRISA = 0x00;
    PORTA = 0x00;

    I2C_Slave_Init(0x50);

    int last_data = 0;

    while (1) {
        if (data_ready) {
            last_data = received_int;
            data_ready = 0;
        }

        actualizarPantalla(last_data);
    }
}
