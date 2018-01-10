#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "tree.h"
#include "lager_aux.h"
#include "tree.h"
#include "list.h"
#include "common.h"



typedef struct item item_t;
struct item {
    char *name;
    char *desc;
    int price;
    list_t *shelves;
};

struct shelf {
  char *name;
  int amount;
};typedef struct shelf shelf_t;

struct action
{
  int type; // NOTHING = 0, ADD = 1, REMOVE = 2, EDIT = 3
  item_t *merch;
  item_t copy;
};typedef struct action action_t;

elem_t element_copy(elem_t elem)
{
  elem_t copy ; //= malloc(sizeof(elem_t));
  copy = elem;
  return copy;
}


void free_key(tree_key_t key)
{
  char *key_to_delete = ((char *)&key);
  free(key_to_delete);
}

void free_elem(elem_t elem)
{
  item_t *item = ((item_t *)&elem);
  free(item->name);
  free(item->desc);

  list_delete(item->shelves, true);
  free(item);
}

int key_compare(tree_key_t key1, tree_key_t key2)
{
  char *k1 = (((char *)(key1.p)));
  char *k2 = (((char *)(key2.p)));
  return strcmp(k1, k2);
}

void print_shelves(list_t *shelves) {
  printf("\nHyllor: ");
    for (int i = 0; i < list_length(shelves); i++){
      shelf_t *shelf = malloc(sizeof(shelf_t));
        
      elem_t get_elem;
        
      get_elem.p = shelf;
        
      list_get(shelves, i, &get_elem);
        
      shelf_t *shelf_printer = ((shelf_t*)get_elem.p);
        
      printf("%s (%d st), ", shelf_printer->name, shelf_printer->amount);

    }
}


//Prints the name, desc etc. of an item
void print_item(elem_t item)
{
  printf("Namn: %s\n", (((item_t*)item.p)->name));
  puts("test");
  printf("Namn: %s\nBeskrivning: %s\nPris: %d öre", (((item_t*)item.p)->name),  (((item_t*)item.p)->desc),  (((item_t*)item.p)->price));

  print_shelves( (((item_t*)item.p)->shelves));
}





/*
  Searches list for specific shelf and returns a pointer to it.
  If no such shelf exists, returns NULL
*/
shelf_t *get_shelf_in_list(list_t *list, char *shelf_name)
{
  int len = list_length(list);
  shelf_t *shelf =malloc(sizeof(shelf_t));
  for(int i=0; i < len; ++i)
    {
      list_get(list, i, (elem_t *) shelf);
      
      // Need strcmp, since str1 == str2 just compares pointers
      if (strcmp(shelf->name, shelf_name) == 0)
        {
          return shelf;
        }
    }
  return false;
}

item_t make_copy(item_t *elem)
{
  item_t temp;
  temp.name=strdup(elem->name);
  temp.desc=strdup(elem->desc);
  temp.price=elem->price;
  elem_t *shelves_elem = malloc(sizeof(elem_t));
  shelves_elem->p=list_new(element_copy, free_elem, key_compare);
  temp.shelves= shelves_elem->p;
  for(int i=0; i <= list_length(elem->shelves);++i)//fel "<=" ska vara "<", skapat för demo.
    { 
      elem_t *original_shelf=malloc(sizeof(elem_t));
      list_get(elem->shelves, i, original_shelf->p);
      shelf_t *temp_shelf = original_shelf->p;
      shelf_t *shelf_copy=malloc(sizeof(shelf_t));
      shelf_copy->name=strdup(temp_shelf->name);
      shelf_copy->amount=temp_shelf -> amount;
      elem_t insert_shelf;
      insert_shelf.p=shelf_copy;

      list_append(temp.shelves, insert_shelf);
    }
   return temp;  
}

elem_t create_shelf(shelf_t *elem, char *shelf_name, int amount)
{
  elem->name = shelf_name;
  elem->amount = amount;

  elem_t shelf;
  shelf.p = elem;
  return shelf;
}






//Lists up to 20 strings from array, starting at start, then returns how many were listed
int list_20_strings(elem_t *strings, int start, int n)
{
  //Only display 20 items at a time
  if (n > 20)
    {
      n = 20;
    }
  for (int i=start; i < n + start; ++i)
    {
      printf("%d. '%s'\n", i+1, (char*)strings[i].p);
    }
  return n;
}

