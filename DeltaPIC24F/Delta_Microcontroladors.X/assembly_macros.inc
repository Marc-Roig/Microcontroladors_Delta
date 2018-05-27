.equ servoinfo_lenght, 26

.equ servinf_duty_cycle, 0
.equ servinf_dc_offset,  2
.equ servinf_max_duty_cycle, 4
.equ servinf_min_duty_cycle, 6
.equ servinf_mean_dc, 8
.equ servinf_last_direction, 10
.equ servinf_slack_comp, 12
.equ servinf_move_servo_from, 14
.equ servinf_angle, 16
.equ servinf_m, 18
.equ servinf_n, 22

.extern _servoinfo 

/*********************************************************************
* Macro: get_servoinfo_mem_pos(servo_num)
*
* Overview: Get the memory position of the array servoinfo[servo_num]
*
* PreCondition: This macro doesn't check if servo_num is a valid number
*               Program will have to check it before calling it or i will
*               return an invalid memory position
*               Use the # to pass a literal: get_servo_info_mem_pos #4
*
********************************************************************/

.macro get_servoinfo_mem_pos servo_num ;CHECKED
    
    PUSH W1
    
    MOV #servoinfo_length, W1
    MUL.UU W1, \servo_num, W0
    MOV #_servoinfo, W1
    ADD W0, W1, W0

    POP W1

.endm

/*********************************************************************
* Macro: get_servoinfo_item_mem_pos(servo_num, item)
*
* Overview: Get the memory position of the array servoinfo[servo_num]
*
* PreCondition: This macro doesn't check if servo_num or item_num are valid numbers
*               Program will have to check it before calling it or i will
*               return an invalid memory position
*
********************************************************************/

.macro get_servoinfo_item_mem_pos servo_num, item_num ;CHECKED

    PUSH W1
    
    MOV #servoinfo_lenght, W1
    MUL.UU W1, #\servo_num, W0
    MOV #_servoinfo, W1
    ADD W0, W1, W0
    ADD #\item_num, W0

    POP W1

.endm
    
    