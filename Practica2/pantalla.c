#include <xc.h>
#include <stdio.h>
#define _XTAL_FREQ 4000000
#define D PORTAbits.RA3
#define C PORTAbits.RA2
#define B PORTAbits.RA1
#define A PORTAbits.RA0

#define DG1 PORTAbits.RA4
#define DG2 PORTAbits.RA6
#define DG3 PORTAbits.RA7

unsigned char getBinarios(int digito) {
    // Retorna directamente el valor del dígito para el decodificador 74LS47
    return (unsigned char)(digito & 0x0F);
}

void actualizarPantalla(int valor) {
    int digitos[3] = {0};  // Inicializamos con ceros

    // Obtener los dígitos (centenas, decenas, unidades)
    /*digitos[0] = (valor / 100) % 10;   // Centenas
    digitos[1] = (valor / 10) % 10;    // Decenas
    digitos[2] = valor % 10;           // Unidades*/
    /*while (valor >= 1000) {
        valor -= 1000;
        digitos[0]++;
    }*/
    while (valor >= 100) {
        valor -= 100;
        digitos[0]++;
    }
    while (valor >= 10) {
        valor -= 10;
        digitos[1]++;
    }
    digitos[2] = valor;
    for (int i = 0; i < 3; i++) {
        unsigned char binarios = getBinarios(digitos[i]);

        // Asignar los bits A, B, C, D para el decodificador 74LS47
        A = (binarios & 0b0001) ? 1 : 0;
        B = (binarios & 0b0010) ? 1 : 0;
        C = (binarios & 0b0100) ? 1 : 0;
        D = (binarios & 0b1000) ? 1 : 0;
        // Seleccionar el display adecuado
        switch (i) {
            case 0:
                DG3 = 1; DG2 = 0; DG1 = 0;
                break;
            case 1:
                DG3 = 0; DG2 = 1; DG1 = 0;
                break;
            case 2:
                DG3 = 0; DG2 = 0; DG1 = 1;
                break;
        }

        // Mantener encendido cada dígito durante un retardo corto
        __delay_ms(5);

        // Apagar todos los dígitos antes de actualizar el siguiente
        DG1 = 0;
        DG2 = 0;
        DG3 = 0;
    }
}
