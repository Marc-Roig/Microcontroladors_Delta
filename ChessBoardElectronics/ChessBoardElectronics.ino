#define PUSH_BUTTON 2
#define PUSH_BUTTON_RESET 3

#define LOAD 8
#define CLOCK_PIN 7

#define TIME_DELAY_DIGITALWRITE 2

#define BOARD_OUTPUT_PIN 4

bool board_output = false;

bool board_states[8][8];

void setup() { 

    pinMode(BOARD_OUTPUT_PIN, INPUT);

    pinMode(PUSH_BUTTON, INPUT_PULLUP);
    pinMode(PUSH_BUTTON_RESET, INPUT_PULLUP);


    pinMode(LOAD, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);

    digitalWrite(LOAD, LOW);
    delay(100);
    digitalWrite(LOAD, HIGH);

    Serial.begin(9600);

    for (int i = 0; i < 8;  i++) {
        for (int j = 0; j < 8; ++j) {

            board_states[i][j];

        }
    }

}

unsigned int row_num = 0;
unsigned int col_num = 0;

void loop(){

    read_board();

    delay(500);

}

void read_board() {
    
    pulse_reset();

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {

            board_states[j][i] = digitalRead(BOARD_OUTPUT_PIN);

            pulse_clock(j);

        }
    }
}

void pulse_reset() {

    digitalWrite(LOAD, LOW);
    delay(TIME_DELAY_DIGITALWRITE);

    digitalWrite(LOAD, HIGH);
    delay(TIME_DELAY_DIGITALWRITE);

}

void pulse_clock(int count) {

  if (count >= 7) {
    for (int i = 0; i < 9; i++) {

      digitalWrite(CLOCK_PIN, HIGH);
      delay(TIME_DELAY_DIGITALWRITE);

      digitalWrite(CLOCK_PIN, LOW);
      delay(TIME_DELAY_DIGITALWRITE);

    }
    return;
  }

  digitalWrite(CLOCK_PIN, HIGH);
  delay(TIME_DELAY_DIGITALWRITE);

  digitalWrite(CLOCK_PIN, LOW);
  delay(TIME_DELAY_DIGITALWRITE);

}

void check_reset_button_pressed() {
  
  static bool S0 = true;
  

  if (!digitalRead(PUSH_BUTTON_RESET) && S0) {
      digitalWrite(LOAD, LOW);
      delay(TIME_DELAY_DIGITALWRITE);
      digitalWrite(LOAD, HIGH);
      S0 = false;
  }
  else if (digitalRead(PUSH_BUTTON)) {
    
    S0 = true;
  }

}



void check_button_pressed(int count) {

  static bool S0 = true;
    
  if (!digitalRead(PUSH_BUTTON) && S0) {
    if (count >= 7) {

        for (int i = 0; i < 9; i++) {

          digitalWrite(CLOCK_PIN, HIGH);
          delay(TIME_DELAY_DIGITALWRITE);
          digitalWrite(CLOCK_PIN, LOW);
          delay(TIME_DELAY_DIGITALWRITE);

        }

        return;

    }
      digitalWrite(CLOCK_PIN, HIGH);
      S0 = false;
  }
  else if (digitalRead(PUSH_BUTTON)) {
    
    S0 = true;
    digitalWrite(CLOCK_PIN, LOW);
  }

}

void read_and_print_BCD_counter() {

  // static int lastA_bit = digitalRead(COUNTER_A);
  // static int lastA2_bit = digitalRead(COUNTER2_A);
  
  // unsigned int A_bit = digitalRead(COUNTER_A);
  // unsigned int B_bit = digitalRead(COUNTER_B);
  // unsigned int C_bit = digitalRead(COUNTER_C);
  // unsigned int D_bit = digitalRead(COUNTER_D);
  // unsigned int A2_bit = digitalRead(COUNTER2_A);
  // unsigned int B2_bit = digitalRead(COUNTER2_B);
  // unsigned int C2_bit = digitalRead(COUNTER2_C);
  // unsigned int D2_bit = digitalRead(COUNTER2_D);

  // unsigned int result = A_bit + (B_bit << 1) + (C_bit << 2) + (D_bit << 3);  
  // unsigned int result2 = A2_bit + (B2_bit << 1) + (C2_bit << 2) + (D2_bit << 3);

  //  if (lastA_bit != A_bit || lastA2_bit != A2_bit) {
  //   Serial.println(result);
  //   Serial.println(result2);
  
  //   Serial.write("-----------\n");

  //   lastA_bit = A_bit;
  //   lastA2_bit = A2_bit;
  // }    

  // check_button_pressed(result);
    // check_reset_button_pressed();

}