/*
  Helper of select_goods. Prints available menu options and returns user's answer.
  Adds specified line and option to a standard menu of:
    Visa [f]ler varor
    [A]vsluta
*/
char get_menu_action(int remaining_items, char *menu_line, char menu_opt)
{
  char menu[255] = "\n";
  strcat(menu, menu_line);

  char opts[4];
  opts[0] = menu_opt;
  
  //We need to print different menus depending on remaining amount of items
  if (remaining_items > 0)
    {
      strcat(menu, "\nVisa [f]ler varor\n[A]vsluta och återgå till huvudmenyn");
      strcat(opts, "FA");
      printf("%d varor återstår, du kan:", remaining_items);
    }
  else
    {
      strcat(menu, "\n[A]vsluta och återgå till huvudmenyn");
      strcat(opts, "A");
      printf("Du kan:");
    }
  return ask_question_char(menu, opts);
}


/*
  Type of function being called when user selects an item.
  item is needed to allow changing/showing item
  tree is needed to be able to check if a shelf is already in use
  act is needed to remember changes to database, if any, to enable undoing them later
*/
typedef void(*goods_action)(elem_t *item, tree_t *tree, action_t *act);


/*
  Lists goods, 20 at a time and lets user select an item.
  A menu is printed with the menu line added, and if user selects that option,
  function 'goods_action' is run with the item.
  act is needed by goods_action to remember action taken to be able to undo it later
*/
void select_goods(tree_t *tree, char *menu_line, char menu_option, action_t *act, goods_action goods_action)
{
  int chosen_index;
  int shown_items;  // Need to keep track how many item were last shown,
                    // to know what elements can be chosen.
  char menu_action = ' ';
  int remaining = tree_size(tree);

  if (remaining == 0)
    {
      puts("Databasen är tom!");
    }
  else
    {
      elem_t *keys = tree_keys(tree);
      int nof_keys = remaining;  // which =tree_size at this point
      puts("Nuvarande varor i databasen:");
      shown_items = list_20_strings(keys, 0, remaining);
      remaining -= shown_items;
      
      while (menu_action != 'A')
        {
          printf("\n");
          menu_action = get_menu_action(remaining, menu_line, menu_option);
          printf("\n");

          switch (menu_action)
            {
            case 'F':  // Show more items
              shown_items = list_20_strings(keys, nof_keys - remaining, remaining);
              remaining -= shown_items;
              break;

            case 'A':  // Quit
              break;

            default: // A custom user option was selected
              chosen_index = ask_question_int("Vilken vara? (skriv ett nummer)");
              while (chosen_index > shown_items || chosen_index > 20 || chosen_index < 1)
                {
                  puts("Varan finns inte i listan, ange korrekt nummer!\n");
                  chosen_index = ask_question_int("Vilken vara? (skriv ett nummer)");
                }
              elem_t *item = malloc(sizeof(elem_t));
              tree_get(tree, keys[chosen_index - 1], item);
              //((item_t *) insert_item.p)->name;
              goods_action(item, tree, act);
            }
                
        }
      // We only need to free the array of pointers to the strings, since the
      // strings themselves (the tree keys) are in the tree.
      //free(keys);
    }
}











/*
  Makes an item with specified properties, where shelf list is a single shelf
  with specified amount.
*/
item_t *make_item(char *name, char *desc, int price, char *shelf_name, int amount)
{
  shelf_t *shelf = malloc(sizeof(shelf_t));
  shelf->name = shelf_name;
  shelf->amount=amount;
  list_t *shelf_list = list_new(element_copy, free_elem, key_compare);
  elem_t elem = create_shelf(shelf, shelf_name, amount);
  list_append(shelf_list, elem);
  
  item_t *new_item = malloc(sizeof(item_t));
  new_item->name=name;
  new_item->desc=desc;
  new_item->price=price;
  new_item->shelves=shelf_list;
  return new_item;
}

  
//returns true if a shelf in the provided tree has the same name as the provided char shelfname
//else returns false
bool shelf_exists(tree_t *tree, elem_t *shelfname)
{
  elem_t *lists=tree_elements(tree); 
  for(int i = 0; i < tree_size(tree); ++i)
    {

      elem_t item = lists[i];
      list_t *shelf = ((item_t *) item.p)->shelves;
      if(list_contains(shelf, *shelfname))
        {
          //free(lists);
          return true;
        }
    }
  //free(lists);
  return false;
}

