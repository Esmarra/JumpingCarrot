/*
 * File:   pic1.c
 * Author: maran
 * Do: 
 * Created on November 08, 2018, 1:52 PM
 */

#include "xc.h"
#include "config.h"
#include "Motion.h"
#include <stdio.h>
#include <stdint.h>

#define KEYMASK 0xF800

//==== Functions ====//
inline void ConfigSys(void);
inline void ConfigCLK(void);
inline void ConfigIO(void);
enum Keys ReadKey(void);
inline void ConfigTMR1(void);
inline void ConfigUART1(unsigned long int baud);

enum Keys{Status=0x01,Front=0x02,Back=0x04,Right=0x08,Left=0x10,
FrontRight=0x0A,FrontLeft=0x12,BackRight=0x0C,BackLeft=0x14};

// ==== Data types ====//
typedef int bool;
#define true 1
#define false 0

// ==== Global Vars ====//
enum Keys cmd;
enum Keys button; //Current Pressed button
enum Keys button_old= 0x05 ;; //Last Pressed button
bool state = false ;

// send a character to the UART1 serial port
int putChar1( int c){
    while ( U1STAbits.UTXBF);   // wait while Tx buffer full
    U1TXREG = c;
    return c;
} // putChar1


int main(void) {
     enum Keys cmd;
    // Run Board Config
    ConfigSys();

    // DO...
    while(1){
        //cmd = ReadKey();
        
        // Status Button(0x01) Pressed
        if(cmd==Status){
            // State flip flop ON-OFF
            if(state==false){
                //Desliga Led Vermelho
                PORTAbits.RA0=1; // 0 ou 1 (verificar na placa)
                //Desliga Led Verde
                PORTAbits.RA1=1; // 0 ou 1 (verificar na placa)
                //Liga Led Azul
                PORTBbits.RB0=0; // 0 ou 1 (verificar na placa)
                state=true; //True
            }
            if(state==true){
                //Liga Led Verde
                PORTAbits.RA1=0; // 0 ou 1 (verificar na placa)
                state=false; //False
            }
        }
        switch(cmd){
            case Front:
                printf("\r Front");
                //Liga Led Vermelho
                PORTAbits.RA0=0; // 0 ou 1 (verificar na placa)
                //Desliga Led Verde
                PORTAbits.RA1=1; // 0 ou 1 (verificar na placa)
                //Desliga Led Azul
                PORTBbits.RB0=1; // 0 ou 1 (verificar na placa)
                
                //Move FW
                MotorVel(300,300);
                break;
            case FrontLeft:
                printf("\r FrontLeft");
                //Liga Led Vermelho
                PORTAbits.RA0=0; // 0 ou 1 (verificar na placa)
                //Desliga Led Verde
                PORTAbits.RA1=1; // 0 ou 1 (verificar na placa)
                //Desliga Led Azul
                PORTBbits.RB0=1; // 0 ou 1 (verificar na placa)
                
                //Move FW-L
                MotorVel(200,300);
                break;
			case FrontRight:
                printf("\r FrontRight");
                //Liga Led Vermelho
                PORTAbits.RA0=0; // 0 ou 1 (verificar na placa)
                //Desliga Led Verde
                PORTAbits.RA1=1; // 0 ou 1 (verificar na placa)
                //Desliga Led Azul
                PORTBbits.RB0=1; // 0 ou 1 (verificar na placa)
                
                //Move FW-R
                MotorVel(300,200);
                break;
			case Left:
                printf("\r Left");
                //Liga Led Vermelho
                PORTAbits.RA0=0; // 0 ou 1 (verificar na placa)
                //Desliga Led Verde
                PORTAbits.RA1=1; // 0 ou 1 (verificar na placa)
                //Desliga Led Azul
                PORTBbits.RB0=1; // 0 ou 1 (verificar na placa)
                
                //Move L
                MotorVel(-300,300); // ou 0 300
                break;
			case Right:
                printf("\r Right");
                //Liga Led Vermelho
                PORTAbits.RA0=0; // 0 ou 1 (verificar na placa)
                //Desliga Led Verde
                PORTAbits.RA1=1; // 0 ou 1 (verificar na placa)
                //Desliga Led Azul
                PORTBbits.RB0=1; // 0 ou 1 (verificar na placa)
                
                //Move R
                MotorVel(300,-300); // ou 300 0
                break;
			case BackRight:
                printf("\r BackRight");
                //Liga Led Vermelho
                PORTAbits.RA0=0; // 0 ou 1 (verificar na placa)
                //Desliga Led Verde
                PORTAbits.RA1=1; // 0 ou 1 (verificar na placa)
                //Desliga Led Azul
                PORTBbits.RB0=1; // 0 ou 1 (verificar na placa)
                
                //Move BR
                MotorVel(-300,-200);
                break;
			case BackLeft:
                printf("\r BackLeft");
                //Liga Led Vermelho
                PORTAbits.RA0=0; // 0 ou 1 (verificar na placa)
                //Desliga Led Verde
                PORTAbits.RA1=1; // 0 ou 1 (verificar na placa)
                //Desliga Led Azul
                PORTBbits.RB0=1; // 0 ou 1 (verificar na placa)
                
                //Move BK-L
                MotorVel(-200,-300);
                break;
			case Back:
                printf("\r Back");
                //Liga Led Vermelho
                PORTAbits.RA0=0; // 0 ou 1 (verificar na placa)
                //Desliga Led Verde
                PORTAbits.RA1=1; // 0 ou 1 (verificar na placa)
                //Desliga Led Azul
                PORTBbits.RB0=1; // 0 ou 1 (verificar na placa)
                
                //Move BK
                MotorVel(-300,-300);
                break;
            default: ;
        } // switch
        
/* Dropped
        // ==== Switch State ====
        if(PORTBbits.RB10==1){ // Button RB10 is pressed
            //Liga Led Verde
            PORTAbits.RA1 = 1;
            //Desliga Led Azul
            PORTBbits.RB0 = 0;
            // faz algo
        }
        else{
            //Desliga Led Verde
            PORTAbits.RA1 = 0;
            //Liga Led Azul
            PORTBbits.RB0 = 1;
        }
        
        // ==== Switch Forward ==== 
        if(PORTBbits.RB11==1 && PORTBbits.RB12==0){
            //Liga Led Vermelho
            PORTAbits.RA1 = 1;
            
            //Forward && Left ==1
            if(PORTBbits.RB14==1 && PORTBbits.RB13==0){
                cmd=FrontLeft;
            }
            
            //Forward && Right ==1
            if(PORTBbits.RB13==1 && PORTBbits.RB14==0){
                cmd=FrontRight;
                printf("\rState");
                
            }
            
            cmd=Front;
            //fazer um if para os outros nao andarem
            
        }
        else{
            //Desliga Led Vermelho
            PORTAbits.RA1=0;
            PORTBbits.RB1=0;
        }
        
        // ==== Switch Backward ====
        if(PORTBbits.RB12==1 && PORTBbits.RB11==0){
            //Liga Led Vermelho
            PORTAbits.RA1 = 1;
            
            //Backward && Left ==1
            if(PORTBbits.RB14==1 && PORTBbits.RB13==0){
                cmd=BackLeft;
            }
            
            //Backward && Right ==1
            if(PORTBbits.RB13==1 && PORTBbits.RB14==0){
                cmd=BackRight;
            }
            
            //Backward 
            cmd=Back;
        }
        else{
            //Desliga Led Vermelho
            PORTAbits.RA1 = 0;
        }
        
        // ==== Switch Right ====
        if(PORTBbits.RB13==1){
            //Liga Led Vermelho
            PORTAbits.RA1 = 1;
            
        }
        else{
            //Desliga Led Vermelho
            PORTAbits.RA1 = 0;
        }
        
        // ==== Switch Left ====
        if(PORTBbits.RB14==1){
            //Liga Led Vermelho
            PORTAbits.RA1 = 1;
        }
        else{
            //Desliga Led Vermelho
            PORTAbits.RA1 = 0;
        }
*/
    }
    return(1);
}

