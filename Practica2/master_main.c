#include <xc.h>
#include "I2C_master.h"

#define _XTAL_FREQ 4000000 // Frecuencia del oscilador

void main(void) {
    unsigned char data = 0x00;
    unsigned char ack;
    I2C_Master_Init(); // Inicializar I2C Master

    while(1) {
        I2C_Master_Start();                // Iniciar comunicación I2C

        // Enviar dirección del esclavo (escritura)
        ack = I2C_Master_Write(0x50 << 1); // Dirección 0x50 desplazada 1 bit a la izquierda
        if (ack != 0) {
            // Si no hay ACK, manejar el error
            I2C_Master_Stop();
            continue; // Intentar de nuevo en el siguiente ciclo
        }

        // Enviar byte de datos
        ack = I2C_Master_Write(data++);
        if (ack != 0) {
            // Si no hay ACK, manejar el error
            I2C_Master_Stop();
            continue; // Intentar de nuevo en el siguiente ciclo
        }

        I2C_Master_Stop();                 // Terminar comunicación I2C
        __delay_ms(1000);                  // Esperar 1 segundo
    }
}