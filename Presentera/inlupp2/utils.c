#include <stdio.h>
#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


int read_string(char *buf, int buf_siz){

  int pos = 0;
  char c;

  do{
    c = getchar();
    buf[pos] = c;
    if (c != '\n'){
      pos++;
    }
  }
  while(c != '\n' && c != EOF && ((buf_siz - 1) > pos));
  buf[pos] = '\0';
  return(pos);
}

bool is_valid_menu(char *input){
  if (strlen(input)>1) return false;
  if(*input != 'A' &&
     *input != 'a' &&
     *input != 'B' &&
     *input != 'b' &&     
     *input != 'R' &&
     *input != 'r' &&
     *input != 'E' &&
     *input != 'e' &&
     *input != 'U' &&
     *input != 'u' &&
     *input != 'L' &&
     *input != 'l' &&
     *input != 'Q' &&
     *input != 'q'){

      return false;
      }
  else{
    return true;
  }
}

bool is_valid_edit_menu(char *input){
  if (strlen(input)>1) return false;
  if(*input != 'D' &&
     *input != 'd' &&
     *input != 'P' &&
     *input != 'p' &&
     *input != 'S' &&
     *input != 's' &&
     *input != 'R' &&
     *input != 'r' &&
     *input != 'Q' &&
     *input != 'q')

    {
      return false;
    }

  else
    {
      return true;
    }
}

bool is_valid_add_shelf_menu(char *input)
{ if (strlen(input)>1) return false;
  if(*input != 'Y' &&
     *input != 'y' &&
     *input != 'N' &&
     *input != 'n' &&
     *input != 'E' &&
     *input != 'e')
  {
    return false;
  }
  return true;
}


bool is_valid_add_menu(char *input)
{ if (strlen(input)>1) return false;
  if(*input != 'Y' &&
     *input != 'y' &&
     *input != 'N' &&
     *input != 'n' &&
     *input != 'E' &&
     *input != 'e')
  {
    return false;
  }
  return true;
}



char convert_to_char(char* s){

  char c = s[0];

  return c;

}


bool is_number(char *str){

  int length = strlen(str);

  if ((str[0] == '-') || (isdigit(str[0]))){

    for(int i = 1; i < length; i++){
      if(isdigit(str[i]) == false){
        return false;
      }
    }
  }
  else{
    return false;
  }
  return true;
}




char *ask_question_string(char *question){

  return ask_question(question, not_empty, (convert_func) strdup).s;
}


char ask_question_menu(){
  char answer;
  print_menu();
  answer = ask_question("\nChoose what to do.", is_valid_menu, (convert_func) convert_to_char).c;

  return answer;
}


char ask_question_edit_menu()
{
  char answer;
  print_edit_menu();
  answer = ask_question("\nChoose what to do.", is_valid_edit_menu, (convert_func) convert_to_char).c;

  return answer;
}
char ask_question_add_shelf_menu()
{
  char answer;
  print_add_shelf_menu();
  answer = ask_question("\nChoose what to do", is_valid_add_shelf_menu, (convert_func) convert_to_char).c;

  return answer;
}

char ask_question_add_menu()
{
  char answer;
  print_add_menu();
  answer = ask_question("\nChoose what to do", is_valid_add_menu, (convert_func) convert_to_char).c;

  return answer;
}
int ask_question_int(char *question){

  answer_t answer = ask_question(question, is_number, (convert_func) atoi);

  return answer.i;
}


answer_t ask_question(char *question, check_func check, convert_func convert){
  int buf_siz = 300;
  char buf[buf_siz];

  do{
    printf("%s\n", question);
    read_string(buf, buf_siz);
  }
  while(check(buf) == false);

  return (convert(buf));
}


void print(char *str){
  for(int n = 0; str[n] != '\0'; n++){
    putchar(str[n]);
  }
}


void println(char *str){
  print(str);
  print("\n");
}


void print_menu(){

  char* X = "|=========MENU==========|";
  char* A = "|[A]dd an item          |";
  char* B = "|[B]alance tree         |";
  char* R = "|[R]emove an item       |";
  char* E = "|[E]dit an item         |";
  char* U = "|[U]ndo the last change |";
  char* L = "|[L]ist all items       |";
  char* Q = "|[Q]uit                 |";
  char* J = "|=======================|";


  printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",X, A, B, R, E, U, L, Q, J);

}

void print_edit_menu()
{

  char* D = "[D]escription";
  char* P = "[P]rice";
  char* R = "[R]emove";
  char* S = "[S]helf";
  char* Q = "[Q]uit";

  printf("%s\n%s\n%s\n%s\n%s\n", D, P, R, S, Q);

}

void print_add_menu()
{
  char* X = "Do you want to save it in the database?";
  char* Y = "[Y]es";
  char* N = "[N]o";
  char* E = "[E]dit";

  printf("%s\n%s\n%s\n%s\n", X, Y, N, E);

}

void print_add_shelf_menu()
{
  char* X = "Do you want to add the item to a new shelf?";
  char* Y = "[Y]es";
  char* N = "[N]o";
  char* E = "[E]xit";

  printf("%s\n%s\n%s\n%s\n",X, Y, N, E);

}




bool not_empty(char *str){
  return strlen(str) > 0;
}



bool is_shelf(char *shelf){

  int length = strlen(shelf);
  for(int x = 1; x <= length; x++)
  {
    if (isalpha(shelf[x]))return false;

  if (isalpha(shelf[0])){

    for(int i = 1; i < length; i++){
      if(isdigit(shelf[i])){
        return true;
      }
    }
  }
  }
  return false;

}



char *ask_question_shelf(char *question)
{
  char *answer;

  
  answer = ask_question(question, is_shelf, (convert_func) strdup).s;


  return answer;
}
