.include "p24fj128ga010.inc"
.include "assembly_macros.inc"
.include "assembly_routines.inc"

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

.extern _servoinfo

_servo_calibration:
    
    ;W0 bool move_servo1
    ;W1 bool move_servo2
    ;W2 bool move_servo3
    ;W3 bool move_servo4

    IOR W1, W0, W1
    IOR W2, W0, W1
    IOR W3, W0, W1

    CP change_dc_mode, #CHANGE_WITH_POTENTIOMETER
    BRA Z, calibrate_with_potentiometer

    CP change_dc_mode, #CHANGE_WITH_BUTTONS
    BRA Z, calibrate_with_buttons

    BRA End_switch_case

    calibrate_with_potentiometer:
        ;SERVO1
        MOV #0, W0 ; servo_num
        BTSC W1, #0
        RCALL _calibration_change_dc_potentiometer
        
        ;SERVO2
        INC W0, W0
        BTSC W1, #1
        RCALL _calibration_change_dc_potentiometer

        ;SERVO3
        INC W0, W0
        BTSC W1, #2
        RCALL _calibration_change_dc_potentiometer
        
        ;SERVO4
        INC W0, W0
        BTSC W1, #3
        RCALL _calibration_change_dc_potentiometer

        BRA End_switch_case

    calibrate_with_buttons:
        ;SERVO1
        MOV #0, W0 ; servo_num
        BTSC W1, #0
        RCALL _calibration_change_dc_buttons
        
        ;SERVO2
        INC W0, W0
        BTSC W1, #1
        RCALL _calibration_change_dc_buttons

        ;SERVO3
        INC W0, W0
        BTSC W1, #2
        RCALL _calibration_change_dc_buttons
        
        ;SERVO4
        INC W0, W0
        BTSC W1, #3
        RCALL _calibration_change_dc_buttons

    End_switch_case:

    MOV #change_dc_mode, W0
    RCALL _calibration_change_dc_mode

    MOV #1000, W0
    RCALL _debug_calibration

    RETURN

_calibration_start:
    
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
        ADD #servinf_move_servo_from, W0
        MOV #MOVE_SERVO_FROM_DC, [W0]

        ADD #_servoinfo_lenght, W0
        MOV #MOVE_SERVO_FROM_DC, [W0]

        ADD #_servoinfo_lenght, W0
        MOV #MOVE_SERVO_FROM_DC, [W0]

        ADD #_servoinfo_lenght, W0
        MOV #MOVE_SERVO_FROM_DC, [W0]


    POP.S

    RETURN

_calibration_change_dc_potentiometer:
    
    ;W0 int servo num

    PUSH W1
    PUSH W2
    PUSH W3
    PUSH W4
    PUSH W5

    get_servoinfo_mem_pos W0
    
    MOV [W0+servinf_min_dc], W3
    MOV [W0+servinf_max_dc], W4
    ADD W0, #servinf_duty_cycle, W5

    MOV #CALIBRATION_POTE_PIN, W0
    RCALL _analogRead

    PUSH W0
    CLR W0
    PUSH W0 ;long x

    MOV #0, W1
    PUSH W1
    PUSH W0 ;long in_min

    MOV #1023, W1
    PUSH W1
    PUSH W0 ;long in_max

    PUSH W3
    PUSH W0 ;long out_min

    PUSH W4
    PUSH W0 ;long out_max

    RCALL _map2

    MOV W0, [W5]
    SUB 

    POP W5
    POP W4
    POP W3
    POP W2
    POP W1

    RETURN

_degug_calibration:
    
    ;W0 int wait_time

    ;--SAVE CONTEXT--;
    LNK #2
    PUSH W1

    MOV W0, [W14] ;Save wait_time in FP

    ;--CONDITIONAL--//
    RCALL _millis ;get millis time

    SUB W0, #debug_startMillis, W1
    CP W0, [W14] ;(millis() - startMilis) > wait_time
    BRA LT, end_debug ;if smaller end routine

    time_reached:

        MOV W0, debug_startMillis
        MOV #0, W0
        RCALL _serial_write_dc_every_ms

    end_debug:

    POP W1
    ULNK
    RETURN
    
_calibration_change_dc_mode:

    ;W0 -> int* change_dc_mode
    LNK #4
    MOV W0, [W14]

    PUSH W1

    MOV #CHANGE_MODE_BUTTON_PIN, W0
    RCALL _digitalRead

    check_if_button_pressed:

        BTST.Z [W14], #0
        BRA Z, check_if_button_pressed
        CP S4_cdm, #0
        BRA NZ, end_calibration_change_mode

        MOV [W14], W0
        INC [W0], W0

        CP W0, #2 ;MAX NUMBER OF MODES
        BRA LT, $+2 ;Skip next line if its smaller than 2
        MOV #0, W0

        MOV [W14], W1
        MOV W0, [W1]

        MOV #1, S4_cdm

        BRA end_calibration_change_mode

    check_if_button_not_pressed:

        MOV #0, S4_cdm

    end_calibration_change_mode:

    POP W1

    ULNK
    RETURN

        

.end