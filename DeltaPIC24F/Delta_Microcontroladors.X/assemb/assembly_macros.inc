#define servoinfo_lenght 16

#define servinf_duty_cycle 0
#define servinf_dc_offset  2
#define servinf_max_duty_cycle 4
#define servinf_min_duty_cycle 6
#define servinf_mean_dc 8
#define servinf_last_direction 10
#define servinf_slack_comp 12
#define servinf_move_servo_from 14
#define servinf_angle 16
#define servinf_m 18
#define servinf_n 20

.extern servoinfo ;check if this works

.macro get_servoinfo_mem_pos servo_num
	
	PUSH W1
	PUSH W2

	MOV #servoinfo, W2
	MUL #\servo_num, #servoinfo_lenght, W1
	ADD W1, W2, W0

	POP W2
	POP W1

.endm

.macro get_servoinfo_item_mem_pos servo_num, item_num

	MOV #servoinfo, W0
	MUL #\servo_num, #servoinfo_lenght, W1
	ADD W1, W0, W0
	ADD #\item_num, W0

.endm