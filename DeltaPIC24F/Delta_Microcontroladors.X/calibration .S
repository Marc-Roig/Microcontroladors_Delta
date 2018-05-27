.include "p24fj128ga010.inc"
.include "assembly_macros.inc"
.include "assembly_routines.inc"
    
#include "defines.h"
;Only works if file extension is " .S". Doesn't not for ".s".

.data

    change_dc_mode: .int CHANGE_WITH_POTENTIOMETER

    S1_cdb: .int 0 ;switch 1 static variable for Change dc With Buttons
    S2_cdb: .int 0 ;switch 2 static variable for Change dc With Buttons
    S3_cdb: .int 0 ;switch 3 static variable for Change dc With Buttons
    S4_cdm: .int 0 ;switch 4 static variable for Change Dc Mode

    step_val_cdb: .int 0 ;step change from pessing buttons for Change dc With Buttons

    ;check if is only local for this single .s
    debug_startMillis: .int 0

.text

.global _initADC ;function declared in c
;.extern _servoinfo

/*********************************************************************
* Function: _servo_calibration:
*
* Overview: Reads the buttons and potentiometer
*           and then moves the servos
*
* PreCondition: Needs to be called in the main loop
*
* Input: bool - Will move the servo 1
*        bool - Will move the servo 2
*        bool - Will move the servo 3
*
* Output: none
*
********************************************************************/

 _servo_calibration: ;CHECKED
    
    ;W0 bool move_servo1
    ;W1 bool move_servo2
    ;W2 bool move_servo3
    ;W3 bool move_servo4

    SL W1, #1, W1
    SL W2, #2, W2
    SL W3, #3, W3

    IOR W1, W0, W1
    IOR W2, W1, W1
    IOR W3, W1, W1 ;Merge all booleans in W1 
    
    MOV #CHANGE_WITH_POTENTIOMETER, W0
    CP change_dc_mode ; CP f -> f - WREG = f - W0
    BRA Z, calibrate_with_potentiometer
    
    MOV #CHANGE_WITH_BUTTONS, W0
    CP change_dc_mode ; CP f -> f - WREG = f - W0
    BRA Z, calibrate_with_buttons

    BRA End_switch_case

    calibrate_with_potentiometer: 
        ;SERVO1
        MOV #0, W0 ; servo_num
        BTSC W1, #0
        RCALL _calibration_change_dc_potentiometer
        
        ;SERVO2
        MOV #1, W0
        BTSC W1, #1
        RCALL _calibration_change_dc_potentiometer

        ;SERVO3
        MOV #2, W0
        BTSC W1, #2
        RCALL _calibration_change_dc_potentiometer
        
        ;SERVO4
        MOV #3, W0
        BTSC W1, #3
        RCALL _calibration_change_dc_potentiometer

        BRA End_switch_case

    calibrate_with_buttons:
       ;SERVO1
       MOV #0, W0 ; servo_num
       BTSC W1, #0
       RCALL _calibration_change_dc_buttons
       
       ;SERVO2
       MOV #1, W0
       BTSC W1, #1
       RCALL _calibration_change_dc_buttons

       ;SERVO3
       MOV #2, W0
       BTSC W1, #2
       RCALL _calibration_change_dc_buttons
       
       ;SERVO4
       MOV #3, W0
       BTSC W1, #3
       RCALL _calibration_change_dc_buttons

    End_switch_case:

    MOV #change_dc_mode, W0
    RCALL _calibration_change_dc_mode

    MOV #1000, W0
    RCALL _debug_calibration

    RETURN
    
