// #include "servos.h"
#include "Config.h"

int scale_angle(unsigned int angle, unsigned int servo_num){
    
    return (int)(angle * servoinfo[servo_num].m + servoinfo[servo_num].n);

}

void servo1_write(unsigned int angle) {

    OC1RS = scale_angle(angle, 0);

}

void servo2_write(unsigned int angle) {

    OC2RS = scale_angle(angle, 1);

}

void servo3_write(unsigned int angle) {

    OC3RS = scale_angle(angle, 2);

}

void servo1_writeMicroseconds(unsigned int microsec) {

    // int us_per_step = 2; //20MS/PR2 = 2us
    OC1RS = microsec;

}


void servo2_writeMicroseconds(unsigned int microsec) {

    OC2RS = microsec/2;

}


void servo3_writeMicroseconds(unsigned int microsec) {

    OC3RS = microsec/2;

}

void servo_writeMicroseconds(unsigned int microsec, unsigned int servo_num) {

    switch (servo_num) {

        case 0: servo1_writeMicroseconds(microsec);
                break;
                
        case 1: servo2_writeMicroseconds(microsec);
                break;
                
        case 2: servo3_writeMicroseconds(microsec);
                break;

    }
}

void disengage_servos() {

    OC1CON = OC1CON & 0XFFF8;
    OC2CON = OC2CON & 0XFFF8;
    OC3CON = OC3CON & 0XFFF8;

}

void servo_attach(unsigned int pin, unsigned int servo_num) {

    //int pin left for compatibility with arduino function

    switch (servo_num) {

        case 0:     PR1 = 9999;
                    OC1RS = 750;
                    OC1R = 750;
                    OC1CON = 0X0006;
                    T1CON = 0X8010; //T1 PREESCALER 1:8

        case 1:     PR2 = 9999;
                    OC2RS = 750;
                    OC2R = 750;
                    OC2CON = 0X0006;
                    T2CON = 0X8010; //T2 PREESCALER 1:8

        case 2:     PR3 = 9999;
                    OC3RS = 750;
                    OC3R = 750;
                    OC3CON = 0X0006;                    
                    T3CON = 0X8010; //T3 PREESCALER 1:8

        case 3:    //There is no pin for the 3rd servo

    }

}

void attach_servos() {

    //OUTPUT COMPARE 1
    PR1 = 9999;
    OC1RS = 750;
    OC1R = 750;
    
    OC1CON = 0X0006;
    
    T1CON = 0X8010; //T1 PREESCALER 1:8

    //OUTPUT COMPARE 2
    PR2 = 9999;
    OC2RS = 750;
    OC2R = 750;
    
    OC2CON = 0X0006;
    
    T2CON = 0X8010; //T2 PREESCALER 1:8
    
    //OUTPUT COMPARE 3
    PR3 = 9999;
    OC3RS = 750;
    OC3R = 750;
    
    OC3CON = 0X0006;
    
    T3CON = 0X8010; //T3 PREESCALER 1:8

}