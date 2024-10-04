#include <xc.h>
#include <pic16f818.h>
#include <stdio.h>

#define A PORTAbits.RA3
#define B PORTAbits.RA2
#define C PORTAbits.RA1
#define D PORTAbits.RA0

#define DG1 PORTAbits.RA6
#define DG2 PORTAbits.RA5
#define DG3 PORTAbits.RA4

void actualizarPantalla(int millar, int centena, int decena, int unidad){
    
    unsigned char binarios;
    unsigned char arregloMascaras[4] = {0b1000, 0b0100, 0b0010, 0b0001};
    int digitos[4] = {millar, centena, decena, unidad};
    
    for(int i = 0; i < 4; i++ ){
        
        binarios = getBinarios(digitos[i]);
            
        A = (binarios & arregloMascaras[0]) ? 1 : 0;
        B = (binarios & arregloMascaras[1]) ? 1 : 0;
        C = (binarios & arregloMascaras[2]) ? 1 : 0;
        D = (binarios & arregloMascaras[3]) ? 1 : 0;
              
        switch(i){
            case 0: 
                DG1 = 1; DG2 = 0; DG3 = 0;
                break;
            case 1: 
                DG1 = 0; DG2 = 1; DG3 = 0;
                break;
            case 2: 
                DG1 = 0; DG2 = 0; DG3 = 1;
                break;
            case 3:
                DG1 = 0; DG2 = 0; DG3 = 0;
                break;
        }
        
        __delay_ms(20);
    }
}

unsigned char getBinarios(int digito) {
    return (unsigned char)(digito & 0x0F);
}
