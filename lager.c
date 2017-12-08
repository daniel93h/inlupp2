#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "tree.h"
#include "lager_aux.h"
#include "tree.h"
//#include "item.h"
/*
struct action
{
  int type; // NOTHING = 0, ADD = 1, REMOVE = 2, EDIT = 3
  item_t *merch;
  item_t copy;
};typedef struct action action_t;
*/

void add_goods()
{}

void remove_goods()
{}

void edit_goods()
{}

void list_goods()
{}

void display_goods()
{}

void undo_action()
{}

void exit_program()
{}

void main_menu()
{}




void event_loop(tree_t *tree)
{
  char val;
  // action_t *act = malloc(sizeof(struct action));
  //act->type = 0;
  
  do
    {
      val = ask_question_char("\n[L]ägga till en vara\n\
[T]a bort en vara\n\
[R]edigera en vara\n\
Ån[g]ra senaste ändringen\n\
Lista [h]ela varukorgen\n\
[A]vsluta\n\n\
Vad vill du göra?", "LTRGHA");


      switch (val)
        {
        case 'L': // Lägga till
          puts("Ta bort;Not yet implemented!");
          break;
          
        case 'T': //Ta bort
          puts("Ta bort;Not yet implemented!");
          break;
          
        case 'R': //Redigera
          puts("Ta bort;Not yet implemented!");
          break;
          
        case 'G': //Ångra
          puts("Ta bort;Not yet implemented!");
          break;
          
        case 'H': //Lista
          puts("Ta bort;Not yet implemented!");
          break;
        }
    }
  while (val != 'A');
}


int main(void)
{
  tree_t *tree = tree_new(NULL,NULL,NULL,NULL);//för o kunna fixa loop
  
  /*
  char *ar[24] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23"};
  printf("printed %d strings\n", list_20_strings(ar, 0, 2));
  exit(0);
  */
  puts("Välkommen till lagerhantering");
  puts("==============================");
  event_loop(tree);
  
  return 0;
}
