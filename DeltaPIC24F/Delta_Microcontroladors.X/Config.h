#ifndef CONFIG_H
    
    #include "xc.h"
    #include <p24fj128ga010.h>
	#include <stdbool.h>

	#include "defines.h"
	#include "structs.h"
	#include "inouts.h"
	 
	#include "calibration.h"
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
    


	#ifndef BUFFER_STRUCT

		#define BUFFER_STRUCT
		extern CommandsBuffer buffer;

	#endif


	#ifndef DELTAINFO_STRUCT

		#define DELTAINFO_STRUCT
		extern DeltaInfo deltainfo;

	#endif


	#ifndef SERVOINFO_STRUCT

		#define SERVOINFO_STRUCT
		extern ServoInfo servoinfo[4];

	#endif


#endif //MAIN_H