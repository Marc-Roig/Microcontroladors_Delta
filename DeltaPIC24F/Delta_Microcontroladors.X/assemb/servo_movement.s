.include "p24fj128ga010.inc"

.extern _servoinfo

_init_ServoInfo:
	
	; W0 ServoInfo adress
	; W1 MAX DUTY CYCLE
	; W2 MIN DUTY CYCLE
	; W3 SLACK COMPENSATION VAL
	; W4 M_ 1
	; W5 M_ 2
	; W6 N_ 1
	; W7 N_ 2

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

	;servo_inf->duty_cycle = servo_inf->mean_dc - 500;
	SUB W1, #500, W1 ;meand_dc was stored in W1 from previous operation
	MOV W1, [W0]

	;servo_inf->last_direction = 0;
	MOV W8, [W0+10] ;W8 was cleared for dc_offset = 0

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


_check_servo_change_direction:
	
	;W0 unsigned int num_servo
	;W1 unsigned int new_duty_cycle

	CP W0, #4
	BRA GE, End

	PUSH W2
	PUSH W3
	PUSH W4
	PUSH W5
	PUSH W6
	
	MUL #26, W0, W2
	ADD #_servoinfo, W2, W2
	ADD W2, #10, W3 ;@servoinfo[servo_num].last_direction
	ADD W2, #0 , W4 ;@servoinfo[servo_num].duty_cycle
	ADD W2, #2 , W5 ;@servoinfo[servo_num].dc_offset
	ADD W2, #12, W6 ;@servoinfo[servo_num].slack_compensation_val

	ADD [W4], #20, W2
	CP W1, W2
	BRA LT, Duty_cycle_smaller:


	Duty_cycle_greater:

		CP [W14], #COUNTERCLOCKWISE
		BRA NZ, End_pop
		MOV #CLOCKWISE, [W3]
		MOV [W6], W6 ;servoinfo[servo_num].slack_compensation_val
		MOV [W5], W2 ;servoinfo[servo_num].dc_offset
		ADD W2, W6, W2
		MOV W2, [W5] ;dc_offset += slack_compensation_val
		BRA End

	Duty_cycle_smaller:

		CP [W14], #CLOCKWISE
		BRA NZ, End_pop
		MOV #COUNTERCLOCKWISE, [W3]
		MOV [W6], W6 ;servoinfo[servo_num].slack_compensation_val
		MOV [W5], W2 ;servoinfo[servo_num].dc_offset
		SUB W2, W6, W2
		MOV W2, [W5] ;dc_offset -= slack_compensation_val
		BRA End_pop
	

	End_pop:

		POP W2
		POP W3
		POP W4
		POP W5
		POP W6


	End:

	RETURN

_set_servo_movement_with_dc:
	
	;W0 -> bool set_servo1
	;W1 -> bool set_servo2
	;W2 -> bool set_servo3
	;W3 -> bool set_servo4

	PUSH W4

	Set_servo1:

		CP W0, #1
		ADD #_servoinfo, #16, W4 ; @servoinfo[0].move_servo_from 
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









