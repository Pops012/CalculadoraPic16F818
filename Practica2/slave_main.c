// slave_main.c
#include <xc.h>
#include "I2C_slave.h"
#define _XTAL_FREQ 4000000

volatile unsigned char received_data = 0;
volatile unsigned char data_ready = 0;

// interrupcion para la recepcion de datos por I2C
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
    //siempre se debe inicializar el esclavo
    I2C_Slave_Init(0x50);
    unsigned char data = 0x00;

    while(1) {
        if (data_ready) {
            data = received_data;
            data_ready = 0;
            // la variable data se actualiza con los datos mas recientes
        }
        //en esta area se puede trabajar con la variable data
        //la cual contiene los datos recibidos mas recientemente 

    }
}