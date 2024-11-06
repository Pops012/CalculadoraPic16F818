#include <xc.h>
#include "I2C_master.h"

#define _XTAL_FREQ 4000000

void main(void) {
    unsigned char status,data;

    I2C_Master_Init(); //Se debe inicializar siempre

    while(1) {
        //usar I2C_Master_Send cada que se quiera comunicar un dato
        status = I2C_Master_Send(data);
        if (status != 0) {
            //llevar a cabo acciones en caso de error
            //1 y 2 son codigos de error
        }  
    }
}