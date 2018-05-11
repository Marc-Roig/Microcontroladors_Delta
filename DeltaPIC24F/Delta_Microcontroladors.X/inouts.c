#include "Config.h"
//Time interruptions

volatile unsigned int RB3_Analog_Value = 512;
volatile unsigned int RB8_Analog_Value = 512;
volatile unsigned int RB9_Analog_Value = 512;

volatile bool RB3_Analog_Active = false;
volatile bool RB8_Analog_Active = false;
volatile bool RB9_Analog_Active = false;

//------------------------//
//---PIN INITIALIZATION---//
//------------------------//

    void pinMode(int pin_name, int mode) {

        switch (pin_name) {

            case IO_RB3:        if (mode == INPUT) _TRISB3 = 1;
    //                            else if(mode == INPUT_PULLUP);
    //                            else if(mode == INUT_PULLDOWN);
                                else if (mode == OUTPUT) _TRISB3 = 0;
                                else if (mode == ANALOG_INPUT) init_analog_input(pin_name);
                                break;

            case IO_RB8:        if (mode == INPUT) _TRISB8 = 1;
                                else if (mode == OUTPUT) _TRISB8 = 0;
                                else if (mode == ANALOG_INPUT) init_analog_input(pin_name);
                                break;

            case IO_RB9:        if (mode == INPUT) _TRISB9 = 1;
                                else if (mode == OUTPUT) _TRISB9 = 0;
                                else if (mode == ANALOG_INPUT) init_analog_input(pin_name);
                                break;

            case IO_RE8:        if (mode == INPUT) _TRISE8 = 1;
                                else if (mode == OUTPUT) _TRISE8 = 0;
                                break;

            case IO_RE9:        if (mode == INPUT) _TRISE9 = 1;
                                else if (mode == OUTPUT) _TRISE9 = 0;
                                break;

            case IO_RA14:       if (mode == INPUT) _TRISA14 = 1;
                                else if (mode == OUTPUT) _TRISA14 = 0;
                                break;

            case IO_RA15:       if (mode == INPUT) _TRISA15 = 1;
                                else if (mode == OUTPUT) _TRISA15 = 0;
                                break;

        }

    }

//-------------------//
//----ANALOG PINS----//
//-------------------//


/*********************************************************************
* Function: void init_ADC();
*
* Overview: Initialize all board registers to read RB3, RB8 or RB9
*           analog values. If any of this pins was initialized to
*           analog input and then changed to digital this function
*           has to be called again to update the registers.
*
* PreCondition: All pins to be set to Analog input have to be initialized
*               with pinmode(pin, ANALOG_INPUT) before calling this function
*
********************************************************************/

void init_ADC() {

    int num_of_active_pins = 0;

    AD1PCFG = 0xffff; // Set all pins as digital pins
    AD1CSSL = 0x0000; // Include none of the channels in scan

    if (RB3_Analog_Active) {
        _TRISB3 = 1; // Set RB3 as input
        _PCFG3 = 0; //Set RB3 as analog input
        _CSSL3 = 1; //Include RB3 in scan
        num_of_active_pins++;
    }

    if (RB8_Analog_Active) {
        _TRISB8 = 1; // Set RB3 as input
        _PCFG8 = 0; //Set RB8 as analog input
        _CSSL8 = 1; //Include RB8 in scan
        num_of_active_pins++;
    }

    if (RB9_Analog_Active) {
        _TRISB9 = 1; // Set RB3 as input
        _PCFG9 = 0; //Set RB9 as analog input
        _CSSL9 = 1; //Include RB9 in scan
        num_of_active_pins++;
    }

    if (num_of_active_pins == 0) {

        AD1CON1bits.ADON = 0;
        AD1CON1bits.ASAM = 0;
        _AD1IE = 0;
        return; //No need to read any value

    }

    AD1CON1 = 0x00E0; // Internal counter triggers conversion
    // AD1CON3 = 0x1F10; // Sample time = 31Tad, Tad = Tcy, A/D Conversion Clock Period = 256 Tcy
    AD1CON3 = 0x0F00; // Sample time = 15Tad
    AD1CON2 = 0x0400 + ((num_of_active_pins - 1) << 2); // Enable Scaning, set AD1IF after every (num_of_active_pins) samples

    _AD1IF = 0; //turn off interrupt flag before enabling interrupt
    _AD1IE = 1; //enable analog interrupt

    AD1CON1bits.ASAM = 1;
    
    AD1CON1bits.ADON = 1; // turn ADC ON

}


/*********************************************************************
* Function: void init_analog_input(int pin_name);
*
* Overview: Internal function to activate RB3, RB8, RB9 to analog.
*           After activating all analog pins needed init_ADC() must
*           be called.
*
********************************************************************/

void init_analog_input(int pin_name) {

    switch (pin_name) {

        case IO_RB3:    RB3_Analog_Active = true;
                        break;

        case IO_RB8:    RB8_Analog_Active = true;
                        break;

        case IO_RB9:    RB9_Analog_Active = true;
                        break;

    }

}

/*********************************************************************
* Function: void turn_off_analog_input(int pin_name);
*
* Overview: Internal function to deactivate RB3, RB8, RB9 from analog.
*           After deactivating all analog pins needed init_ADC() must
*           be called.
*
********************************************************************/

