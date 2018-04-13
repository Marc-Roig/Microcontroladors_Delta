#ifndef CONFIG_H
  
	#include <Servo.h>
	
	//ESENTIAL
	#include "Defines.h"
	#include "Structs.h"

	#include "SerialCom.h"
	#include "Kinematics.h"
	#include "Servos.h"
	
	//NON ESENTIAL
	#include "Calibration.h"
	#include "EaseServos.h"
	#include "Sequence.h"

	//ONLY IN ARDUINO
	#include "PIC24_Compatibility.h"

	#define CONFIG_H

	//------------------------//
	//----GLOBAL VARIABLES----//
	//------------------------//

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
	
#endif