/*
  Return true if item with specified name has shelf with specified name
  If item with specified name does not exist, returns false
*/
bool item_has_shelf(tree_t *tree, elem_t *item_name, char *shelf_name)
{
  if (tree_has_key(tree, *item_name))  // Can't do tree_get if item does not exist
    {
      item_t *item = malloc(sizeof(item_t));
      tree_get(tree, *item_name, (elem_t*)item);
      if (get_shelf_in_list(item->shelves, shelf_name) != NULL)
        {
          return true;
        }
      else
        {
          return false;
        }
    }
  else
    {
      return false;
    }
}

void edit_before_adding(char *item_name, char *desc, int *price, char *shelf, int *amount, bool existing_item, tree_t *tree)
{
  char menu_action;
  do
    {
      if (existing_item)
        {
          printf("\nNamn: %s\nBeskrivning: %s\nPris: %d\n[L]agerylla att lägga på: %s\nAn[t]al: %d\n", item_name, desc, *price, shelf, *amount);
          menu_action = ask_question_char("\nVälj rad eller [a]vsluta redigering:", "LTA");
        }
      else
        {
          printf("\nNamn: %s\n[B]eskrivning: %s\n[P]ris: %d\n[L]agerylla att lägga på: %s\nAn[t]al: %d\n", item_name, desc, *price, shelf, *amount);
          menu_action = ask_question_char("\nVälj rad eller [a]vsluta redigering:", "BPLTA");
        }

      switch (menu_action)
        {
        case 'B':
          printf("Nuvarande beskrivning: %s\n", desc);
          strcpy(desc, ask_question_string("Ny beskrivning:"));
          puts("Beskrivning redigerad.");
          break;

        case 'P':
          printf("Nuvarande pris: %d\n", *price);
          *price = ask_question_int("Nytt pris:");
          puts("Pris redigerat.");
          break;

        case 'L':
          printf("Nuvarande vald hylla: %s\n", shelf);

          elem_t *new_name =malloc(sizeof(elem_t));
          new_name->p = ask_question_shelf("Välj ny hylla");
          // New shelf must not be used anywhere else, except in item with same name
          while(shelf_exists(tree, new_name) && !item_has_shelf(tree, (elem_t *)item_name, new_name->p))
            {
              new_name->p = ask_question_shelf("Den hyllan används redan!\nSkriv det nya hyllnamnet");
            }
          strcpy(shelf, new_name->p);
          puts("Ny hylla vald");
          break;

        case 'T':
          printf("Nuvarande antal: %d\n", *amount);
          *amount = ask_question_int("Välj nytt antal:");
          puts("Nytt antal valt.");
          break;

        case 'A':
          break;
        }
      printf("\n");
    }
  while (menu_action != 'A');
}