void turn_off_analog_input(int pin_name) {

    switch (pin_name) {

        case IO_RB3:    RB3_Analog_Active = false;
                        break;

        case IO_RB8:    RB8_Analog_Active = false;
                        break;

        case IO_RB9:    RB9_Analog_Active = false;
                        break;

    }
}

/*********************************************************************
* Function: void _ISR _ADC1Interrupt();
*
* Overview: Analog reading interrupt. The measurements are saved in a
*           buffer (ADC1BUF0).
*
********************************************************************/

void _ISR _ADC1Interrupt() {

    int *ADC16Ptr;
    
    ADC16Ptr = &ADC1BUF0;
    // AD1CON1bits.ASAM = 0; // Stop sampling

    if (RB3_Analog_Active) RB3_Analog_Value = *ADC16Ptr++;
    if (RB8_Analog_Active) RB8_Analog_Value = *ADC16Ptr++;
    if (RB9_Analog_Active) RB9_Analog_Value = *ADC16Ptr++;

    // AD1CON1bits.ASAM = 1; // Continue sampling
    _AD1IF = 0;

}

/*********************************************************************
* Function: void ADC_update_values();
*
* Overview: An alternative to the analog interrupt. This function can be called
*           from a time interrupt or from main loop if analog interrupt is 
*           disabled, thus allowing you to refresh values with a more precise
*           timing.
*
********************************************************************/

void ADC_update_values() {

    static unsigned int ADC_timer = 0;

    ADC_timer++;

        if (ADC_timer >= MS_BETWEEN_ADC_CONVERSION) {

            unsigned int *ADC16Ptr;
            ADC16Ptr = &ADC1BUF0;

            if (IFS0bits.AD1IF) {
                //read as many of active analog inputs
                if (RB3_Analog_Active) RB3_Analog_Value = *ADC16Ptr++;
                if (RB8_Analog_Active) RB8_Analog_Value = *ADC16Ptr++;
                if (RB9_Analog_Active) RB9_Analog_Value = *ADC16Ptr++;

                IFS0bits.AD1IF = 0;
            }
            
            ADC_timer = 0;

        }


}


/*********************************************************************
* Function: analogRead(int pin_num)
*
* Overview: Returns the values of the analog pins. Currently it does not check
*           if the pins are initialized as analog pins. This functions is 
*           the one to be called in program.
*
********************************************************************/

int analogRead(int pin_num) {
    //In the initialization make a poll read (lectura per enquesta)
    //to get the real value of the analog pin

    switch (pin_num) {

        case IO_RB3:    return RB3_Analog_Value;
                        break;

        case IO_RB8:    return RB8_Analog_Value;
                        break;

        case IO_RB9:    return RB9_Analog_Value;
                        break;
                        
        default:        return 512;
                        break;

    }
    
}

//--------------------//
//----DIGITAL PINS----//
//--------------------//

/*********************************************************************
* Function: digitalWrite(int pin_num, int value);
*
* Overview: Write a value on a previously set as a digital output pin.
*
********************************************************************/

void digitalWrite(int pin_num, int value) {

    if (value > 1) return; //Valid values: 0, 1

    switch (pin_num) {

        case IO_RB3:         if (_TRISB3 == 0) _LATB3 = value;
                            break;

        case IO_RB8:         if (_TRISB8 == 0) _LATB8 = value;
                            break;

        case IO_RB9:         if (_TRISB9 == 0) _LATB9 = value;
                            break;

        case IO_RE8:         if (_TRISE8 == 0) _LATE8 = value;
                            break;

        case IO_RE9:         if (_TRISE9 == 0) _LATE9 = value;
                            break;

        case IO_RA14:        if (_TRISA14 == 0) _LATA14 = value;
                            break;

        case IO_RA15:        if (_TRISA15 == 0) _LATA15 = value;
                            break;

    }

}

/*********************************************************************
* Function: digitalRead(int pin_num);
*
* Overview: Read a value of a previously set as a digital input pin.
*
********************************************************************/

int digitalRead(int pin_num) {
    
    switch (pin_num) {

        case IO_RB3:         if (_TRISB3 == 1) return _RB3;
                            break;

        case IO_RB8:         if (_TRISB8 == 1) return _RB8;
                            break;

        case IO_RB9:         if (_TRISB9 == 1) return _RB9;
                            break;

        case IO_RE8:         if (_TRISE8 == 1) return _RE8;
                            break;

        case IO_RE9:         if (_TRISE9 == 1) return _RE9;
                            break;

        case IO_RA14:        if (_TRISA14 == 1) return _RA14;
                            break;

        case IO_RA15:        if (_TRISA15 == 1) return _RA15;
                            break;

    }
    
    return LOW;
} 

//----------------------//
//----MISC FUNCTIONS----//
//----------------------//

/*********************************************************************
* Function: map(long x, long in_min, long in_max, long out_min, long out_max);
*
* Overview: Change range of value.
*
********************************************************************/

long map(long x, long in_min, long in_max, long out_min, long out_max) {

  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;

}

/*********************************************************************
* Function: abs2(float val);
*
* Overview: Returns absolute value of a number. Could not call it abs
*           due to some incompatibilities with the mplab ide.
*
********************************************************************/

float abs2(float val) {

    if (val < 0) return val*(-1.0);
    return val;
}