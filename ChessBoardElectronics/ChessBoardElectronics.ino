#define COUNTER_A 12
#define COUNTER_B 11
#define COUNTER_C 10
#define COUNTER_D 9


#define PUSH_BUTTON 2
#define PUSH_BUTTON_RESET 3

#define LOAD 8
#define CLOCK 7

void setup() {
  
  pinMode(COUNTER_A, INPUT);
  pinMode(COUNTER_B, INPUT);
  pinMode(COUNTER_C, INPUT);
  pinMode(COUNTER_D, INPUT);

  pinMode(PUSH_BUTTON, INPUT_PULLUP);
  pinMode(PUSH_BUTTON_RESET, INPUT_PULLUP);
  

  pinMode(LOAD, OUTPUT);
  pinMode(CLOCK, OUTPUT);

  digitalWrite(LOAD, LOW);
  delay(100);
  digitalWrite(LOAD, HIGH);

  Serial.begin(9600);
  
}

unsigned int A_bit = 0;
unsigned int B_bit = 0;
unsigned int C_bit = 0;
unsigned int D_bit = 0;

unsigned int result = 0;
void loop(){
  char temp = 0;
  
  A_bit = digitalRead(COUNTER_A);
  B_bit = digitalRead(COUNTER_B);
  C_bit = digitalRead(COUNTER_C);
  D_bit = digitalRead(COUNTER_D);

  
  result = A_bit + (B_bit << 1) + (C_bit << 2) + (D_bit << 3);  
//  if (A_bit) Serial.write("X");
//  else Serial.write("-");
//  if (B_bit) Serial.write("X");
//  else Serial.write("-");
//  if (C_bit) Serial.write("X");
//  else Serial.write("-");
//  if (D_bit) Serial.write("X");
//  else Serial.write("-");  
  Serial.println(result);
  
//  Serial.write("\n");

  check_button_pressed();
  check_reset_button_pressed();

  delay(300);
  
}


void check_reset_button_pressed() {
  static bool S0 = true;
  

  if (!digitalRead(PUSH_BUTTON_RESET) && S0) {
      digitalWrite(LOAD, LOW);
      delay(50);
      digitalWrite(LOAD, HIGH);
      S0 = false;
  }
  else if (digitalRead(PUSH_BUTTON)) {
    
    S0 = true;
  }
}

void check_button_pressed() {
  static bool S0 = true;
  

  if (!digitalRead(PUSH_BUTTON) && S0) {
      digitalWrite(CLOCK, HIGH);
      S0 = false;
  }
  else if (digitalRead(PUSH_BUTTON)) {
    
    S0 = true;
    digitalWrite(CLOCK, LOW);
  }
}

