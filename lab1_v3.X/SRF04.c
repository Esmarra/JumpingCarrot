/*
 * RA4 (pin 12) - Trigger
 * RA2 (pin 9) - Echo
 * Utiliza o Timer 1
 * 
 */


#include <xc.h>
#include "config.h"
#include <stdio.h>

// Tempo de trigger (12us=3*4us)
#define T12 (12/4)
// Tempo de ciclo (200ms)
#define T200 (200000UL/4)
// distancia por timer tick (um)
#define Kd 680UL

/*
 * Prototipos de funções
 */
inline void ConfigCLK(void);
inline void ConfigIO(void);
inline void UART1Init(unsigned long int brg);
inline void ConfigTMR1(void);

int main(void) {

    ConfigCLK();
    ConfigIO();
    UART1Init(19200);

    ConfigTMR1();

    printf("\rInterface com Sonar SRF04");

    unsigned int tEcho;
    unsigned int d;
    while(1)
    {
        LATAbits.LATA4 = 1; // Trigger
        TMR1 = 0;
        while(TMR1<T12);     // 12 us
        LATAbits.LATA0 = 0;
        
        while(PORTAbits.RA2==0);    // espera echo 0->1
        TMR1=0;
        while(PORTAbits.RA2==1);
        tEcho = TMR1;
        
        d = Kd*tEcho/1000;
        if(d<4000){
            printf("\r%u mm", d);
        }
        
        //TMR1=0;
        while(TMR1<T200); // Espera 200ms
                    
    }
    return (1);
}

inline void ConfigCLK(void)
{
    //configurar oscilador para 32MHz
    CLKDIVbits.DOZE = 0;    // 1:1
    CLKDIVbits.RCDIV = 0;   // 8 MHz
}

inline void ConfigIO(void)
{
    ANSA = 0;               // Porto A digital
    TRISAbits.TRISA2 = 1;   // entrada (Echo)
    TRISAbits.TRISA4 = 0;   // saida (Trigger)
    ANSB = 0;               // U1RX e U1TX digitais
    TRISBbits.TRISB1 = 1;   // entrada (U1RX)
    TRISBbits.TRISB0 = 0;   // saida (U1TX)
}

inline void UART1Init(unsigned long int baud){
    U1BRG =  (FCY / (16 * baud)) -1; //BAUDRATEREG1;
    U1MODE = 0;
    U1MODEbits.BRGH = 0;
    U1STA = 0;
    U1MODEbits.UARTEN = 1;
    U1STAbits.UTXEN = 1;
    U1STAbits.URXISEL = 2;
    U1MODEbits.UEN = 3;
    //IEC0bits.U1TXIE = 1;        // enable U1TX ISR
}

inline void ConfigTMR1(void)
{   // 4 microsegundos por Tcount
    T1CON = 0x8020; // T1 ON; :64
    PR1 = 0