#ifndef MAIN_H
    
    #include "xc.h"
    #include <p24fj128ga010.h>
	#include <stdbool.h>

	#include "inouts.h"
	#include "defines.h"
	
	#include "serialcom.h"
	#include "servos.h"
	
	#include "servo_movement.h"

    
	#define MAIN_H //Include guard

    //--MISC--//
	void setup();
	void loop();
	
    void polsadors(int* angle);
    void show_dutycycle_leds(int duty_cycle);
    void delay(int cicles);

//    ServoInfo servoinfo;

#endif //MAIN_H