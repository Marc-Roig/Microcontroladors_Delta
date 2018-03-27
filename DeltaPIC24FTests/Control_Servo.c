/*
 * File:   Control_Servo.c
 * Author: Laia
 *
 * Created on 23 de marzo de 2018, 10:37
 */

// PIC24FJ128GA010 Configuration Bit Settings

// 'C' source line config statements

// CONFIG2
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

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <p24fj128ga010.h>

void delay();
void polsadors(int* duty_cycle);
void show_dutycycle_leds(int duty_cycle);
void init_UART();

int main(void) {
    init_UART();
    TRISD = 0xFFFF; //POLSADORS
    TRISA = 0xFF00; //LEDS
    
    T4CON = 0x8030;
    
    PR2 = 9999;                     // PWM Period
    OC2RS = 500;                       // Duty cycle
    OC2R = 500;                        // Initial duty cycle
    OC2CON = 0x0006;   // Configure PWM Operation mode
    T2CON = 0x8010;                 // Set the TMR2 prescale value; 
    
    int duty_cicle = 500;
    
    while (1){                      // Bucle per poder provar que el servo va canviant de pos en funció de D
        polsadors(&duty_cicle);
        OC2RS = duty_cicle;
        show_dutycycle_leds(duty_cicle);
        delay(500);
    }
    
    return 0;
}

void delay(int valor_retard) {

    TMR4 = 0;
    while (TMR4 < valor_retard);

}
void init_UART() {

	// U2MODEbits.UARTEN = 1; //Enable UART2

	// U2STAbits.UTXISEL1 = 0; //Interrupt when any char is transfered
	// U2STAbits.URXISEL1 = 0; //Interrupt flag bit is set when a character is recieved 

	// U2STAbits.UTXEN = 1; //Enable UART2 transmiter
	// U2STAbits.URXEN = 1; //Enable UART2 reciever 

	U2MODE = 0x8000;

	U2STA = 0x1400;

	U2BRG = 25; //9600 BAUDRATE

	_U2TXIF = 0;
	_U2TXIE = 1; //Enable TX interrupt

	_U2RXIF = 0;
	_U2RXIE = 1; //Enable RX interrupt

}

void _ISR _U2TXInterrupt() {


	_U2TXIF = 0;

}

void _ISR _U2RXInterrupt() {

	char incomingByte;
  incomingByte = U2RXREG;

  U2TXREG  = 'a';

	_U2RXIF = 0;

}
void show_dutycycle_leds(int duty_cycle) {

    LATA = (duty_cycle-300)/4;
    
    
}

void polsadors(int* duty_cycle) {

    static int S3 = 0, S6 = 0, S4 = 0;
    static int mode = 0; 

    int max_duty_cycle = 1100;
    int min_duty_cycle = 300;

    int step1 = 10, step2 = 20, step3 = 30;

    //--INCREMENT duty_cycle--//
    if ((PORTD & 0x0040) == 0 && S3 == 0) {

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

     if (*duty_cycle >= max_duty_cycle) *duty_cycle = max_duty_cycle;

     S3 = 1;
    }
    else if (PORTD & 0x0040) S3 = 0;

    //--DECREMENT duty_cycle--//
    if ((PORTD & 0x0080) == 0 && S6 == 0) {
     
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

     if (*duty_cycle <= min_duty_cycle) *duty_cycle = min_duty_cycle;
     S6 = 1;
    }
    else if (PORTD & 0x0080) S6 = 0;

    //--CHANGE STEP--//
    //if ((PORTD & 0x2000) == 0 && S4 == 0) {
    // mode = (mode + 1) % 3; 
    // S4 = 1;
    //}
    //else if (PORTD & 0x2000) S4 = 0;
}



