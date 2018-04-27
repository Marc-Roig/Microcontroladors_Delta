#ifndef CONFIG_H
    
    #include "xc.h"
    #include <p24fj128ga010.h>
	#include <stdbool.h>
	#include "Math.h"
		

	#include "defines.h"
	#include "structs.h"

	#include "main.h"

	#include "inouts.h"
	#include "servos.h"
	#include "servo_movement.h"
	 
	#include "calibration.h"
	#include "joystick.h"

	#include "serialcom.h"
	#include "serialcom_simulator.h"
	
	#include "kinematics.h"

	#include "time_events.h"

	#define CONFIG_H //Include guard

	#ifdef SERIALCOM_SIMULATOR

		extern CommandsBuffer buffer;

	#endif


	#ifdef KINEMATICS_H

		extern DeltaInfo deltainfo;

	#endif


	#ifdef SERVO_MOVEMENT_H

		extern ServoInfo servoinfo[4];

	#endif


#endif //MAIN_H