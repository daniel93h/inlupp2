// Här lägger vi alla funktioner som vi brutit ut ur funktionerna i lager.c

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include "lager_aux.h"



bool is_number(char *str)
{
  int as = strlen(str);
  bool sant = true;
  char first = '-';
  char first2 = str[0];
  for(int i = 0; i < as; i = i+1)
    {
      if(i==0 && first2 == first)
        {
          sant = true;
        }
      else if(isdigit(str[i]) == false)
        {
          sant = false;
        }
    }
  return sant; 
}

void clear_buffer(void)
{
  int c;
  do
    {
      c = getchar();
    }
  while (c != '\n'); //&& c != EOF);
}

int read_string(char *buf, int buf_size)
{
  char c;
  int readcount = 0;
  for (int i=0; i < (buf_size-1); i++)
    {
      c = getchar();
      if (c == '\n')
        {
          break;
        }
      else
        { 
          buf[i] = c;
          readcount++;
        }
    }
  if (c != '\n')
    {
      clear_buffer();
    }
  buf[readcount] = '\0';
  return readcount;
}

answer_t ask_question(char* question, check_func check, convert_func convert)
{
  int buf_siz = 255;
  char buf[buf_siz];
  printf("%s \n", question);
  read_string(buf, buf_siz);
  
  while (!check(buf))
    {
      printf("Felaktig inmatning: '%s'\n", buf);
      printf("%s \n", question);
      read_string(buf, buf_siz);
    }
  

  answer_t svar = convert(buf);
  //free(buf);
  return svar;
}

bool not_empty(char *str)
{
  return strlen(str) != 0;
}


void get_goods_for_storage_location()
{}

char ask_question_char(char *question, char *answers)
{
  char svar;
  bool giltigt_svar = false;
  char *cursor;
  do
    {
      svar = toupper(ask_question_string(question)[0]);
      cursor = answers;
      while (*cursor)
        {
          if (toupper(*cursor) == svar)
            {
              giltigt_svar = true;
              break;
            }
          ++cursor;
        }
    }
  while(!giltigt_svar);
  //free(cursor);
  //free(question);
  //free(answers);
  return svar;
}

//void print_goods()
//{}

char *ask_question_string(char *question)
{
  return ask_question(question, not_empty, (convert_func) strdup).s;
}



int ask_question_int(char *question)
{
  answer_t answer = ask_question(question, is_number, (convert_func) atoi);
  return answer.i; // svaret som ett heltal
}
