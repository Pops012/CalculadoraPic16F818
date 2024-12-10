#include <xc.h>
#include "adc.h"
#define _XTAL_FREQ 4000000

#define BUZZER PORTAbits.RA7

void actualizarPantalla(unsigned int display_value) {
    int d1 = (display_value / 1000) % 10;
    int d2 = (display_value / 100) % 10;
    int d3 = (display_value / 10) % 10;
    int d4 = display_value % 10;

    // Display d1
    PORTB = (d1 & 0x0F) | 0x10; // RB4 indicates d1
    BUZZER = 1; // Decimal point after d1
    __delay_ms(5);
    BUZZER = 0;

    // Display d2
    PORTB = (d2 & 0x0F) | 0x20; // RB5 indicates d2
    __delay_ms(5);

    // Display d3
    PORTB = (d3 & 0x0F) | 0x40; // RB6 indicates d3
    __delay_ms(5);

    // Display d4
    PORTB = (d4 & 0x0F) | 0x80; // RB7 indicates d4
    __delay_ms(5);
}

void main() {
    TRISB = 0xF0;
    TRISA = 0x01; // Set RA0/AN0 as input
    adc_init();
    
    while (1) {
        unsigned int adc_value = adc_read();
        unsigned long temp = (unsigned long)adc_value * 50000UL; // Multiply by 50000 to preserve precision
        unsigned int display_value = temp / 1023; // Now display_value ranges from 0 to 50000 (0.000 to 5.000 V)
        
        actualizarPantalla(display_value); // Pass display value directly to actualizarPantalla
        __delay_ms(500);
    }
}