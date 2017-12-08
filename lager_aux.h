#ifndef __lager_aux_h__
#define __lager_aux_h__
extern char *strdup(const char *);
#include <stdbool.h>

typedef union
{
  int i;
  float f;
  char *s;
}answer_t;
typedef bool(*check_func)(char*);
typedef answer_t(*convert_func)(char*);

void get_goods_for_storage_location();

char ask_question_char(char *question, char *answers);

//void print_goods(char *question);

int ask_question_int(char *question);

char *ask_question_string(char *question);


#endif
