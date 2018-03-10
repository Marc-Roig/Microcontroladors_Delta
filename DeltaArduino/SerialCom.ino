//Comands 
// G00 - Ready to take command?
// G01 - Proceed
// G02 - Transmision ended
// G03 XXX XXX XXX - Angles of servos (S1, S2, S3, S4 GRIP)
// G04 XXX - % Speed Overall
// G05 XYZ - Position end of arm
// G06 - End Of Stream
// G07 - Send More
// G08 - Ask to listen
// G09 - Emergency Stop

// G11 - Send Angles


//--COMMAND--//

void parse_command(char command[SERIAL_COMMAND_MAX_LEN]) {
  int command_num = chars_to_int('0', command[1], command[2]);

  switch(command_num) {
    case END_OF_STREAM: serial_next_instruction();
                        break;
      
    case MOVE_SERVOS:   serial_recieve_angles(command);
                        break;
      
    case MOVE_EF:       serial_recieve_ef_pos(command);
                        break;

    default:            //Serial.write("BAD COMMAND");
                        break;
  }
}

void serial_next_instruction() {

  switch (serial_mode) {
    case ASK_FOR_ANGLES:  Serial.write("G20\n"); //Ask for angles
                          Serial.write("G06\n"); //End of stream
                          break;

    case GIVE_ANGLES:     serial_send_angles();
                          Serial.write("G06\n"); //End of stream
                          break;

    default:              break;
  }

}

//--RECIEVE DATA--//
void check_serial() {
  static int i = 0; //Number of char recieved from current command
  char incomingByte;
  
  if (buffer.full) {
    //How to get the last one.
    //Get the first one, introduce the G06
    //Wait some time to start again the communication
  }
  else if (Serial.available() > 0 ) {
    incomingByte = Serial.read();

    buffer.command[buffer.end_][i] = incomingByte;
    i++;
    
    if (incomingByte == '\n') {
      buffer.command_len[buffer.end_] = i; 

      i = 0;

      //G06, END OF STREAM
      if (buffer.command[buffer.end_][2] == '6') {
        command_recieved = true;
      }

      //Increment position
      inc_buffer_end_pointer();

    }
  }
}

bool inc_buffer_end_pointer() {
  buffer.end_ = (buffer.end_ + 1) % SERIAL_BUFFER_LEN;
  buffer.empty = false;
  if (buffer.end_ == buffer.start) {
    buffer.full = true;
    return true;   
  }
  return false;
}

bool inc_buffer_start_pointer() {
  buffer.start = (buffer.start + 1) % SERIAL_BUFFER_LEN;
  buffer.full = false;

  if (buffer.end_ == buffer.start) {
    buffer.empty = true;
    return true;
  }
  return false;
}


void serial_recieve_angles(char command[SERIAL_COMMAND_MAX_LEN]) {
  bool bad_request = false;
  
  for (int i = 4; i < 7; i++) {
    if (!is_alphanumeric(command[i])) bad_request = true; 
  }
  for (int i = 8; i < 11; i++) {
    if (!is_alphanumeric(command[i])) bad_request = true;
  }
  for (int i = 12; i < 15; i++) {
    if (!is_alphanumeric(command[i])) bad_request = true;
  }

  if (!bad_request) {
  //    Serial.write("GOOD REQUEST\n");
    servos_angles[0] = chars_to_int(command[4], command[5], command[6]);
    servos_angles[1] = chars_to_int(command[8], command[9], command[10]);
    servos_angles[2] = chars_to_int(command[12], command[13], command[14]);
  }
  else Serial.write("BAD REQUEST\n");
}

void serial_recieve_ef_pos(char command[SERIAL_COMMAND_MAX_LEN]) {
  bool bad_request = false;

  for(int i = 4; i < 7; i++) {
    if (!is_alphanumeric(command[i])) bad_request = true;
  }

  if (!bad_request) {
    end_effector_pos[0] = command[4];
    end_effector_pos[1] = command[5];
    end_effector_pos[2] = command[6];
  }
}

//--SEND DATA--//
void serial_send_angles() {
  Serial.write("G03 ");
  for (int i = 0; i < 3; i++) {
    Serial.write(int_to_char(servos_angles[i]));
    
    if (i == 2) Serial.write("\n");
    else Serial.write(' '); 
  }
}

void serial_send_speed() {
  Serial.write("G04 ");
  Serial.write(end_effector_speed + 48);
  Serial.write('\n');
}

void serial_send_ef_pos() {
  Serial.write("G05 ");
  Serial.write(end_effector_pos[0]+'0');
  Serial.write(end_effector_pos[1]+'0');
  Serial.write(end_effector_pos[1]+'0');
  Serial.write('\n');
}

//--UTILITIES--//

char* int_to_char(int numb) {
  char* sentence = "   ";

  sentence[2] = numb % 10 + 48;
  sentence[1] = numb / 10 % 10 + 48;
  sentence[0] = numb / 100 % 10 + 48;

  return sentence;  
}

int chars_to_int(char a, char b, char c) {
  
  int result = (a-48)*100 + (b-48)*10 + c-48;
  
  return result;
}

bool is_alphanumeric(char a){
  if (a > 47 && a < 58) return true;
  return false;
}

