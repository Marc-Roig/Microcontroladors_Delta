#ifndef CONFIG_H
  
	#include <Servo.h>

	#include "Defines.h"
	#include "Structs.h"
	
	#include "Servos.h"
	#include "Calibration.h"
	#include "EaseServos.h"
	#include "Sequence.h"

	#define CONFIG_H

	//------------------------//
	//----GLOBAL VARIABLES----//
	//------------------------//
	Buffer buffer;

	DeltaInfo deltainfo;

	Servo servos[4];
	ServoInfo servoinfo[4];

	// ServoEaser servoeaser;
	ServoEaser servoseased[4];

	Sequence sequence;

	
#endif
