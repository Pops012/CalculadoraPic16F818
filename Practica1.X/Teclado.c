#include <xc.h>
#include <pic16f818.h>
#include "Teclado.h"

#define _XTAL_FREQ 4000000


#define ROW1 PORTBbits.RB0
#define ROW2 PORTBbits.RB1
#define ROW3 PORTBbits.RB2
#define ROW4 PORTBbits.RB3
#define COL1 PORTBbits.RB4
#define COL2 PORTBbits.RB5
#define COL3 PORTBbits.RB6
#define COL4 PORTBbits.RB7

char lector(){
    TRISB = 0xF0;
    char tecla = 0;
    ROW1= 1;
    ROW2= 0;
    ROW3= 0;
    ROW4= 0;
    if(!COL1) tecla = '1';
    if(!COL2) tecla = '2';
    if(!COL3) tecla = '3';
    if(!COL4) tecla = 'A';
    ROW1= 0;
    ROW2= 1;
    ROW3= 0;
    ROW4= 0;
    if(!COL1) tecla = '4';
    if(!COL2) tecla = '5';
    if(!COL3) tecla = '6';
    if(!COL4) tecla = 'B';
    ROW1= 0;
    ROW2= 0;
    ROW3= 1;
    ROW4= 0;
    if(!COL1) tecla = '7';
    if(!COL2) tecla = '8';
    if(!COL3) tecla = '9';
    if(!COL4) tecla = 'C';
    ROW1= 0;
    ROW2= 0;
    ROW3= 0;
    ROW4= 1;
    if(!COL1) tecla = '*';
    if(!COL2) tecla = '0';
    if(!COL3) tecla = '#';
    if(!COL4) tecla = 'D';
    if (tecla!=0){
        __delay_ms(100); 
    }
    return tecla;
}

