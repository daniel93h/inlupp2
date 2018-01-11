#ifndef __UTILS_H__
#define __UTILS_H__
#include <stdbool.h>

typedef union {
  int   i;
  float f;
  char *s;
  char  c;
} answer_t;
typedef answer_t(*convert_func)(char *s);

typedef bool(*check_func)(char *s);

extern char *strdup(const char *);


//reads string from buffer
//
//param buf - the buffer to read from
//param buf_siz - an inte that represents the buf size
int read_string(char *buf, int buf_siz);

//Checks if input is an Int
//
//param str - the inout to check
//returns true if input is an int
bool is_number(char *str);

//Prints a desired question and expects an Int back
//
//param question, the question that is getting printed
////returns the input

int ask_question_int(char *question);

//Prints a desired question and expects a string back
//
//param question, the question that is getting printed
//returns the input
char *ask_question_string(char *question);

//prints a string
//
//param str the string to print
//returns the input
void print(char *str);

//prints a string with a new line at end.
//
//param str - the string to be printed
void println(char *str);

//check if input string is not empty
//
//param str - string to check
//returns true if string is not empty
bool not_empty(char *str);

//prints the desired question, checks if the input is valid for the specific question and
//converts the input to desired type
//
//param question - the question to print
//param check - the function to check the string with
//param onvert - the function that converts the input to desired type
//returns answer_t
answer_t ask_question(char *question, check_func check, convert_func convert);

//checks if input is valid shelf
//
//param shelf - the input to check
//returns true if shelf is a valid shelf
bool is_shelf(char *shelf);

//prints the desired quiestion and returns the input
//
//returns the input
char *ask_question_shelf();

//checks if the input is valid
//
//param input - the input
//returns true if input is valid
bool is_valid_menu(char *input);

//prints the desired question and returns the input
//
//returns  the input
char ask_question_menu();

//prints the desired question and returns the input
//
//returns  the input
char ask_question_edit_menu();

//converts input to type char
//
//param s - the string that is getting converted
char convert_to_char(char *s);

//prints out the desired menu
//
//
void print_menu();

//checks if the input is a valid edit menu input
//
//param input - the input
//returns true if the input is valid
bool is_valid_edit_menu(char *input);

//prints the edit menu
//
//
void print_edit_menu();

//prints the desired question and returns the input
//
//returns the user input
char ask_question_add_menu();

//prints the desired question and returns the input
//
//returns the user input
char ask_question_add_shelf_menu();

//prints the add item menu
//
//
void print_add_menu();

//prints the add item menu
//
//
void print_add_shelf_menu();

//checks if the user input is a valid add menu input
//
//param input - input by the user
//return true if the input is a valid add menu input
bool is_valid_add_menu(char *input);

//checks if the user input is a valid add shelf menu input
//
//param input - input by the user
//returns true if the input is a valid add shelf menu input
bool is_valid_add_shelf_menu(char *input);

#endif
