#include <xc.h>
#include "config.h"

// initialize the UART1 serial port
inline void UART1Init(unsigned long int baud)
{
    U1BRG = (FCY / (16 * baud)) -1; // 19200 bps @ 16 MHz -> BRG = 51
    U1MODE= 0x8000;    // UARTEN
    U1STA = 0x0400;    // enable transmission
} // UART1Init

// send a character to the UART1 serial port
int putChar1( int c)
{
    while ( U1STAbits.UTXBF);   // wait while Tx buffer full
    U1TXREG = c;
    return c;
} // putChar1

// wait for a new character to arrive to the UART1 serial port
char getChar1( void)
{
    while ( !U1STAbits.URXDA);  // wait
    return U1RXREG;             // read from the receive buffer
}// getChar1

int main()
{
    char c;

    //configurar oscillador para 32MHz
    CLKDIVbits.DOZE = 0;    // 1:1
    CLKDIVbits.RCDIV = 0;   // 8 MHz

    //pinos digitais
    ANSB = 0;

    // 1. init the UART1 serial port
    UART1Init(19200);

    // 2. prompt
    putChar1( '>');

    // 3. main loop
    while (1)
    {
        // 3.1 wait for a character
        c = getChar1();

        // 3.2 echo the character
        putChar1( c);
    } // main loop
}// main