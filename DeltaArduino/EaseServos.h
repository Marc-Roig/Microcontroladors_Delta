#ifndef EASESERVOS_H

    #define EASESERVOS_H

    typedef float (*EasingFunc)(float t, float b, float c, float d); 

    // define "ArrivedFunc" to be called when servo arrives at position
    // arguments provided are: currPos of servo & movesIndex of move list (if appl)
    typedef void (*ArrivedFunc)(int servo_num);

    typedef void (*BufferEmptiedFunc)();

    class ServoEaser {

        private:

            int servo_num;

            int frameMillis;  // minimum update time between servo moves
            int delay_after_move;
            int startPos;   // where servo started its tween
            int currPos;    // current servo position, best of our knowledge

            int changePos;  

            int durMillis;    
            int tick;         // count of easing moves within move duration 
            int tickCount;    // number of frames between start & end pos
            unsigned long lastMillis; // time time we did something

            EasingFunc easingFunc; // func that describes tween motion
            ArrivedFunc arrivedFunc; // func to call when servo arrives at dest
            BufferEmptiedFunc bufferemptiedFunc;

            // Circular buffer
            int moves[MOVES_BUFFER_LEN];
            int moves_dur[MOVES_BUFFER_LEN];

            int buffer_start, buffer_end;
            bool buffer_full, buffer_empty;

            // Status values
            bool running;  // is servo easer running?
            bool arrived;  // has servo arrived at its destination

        public:

            void update();

            void getNextPos();

            bool inc_moves_end_pointer();
            bool inc_moves_start_pointer();
            void init_moves_buffer();

            void init(int servo_num_, int frameTime);

            void reset();

            void addMoves(int inp_moves[], int durations[], int moves_len);
            void addMovesAndPlay(int inp_moves[], int durations[], int moves_len);
            void addMoveAndPlay(int inp_move, int duration);
            bool buffer_has_space(int positions_to_add);

            void play();

            void stop_ease();
            void proceed(int delayaftermove);
            void proceed();

            bool isRuning();

            void setEasingFunc(EasingFunc func);
            void setArrivedFunc(ArrivedFunc func);
            void setBufferEmptyFunc( BufferEmptiedFunc func );

    };

#endif