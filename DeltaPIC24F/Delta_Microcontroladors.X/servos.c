// #include "servos.h"
#include "main.h"

int scale_angle(int angle){
    int ms_min = 1;
    int ms_max = 2;
    int ms_duty_cycle = angle * (ms_max - ms_min)/180 + ms_min;
    return ms_duty_cycle * 1024 / 20;
}

void servo1_write(int angle) {

    OC1RS = scale_angle(angle);

}


void servo2_write(int angle) {

    OC2RS = scale_angle(angle);

}

void servo3_write(int angle) {

    OC3RS = scale_angle(angle);

}

void servo1_write_duty_cycle(int duty_cycle) {

    OC1RS = duty_cycle;
    
}

void servo2_write_duty_cycle(int duty_cycle) {

    OC2RS = duty_cycle;
    
}

void servo3_write_duty_cycle(int duty_cycle) {

    OC3RS = duty_cycle;
    
}

void servo1_writeMicroseconds(int duty_cycle) {

    // int us_per_step = 2; //20MS/PR2 = 2us
    OC1RS = duty_cycle/2;

}


void servo2_writeMicroseconds(int duty_cycle) {

    OC2RS = duty_cycle/2;

}


void servo3_writeMicroseconds(int duty_cycle) {

    OC3RS = duty_cycle/2;

}

void disengage_servos() {

    OC1CON = OC1CON & 0XFFF8;
    OC2CON = OC2CON & 0XFFF8;
    OC3CON = OC3CON & 0XFFF8;

}

void engage_servos() {

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