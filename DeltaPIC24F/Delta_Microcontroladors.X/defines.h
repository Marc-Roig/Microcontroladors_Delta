//---------------------------//
//----------DEFINES----------//
//---------------------------//


    //---------------//
    //------I/O------//
    //---------------//
    
    //SERVOS
    #define SERVO1_PIN 9
    #define SERVO2_PIN 10
    #define SERVO3_PIN 3
    #define SERVO4_PIN 5 //GRIPPER

    //CALIBRATION
    #define CHANGE_MODE_BUTTON_PIN 3 //RE8
    #define INCREASE_DC_BUTTON_PIN 4 //RE9
    #define DECREASE_DC_BUTTON_PIN 5 //RA14
    #define CHANGE_STEP_CHANGE_PIN 6 //RA15

    #define CALIBRATION_POTE_PIN 0 //RB3
    
    //JOYSTICK
    #define JOYSTICK_X_PIN 0 //RB3
    #define JOYSTICK_Y_PIN 1 //RB8
    #define JOYSTICK_X_GRIPPER_PIN 2 //RB9
    #define JOYSTICK_BUTTON_PIN 6 //RA15
    
    #define JOYSTICK_CHANGE_AXIS_ANGLES 3 //RE8
    #define BUTTONS_STEP_XYZ_PIN 4 //RE9
    #define INCREASE_XYZ_BUTTON_PIN 5 //RA14
    #define DECREASE_XYZ_BUTTON_PIN 6 //RA15

    //SEQUENCE
    #define SEQUENCE_SAVE_POS_BUTTON_PIN 2
    #define SEQUENCE_PLAY_BUTTON_PIN 6
    
    //----------------//
    //-----SERVOS-----//    
    //----------------//

    //MOVE SERVOS FROM

    #define MOVE_SERVO_1_FROM 0
    #define MOVE_SERVO_2_FROM 0
    #define MOVE_SERVO_3_FROM 0
    #define MOVE_SERVO_4_FROM 1

    #define MOVE_SERVO_FROM_ANGLE 0
    #define MOVE_SERVO_FROM_DC 1
    #define MOVE_SERVO_FROM_XYZ 2

    //ARMS
    #define MAX_DC_SERVO1 1100
    #define MAX_DC_SERVO2 1600
    #define MAX_DC_SERVO3 2400

    #define MIN_DC_SERVO1 400
    #define MIN_DC_SERVO2 1000
    #define MIN_DC_SERVO3 1400

    #define SERVO1_COMPENSATION_VAL 20
    #define SERVO2_COMPENSATION_VAL 20
    #define SERVO3_COMPENSATION_VAL 20

    #define SERVO1_M_ANGLE_TO_DC -4.4109
    #define SERVO2_M_ANGLE_TO_DC -4.8262
    #define SERVO3_M_ANGLE_TO_DC -4.5929

    #define SERVO1_N_ANGLE_TO_DC 1448
    #define SERVO2_N_ANGLE_TO_DC 1134
    #define SERVO3_N_ANGLE_TO_DC 2231

    //GRIPPER
    #define MAX_DC_SERVO4 1900
    #define MIN_DC_SERVO4 965
    #define SERVO4_COMPENSATION_VAL 0

    //--------------------//
    //----EASED SERVOS----//
    //--------------------//
    #define MOVES_BUFFER_LEN 10

    //--------------------//
    //------SEQUENCE------//
    //--------------------//
    #define SEQUENCE_MAX_MOVES 10

    #define SEQUENCE_DEFAULT_MOVEMENT_TIME 2000

    //--------------------//
    //----DELTA PARAMS----//
    //--------------------//

    #define DELTA_MAX_Z -100
    #define DELTA_MIN_Z -200

    #define DELTA_START_X 0
    #define DELTA_START_Y 0
    #define DELTA_START_Z -166

    #define SEQUENCE_MODE 0
    #define CALIBRATION_MODE 1
    #define JOYSTICK_MODE 2

    //-------------------//
    //----CALIBRATION----//
    //-------------------//

    //CHANGE DC MODE
    #define CHANGE_WITH_BUTTONS 0
    #define CHANGE_WITH_POTENTIOMETER 1
    #define CHANGE_WITH_SERIAL 2

    //SERVO MOVEMENT
    #define CLOCKWISE 0
    #define COUNTERCLOCKWISE 1

    //------------------//
    //-----JOYSTICK-----//
    //------------------//

    #define JOYSTICK_NUMBER_OF_SPEEDS 20
    
    #define JOYSTICK_MOVE_ANGLES 0
    #define JOYSTICK_MOVE_AXIS 1

    //-----------------ñ

    //SERIAL COM MODES
    #define ASK_FOR_ANGLES  0
    #define GIVE_ANGLES  1

    //GCODE CODES

    #define MOVE_SERVOS 3
    #define CHANGE_SPEED 4
    #define MOVE_EF 5
    #define END_OF_STREAM 6
    #define EMERGENCY_STOP 9
    #define NEW_DC_VALUES 10

    #define SEND_ME_ANGLES 20
    #define SEND_ME_DC 21
    #define SEND_ANGLES_EFPOS 22

    #define CHANGE_TO_CALIBRATION 30
    #define CHANGE_TO_JOYSTICK 31
    #define CHANGE_TO_SEQUENCE 32
    #define CHANGE_TO_POWER_OFF 33

    //SERIAL BUFFER

    #define MAX_COMMANDS_NUM 10
    #define SERIAL_COMMAND_MAX_LEN 20

    #define SERIAL_BUFFER_LEN 300