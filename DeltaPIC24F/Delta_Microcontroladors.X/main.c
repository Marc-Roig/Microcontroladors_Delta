/*
 * File:   main.c
 * Author: MARC ROIG
 *
 * Created on 10 de marzo de 2018, 02:25
 */

//CONFIG2
#pragma config POSCMOD = HS             // Primary Oscillator Select (HS Oscillator mode selected)
#pragma config OSCIOFNC = ON            // Primary Oscillator Output Function (OSC2/CLKO/RC15 functions as port I/O (RC15))
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor (Clock switching and Fail-Safe Clock Monitor are disabled)
#pragma config FNOSC = PRI              // Oscillator Select (Primary Oscillator (XT, HS, EC))
#pragma config IESO = ON                // Internal External Switch Over Mode (IESO mode (Two-Speed Start-up) enabled)

// CONFIG1
#pragma config WDTPS = PS32768          // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128            // WDT Prescaler (Prescaler ratio of 1:128)
#pragma config WINDIS = ON              // Watchdog Timer Window (Standard Watchdog Timer enabled,(Windowed-mode is disabled))
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config ICS = PGx2               // Comm Channel Select (Emulator/debugger uses EMUC2/EMUD2)
#pragma config GWRP = OFF               // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF                // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG port is disabled)

#include "xc.h"
#include <p24fj128ga010.h>


int main(void) {
    
    
    
    
    return 0;
}

int scale_angle(int angle) {

    int ms_min = 1;
    int ms_max = 2;
    int ms_duty_cicle = angle * (ms_max - ms_min)/180 + ms_min;
    return ms_duty_cicle * 1024 / 20;
    
}

void servo1_write(int angle) {

    OC1RS = scale_angle(angle);

}

void servo2_write(int angle) {

    OC2RS = scale_angle(angle);

}

void servo3_write(int angle) {

    OC3RS = scale_angle(angle);

}

void disengage_servos() {

    OC1CON = OC1CON & 0XFFF8;
    OC2CON = OC2CON & 0XFFF8;
    OC3CON = OC3CON & 0XFFF8;

}

void engage_servos() {

    //OUTPUT COMPARE 1
    PR1 = 9999;
    OC1RS = 90;
    OC1R = 90;
    
    OC1CON = OC1CON | 0XFFF6; //PWM MODE ON OC1, FAULT PIN DISABLED
    OC1CON = OC1CON & 0xFFFC; 
    
    T1CON = 0X8010; //T1 PREESCALER 1:8

    //OUTPUT COMPARE 2
    PR2 = 9999;
    OC2RS = 90;
    OC2R = 90;
    
    OC2CON = OC2CON | 0XFFF6; //PWM MODE ON OC2, FAULT PIN DISABLED
    OC2CON = OC2CON & 0xFFFC; 
    
    T2CON = 0X8010; //T2 PREESCALER 1:8
    
    //OUTPUT COMPARE 3
    PR3 = 9999;
    OC3RS = 90;
    OC3R = 90;
    
    OC3CON = OC3CON | 0XFFF6; //PWM MODE ON OC3, FAULT PIN DISABLED
    OC3CON = OC3CON & 0xFFFC; 
    
    T3CON = 0X8010; //T3 PREESCALER 1:8

}