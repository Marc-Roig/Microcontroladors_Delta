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


#include "main.h"

//int angles[] = {0, 25, 45, 70, 90};

void setup() {

    T4CON = 0X8030;

    TRISD = 0xFFFF; //POLSADORS
    TRISA = 0xFF00; //LEDS

    engage_servos();

    // init_buffer(&RX_buffer);
    // init_buffer(&TX_buffer);

}

int main(void) {
    
    setup();

    int duty_cycle = 0;

    while (1) {
        
        polsadors(&duty_cycle);

        servo1_write_duty_cycle(duty_cycle);
//        servo2_write(duty_cycle);
//        servo3_write(duty_cycle);        

        show_dutycycle_leds(duty_cycle);
    }
    
    return 0;

}

void delay(int valor_retard) {

    TMR4 = 0;
    while (TMR4 < valor_retard);

}

void show_dutycycle_leds(int duty_cycle) {

    if (duty_cycle <= 0xff ) {
        LATA = LATA | duty_cycle;
        LATA = LATA & (0xFF00 + duty_cycle);
    }
    
}

void polsadors(int* duty_cycle) {

    static int S3 = 0, S6 = 0, S4 = 0;
    static int mode = 0; 

    int max_duty_cycle = 1100;
    int min_duty_cycle = 430;

    int steps[] = {25, 50, 100};
    int step1 = 25, step2 = 50, step3 = 100;

    //--INCREMENT duty_cycle--//
    if ((PORTD & 0x0040) == 0 && S3 == 0) {

        *duty_cycle += steps[mode];

     if (*duty_cycle >= max_duty_cycle) *duty_cycle = max_duty_cycle;

     S3 = 1;
    }
    else if (PORTD & 0x0040) S3 = 0;

    //--DECREMENT duty_cycle--//
    if ((PORTD & 0x0080) == 0 && S6 == 0) {
     
        *duty_cycle -= steps[mode];

        if (*duty_cycle <= min_duty_cycle) *duty_cycle = min_duty_cycle;
        S6 = 1;

    }
    else if (PORTD & 0x0080) S6 = 0;

    //--CHANGE STEP--//
    if ((PORTD & 0x2000) == 0 && S4 == 0) {

     mode = (mode + 1) % 3; 
     S4 = 1;
     
    }
    else if (PORTD & 0x2000) S4 = 0;
}