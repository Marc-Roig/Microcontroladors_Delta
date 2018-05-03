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


/*********************************************************************
* Function: char* int_to_char_2digits(int numb);
*
* Overview: Returns a String from a number (0 - 99)
*           String will always have two characters
*
* PreCondition: none
*
* Input: int - number to convert
*
* Output: String - number converted to String
*
********************************************************************/

char* int_to_char_2digits(int numb) {

  char* sentence = "   ";

  sentence[1] = numb % 10 + '0';
  sentence[0] = numb / 10 % 10 + '0';

  sentence[2] = '\0';

  return sentence;

}

/*********************************************************************
* Function: char* int_to_char_3digits(int numb);
*
* Overview: Returns a String from a number (0 - 999)
*           String will always have three characters
*
* PreCondition: none
*
* Input: int - number to convert
*
* Output: String - number converted to String
*
********************************************************************/

char* int_to_char_3digits(int numb) {

  char* sentence = "    ";

  sentence[2] = numb % 10 + '0';
  sentence[1] = numb / 10 % 10 + '0';
  sentence[0] = numb / 100 % 10 + '0';

  sentence[3] = '\0';

  return sentence;

}

/*********************************************************************
* Function: char* int_to_char_4digits(int numb);
*
* Overview: Returns a String from a number (0 - 9999)
*           String will always have four characters
*
* PreCondition: none
*
* Input: int - number to convert
*
* Output: String - number converted to String
*
********************************************************************/

char* int_to_char_4digits(int numb) {

  char* sentence = "     ";

  sentence[3] = numb % 10 + '0';
  sentence[2] = numb / 10 % 10 + '0';
  sentence[1] = numb / 100 % 10 + '0';
  sentence[0] = numb / 1000 % 10 + '0';

  sentence[4] = '\0';

  return sentence;

}

/*********************************************************************
* Function: int chars_to_int(char a, char b, char c);
*
* Overview: Returns integer from the three chars introduced
*
* PreCondition: none
*
* Input: char a - hundreds
*        char b - tens
*        char c - units
*
* Output: Int - number converted
*
********************************************************************/

int chars_to_int(char a, char b, char c) {
  
  return (a-'0')*100 + (b-'0')*10 + c-'0';

}

/*********************************************************************
* Function: bool is_alphanumeric(char a);
*
* Overview: none
*
* PreCondition: none
*
* Input: char - value to check if its a number
*
* Output: Bool - true if it is a number
*                false if it is not
*
********************************************************************/

bool is_alphanumeric(char a) {

  if (a >= '0' && a <= '9') return true;
  return false;

}