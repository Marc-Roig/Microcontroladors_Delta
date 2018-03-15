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
#include "servos.h"


//int angles[] = {0, 25, 45, 70, 90};

 
int main(void) {
    
    TRISD = 0xFFFF; //POLSADORS
    TRISA = 0xFF00; //LEDS

    engage_servos();
    
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

void show_dutycycle_leds(int duty_cycle) {
    // 0-256
    // 1001 1100
    LATA = LATA | duty_cycle;
    LATA = LATA & (0xFF00 + duty_cycle);
}

void polsadors(int* duty_cycle) {

    static int S3 = 0, S6 = 0, S4 = 0;
    static int mode = 0; //Used to 

    static int max_duty_cycle = 200;
    static int min_duty_cycle = 10;

    static int step1 = 10, step2 = 25, step3 = 50;

    //--INCREMENT duty_cycle--//
    if ((PORTD & 0x0040) == 0 & S3 == 0) {

     switch(mode) {
        case 0:
            *duty_cycle = *duty_cycle + step1;
            break;
        case 1:
            *duty_cycle = *duty_cycle + step2;
            break;
        default:
            *duty_cycle = *duty_cycle + step3;
            break;
     }

     if (duty_cycle >= max_duty_cycle) duty_cycle = 150;

     S3 = 1;
    }
    else if (PORTD & 0x0040) S3 = 0;

    //--DECREMENT duty_cycle--//
    if ((PORTD & 0x0080) == 0 & S6 == 0) {
     
     switch(mode) {
        case 0:
            *duty_cycle = *duty_cycle - step1;
            break;
        case 1:
            *duty_cycle = *duty_cycle - step2;
            break;
        default:
            *duty_cycle = *duty_cycle - step3;
            break;
     }

     if (duty_cycle <= min_duty_cycle) duty_cycle = 20;
     S6 = 1;
    }
    else if (PORTD & 0x0080) S6 = 0;

    //--CHANGE STEP--//
    if ((PORTD & 0x2000) == 0 & S4 == 0) {
     mode = (mode + 1) % 3; 
     S4 = 1;
    }
    else if (PORTD & 0x2000) S4 = 0;
}


//JOYSTICK
 // set new angles 
 // Set modes of movement
    // Simulation
    // Move with joystick
    // Move preset values

//

 //Implement delays
 //Implement Serial.write
    