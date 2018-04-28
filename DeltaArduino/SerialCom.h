#ifndef SERIALCOM_H
	#define SERIALCOM_H
	
	//------------------//  
    //------SERIAL------//
    //------------------//  

    #define SERIAL_DELAY_MS 100

    //SERIAL COM MODES
    #define ASK_FOR_ANGLES  0
    #define GIVE_ANGLES  1

    //GCODE CODES

    #define MOVE_SERVOS 3
    #define CHANGE_SPEED 4
    #define MOVE_EF 5
    #define END_OF_STREAM 6
    #define EMERGENCY_STOP 9

    #define SEND_ME_ANGLES 20
    #define SEND_ANGLES_EFPOS 21

    //SERIAL BUFFER

    #define MAX_COMMANDS_NUM 5
    #define SERIAL_COMMAND_MAX_LEN 16

#endif