int scale_angle(int angle) {

    int ms_min = 1;
    int ms_max = 2;
    int ms_duty_cicle = angle * (ms_max - ms_min)/180 + ms_min;
    return ms_duty_cicle * 1024 / 20;

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

void disengage_servos() {

    OC1CON = OC1CON & 0XFFF8;
    OC2CON = OC2CON & 0XFFF8;
    OC3CON = OC3CON & 0XFFF8;

}

void engage_servos() {

    //OUTPUT COMPARE 1
    PR1 = 9999;
    OC1RS = 90;
    OC1R = 90;
    
    OC1CON = OC1CON | 0XFFF6; //PWM MODE ON OC1, FAULT PIN DISABLED
    OC1CON = OC1CON & 0xFFFC; 
    
    T1CON = 0X8010; //T1 PREESCALER 1:8

    //OUTPUT COMPARE 2
    PR2 = 9999;
    OC2RS = 90;
    OC2R = 90;
    
    OC2CON = OC2CON | 0XFFF6; //PWM MODE ON OC2, FAULT PIN DISABLED
    OC2CON = OC2CON & 0xFFFC; 
    
    T2CON = 0X8010; //T2 PREESCALER 1:8
    
    //OUTPUT COMPARE 3
    PR3 = 9999;
    OC3RS = 90;
    OC3R = 90;
    
    OC3CON = OC3CON | 0XFFF6; //PWM MODE ON OC3, FAULT PIN DISABLED
    OC3CON = OC3CON & 0xFFFC; 
    
    T3CON = 0X8010; //T3 PREESCALER 1:8

}