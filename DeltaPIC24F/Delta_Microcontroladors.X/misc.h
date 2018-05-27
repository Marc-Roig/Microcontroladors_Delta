#ifndef misc_H
#define misc_H
 
void int_to_char(int number, char* converted_char);
// char* int_to_char_2digits(int numb);
void int_to_char_2digits_2(int numb, char* sentence);
// char* int_to_char_3digits(int numb);
void int_to_char_3digits_2(int numb, char* sentence);
void int_to_char_4digits_2(int numb, char* sentence);
char* int_to_char_4digits(int numb);
int chars_to_int(char a, char b, char c);
bool is_alphanumeric(char a);
int strlength(char *p);
double power(double base, double expon);
 
#endif /* misc_H */