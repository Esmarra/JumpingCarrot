/*
 * File:   pic1_v3.c
 * Author: maran
 *
 * Created on November 12, 2018, 2:48 PM
 */


#include <xc.h>
#include "config.h"
#include <stdio.h>
 
//==== Opto_acustic Def ====//
#define OPTO PORTAbits.RA1
#define SOM PORTAbits.RA2

// distancia por timer tick (um)
#define Kd 340UL

// Tempo de ciclo (200ms)
#define T200 (200000UL/4)

//==== Sonar Def====//
// Tempo de trigger (12us=3*4us)
#define T12 (12/4)
// Tempo de ciclo (200ms)
#define T200 (200000UL/4)
// distancia por timer tick (um)
#define Kd 680UL

//==== Functions ====//
inline void ConfigCLK(void);
inline void ConfigIO(void);
inline void UART1Init(unsigned long int brg);
inline void ConfigTMR1(void);

// ==== Data types ====//
typedef int bool;
#define true 1
#define false 0

// ==== Global Vars ====//
bool sonar = false; //Se estiver a usar sonar sonar=true
bool opto = false; //Se estiver a usar opto opto=true
bool start = true; //Tirar o timer do sinal optico
unsigned int delay;
unsigned int opto_time;


int main(void) {
    unsigned int d_opt;
    unsigned int delay_aux;
 
    ConfigCLK();
    UART1Init(19200);
 
    ConfigIO();
    ConfigTMR1();

    printf("\rReceptor OptoAcustico");
 
    unsigned char Acust;
    unsigned int tEcho;
    unsigned int d_son;
    int count=0;
    
    while(1){ 
        //Espera sinal optico (first run)
        if(start==true){
            while(OPTO == 1); // Espera sinal optico
            TMR1 = 0;     // Inicia contagem de tempo
            Acust = SOM;
            while(SOM == Acust); // Espera sinal acustico
            delay = TMR1;
            delay_aux = delay;
            //Calc distance
            d_opt = Kd*delay_aux/1000;
            
            //Exits First Run
            start=false;
        }
        
        //New Light Signal
        if(OPTO==0&&sonar==false){
            // In opto mode
            opto=true;
            // First light signal
            if(count==0){
                //Calculates Opto Light Signal Interval
                opto_time=TMR1;
            }
            count=1;
            TMR1 = 0;     // Inicia contagem de tempo
            Acust = SOM;
            while(SOM == Acust); // Espera sinal acustico
            delay = TMR1;
            delay_aux = delay;
            //Calc distance
            d_opt = Kd*delay_aux/1000;
            if(d_opt < 1210){ // 1,21m comprimento da plataforma
                printf("\r%u mm    delay = %u", d_opt, delay_aux);
            }
        }
        //Run only when outside opto_time iterval and opto=false
        else{
            
            LATAbits.LATA4 = 1; // Trigger
            TMR1 = 0;
            while(TMR1<T12);     // 12 us
            LATAbits.LATA0 = 0;

            while(PORTAbits.RA2==0);    // espera echo 0->1
            TMR1=0;
            while(PORTAbits.RA2==1);
            tEcho = TMR1;
            d_son = Kd*tEcho/1000;
            if(d_son<4000){
                printf("\r%u mm", d_son);
            }
        }
        
        // Implementar Odometria
        
        //Usando as 3 distancias d_opt e d_sonar e d_enc 
        // calcular posição
    }
    return(1);
}

inline void ConfigIO(void){
    ANSA = 0;               // Porto A digital
    TRISAbits.TRISA0 = 0;   // saida (LED da Microstick)
    TRISAbits.TRISA1 = 1;
    TRISAbits.TRISA2 = 1;
    TRISAbits.TRISA4 = 0;   // saida (Trigger)
    // TX1, RX1 digitais, restantes analogicos
    ANSB = 0;         // Todos digitais
    TRISB= 0b1110011101111111;
}
 
inline void ConfigCLK(void){
    //configurar oscilador para 32MHz
    CLKDIVbits.DOZE = 0;    // 1:1
    CLKDIVbits.RCDIV = 0;   // 8 MHz
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
}
 
inline void ConfigTMR1(void){   // 4 microsegundos por Tcount
    T1CON = 0x8020; // T1 ON; :64
    PR1 = 0xFFFF;
}