/*********************************************************************
* Function: _calibration_start:
*
* Overview: Prepare board to calibrate servos
*
* PreCondition: Needs to be called in the setup function
*
* Input: bool - W0 - Will move the servo 1
*        bool - W1 - Will move the servo 2
*        bool - W2 - Will move the servo 3
*
* Output: none
*
********************************************************************/
 
 _calibration_start: ;CHECKED
    
    PUSH.S

    init_digital_pins:

        MOV #CHANGE_MODE_BUTTON_PIN, W0
        MOV #INPUT, W1
        RCALL _pinMode

        MOV #INCREASE_DC_BUTTON_PIN, W0
        MOV #INPUT, W1
        RCALL _pinMode
        
        MOV #DECREASE_DC_BUTTON_PIN, W0
        MOV #INPUT, W1
        RCALL _pinMode
    
    init_analog_pin:

        MOV #CHANGE_STEP_CHANGE_PIN, W0
        MOV #INPUT, W1
        RCALL _pinMode

        MOV #CALIBRATION_POTE_PIN, W0
        MOV #ANALOG_INPUT, W1
        RCALL _pinMode

        RCALL _init_ADC

    move_servos_from_dc:

        MOV #_servoinfo, W0
        MOV #MOVE_SERVO_FROM_DC, W1
        ADD #servinf_move_servo_from, W0
        MOV W1, [W0]

        ADD #servoinfo_lenght, W0
        MOV W1, [W0]

        ADD #servoinfo_lenght, W0
        MOV W1, [W0]

        ADD #servoinfo_lenght, W0
        MOV W1, [W0]


    POP.S

    RETURN
;
;_calibration_change_dc_potentiometer: ;CHECKED
;    
;    ;W0 int servo num
;
;    PUSH W1
;    PUSH W2
;    PUSH W3
;    PUSH W4
;    PUSH W5
;
;    GET_SERVOINFO_MEM_POS W0
;    
;    MOV [W0+servinf_min_dc], W3
;    MOV [W0+servinf_max_dc], W4
;    ADD W0, #servinf_duty_cycle, W5
;
;    MOV #CALIBRATION_POTE_PIN, W0
;    RCALL _analogRead
;    MOV #1023, W1
;    
;    ;Prepare to push data to call map
;    PUSH W0
;    MOV #0, W0
;    PUSH W0
;    PUSH W0
;    PUSH W0
;    PUSH W1
;    PUSH W0
;    PUSH W3
;    PUSH W0
;    PUSH W4
;    PUSH W0
;    
;    RCALL _map2
;    
;    SUB #20, W15
;    MOV W0, [W5]
;
;
;    POP W5
;    POP W4
;    POP W3
;    POP W2
;    POP W1
;
;    RETURN
;
;_degug_calibration:
;    
;    ;W0 int wait_time
;
;    ;--SAVE CONTEXT--;
;    LNK #2
;    PUSH W1
;
;    MOV W0, [W14] ;Save wait_time in FP
;
;    ;--CONDITIONAL--//
;    RCALL _millis ;get millis time
;
;    SUB W0, #debug_startMillis, W1
;    CP W0, [W14] ;(millis() - startMilis) > wait_time
;    BRA LT, end_debug ;if smaller end routine
;
;    time_reached:
;
;        MOV W0, debug_startMillis
;        MOV #0, W0
;        RCALL _serial_write_dc_every_ms
;
;    end_debug:
;
;    POP W1
;    ULNK
;    RETURN
;    
;_calibration_change_dc_mode:
;
;    ;W0 -> int* change_dc_mode
;    LNK #4
;    MOV W0, [W14]
;
;    PUSH W1
;
;    MOV #CHANGE_MODE_BUTTON_PIN, W0
;    RCALL _digitalRead
;
;    check_if_button_pressed:
;
;        BTST.Z W0, #0
;        BRA Z, change_dc_mode_button_not_pressed
;        
;        MOV #0, W0
;        CP S4_cdm ;CP f -> S4_cdm - Wreg = S4_cdm - W0 = S4_cdm - 0
;        BRA NZ, end_calibration_change_mode ; if S4_cdm = 1 program is waiting the user to release the button
;
;        MOV [W14], W0 ;@change_dc_mode -> W0
;        INC [W0], W0 ;change_dc_mode++
;
;        CP W0, #2 ;MAX NUMBER OF MODES
;        BRA LT, $+2 ;Skip next line if its smaller than 2
;        MOV #0, W0 ;if change_dc_mode + 1 > 2 change_dc_mode = 0
;
;        MOV [W14], W1
;        MOV W0, [W1]
;    
;        MOV #1, W0
;        MOV W0, S4_cdm
;
;        BRA end_calibration_change_mode
;
;    change_dc_mode_button_not_pressed:
;    
;    MOV #0, W0 
;        MOV W0, S4_cdm
;
;    end_calibration_change_mode:
;
;    POP W1
;
;    ULNK
;    RETURN
;
;        

.end
    