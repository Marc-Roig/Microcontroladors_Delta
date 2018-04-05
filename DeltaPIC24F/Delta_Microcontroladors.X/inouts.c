 #include "main.h"
//Time interruptions

//INITIALIZE AS ANALOG???
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

void init_analog_input(char* pin_name) {



}

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

volatile unsigned int RB3_Analog_Value = 512;
volatile unsigned int RB8_Analog_Value = 512;
volatile unsigned int RB9_Analog_Value = 512;

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

long map(long x, long in_min, long in_max, long out_min, long out_max) {

  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;

}