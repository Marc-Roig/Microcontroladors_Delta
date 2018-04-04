// Code adapted from: https://github.com/todbot/ServoEaser

typedef float (*EasingFunc)(float t, float b, float c, float d); 

// define "ArrivedFunc" to be called when servo arrives at position
// arguments provided are: currPos of servo & movesIndex of move list (if appl)
typedef void (*ArrivedFunc)(int currPos, int movesIndex);

class ServoEaser {

    private:

        Servo servo;      // what servo we're operating on
        int servo_num;
        
        int frameMillis;  // minimum update time between servo moves
        int startPos;   // where servo started its tween
        int currPos;    // current servo position, best of our knowledge

        int changePos;  // from servoMove list

        int durMillis;    // from servoMove list
        int tick;         // count of easing moves within move duration 
        int tickCount;    // number of frames between start & end pos
        unsigned long lastMillis; // time time we did something

        EasingFunc easingFunc; // func that describes tween motion
        ArrivedFunc arrivedFunc; // func to call when servo arrives at dest

        // Circular buffer
        int moves[];
        int moves_dur[];

        int buffer_start, buffer_end;
        bool buffer_full, buffer_empty;

        // Status values
        bool running;  // is servo easer running?
        bool arrived;  // has servo arrived at its destination

    public:

        void move_servos();

        void getNextPos();

        void inc_end_pointer();
        void inc_start_pointer();
        void init_buffer();

        void init();

}

ServoEaser servoeaser;

inline float ServoEaser_easeInOutCubic(float t, float b, float c, float d) {

    if ((t/=d/2) < 1) return c/2*t*t*t + b;
    return c/2*((t-=2)*t*t + 2) + b;

}

void ServoEaser::move_servos() {

    if( ((millis() - lastMillis) < frameMillis) || !running ) return;

    lastMillis = millis();

    currPos = easingFunc( tick, startPos, changePos, tickCount );

    if( !arrived ) tick++;
    if( tick == tickCount ) { // time for new position
        getNextPos(); 
    }
    
    servo.writeMicroseconds(currPos);
}

void ServoEaser::getNextPos() {

    if (buffer_empty) {
        running = false;
        if( arrivedFunc != NULL ) arrivedFunc( currPos, movesIndex );
        return;
    }

    startPos  = currPos; // current position becomes new start position

    changePos = moves[buffer_start] - startPos;
    durMillis = moves_dur[buffer_start];

    inc_buffer_start_pointer();

    tickCount = durMillis / frameMillis;
    tick = 0;

}

bool ServoEaser::inc_end_pointer() {

  buffer_end_ = (buffer_end_ + 1) % SERIAL_BUFFER_LEN;
  buffer_empty = false;
  if (buffer_end_ == buffer_start) {
    buffer_full = true;
    return true;   
  }
  return false;

}

bool ServoEaser::start_pointer() {

  buffer_start = (buffer_start + 1) % SERIAL_BUFFER_LEN;
  buffer_full = false;

  if (buffer_end_ == buffer_start) {
    buffer_empty = true;
    return true;
  }
  return false;

}

void ServoEaser::init_buffer() {

  buffer_start = 0;
  buffer_end_ = 0;
  buffer_empty = true;
  buffer_full = false;
  
}


void ServoEaser::begin(Servo s, int frameTime, 
                       ServoMove* mlist, int mcount)
{
    begin( s, frameTime ); //, servo.read() );
    play( mlist, mcount );
}

// set up an easer with just a servo and a starting position
void ServoEaser::init(Servo s, int servo_num_, int frameTime) {

    servo = s;
    servo_num = servo_num_;

    init_buffer();

    frameMillis = frameTime;

    easingFunc = ServoEaser_easeInOutCubic;
    arrivedFunc = NULL;
    
    reset();

}

// reset easer to initial conditions, does not nuke easingFunc or arrivedFunc
void ServoEaser::reset()
{

    currPos = servo;
    startPos = currPos;  // get everyone in sync
    changePos = 0;       // might be overwritten below

    if( !buffer_empty ) {
        changePos = moves[buffer_start] - startPos ;
        durMillis = moves_dur[buffer_start]
    }

    tickCount = durMillis / frameMillis;
    tick = 0;
    
    debug_reset();

}

bool ServoEaser::addMoves(int inp_moves[], int durations[], int moves_len) {

    for (int i = 0; i < moves_len; i++) {

        if (!buffer_full) {
            moves[buffer_end] = inp_moves[i];
            moves_dur[buffer_end] = durations[i];
        } else return false;

    }

    return true;

}

void ServoEaser::play(int inp_moves[], int moves_len) {

    running = true;

    reset();

}

// manual, non-moves list, control of easer position
void ServoEaser::easeTo( int pos, int dur ) {

    movesCount = 0;  // no longer doing moves list
    startPos = currPos;
    changePos = pos - startPos;
    durMillis = dur;
    tickCount = durMillis / frameMillis;
    tick = 0;
    running = true;
}