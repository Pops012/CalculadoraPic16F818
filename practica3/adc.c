#include <xc.h>
#define _XTAL_FREQ 4000000

void adc_init() {
    ADCON0 = 0x01; // ADC enabled, channel 0 (AN0)
    ADCON1 = 0x06; // Configure Vref+ (RA1) as external voltage reference, AN0 as analog input
}

unsigned int adc_read() {
    ADCON0bits.GO = 1; // Start conversion
    while (ADCON0bits.GO_nDONE); // Wait for conversion to complete
    return (unsigned int)((ADRESH << 8) + ADRESL); // Return result
}