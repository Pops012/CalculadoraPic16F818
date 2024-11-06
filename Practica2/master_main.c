#include <xc.h>
#include "I2C_master.h"

#define _XTAL_FREQ 4000000

void main(void) {
    unsigned char test_data[] = {0x00, 0xFF, 0xAA, 0x55};
    unsigned char index = 0;
    unsigned char status;
    TRISAbits.TRISA6 = 0; // RA6 as output (LED for data reception)


    I2C_Master_Init(); // Inicializar I2C Master

    while(1) {
        status = I2C_Master_Send(test_data[index]);
        if (status != 0) {
            // Manejar errores
            // Por ejemplo, encender LED de error
            PORTAbits.RA6 = 1; // Asumiendo RA6 como LED de error
        } else {
            PORTAbits.RA6 = 0; // Apagar LED de error
        }
        index = (index + 1) % 4; // Ciclar a través de los patrones de prueba
        __delay_ms(1000);   
    }
}