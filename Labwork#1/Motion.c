/*
 * File:   Motion.c
 * Author: Lino
 *
 * Created on 5 de Novembro de 2018, 20:47
 */

#include "Motion.h"

/*
 * Controla a velocidade dos motores
 * -MAXPWM <= vRight <= MAXPWM
 * -MAXPWM <= vLeft <= MAXPWM
 */
void MotorVel(int vLeft, int vRight){
    /*
     * E NECESSARIO VERIFICAR AS DIRECCOES
     */
    
    if(vRight<0){
        DirM_R = 1;
        vRight = -vRight;
    } else {
        DirM_R = 0;
    }
    if(vRight>1000) 
        vRight=1000;
    PWM_R = vRight;
    
    if(vLeft<0){
        DirM_L = 1;
        vLeft = -vLeft;
    } else {
        DirM_L = 0;
    }
    if(vLeft>1000) 
        vLeft=1000;
    PWM_L = vLeft;

}


inline void ConfigCCP4(void){
/*
 * Configura CCP4 em modo PWM
 * Fpwm = Fcy/MAXPWM = 16MHz/4/1000 = 4kHz
 * Duty = CCP4RB / MAXPWM
 * Saida PWM_L: OC4 (pin18)
 * Consultar: Section 64 - Capture Compare PWM Timer (MCCP and SCCP)
 */
    // Set MCCP operating mode
    CCP4CON1L = 0;
    // CCP1CON1Lbits.CCSEL = 0; // Set MCCP operating mode (OC mode)
    CCP4CON1Lbits.MOD = 0b0101; // Set mode (Buffered Dual-Compare/PWM mode)
    //Configure MCCP Timebase
    // CCP1CON1Lbits.TMR32 = 0;   // Set timebase width (16-bit)
    // CCP1CON1Lbits.TMRSYNC = 0; // Set timebase synchronization (Synchronized)
    // CCP1CON1Lbits.CLKSEL = 0b000; // Set the clock source (Tcy)
    CCP4CON1Lbits.TMRPS = 0b01;   // Set the clock pre-scaler (4:1)
    CCP4CON1H = 0;
    // CCP1CON1Hbits.TRIGEN = 0; // Set Sync/Triggered mode (Synchronous)
    // CCP1CON1Hbits.SYNC = 0b00000; // Select Sync/Trigger source (Self-sync)

    //Configure MCCP output for PWM signal
    CCP4CON2Hbits.OCAEN = 1; // Enable output signal (OC4/pin18)
    CCP4CON3H = 0;
    // CCP1CON3Hbits.OUTM = 0b000; // Set advanced output modes (Standard output)
    // CCP1CON3Hbits.POLACE = 0;   //Configure output polarity (Active High)
    CCP4TMRL = 0;      //Initialize timer prior to enable module.
    CCP4PRL = MAXPWM;  //Configure timebase period
    CCP4RA = 0;        // Set the rising edge compare value
    CCP4RB = 0;        // Set the falling edge compare value
    CCP4CON1Lbits.CCPON = 1; // Turn on MCCP module
}

inline void ConfigCCP5(void){
/*
 * Configura CCP5 em modo PWM
 * Fpwm = Fcy/MAXPWM = 16MHz/4/1000 = 4kHz
 * Duty = CCP4RB / MAXPWM
 * Saida PWM_R: OC5 (pin19)
 * Consultar: Section 64 - Capture Compare PWM Timer (MCCP and SCCP)
 */
    // Set MCCP operating mode
    CCP5CON1L = 0;
    // CCP1CON1Lbits.CCSEL = 0; // Set MCCP operating mode (OC mode)
    CCP5CON1Lbits.MOD = 0b0101; // Set mode (Buffered Dual-Compare/PWM mode)
    //Configure MCCP Timebase
    // CCP1CON1Lbits.TMR32 = 0;   // Set timebase width (16-bit)
    // CCP1CON1Lbits.TMRSYNC = 0; // Set timebase synchronization (Synchronized)
    // CCP1CON1Lbits.CLKSEL = 0b000; // Set the clock source (Tcy)
    CCP5CON1Lbits.TMRPS = 0b01;   // Set the clock pre-scaler (4:1)
    CCP5CON1H = 0;
    // CCP1CON1Hbits.TRIGEN = 0; // Set Sync/Triggered mode (Synchronous)
    // CCP1CON1Hbits.SYNC = 0b00000; // Select Sync/Trigger source (Self-sync)

    //Configure MCCP output for PWM signal
    CCP5CON2Hbits.OCAEN = 1; // Enable output signal (OC5/pin19)
    CCP5CON3H = 0;
    // CCP1CON3Hbits.OUTM = 0b000; // Set advanced output modes (Standard output)
    // CCP1CON3Hbits.POLACE = 0;   //Configure output polarity (Active High)
    CCP5TMRL = 0;      //Initialize timer prior to enable module.
    CCP5PRL = MAXPWM;  //Configure timebase period
    CCP5RA = 0;        // Set the rising edge compare value
    CCP5RB = 0;        // Set the falling edge compare value
    CCP5CON1Lbits.CCPON = 1; // Turn on MCCP module
}

inline void ConfigMotors(void){
    
    _ANSA2 = 0;    // DIRM_R
    _ANSA3 = 0;    // DIRM_L
    _ANSB9 = 0;    // PWM_R
    //_ANSA7 = 0;    // PWM_L pino ja e digital

    _TRISA2 = 0;    // DIRM_R
    _TRISA3 = 0;    // DIRM_L
    _TRISB9 = 0;    // PWM_R
    _TRISA7 = 0;    // PWM_L
    
    ConfigCCP4();   // Left motor
    ConfigCCP5();   // Right motor
}