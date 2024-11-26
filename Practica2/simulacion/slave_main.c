// slave_main.c
#include <xc.h>
#include "I2C_slave.h"
#define _XTAL_FREQ 4000000

volatile unsigned char received_data = 0;
volatile unsigned char data_ready = 0;

// Interrupt Service Routine
void __interrupt() I2C_Slave_Receive(void) {
    if (PIR1bits.SSPIF) {
        if (!SSPSTATbits.R_nW) { // Master write
            if (!SSPSTATbits.D_nA) { // Last byte was address
                volatile unsigned char dummy = SSPBUF; // Dummy read
                SSPCONbits.CKP = 1; // Release SCL
            } else { // Last byte was data
                received_data = SSPBUF; // Store received data
                data_ready = 1;         // Indicate data is ready
                SSPCONbits.CKP = 1;     // Release SCL
            }
        }
        PIR1bits.SSPIF = 0; // Clear interrupt flag
    }
}

void main(void) {
    // Configure LEDs
    TRISAbits.TRISA0 = 0; // RA0 as output (LED for data reception)
    TRISAbits.TRISA1 = 0; // RA1 as output (Debug LED)
    // Configure additional pins for binary display (e.g., RA2 to RA5)
    TRISAbits.TRISA2 = 0; // RA2
    TRISAbits.TRISA3 = 0; // RA3
    TRISAbits.TRISA4 = 0; // RA4

    // Initialize LEDs and binary display pins
    PORTAbits.RA0 = 0;    // Turn off LED
    PORTAbits.RA1 = 0;    // Turn off Debug LED
    PORTAbits.RA2 = 0;
    PORTAbits.RA3 = 0;
    PORTAbits.RA4 = 0;

    ADCON1 = 0x06;
    I2C_Slave_Init(0x50);
    unsigned char data = 0x00;

    while(1) {
        if (data_ready) {


            PORTAbits.RA4 ^= 1; // Toggle LED

            data = received_data;
            data_ready = 0;
            // Additional data processing can be done here
        }
        // Display lower 4 bits of received_data on RA2 to RA5
            PORTAbits.RA0 = data & 0x01;
            PORTAbits.RA1 = (data >> 1) & 0x01;
            PORTAbits.RA2 = (data >> 2) & 0x01;
            PORTAbits.RA3 = (data >> 3) & 0x01;

    }
}