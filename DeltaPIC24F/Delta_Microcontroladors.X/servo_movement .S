.include "p24fj128ga010.inc"
.include "assembly_routines.inc"
#include "defines.h" 
    ;Only works if file extension is " .S". Doesn't not for ".s".
    
.extern _servoinfo

.extern _servo1_m
.extern _servo2_m
.extern _servo3_m
.extern _servo4_m

.extern _servo1_n
.extern _servo2_n
.extern _servo3_n
.extern _servo4_n


;/*********************************************************************
;* Function: _init_ServoInfo(struct ServoInfo* servo_inf, int max_duty_cycle_, int min_duty_cycle_, int slack_compensation_val_, float m_, float n_);
;*
;* Overview: Initialize al servoinfo parameter values
;*
;* PreCondition: Transfer all parameters through working registers W0 to W7
;*
;* Input: ServoInfo*                    -W0 - the servo information struct
;*        int max_duty_cycle            -W1 - max duty cycle of the servo 
;*        int min_duty_cycle            -W2 - mix duty cycle of the servo 
;*        int slack_compensation_val    -W3 - value of compensation when servo is changing direction 
;*        long m                        -W4 - slope of the equation duty_cycle - angle 
;*        long n                        -W6 - offset of the equation duty_cycle - angle
;*
;* Output: none
;*
;********************************************************************/

_init_ServoInfo: ;CHECKED

    ; servo_inf->angle = 90;
    MOV #90, W8
    MOV W8, [W0+16]

    ; servo_inf->max_duty_cycle = max_duty_cycle_;
    MOV W1, [W0+4]

    ; servo_inf->min_duty_cycle = min_duty_cycle_;
    MOV W2, [W0+6]

    ;servo_inf->dc_offset = 0;
    PUSH W8 ;Save whatever was in W8
    CLR W8
    MOV W8, [W0+2]

    ;servo_inf->mean_dc = (max_duty_cycle_ + min_duty_cycle_)/2; 
    ADD W1, W2, W1
    ASR W1, W1
    MOV W1, [W0+8]  

    ;servo_inf->duty_cycle = servo_inf->mean_dc0;   
    MOV W1, [W0]

    ;servo_inf->last_direction = 0;
    MOV W8, [W0+10] ;W8 was cleared for dc_offset = 0
    
     ;servo_inf->move_servo_from = MOVE_SERVO_FROM_DC;
    MOV #MOVE_SERVO_FROM_DC, W8 ;W8 not used anymore and its previous value is saved in the stack
    MOV W8, [W0+14]
    
    ; servo_inf->slack_compensation_val = slack_compensation_val_;
    MOV W3, [W0+12]

    ; servo_inf->m = m_;
    MOV W4, [W0+18]
    MOV W5, [W0+20]

    ; servo_inf->n = n_;
    MOV W6, [W0+22]
    MOV W7, [W0+24]

    POP W8 ;Recover whatever was in W8

    RETURN


;/*******************************************************************
;* Function: check_servo_change_direction(int num_servo, int new_duty_cycle);
;*
;* Overview: Servos need an extra duty cycle value to compensate the slack
;*           when changing direction. 
;*
;* PreCondition: The new duty cycle value has to be checked in this function
;*               first before changing the duty_cycle value of servoinfo.
;*
;* Input: unsigned int num_servo      - W0 - Servo number to check 
;*        unsigned int new_duty_cycle - W1 - The new duty_cycle to update
;*
;* Output: none
;*
;********************************************************************/

_check_servo_change_direction: ;CHECKED

    CP W0, #4
    BRA GE, End_change_dir

    PUSH W2
    PUSH W3
    PUSH W4
    PUSH W5
    PUSH W6
    
    MUL.UU W0, #26,  W2
    MOV #_servoinfo, W3 ;@servoinfo -> W3
    ADD W2, W3, W2
    
    ADD W2, #10, W3 ;@servoinfo[servo_num].last_direction
    ADD W2, #0 , W4 ;@servoinfo[servo_num].duty_cycle
    ADD W2, #2 , W5 ;@servoinfo[servo_num].dc_offset
    ADD W2, #12, W6 ;@servoinfo[servo_num].slack_compensation_val

    MOV [W4], W2
    ADD W2, #20, W2 ; duty_cycle += min_step_to_change_dir
    CP W1, W2 ; compare new_duty_cycle > duty_cycle + min_step_to_change_dir
    BRA LT, Duty_cycle_smaller ;if smaller


    Duty_cycle_greater:
    
        MOV [W3], W2
        CP W2, #COUNTERCLOCKWISE
        BRA NZ, End_pop

        MOV #CLOCKWISE, W2
        MOV W2, [W3]

        MOV [W6], W6 ;servoinfo[servo_num].slack_compensation_val
        MOV [W5], W2 ;servoinfo[servo_num].dc_offset

        ADD W2, W6, W2
        MOV W2, [W5] ;dc_offset += slack_compensation_val
        BRA End_pop

    Duty_cycle_smaller:
    
        MOV [W3], W2
        CP W2, #CLOCKWISE
        BRA NZ, End_pop
        
        MOV #COUNTERCLOCKWISE, W2
        MOV W2, [W3]

        MOV [W6], W6 ;servoinfo[servo_num].slack_compensation_val
        MOV [W5], W2 ;servoinfo[servo_num].dc_offset

        SUB W2, W6, W2
        MOV W2, [W5] ;dc_offset -= slack_compensation_val
        BRA End_pop

    End_pop:

        POP W6
        POP W5
        POP W4
        POP W3
        POP W2


    End_change_dir:

    RETURN