inline void ConfigSys(void){
    ConfigCLK();
    ConfigIO();
    ConfigTMR1();
    ConfigUART1(19200);
    ConfigMotors();
}

enum Keys ReadKey(void){
    unsigned int keys;

    keys = ~PORTB & KEYMASK;
    keys >>= 11;
    
    return(keys);
}

inline void ConfigCLK(void){
    //configurar oscilador para 32MHz
    CLKDIVbits.DOZE = 0;    // 1:1
    CLKDIVbits.RCDIV = 0;   // 8 MHz
}

inline void ConfigUART1(unsigned long int baud){
    U1BRG =  (FCY / (16 * baud)) -1; //BAUDRATEREG1;
    U1MODE = 0;
    U1STA = 0;
    U1MODEbits.UARTEN = 1;
    U1STAbits.UTXEN = 1;
}

inline void ConfigTMR1(void){
	// Uma interrupcao a cada 100ms
    T1CON = 0x8020; // enabled, prescaler 1:64, 4 us tck @ Fcy=16MHz
    TMR1 = 0;
    PR1 = 25000-1;    // 100ms TTick

    _T1IF = 0;
    _T1IE = 1;      // Activa interrupcoes
}

void _ISRFAST _T1Interrupt(void){
    // Timer1 interrupt service routine
    // Ler Button States (if switching dropped)
    button= ReadKey(); //Read current Button
    
    // Current == Old
    if(button == button_old){
        cmd = button; // Its current
    }
    button_old=button;
    
    _T1IF = 0;              // clear the interrupt flag
} //T1Interrupt

inline void ConfigIO(void){
    ANSA = 0;               // Porto A digital
    ANSB = 0;               // Porto B digital
    TRISA= 0b1111111101110000;
    TRISB= 0b1111110101111100;
    CNPU1= 0b0000000000000000; // Activa pull-up interno dos botoes
}