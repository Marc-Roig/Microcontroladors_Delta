.include "p24fj128ga010.inc"
.include "assembly_routines.inc"

.extern _RB3_Analog_Active
.extern _RB8_Analog_Active
.extern _RB9_Analog_Active

.extern _RB3_Analog_Value
.extern _RB8_Analog_Value
.extern _RB9_Analog_Value


;/*********************************************************************
;* Function: _pinMode
;*
;* Overview: Set a pin to digital input, digital output, analog input
;*
;*			OUTPUT 0
;*			INPUT 1
;*			ANALOG_INPUT 2
;*           
;* PreCondition: none
;*
;* Input: int pin_num - W0 - pin number (numeration at inouts.h)
;*		 int mode 	 - W1 - set as digital input, output, analog input
;*
;* Output: none
;*
;********************************************************************/

_pinMode: ;CHECKED
	; int pin_num at W0
	; int mode at W1

	CP W1, #3 ; check if modes are valid, there are only 3 (0 to 2)
	BRA GE, END_PINMODE

	ANALOG:

		CP W1, #2 ;if mode == 2 its analog
		BRA NZ, DIGITAL

		CP W0, #3	;only RB3, RB8 and RB9 can be analog
		BRA GE, END_PINMODE
		
		BRA PM_ANALOG_INPUT
		

	DIGITAL:

		CP W0, #7 ; Check if input is valid, there are only 7 pins (0 to 6)
		BRA GE, END_PINMODE

		CP W1, #1 ; OUTPUT = 0
		BRA NZ, DIGITAL_OUTPUT
		
		BRA DIGITAL_INPUT


	DIGITAL_INPUT:
		
		SL W0, W0 ; pin_num * 2 -> num of lines to jump
		BRA W0
		
		BSET TRISB, #3
		BRA END_PINMODE
		BSET TRISB, #8
		BRA END_PINMODE
		BSET TRISB, #9
		BRA END_PINMODE
		BSET TRISE, #8
		BRA END_PINMODE
		BSET TRISE, #9
		BRA END_PINMODE
		BSET TRISA, #14
		BRA END_PINMODE
		BSET TRISA, #15
		BRA END_PINMODE

	DIGITAL_OUTPUT:
    		
		SL W0, W0 ; pin_num * 2 -> num of lines to jump
		BRA W0
		
		BCLR TRISB, #3
		BRA END_PINMODE
		BCLR TRISB, #8
		BRA END_PINMODE
		BCLR TRISB, #9
		BRA END_PINMODE
		BCLR TRISE, #8
		BRA END_PINMODE
		BCLR TRISE, #9
		BRA END_PINMODE
		BCLR TRISA, #14
		BRA END_PINMODE
		BCLR TRISA, #15
		BRA END_PINMODE

	PM_ANALOG_INPUT:
    
		SL W0, W0 ; pin_num * 2 -> num of lines to jump
		BRA W0
		
		BSET _RB3_Analog_Active, #0
		BRA END_PINMODE
		BSET _RB8_Analog_Active, #0
		BRA END_PINMODE
		BSET _RB9_Analog_Active, #0
		BRA END_PINMODE

	END_PINMODE:

	RETURN

;/*********************************************************************
;* Function: digitalWrite(int pin_num, int value);
;*
;* Overview: Write a value on a previously set as a digital output pin.
;*
;********************************************************************/