;/*********************************************************************
;* Function: void servos_initial_positions(bool move_servo1, bool move_servo2, bool move_servo3);
;*
;* Overview: To reduce the slack of the gears all servos are moved
;*           clockwise. To ensure they are left moved clockwise
;*           they are first moved counterclockwise just in case they
;*           coudln't move any more clockwise.
;*
;* PreCondition: none
;*
;* Input: bool - W0 - Will move the servo 1
;*        bool - W1 - Will move the servo 2
;*        bool - W2 - Will move the servo 3
;*        bool - W3 - Will move the servo 4 -> gripper is not used in this board
;*
;* Output: none
;*
;********************************************************************/

_servos_initial_positions: ;CHECKED

    ;W4, W5, W6, W7 will be modified in this routine
    PUSH W4
    PUSH W5
    PUSH W6
    PUSH W7

    ;In every loop the boolean condition of servo movement
    ;will be poped to know if it has to move it.

    ; PUSH W3 ;UNCOMMENT IF NEED TO USE GRIPPER
              ;and change cp #3 to #4 in end_or_repeat_loop
    PUSH W2 
    PUSH W1
    PUSH W0
   ;PUSH W3 -> gripper (servo 4) is not used in this board
    PUSH W2 
    PUSH W1
    PUSH W0
    
    
    MOV #0, W4 ;pivot to loop 3 times
    MOV #_servoinfo, W7

    ; All three servos are moved to 55 degrees. To do that the duty cycle
    ; of each has to be calculated from its m and n (regression line got from
    ; calibrationg every servo) after that this value is send to the 
    ; _servo_writeMicroseconds routine to move every servo.

    ; This is a loop to calculate this duty cycle and calling the _servo_writeMicroseconds
    ; functions for every servo.
    
    first_move_loop:

        POP W5 ;move_servo booleans had been pushed into the stack previously
        BTST.Z W5, #0
        BRA Z, end_or_repeat_loop ; move_servos[i] = false

        MOV [W7+18], W0 ;servoinfo.m
        MOV [W7+20], W1 ;servoinfo.n
        MOV #55, W2
        RCALL _mul_long_int
    
        MOV [W7+22], W2
        MOV [W7+24], W3
        RCALL _add_longs

        MOV #10, W2
        RCALL _div_long_int
    
        ;long -> int

        MOV [W7+12], W1
        SUB W0, W1, W0

        MOV W0, [W7] ;[w7] = duty_cycle
        MOV W4, W1; W1 = servo_to_move
        RCALL _servo_writeMicroseconds

        end_or_repeat_loop:

            INC W4, W4 ;increment pivot
            ADD W7, #26, W7; @servoinfo + 26 <--> servoinfo[++i] 
            CP W4, #3 ;Check if loop has to end
            BRA LT, first_move_loop ;if (W4 < 3) repeat loop

    end_first_loop:

    MOV #1000, W0
    MOV #0, W1
    RCALL _delay
    
    MOV #0, W4 ;pivot to loop 3 times
    MOV #_servoinfo, W7

    ; Same procedure, move servos to 90 degrees

    second_move_loop:

        POP W5
        BTST.Z W5, #0
        BRA Z, end_or_repeat_loop2 ; move_servos[i] = false

        MOV [W7+18], W0
        MOV [W7+20], W1
        MOV #90, W2
        RCALL _mul_long_int
    
        MOV [W7+22], W2
        MOV [W7+24], W3
        RCALL _add_longs

        MOV #10, W2
        RCALL _div_long_int
    
        ;long -> int

        MOV [W7+12], W1
        SUB W0, W1, W0

        MOV W0, [W7] ;[w7] = duty_cycle
        MOV W4, W1; W1 = servo_to_move
        RCALL _servo_writeMicroseconds

        end_or_repeat_loop2:

            INC W4, W4 ;increment pivot
            ADD W7, #26, W7; @servoinfo + 26 <--> servoinfo[++i] 
            CP W4, #3 ;Check if loop has to end
            BRA LT, second_move_loop ;if (W4 < 3) repeat loop

    end_second_loop:

    POP W7
    POP W6
    POP W5
    POP W4
    
    RETURN
