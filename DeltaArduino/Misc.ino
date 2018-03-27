/*********************************************************************
* Function: char* int_to_char(int number);
*
* Overview: Returns a String from a number of any lenght
*           String will end with \n and \0
*
* PreCondition: none
*
* Input: int - number to convert
*
* Output: String - number converted to String
*
********************************************************************/

char* int_to_char(int number) {
    
    int number_len = 1;
    int temp = number;

    while(temp > 9) {
        temp /= 10;
        number_len++;
    }

    char* val = (char*) malloc(number_len+2);

    for (int i = 0; i < number_len; i++) {

        val[i] = number / (int)(pow(10, (number_len-i-1))) % 10 + '0';

    }
    
    val[number_len] = '\n';
    val[number_len+1] = '\0';

    return val;
}

char* int_to_char_2digits(int numb) {

  char* sentence = "  ";

  sentence[1] = numb % 10 + '0';
  sentence[0] = numb / 10 % 10 + '0';

  return sentence;

}

char* int_to_char_3digits(int numb) {

  char* sentence = "   ";

  sentence[2] = numb % 10 + '0';
  sentence[1] = numb / 10 % 10 + '0';
  sentence[0] = numb / 100 % 10 + '0';

  return sentence;

}

int chars_to_int(char a, char b, char c) {
  
  return (a-'0')*100 + (b-'0')*10 + c-'0';

}

bool is_alphanumeric(char a) {

  if (a > 47 && a < 58) return true;
  return false;

}