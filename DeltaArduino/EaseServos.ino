#include "Config.h"

ServoEaser servoseased[4];

// Code adapted from: https://github.com/todbot/ServoEaser
// Easer functions from: https://github.com/jesusgollonet/ofpennereasing


inline float ServoEaser_easeInOutCubic(float t, float b, float c, float d) {

    if ((t/=d/2) < 1) return c/2*t*t*t + b;
    return c/2*((t-=2)*t*t + 2) + b;

}

inline float ServoEaser_easeInOut(float t,float b , float c, float d) {

    return c*t/d + b;

}

inline float ServoEaser_SquaredeaseInOut(float t,float b , float c, float d) {

    if ((t/=d/2) < 1) return ((c/2)*(t*t)) + b;
    return -c/2 * (((--t)*(t-2)) - 1) + b;

}

inline float ServoEaser_QuarteaseInOut(float t,float b , float c, float d) {

    if ((t/=d/2) < 1) return c/2*t*t*t*t + b;
    return -c/2 * ((t-=2)*t*t*t - 2) + b;

}

inline float ServoEaser_QuinteaseInOut(float t,float b , float c, float d) {

    if ((t/=d/2) < 1) return c/2*t*t*t*t*t + b;
    return c/2*((t-=2)*t*t*t*t + 2) + b;

}

inline void ServoEaser_stop_until_confirmation() {

    delay(1000);
    // servoeaser.stop_ease();
    return;

}

void ServoEaser::update() {

    if( ((millis() - lastMillis) < frameMillis + delay_after_move) || !running ) return;
    lastMillis = millis();
    delay_after_move = 0;

    currPos = easingFunc( tick, startPos, changePos, tickCount );

    if( !arrived ) tick++;

    servoinfo[servo_num].duty_cycle = currPos;

    // if(servo_num == 0) Serial.println(currPos);

    if( tick == tickCount + 1) getNextPos(); 

}

void ServoEaser::getNextPos() {

    if (arrivedFunc != NULL) arrivedFunc(servo_num);

    arrived = true;

    if (buffer_empty) {

        if (bufferemptiedFunc != NULL) bufferemptiedFunc(servo_num);
        running = false;
        return;
    
    }

    startPos  = currPos; // current position becomes new start position

    changePos = moves[buffer_start] - startPos;
    durMillis = moves_dur[buffer_start];

    inc_moves_start_pointer();

    tickCount = durMillis / frameMillis;
    tick = 0;

    arrived = false;

}

bool ServoEaser::inc_moves_end_pointer() {

    buffer_end = (buffer_end + 1) % MOVES_BUFFER_LEN;
    buffer_empty = false;

    if (buffer_end == buffer_start) {

        buffer_full = true;
        return true;   

    }

    return false;

}

bool ServoEaser::inc_moves_start_pointer() {

    buffer_start = (buffer_start + 1) % MOVES_BUFFER_LEN;
    buffer_full = false;

    if (buffer_end == buffer_start) {
        buffer_empty = true;
        return true;
    }
    return false;   

}

void ServoEaser::init_moves_buffer() {

    buffer_start = 0;
    buffer_end = 0;
    buffer_empty = true;
    buffer_full = false;
  
}

// set up an easer with just a servo and a starting position
void ServoEaser::init(int servo_num_, int frameTime) {

    servo_num = servo_num_;

    init_moves_buffer();

    arrived = true;
    running = false;

    frameMillis = frameTime;
    delay_after_move = 0;

    easingFunc = ServoEaser_easeInOutCubic;
    arrivedFunc = NULL;
    bufferemptiedFunc = NULL;
    
}

// reset easer to initial conditions, does not nuke easingFunc or arrivedFunc
void ServoEaser::reset() {

    currPos = servoinfo[servo_num].duty_cycle;
    startPos = currPos;  // get everyone in sync
    changePos = 0;       // might be overwritten below

    if( !buffer_empty ) {
        changePos = moves[buffer_start] - startPos ;
        durMillis = moves_dur[buffer_start];
    }

    inc_moves_start_pointer();

    tickCount = (durMillis / frameMillis);
    tick = 0;
    
}

void ServoEaser::addMoves(int inp_moves[], int durations[], int moves_len) {

    for (int i = 0; i < moves_len; i++) {

        if (!buffer_full) {

            moves[buffer_end] = inp_moves[i];
            moves_dur[buffer_end] = durations[i];

            if (inc_moves_end_pointer()) return;

        } 
    }

}

void ServoEaser::addMovesAndPlay(int inp_moves[], int durations[], int moves_len) {

    for (int i = 0; i < moves_len; i++) {

        if (!buffer_full) {

            moves[buffer_end] = inp_moves[i];
            moves_dur[buffer_end] = durations[i];

            if (inc_moves_end_pointer()) return;

        } 
    }

    if (!running && arrived) play();
    else if (!running && !arrived) proceed();

}

void ServoEaser::addMoveAndPlay(int inp_move, int duration) {

    if (!buffer_full) {

        moves[buffer_end] = inp_move;
        moves_dur[buffer_end] = duration;

        inc_moves_end_pointer();

        if (!running && arrived) play();
        else if (!running && !arrived) proceed();

    }

}

bool ServoEaser::buffer_has_space(int positions_to_add) {

    if (buffer_full) return false;

    for (int i = 0; i < positions_to_add; i++) {

        if ( (buffer_end + 1) % MOVES_BUFFER_LEN == buffer_start ) return false;

    }

    return true;

}

void ServoEaser::play() {

    running = true;
    arrived = false;
    reset();

}

void ServoEaser::stop_ease() {

    running = false;

}

void ServoEaser::proceed() {

    if (!arrived || !buffer_empty) running = true;

}

void ServoEaser::proceed(int delayaftermove) {

    delay_after_move = delayaftermove;

    if (!arrived || !buffer_empty) running = true;

}

bool ServoEaser::isRuning() {

    return running;

}

void ServoEaser::setEasingFunc( EasingFunc func ) {

    easingFunc = func;

}

void ServoEaser::setArrivedFunc( ArrivedFunc func ) {

    arrivedFunc = func;

}