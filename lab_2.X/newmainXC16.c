/*=================
 * By: Miguel Maranha 2012138309
 * With: Diogo Gomes 
 *Pin Mode:
 * +5V(Pin28)
 *  |
 *  Resistor(1k)
 *  |--o Vout(Pin26)
 *  |
 *  Termistor
 *  |
 *  Gnd
 * =================*/
#include <xc.h>
#include "config.h"
#include <libpic30.h>
#include <stdio.h>
#include <math.h>

//======== Functions Initialize ========//
inline void ConfigADC(void); // Configures Analog To Digital
inline void ConfigUART1(unsigned long int brg); // Serial Link Via Uart
inline void ConfigTMR1(void);

unsigned int readADC(unsigned int ch); // Read ADC values input[PIN]

int i;
unsigned int cnt, acq;
double v;
double Rt;
double temp;


int main(void){
    // Configurar oscillador para 32MHz
    CLKDIVbits.DOZE = 0; // 1:1
    CLKDIVbits.RCDIV = 0; // 8 MHz
    //pins digitais
    ANSA = 0;
    ANSB = ~0x0484;         // RX1, TX1, RB10 digitais, restantes analogicos;
    TRISB= 0b1111111101111111;
    ConfigUART1(19200);
    ConfigADC();
    ConfigTMR1();
    
    while(1){
        //==== (1)- Medir Ruido ADC ====//
        /*
        acq=readADC(9); // L� Val e d� Map para Digital (Canal 9 Pin)
        printf("\n%d,%u",i,acq); // Prints val count and ADC bytes (CSV)
        i++;
        */
        
        
        _T1IE = 1;  //Activa interrup��o do TIMER1
        //__delay_ms(500);
    }
}

//======== Functions Code ========//

//==== Timer Rotine 1s ====//
void _ISRFAST _T1Interrupt(void) {
    // Timer1 interrupt service routine
    if(cnt==10){ // 10 Timer Counts (10*1s = 10s)
        acq=readADC(9); // L� Val e d� Map para Digital (Canal 9 Pin)
        // Valor Tens�o no Ponto --> 5volts/(2^12bits - 1)
        v=acq*(0.001221001221);
        // Calc Rt com Divisor de Tens�o
        Rt = (1000*v)/(5-v);
        //temp = 8068.9*exp(-0.026*Rt);
        temp = 280.2*exp(-50E-5*Rt);
        printf("\n\n [%d] ADC=%d(bytes) Temp=%.2f(C�) Rt=%.2f(Ohms) v=%.2f(Volts)",i,acq,temp,Rt,v);
        i++;
        cnt = 0; // Reset Clock Count
    }
    cnt++; // Count Number o Interruptions (Secs)
    _T1IF = 0; // Stop Interrupt
}

//==== Read Value From Pin Convert to Digital 12bits ====//
unsigned int readADC(unsigned int ch){
    AD1CHS = ch;            // Select analog input channel
    AD1CON1bits.SAMP = 1;   // start sampling, then go to conversion

    while (!AD1CON1bits.DONE); // conversion done?
    return(ADC1BUF0);       // yes then get ADC value
}

//==== Timer Config 1s ====//
inline void ConfigTMR1(void){
    T1CON = 0x8030; // activa TMR1, prescale 1:256, relogio interno
    TMR1 = 0; // inicializa o registo do timer 1
    PR1 = 62500 - 1;   // 1.000 ms Tick (625000 not working?)
    _T1IF = 0;
    _T1IE = 1; //Activa Interrupts
}

//==== Serial Link Comms ====//
inline void ConfigUART1(unsigned long int brg){
    U1BRG = (FCY / (16 * brg)) -1; // 19200 bps @ 16 MHz -> BRG = 51
    U1MODE= 0x8000; // UARTEN
    U1STA = 0x0400; // enable transmission
}// UART1Init

//==== Analog to Digital Config ====//
inline void ConfigADC(void){
    AD1CON1 = 0x0470; // 12-bit A/D operation (MODE12=1)
                      // SSRC<3:0> = 0111 Internal counter ends sampling
                      // and starts conversion (auto-convert)
    AD1CON2 = 0x0000; // Configure A/D voltage reference and buffer fill modes.
                      // Vr+ and Vr- from AVdd and AVss(PVCFG<1:0>=00, NVCFG=0),
                      // Inputs are not scanned,
                      // Buffer in FIFO mode (BUFM=0)
    AD1CON3 = 0x1003; // Sample time = 16Tad, Tad = 4Tcy (= 250ns)
    AD1CSSL = 0; // No inputs are scanned.
    _AD1IF = 0; // Clear A/D conversion interrupt.
    _AD1IE = 0; // Disable A/D conversion interrupt
    AD1CON1bits.ADON = 1; // Turn on A/D
}
