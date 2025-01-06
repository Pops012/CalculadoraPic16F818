
// PIC16F818 Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTRC oscillator; port I/O function on both RA6/OSC2/CLKO pin and RA7/OSC1/CLKI pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is digital I/O, MCLR internally tied to VDD)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (RB3/PGM pin has digital I/O function, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off)
#pragma config CCPMX = RB2      // CCP1 Pin Selection bit (CCP1 function on RB2)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <pic16f818.h>
#define _XTAL_FREQ 500000
#define screen_delay 20

void actualizarPantalla(unsigned int display_value) {
    int digits[4];
    int i = 0;
    digits[0] = display_value / 1000;            // Thousands digit
    digits[1] = (display_value / 100) % 10;      // Hundreds digit
    digits[2] = (display_value / 10) % 10;       // Tens digit
    digits[3] = display_value % 10;              // Units digit

    for (i = 0; i < 4; i++) {
        PORTBbits.RB0 = (digits[i] >> 0) & 0x01;
        PORTBbits.RB1 = (digits[i] >> 1) & 0x01;
        PORTBbits.RB2 = (digits[i] >> 2) & 0x01;
        PORTBbits.RB3 = (digits[i] >> 3) & 0x01;

        // Set digit select bits: turn on only one of RB4 to RB7
        PORTBbits.RB4 = (i == 3);
        PORTBbits.RB5 = (i == 2);
        PORTBbits.RB6 = (i == 1);
        PORTBbits.RB7 = (i == 0);

        // Set decimal point only when printing the integer part
        PORTAbits.RA2 = (i != 0);

        __delay_ms(screen_delay);
    }
}

void main() {
    unsigned int adc_value = 0;
    unsigned int voltage_mv = 0;

    TRISB = 0x00;    // Set all PORTB pins as output 
    TRISAbits.TRISA2 = 0;   // Set RA2 as output    
    TRISAbits.TRISA0 = 1;   // Set RA0/AN0 as input

    // Wait for ADC to stabilize
    __delay_ms(10);

    while (1) {
        // Correct ADC Configuration
        ADCON0 = 0b01000001; // FOSC/8, channel AN0, ADC on
        ADCON1 = 0b10001110; // Right justified, FOSC/8, AN0=analog, others digital
        // Start ADC conversion
        ADCON0 |= 0b00000100; // GO=1
        while (ADCON0 & 0b00000100) {
            __delay_ms(10);
        }  // Wait for conversion to finish

        // Read ADC result (10-bit value)
        adc_value = (unsigned int)( (ADRESH << 8) | ADRESL );

        // Convert ADC value to millivolts (0-5000 mV)
        voltage_mv = ((unsigned int)adc_value * 5000) / 1023;

        // Display voltage on screen
        actualizarPantalla(voltage_mv);

        // Add a small delay before the next reading
        __delay_ms(10);
    }
}