;/*
_init_servos:
    
    ;W0 - Bool move_servo_1
    ;W1 - Bool move_servo_2
    ;W2 - Bool move_servo_3
    ;W3 - Bool move_servo_4

    LNK #0X08
    MOV W0, [W14]
    MOV W1, [W14+2]
    MOV W2, [W14+4]
    MOV W3, [W14+6]

    PUSH W4
    PUSH W5
    PUSH W6
    PUSH W7

    check_move_servo_1:

        BTST.Z [W14], #0   ;Check if move_servo_1 == true
        BRA Z, check_move_servo_2 ;If not jump to check_move_servo_2

    init_servo_1:

        MOV #0, W0
        MOV #0, W1
        RCALL _servo_attach

        MOV #_servoinfo, W0 ; W0 = @servoinfo[0]
        MOV #MAX_DC_SERVO1, W1
        MOV #MIN_DC_SERVO1, W2
        MOV #SERVO1_COMPENSATION_VAL, W3
        MOV.D _servo1_m, W4
        MOV.D _servo1_n, W6
        RCALL _init_ServoInfo

    check_move_servo_2:

        MOV [W14+2], W1
        BTST.Z W1, #0
        BRA Z check_move_servo_3

    init_servo_2:

        MOV #0, W0
        MOV #1, W1 
        RCALL _servo_attach

        MOV #_servoinfo, W0
        ADD W0, #26, W0 ; W0 = @servoinfo[1]
        MOV #MAX_DC_SERVO2, W1
        MOV #MIN_DC_SERVO2, W2
        MOV #SERVO2_COMPENSATION_VAL, W3
        MOV.D _servo2_m, W4
        MOV.D _servo2_n, W6
        RCALL _init_ServoInfo
    
    check_move_servo_3:

        MOV [W14+4], W2
        BTST.Z W2, #0
        BRA Z, move_servos_to_init_pos

    init_servo_3:

        MOV #0, W0
        MOV #2, W1
        RCALL _servo_attach

        MOV #_servoinfo, W0
        ADD W0, #52 , W0  ; W0 = @servoinfo[2]
        MOV #MAX_DC_SERVO3, W1
        MOV #MIN_DC_SERVO3, W2
        MOV #SERVO3_COMPENSATION_VAL, W3
        MOV.D _servo3_m, W4
        MOV.D _servo3_n, W6
        RCALL _init_ServoInfo

    move_servos_to_init_pos:

        MOV [W14]  , W0
        MOV [W14+2], W1
        MOV [W14+4], W2
        MOV [W14+6], W3
        RCALL _servos_initial_positions

    end_init_servos:

    POP W7
    POP W6
    POP W5
    POP W4

    ULNK
    RETURN
    ;*/
    
    ;/*
_set_servo_movement_with_dc:
    
    ;W0 -> bool set_servo1
    ;W1 -> bool set_servo2
    ;W2 -> bool set_servo3
    ;W3 -> bool set_servo4

    PUSH W4

    Set_servo1:

        CP W0, #1
	MOV #_servoinfo, W4
        ADD W4, #16, W4 ; @servoinfo[0].move_servo_from 
        BRA NZ, Set_servo2
        MOV #MOVE_SERVO_FROM_DC, [W4] ; servoinfo[0].move_servo_from = MOVE_SERVO_FROM_DC

    Set_servo2:

        CP W1, #1
        ADD W4, #26, W4 ; W4 = @servoinfo[1].move_servo_from 
        BRA NZ, Set_servo3
        MOV #MOVE_SERVO_FROM_DC, [W4] ; servoinfo[1].move_servo_from = MOVE_SERVO_FROM_DC

    Set_servo3:

        CP W2, #1
        ADD W4, #26, W4 ; W4 = @servoinfo[2].move_servo_from 
        BRA NZ, Set_servo4
        MOV #MOVE_SERVO_FROM_DC, [W4] ; servoinfo[2].move_servo_from = MOVE_SERVO_FROM_DC

    Set_servo4:

        CP W3, #1
        ADD W4, #26, W4 ; W4 = @servoinfo[3].move_servo_from 
        BRA NZ, End
        MOV #MOVE_SERVO_FROM_DC, [W4] ; servoinfo[3].move_servo_from = MOVE_SERVO_FROM_DC

    End:

        POP W4

    RETURN

    ;*/