_digitalWrite:          ;CHECKED
    ;W0 = int pin_num
    ;W1 = int value -> Valid values: 0, 1
    
    CP W1, #1
    BRA GT, End_Write ;Com que el pin és digital només hi podem escriure 1 o 0
    
	;Mirem a quin dels pins hem d'escriure
    CP W0, #0
    BRA Z, io_RB3
    CP W0, #1
    BRA Z, io_RB8
    CP W0, #2
    BRA Z, io_RB9
    CP W0, #3
    BRA Z, io_RE8
    CP W0, #4
    BRA Z, io_RE9
    CP W0, #5
    BRA Z, io_RA14
    CP W0, #6
    BRA Z, io_RA15
       
    io_RB3:
    BTST _TRISB, #3     ;Comprobem que el Pin està configurat com a digital = 0
    BRA NZ, End_Write   ;Si no és digital, no fem res
    CP W1, #0       ;Mirem quin valor té value
    BRA Z, Posar0_RB3  
    BSET LATB, #3       ;Si value = 1
    BRA End_Write
    
    Posar0_RB3:     ;Si value = 0
        BCLR LATB, #3
        BRA End_Write
    
    io_RB8:
	    BTST _TRISB, #8     ;Comprobem que el Pin està configurat com a digital = 0
	    BRA NZ, End_Write   ;Si no és digital, no fem res
	    CP W1, #0       ;Mirem quin valor té value
	    BRA Z, Posar0_RB8
	    BSET LATB, #8       ;Si value = 1
	    BRA End_Write
    
    Posar0_RB8:
        BCLR LATB, #8   ;Si value = 0
        BRA End_Write
    
    io_RB9:
	    BTST _TRISB, #9
	    BRA NZ, End_Write
	    CP W1, #0
	    BRA Z, Posar0_RB9
	    BSET LATB, #9
	    BRA End_Write
    
    Posar0_RB9:
        BCLR LATB, #9
        BRA End_Write    
                
    io_RE8:
	    BTST _TRISE, #8
	    BRA NZ, End_Write
	    CP W1, #0
	    BRA Z, Posar0_RE8
	    BSET LATE, #8
	    BRA End_Write
    
    Posar0_RE8:
        BCLR LATE, #8
        BRA End_Write

    io_RE9:
	    BTST _TRISE, #9
	    BRA NZ, End_Write
	    CP W1, #0
	    BRA Z, Posar0_RE9
	    BSET LATE, #9
	    BRA End_Write
    
    Posar0_RE9:
        BCLR LATE, #9
        BRA End_Write
    
    io_RA14:
        BTST _TRISA, #14
	    BRA NZ, End_Write
	    CP W1, #0
	    BRA Z, Posar0_RA14
	    BSET LATA, #14
	    BRA End_Write
    
    Posar0_RA14:
        BCLR LATA, #14
        BRA End_Write
    

    io_RA15:
	    BTST _TRISA, #15
	    BRA NZ, End_Write
	    CP W1, #0
	    BRA Z, Posar0_RA15
	    BSET LATA, #15
	    BRA End_Write
    
    Posar0_RA15:
        BCLR LATA, #15
        BRA End_Write
    
	End_Write:
    RETURN


;/*********************************************************************
;* Function: _digitalRead:
;*
;* Overview: Read state of a pin previously set as digital input.
;*           
;* PreCondition: Set pin as digital input in pinMode
;*
;* Input: int pin_num - W0 - pin number (numeration at inouts.h)
;*
;* Output: bool pin_state - W0 - state of the required pin
;*
;********************************************************************/

_digitalRead: ;CHECKED     
    
    CP W0, #0	;En funció del pin_num entrarà en una etiqueta o altra
    BRA Z, Io_RB3_digitalR
    CP W0, #1
    BRA Z, Io_RB8_digitalR
    CP W0, #2
    BRA Z, Io_RB9_digitalR
    CP W0, #3
    BRA Z, Io_RE8_digitalR
    CP W0, #4
    BRA Z, Io_RE9_digitalR
    CP W0, #5
    BRA Z, Io_RA14_digitalR
    CP W0, #6
    BRA Z, Io_RA15_digitalR
    
    Io_RB3_digitalR:
    BTST _TRISB, #3     ;Com que mirem 1 bit, només pot ser 0 o 1.
    BRA Z, End_Return   ;Comprovem que està configurat com a input pin. Si no, return LOW
    BTST PORTB, #3      ;Hem de saber quant val _RB3 per assignar-li el valor corresponent (0/1)
    BRA Z, Escriure0
    MOV #1, W0
    BRA End_DigitalR
	;Fem els mateixos passos per a diferents pins
    Io_RB8_digitalR:
    BTST _TRISB, #8     
    BRA Z, End_Return
    BTST PORTB, #8
    BRA Z, Escriure0
    MOV #1, W0
    BRA End_DigitalR
    
    Io_RB9_digitalR:
    BTST _TRISB, #9     
    BRA Z, End_Return
    BTST PORTB, #9
    BRA Z, Escriure0
    MOV #1, W0
    BRA End_DigitalR
    
    Io_RE8_digitalR:
    BTST _TRISE, #8     
    BRA Z, End_Return
    BTST PORTE, #8
    BRA Z, Escriure0
    MOV #1, W0
    BRA End_DigitalR
    
    Io_RE9_digitalR:
    BTST _TRISE, #9     
    BRA Z, End_Return
    BTST PORTE, #9
    BRA Z, Escriure0
    MOV #1, W0
    BRA End_DigitalR
    
    Io_RA14_digitalR:
    BTST _TRISA, #14    
    BRA Z, End_Return
    BTST PORTA, #14
    BRA Z, Escriure0
    MOV #1, W0
    BRA End_DigitalR
    
    Io_RA15_digitalR:
    BTST _TRISA, #15    
    BRA Z, End_Return
    BTST PORTA, #15
    BRA Z, Escriure0
    MOV #1, W0
    BRA End_DigitalR
    
	Escriure0: 
    MOV #0, W0
    BRA End_DigitalR
	End_Return:
    MOV  #0,W0      ;Cas que el pin està configurat com a output pin
    
	End_DigitalR:
    RETURN    

;/*********************************************************************
;* Function: void init_analog_input(int pin_name);
;*
;* Overview: Internal function to activate RB3, RB8, RB9 to analog.
;*           After activating all analog pins needed init_ADC() must
;*           be called.
;*
;********************************************************************/

