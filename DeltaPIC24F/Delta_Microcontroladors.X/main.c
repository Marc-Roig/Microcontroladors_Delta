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

#include "main.h"
#include "servos.c"


int angles[] = {0, 25, 45, 70, 90};


int main(void) {
    
    TRISD = 0XFFFF; //POLSADORS

    engage_servos();

    int pivot = 0;

    while (1) {
        
        polsadors(&pivot);

        servo1_write(angles[pivot]);
        servo2_write(angles[pivot]);
        servo3_write(angles[pivot]);        

    }
    
    return 0;

}


void polsadors(int* pivot) {
    
    static int S3 = 0;

    if ((PORTD & 0x0040) == 0 & S3 == 0) {

        &pivot = (&pivot + 1) % 5;
        S3 = 1;

    }

    if (PORTD & 0x0040) S3 = 0;

}