void add_goods(tree_t *tree, action_t *act)
{
  bool item_exists;
  char *desc;
  int price;
  item_t *existing_item = malloc(sizeof(item_t));
  elem_t *shelf_name= malloc(sizeof(item_t));
  //First get item name, description and price.
  //If item exists, we get from db, otherwise ask for it.
  elem_t *item_name= malloc(sizeof(item_t));
  item_name->p = ask_question_string("Skriv varans namn:");
  if (tree_has_key(tree, *item_name) == false)
    {
      item_exists = false;
      desc  = ask_question_string("Skriv varans beskrivning:");
      price = ask_question_int("Skriv varans pris i öre:");
    }
  else
    {
      item_exists = true;
      printf("'%s' finns redan, använder samma beskrivning och pris\n", (char*)item_name->p);
      tree_get(tree, *item_name, (elem_t*)existing_item);
      desc  = existing_item->desc;
      price = existing_item->price;
    }

  //Get shelf to store on and amount to store
   if(item_exists)
     {
       shelf_name->p = ask_question_shelf("Skriv hylla att lägga varan på:");
       if(shelf_exists(tree, shelf_name))
         {
           // If shelf is not used by this item, and already exists, we have to ask for another one
           while(get_shelf_in_list(existing_item->shelves, shelf_name->p)==NULL && shelf_exists(tree, shelf_name))
             {
               puts("Den hyllan används redan av en annan vara!");
               shelf_name->p = ask_question_shelf("Skriv hylla att lägga varan på:");
             }
         }
     }
   else
     {
       shelf_name->p = ask_question_shelf("Skriv hyllan att lägga varan på:");
       while(shelf_exists(tree, shelf_name))
         {
           puts("Den hyllan används redan av en annan vara!");
           shelf_name->p = ask_question_shelf("Skriv hylla att lägga varan på:");
         }
     }
   int amount = ask_question_int("Hur många av den här varan vill du lägga till?");



   //Allow user to confirm, edit or abort adding item
   char choice;
   do
     {
       printf("\nNamn: %s\nBeskrivning: %s\nPris: %d öre\nAntal: %d\nHylla att lägga på: %s\n", (char*)item_name->p, desc, price, amount, (char*)shelf_name->p);		
       choice = ask_question_char("Vill du lägga till denna vara?\n[J]a\n[N]ej\n[R]edigera", "JNR");
       switch(choice)		
         {		
         case 'J':		
           break;		
           
         case 'N':		
           puts("Tilläggning avbruten");
           //should free things here
           return;		
           
         case 'R':
           edit_before_adding((char*)item_name->p, desc, &price, (char*)shelf_name->p, &amount, item_exists, tree);
           break;
         }
       printf("Lägger till vara '%s'\n", (char*)item_name->p);
     }
   while(choice != 'J');

  if(item_exists==true){act->type=3;}
  else if(item_exists==false){act->type=1;}
  
  //Need to insert item if it did not exist
  if (!item_exists)
    {
      existing_item = make_item(item_name->p, desc, price, shelf_name->p, amount);
      
      act->merch = existing_item;
      elem_t insert_item;
      elem_t insert_item_key;
      insert_item.p = existing_item;
      insert_item_key.p = existing_item->name;
      elem_t *test_item = malloc(sizeof(elem_t));
      elem_t test_key;
      test_key.p="a";

      
      tree_insert(tree, insert_item_key, insert_item);

      tree_get(tree, test_key, test_item);

    }
  //Otherwise add amount to new or existing shelf.
  else
    {
      act->copy=make_copy(existing_item);

      shelf_t *shelf = get_shelf_in_list(existing_item->shelves, shelf_name->p);
      if (shelf == NULL)
        {
          shelf_t *new_shelf=malloc(sizeof(shelf_t));
          new_shelf->name=shelf_name->p;
          new_shelf->amount=amount;
          elem_t shelf_insert;
          shelf_insert.p=new_shelf;
          list_append(existing_item->shelves, shelf_insert);
        }
      else
        {
          shelf->amount += amount;
        }
      act->merch=existing_item;
    }
}




void remove_goods()
{}

void edit_goods()
{}

//===================List Goods===================
  
void action_print_item(elem_t *item, tree_t *tree, action_t *act)
{
  print_item(*item);
}


void list_goods(tree_t *tree, action_t *act)
{
  char *menu_add = "[V]isa en vara";
  char menu_opt = 'V';
  // If user selects 'V', action_print_item is run
  select_goods(tree, menu_add, menu_opt, act, action_print_item);
}

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
  action_t *act = malloc(sizeof(struct action));
  act->type = 0;
  
  do
    {
       val = ask_question_char("\n[L]ägga till en vara\n\
[T]a bort en vara\n\
[R]edigera en vara\n\
Ån[g]ra senaste ändringen\n\
Lista [h]ela varukorgen\n\
[S]ortera databasen\n\
[K]ontrollera databasens sortering\n\
[A]vsluta\n\n\
Vad vill du göra?", "LTRGHASK");

      
      switch (val)
        {
        case 'L': // Lägga till
          add_goods(tree, act);
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
          list_goods(tree, act);
          break;
          
        case 'S': //sortera
          puts("Ta bort;Not yet implemented!");
          break;
          
        case 'K': //kontrollera sortering
          puts("Ta bort;Not yet implemented!");
          break;
        }
    }
  while (val != 'A');
}


int main(void)
{
  tree_t *tree = tree_new(element_copy, free_key ,free_elem,key_compare);//för o kunna fixa loop

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