_init_analog_input:     ;CHECKED
    
    ;W0 = pin_name, ho passa la funció
	;En funció del pin_num entrarà en una etiqueta o altra
    PUSH W1
    MOV #1, W1
    CP W0, #0
    BRA Z, io_RB3_init
    CP W0, #1 
    BRA Z, io_RB8_init
    CP W0, #2
    BRA Z, io_RB9_init
    
    
    io_RB3_init:
    MOV W1, _RB3_Analog_Active      
    BRA End_init

    io_RB8_init:
    MOV W1, _RB8_Analog_Active
    BRA End_init
    
    io_RB9_init:
    MOV W1, _RB9_Analog_Active
    BRA End_init
    
    POP W1
	End_init:
    RETURN


;/*********************************************************************
;* Function: _analogRead:
;*
;* Overview: Read value of a pin previously set as analog input.
;*			Only RB3, RB8 and RB9 can be set as analog input in 
;*			pinMode().
;*           
;* PreCondition: Set pin as analog input in pinMode().
;*
;* Input: int pin_num - W0 - pin number (numeration at inouts.h)
;*
;* Output: bool pin_state - W0 - state of the required pin
;*
;********************************************************************/

; _analogRead: 
;
;	;W0 int pin_num
;
;	CP W0, #3
;	BRA GE, End_analogRead
;	
;	SL W0, W0, W0;Jump 2 lines for every case
;	BRA W0 ; PC += (W0*2)
;
;	IO_RB3_AR:
;
;		MOV _RB3_Analog_Value, W0
;		BRA End_analogRead
;
;	IO_RB8_AR:
;
;		MOV _RB8_Analog_Value, W0
;		BRA End_analogRead
;
;	IO_RB9_AR:
;
;		MOV _RB9_Analog_Value, W0
;		BRA End_analogRead
;
;	End_analogRead:
;
;	RETURN


    
;/*********************************************************************
;* Function: _map2:
;*
;* Overview: Re-maps a number from one range to another. That is, a value of
;*			fromLow would get mapped to toLow, a value of fromHigh to toHigh, 
;*			values in-between to values in-between, etc. Does not constrain 
;*			values to within the range, because out-of-range values are sometimes 
;*			intended and useful.
;*
;*			Used c code from Arduino reference page:
;*			https://www.arduino.cc/reference/en/language/functions/math/map/
;*           
;* PreCondition: In assembly all parameters have to be pushed into the stack before
;*				calling this function in the following order
;*
;* Input: long x - [W14-26] - value to remap
;*		 long in_min - [W14-22] - the lower bound of the value's current range
;*		 long in_max - [W14-16] - the upper bound of the value's current range
;*		 long out_min - [W14-14] - the lower bound of the value's target range
;*		 long out_max - [W14-10] - the upper bound of the value's target range
;*
;* Output: long mapped_value - W0, W1 - The mapped value
;*
;********************************************************************/

_map2: ;CHECKED (NOT FOR NEGATIVE NUMBERS)

	LNK #4 ;Save two positions in stack to save intermediate values

	PUSH W2 ;W2 and W3 are modified in this routine
	PUSH W3

	MOV [W14-26], W0 ; x lower word
	MOV [W14-24], W1 ; x upper word
	MOV [W14-22], W2 ; in_min lower word
	MOV [W14-20], W3 ; in_min upper word

	RCALL _sub_longs ; (x - in_min)

	MOV.D W0, [W14] ; Store value in stack

	MOV [W14-10], W0 ; out_max lower word
	MOV [W14-8], W1  ; out_max upper_word
	MOV [W14-14], W2 ; out_min lower_word
	MOV [W14-12], W3 ; out_min upper_word

	RCALL _sub_longs ;(out_max - out_min) 

	MOV [W14], W2 
	MOV [W14+2], W3

	RCALL _mul_longs ;(x - in_min) * (out_max - out_min) 

	MOV.D W0, [W14] 

	MOV [W14-18], W0 ; in_max lower word 
	MOV [W14-16], W1 ; in_max upper word
	MOV [W14-22], W2 ; in_min lower word
	MOV [W14-20], W3 ; in_min upper word

	RCALL _sub_longs ; (in_max - in_min)

	MOV W0, W2 ;move result of substraction to w2 and w3
	MOV W1, W3

	MOV [W14], W0 
	MOV [W14+2], W1 ; result of (x - in_min) * (out_max - out_min) 

	RCALL _div_longs ; (x - in_min) * (out_max - out_min) / (in_max - in_min)

	MOV [W14-14], W2 ; out_min lower word
	MOV [W14-12], W3 ; out_min upper word

	RCALL _add_longs ; (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min

	POP W3
	POP W2
	
	ULNK
	
	RETURN