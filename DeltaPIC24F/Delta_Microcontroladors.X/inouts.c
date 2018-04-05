 #include "main.h"
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

void pinMode(char* pin_name, char* mode) {

    switch (pin_name) {

        case "RB3":         if (mode == INPUT) _TRISB3 = 1;
                            else if (mode == OUTPUT) _TRISB3 = 0;
                            else if (mode == ANALOG_INPUT) init_analog_input(pin_name);
                            break;

        case "RB8":         if (mode == INPUT) _TRISB8 = 1;
                            else if (mode == OUTPUT) _TRISB8 = 0;
                            else if (mode == ANALOG_INPUT) init_analog_input(pin_name);
                            break;

        case "RB9":         if (mode == INPUT) _TRISB9 = 1;
                            else if (mode == OUTPUT) _TRISB9 = 0;
                            else if (mode == ANALOG_INPUT) init_analog_input(pin_name);
                            break;

        case "RE8":         if (mode == INPUT) _TRISE8 = 1;
                            else if (mode == OUTPUT) _TRISE8 = 0;
                            break;

        case "RE9":         if (mode == INPUT) _TRISE9 = 1;
                            else if (mode == OUTPUT) _TRISE9 = 0;
                            break;

        case "RA14":        if (mode == INPUT) _TRISA14 = 1;
                            else if (mode == OUTPUT) _TRISA14 = 0;
                            break;

        case "RA15":        if (mode == INPUT) _TRISA15 = 1;
                            else if (mode == OUTPUT) _TRISA15 = 0;
                            break;

    }

}

//-------------------//
//----ANALOG PINS----//
//-------------------//

//Call when the pins are set to analog mode
void init_ADC() {

    int num_of_active_pins = 0;

    AD1PCFG = 0xffff; // Set all pins as digital pins
    AD1CSSL = 0x0000; // Include none of the channels in scan

    if (RB3_Analog_Active) {
        _PCFG3 = 0; //Set RB3 as analog input
        _CSSL3 = 1; //Include RB3 in scan
        num_of_active_pins++;
    }

    if (RB8_Analog_Active) {
        _PCFG8 = 0; //Set RB8 as analog input
        _CSSL8 = 1; //Include RB8 in scan
        num_of_active_pins++;
    }

    if (RB9_Analog_Active) {
        _PCFG9 = 0; //Set RB9 as analog input
        _CSSL9 = 1; //Include RB9 in scan
        num_of_active_pins++;
    }

    if (num_of_active_pins == 0) return; //No need to read any value

    AD1CON1 = 0x00E0; // Internal counter triggers conversion
    AD1CON3 = 0x1F10; // Sample time = 31Tad, Tad = Tcy, A/D Conversion Clock Period = 256 Tcy
    // AD1CON3 = 0x1f10; // Sample time = 15Tad
    AD1CON2 = 0x0400 + ((num_of_active_pins) << 2); // Enable Scaning, set AD1IF after every (num_of_active_pins) samples

    _AD1IF = 0; //turn off interrupt flag before enabling interrupt
    _AD1IE = 1; //enable analog interrupt

    AD1CON1bits.ADON = 1; // turn ADC ON

}

void init_analog_input(char* pin_name) {

    switch (pin_num) {

        case "RB3":     RB3_Analog_Active = true;
                        _PCFG3 = 0;
                        break;

        case "RB8":     RB8_Analog_Active = true;
                        _PCFG8 = 0;
                        break;

        case "RB9":     RB9_Analog_Active = true;
                        _PCFG9 = 0;
                        break;

    }

}

void _ISR _ADCInterrupt() {

    int* ADC1BUF_Pointer = &ADC1BUF0;

    AD1CON1bits.ASAM = 0; // Stop sampling

    if (RB3_Analog_Active) RB3_Analog_Value = *(ADC1BUF_Pointer + 3);
    if (RB8_Analog_Active) RB8_Analog_Value = *(ADC1BUF_Pointer + 8);
    if (RB9_Analog_Active) RB9_Analog_Value = *(ADC1BUF_Pointer + 9);

    AD1CON1bits.ASAM = 1; // Continue sampling
    _AD1IF = 0; // Baixar flanc al acabar la interrupicó

}


int analogRead(int pin_num) {
    //In the initialization make a poll read (lectura per enquesta)
    //to get the real value of the analog pin

    switch (pin_num) {

        case "RB3":     return RB3_Analog_Value;
                        break;

        case "RB8":     return RB8_Analog_Value;
                        break;

        case "RB9":     return RB9_Analog_Value;
                        break;

    }

}

//--------------------//
//----DIGITAL PINS----//
//--------------------//

void digitalWrite(int pin_num, unsigned int value) {

    if (value > 1) return; //Valid values: 0, 1

    switch (pin_num) {

        case "RB3":         if (_TRISB3 == 0) _LATB3 = value;
                            break;

        case "RB8":         if (_TRISB8 == 0) _LATB8 = value;
                            break;

        case "RB9":         if (_TRISB9 == 0) _LATB9 = value;
                            break;

        case "RE8":         if (_TRISE8 == 0) _LATE8 = value;
                            break;

        case "RE9":         if (_TRISE9 == 0) _LATE9 = value;
                            break;

        case "RA14":        if (_TRISA14 == 0) _LATA14 = value;
                            break;

        case "RA15":        if (_TRISA15 == 0) _LATA15 = value;
                            break;

    }

}

int digitalRead(int pin_num) {

    switch (pin_num) {

        case "RB3":         if (_TRISB3 == 1) return _RB3;
                            break;

        case "RB8":         if (_TRISB8 == 1) return _RB8;
                            break;

        case "RB9":         if (_TRISB9 == 1) return _RB9;
                            break;

        case "RE8":         if (_TRISE8 == 1) return _RE8;
                            break;

        case "RE9":         if (_TRISE9 == 1) return _RE9;
                            break;

        case "RA14":        if (_TRISA14 == 1) return _RA14;
                            break;

        case "RA15":        if (_TRISA15 == 1) return _RA15;
                            break;

        default:            return LOW;
                            break;
    }
} 

//----------------------//
//----MISC FUNCTIONS----//
//----------------------//

long map(long x, long in_min, long in_max, long out_min, long out_max) {

  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;

}