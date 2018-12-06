#include <xc.h>
#include "config.h"
#include <libpic30.h>
#include <stdio.h>

//==== Serial Link Comms ====//
inline void ConfigUART1(unsigned long int brg){
    U1BRG = (FCY / (16 * brg)) -1; // 19200 bps @ 16 MHz -> BRG = 51
    U1MODE= 0x8000; // UARTEN
    U1STA = 0x0400; // enable transmission
}// UART1Init

inline void ConfigADC(void)
{
/*
 * Configura o ADC para funcionar comandado por SW
 * Consultar: Section 51 - 12-Bit AD Converter with Threshold Detect
 * e Ch 19 de PIC24FV16KM204 FAMILY Data Sheet
 */

    AD1CON1 = 0x0470;   // 12-bit A/D operation (MODE12=1)
                // SSRC<3:0> = 0111 Internal counter ends sampling
                // and starts conversion (auto-convert)
    AD1CON2 = 0x0000;   // Configure A/D voltage reference and buffer fill modes.
                // Vr+ and Vr- from AVdd and AVss(PVCFG<1:0>=00, NVCFG=0),
                // Inputs are not scanned,
                // Buffer in FIFO mode (BUFM=0)
    AD1CON3 = 0x1003;   // Sample time = 16Tad, Tad = 4Tcy (= 250ns)

    AD1CSSL = 0;        // No inputs are scanned.

    _AD1IF = 0; // Clear A/D conversion interrupt.
    _AD1IE = 0; // Disable A/D conversion interrupt
    AD1CON1bits.ADON = 1; // Turn on A/D
}

unsigned int readADC(unsigned int ch){
    AD1CHS = ch;            // Select analog input channel
    AD1CON1bits.SAMP = 1;   // start sampling, then go to conversion

    while (!AD1CON1bits.DONE); // conversion done?
    return(ADC1BUF0);       // yes then get ADC value
}

void wait10msTMR1(unsigned int ms10){
T1CON = 0x00; // stop TMR1 e faz reset ao registo de controlo
TMR1 = 1; // inicializa o registo do timer 1
ms10 *= 625; // 625 timer ticks = 10 ms
T1CON = 0x8030; // activa TMR1, prescale 1:256, relogio interno
while(TMR1 < ms10); // espera ms10 unidades de 10ms
}

int main(void){
    //configurar oscillador para 32MHz
    CLKDIVbits.DOZE = 0; // 1:1
    CLKDIVbits.RCDIV = 0; // 8 MHz
    //pins digitais
    ANSA = 0;
    ANSB = ~0x0484;         // RX1, TX1, RB10 digitais, restantes analogicos;
    TRISB= 0b1111111101111111;
    ConfigUART1(19200);
    ConfigADC();
    // BAD CODE
    int i;
    unsigned int acq;
    while(1){
        wait10msTMR1(10);
        acq=readADC(9);     // Adquire canal 9
        printf("\n Arroz doce Count [%d]  Val %u mV",i,acq);
        i++;
        //__delay_ms(500);
    }
}