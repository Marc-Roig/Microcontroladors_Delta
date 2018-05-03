#ifndef misc_H
#define misc_H
 
void int_to_char(int number, char* converted_char);
char* int_to_char_2digits(int numb);
char* int_to_char_3digits(int numb);
char* int_to_char_4digits(int numb);
int chars_to_int(char a, char b, char c);
bool is_alphanumeric(char a);
int strlength(char *p);
double power(double base, double expon);
 
#endif /* misc_H */