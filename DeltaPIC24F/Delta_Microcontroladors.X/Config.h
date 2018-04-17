#ifndef CONFIG_H
    
    #include "xc.h"
    #include <p24fj128ga010.h>
	#include <stdbool.h>

	#include "defines.h"
	#include "inouts.h"
	
	#include "serialcom.h"
	#include "serialcom_simulator.h"
	
	#include "servos.h"
	#include "servo_movement.h"
	
	#include "kinematics.h"

	#include "time_events.h"
    
	#define CONFIG_H //Include guard

    //--MISC--//
	void setup();
	void loop();
	
    void polsadors(int* angle);
    void show_dutycycle_leds(int duty_cycle);

    #ifdef SERIALCOM_H
		CommandsBuffer buffer;
	#endif

	#ifdef KINEMATICS_H
		DeltaInfo deltainfo;
	#endif

	#ifdef SERVOS_H
		ServoInfo servoinfo[4];
	#endif

	#ifdef EASESERVOS_H
		ServoEaser servoseased[4];
	#endif

	#ifdef SEQUENCE_H
		Sequence sequence;
	#endif

#endif //MAIN_H