.include "p24fj128ga010.inc"

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




