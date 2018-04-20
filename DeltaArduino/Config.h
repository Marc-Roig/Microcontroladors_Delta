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

	#if defined(SERIALCOM_H) && !defined(BUFFER_STRUCT)

    	#define BUFFER_STRUCT
		CommandsBuffer buffer;

	#endif


	#ifdef KINEMATICS_H && DELTAINFO_STRUCT

		#define DELTAINFO_STRUCT
		DeltaInfo deltainfo;

	#endif


	#ifdef SERVOS_H && SERVOINFO_STRUCT

		#define SERVOINFO_STRUCT
		ServoInfo servoinfo[4];

	#endif


	#ifdef EASESERVOS_H && SERVOEASED_STRUCT

		#define SERVOEASED_STRUCT
		ServoEaser servoseased[4];

	#endif


	#ifdef SEQUENCE_H && SEQUENCE_STRUCT

		#define SEQUENCE_STRUCT
		Sequence sequence;

	#endif
	
#endif
