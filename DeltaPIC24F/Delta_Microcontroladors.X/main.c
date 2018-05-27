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


#include "Config.h"

volatile bool delta_on; //If false delta servomotors will not move

/*********************************************************************
* Function: void setup()
*
* Overview: Initialize all parameters, timers, ...
*           
* PreCondition: Has to be the first function to be called in main.
*
********************************************************************/

void setup() {

    //--TIMERS--//
    T4CON = 0X8010; //Enable T4, Preescaler 1:8
    PR4 = 3999; //Initialize timer4 to tick every 1 ms.
    
    //--EMERGENCY STOP--//
    init_INT4(); 

    //--SERIAL COM--//
    Serial_begin(9600); 
    init_buffer(); //Buffer to communicate with control panel (Processing)
    Serial_write("G06\n"); //Processing is waiting to communicate, wake him up.

    //--SERVOS--//
    init_servos(true, true, true, true);

    //--DELTA--// SEQUENCE_MODE, CALIBRATION_MODE, JOYSTICK_MODE

    init_delta(JOYSTICK_MODE);
    
    //Delta has 3 different working modes, it has been defined in the
    //init_delta(...) function.

    switch (deltainfo.mode) { 

        case SEQUENCE_MODE:     //--JOYSTICK--//
                                init_joystick();

                                
                                //--SEQUENCE--//
//                                init_sequence();

                                break;

        case CALIBRATION_MODE:  calibration_start(true, true, true, false);
                                break;

        case JOYSTICK_MODE:     init_joystick();
                                break;
                                
    }

}


/*********************************************************************
* Function: void loop()
*
* Overview: Read specified peripherals (buttons, joysticks,...) depending
*           on the mode, move the servos and communicate with the panel.
*           
* PreCondition: Has to be called in a loop in main.
*
********************************************************************/

void loop() {

    switch (deltainfo.mode){

        case SEQUENCE_MODE:  //   update_sequence_mode();
                                break;

        case CALIBRATION_MODE:  servo_calibration(true, true, true, false);
                                break;

        case JOYSTICK_MODE:     joystick_movement();
                                break;

    }

    //Only the following function can change the position of the servos.
    //Only the value of servoinfo duty_cycle, angle or xyz may be changed
    //and specified in servoinfo[].move_servo_from which of the three will
    //be the used one to move the servo.
    if (delta_on) move_selected_servos(true, true, true, false);

    //--SERIAL WITH PROCESSING--//
    serial_com_with_simulator();
    
}


/*********************************************************************
* Function: int main()
*
* Overview: Calls setup and loop to copy the Arduino format
*
********************************************************************/

int main(void) {
    
    setup();

    while (1) {
        loop();
    }
    
    return 0;

}


/*********************************************************************
* Function: void init_INT4()
*
* Overview: Initialize external interrupt 4
*
********************************************************************/

void init_INT4() {
    
    _INT4EP = 0;
    _INT4IF = 0;
    _INT4IE = 1;
    
}


/*********************************************************************
* Function: _ISR _INT4Interrupt()
*
* Overview: External interrupt 4 is called whenever INT4 pin goes high.
*           
* PreCondition: Has to be initialized in init_INT4();
*
********************************************************************/

void _ISR _INT4Interrupt() {
    
    if (!delta_on) { //If delta is on turn it off
        delta_on = true;
        attach_servos();
    }
    else {  //turn it on
        delta_on = false;
        disengage_servos();
    }
        _INT4IF = 0;

}