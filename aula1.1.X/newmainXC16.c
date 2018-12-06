/*
 * File:   newmainXC16.c
 * Author: maran
 *
 * Created on September 20, 2018, 9:58 AM
 */


#include "xc.h"

int main(void) {
    ANSA = ~3; //Port A toda analogica (menos os 2 bits menos sig)
    TRISAbits.TRISA0 = 0; //A0 set output
    while(1){
        LATAbits.LATA0 = ~PORTAbits.RA1 ; //Lê Bit entrada(Registo) Inverte e coloca na saida
        
    }
    return 0;
}
