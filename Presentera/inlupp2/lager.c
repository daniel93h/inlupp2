#include <stdio.h>
#include "utils.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "tree.h"
#include "lager.h"
#include "filhanterare.h"

typedef struct item item_t;
typedef struct shelf shelf_t;
typedef struct action action_t;
typedef struct edit edit_t;

struct item
{
  char *name;
  char *description;
  int price;
  list_t *storage;
};

struct shelf
{
    char *name;
    int amount;
};

struct action
{
  int type; // NOTHING = 0, ADD = 1, REMOVE = 2, EDIT = 3
  item_t *merch;
  item_t copy;
  
};

struct edit
{
  char* name;
  shelf_t *found;
};

bool print_shelf(elem_t shelf, void *data)
{
  printf("Shelf: ");
  printf("%s\n", ((shelf_t*)shelf.p)->name);
  printf("Amount: ");
  printf("%d\n", ((shelf_t*)shelf.p)->amount);
  return true;
}

void print_item(elem_t item, void *data)
{
  printf("Name: ");
  printf("%s\n",((item_t*)item.p)->name);
  printf("Desc: ");
  printf("%s\n", ((item_t*)item.p)->description);
  printf("Price: ");
  printf("%.2f\n", (((item_t*)item.p)->price/100.0));
  list_apply(((item_t*)item.p)->storage, print_shelf, data);
}

int tree_key_compare(tree_key_t key, tree_key_t data)
{
  return strcmp(key.p, data.p);
}

int shelf_compare(elem_t shelf, elem_t data)
{
  if(strcmp(((shelf_t*)shelf.p)->name, data.p) == 0)
    {
      return 0;
    }
  else if(strcmp(((shelf_t*)shelf.p)->name, data.p) < 0)
    {
      return -1;
    }
  else
    {
  
  return 1;

    }
}


elem_t elem_copy_fun(elem_t elem)
{
  elem_t tmp = elem;
  return tmp;
}

void free_key(tree_key_t key)
{
  char* namn = key.p;
  free(namn);
  
}

void free_item(elem_t elem)
{
  item_t *item = elem.p;
  free(item->name);
  free(item->description);
  list_delete(item->storage, true);
  free(item);
}

void free_shelf(elem_t elem)
{
  shelf_t *shelf = elem.p;
  free(shelf->name);
  free(shelf);
}

shelf_t *make_shelf()
{

  shelf_t *shelf = calloc(1, sizeof(shelf_t));
  shelf->name =  ask_question_shelf("What shelf?");
  shelf->amount = ask_question_int("How many items is on the shelf?");

  return shelf ;

}

bool check_shelf_tree_apply_fun(tree_key_t key, elem_t elem, void *data)
{
  elem_t tmp = { .p = data};
  int i = list_contains(((item_t*)elem.p)->storage, tmp);
  if(i < 0)
    {
      return false;
    }

  return true;
  
}



void add_shelf_loop(tree_t *tree, elem_t elem, elem_t *result)
{
  char input_shelf = ask_question_add_shelf_menu("");
  tree_get(tree, elem, result);
  bool shelf_loop = true;
  while(shelf_loop)
    {
      if(input_shelf == 'Y' || input_shelf == 'y')
        {

          shelf_t *shelf_to_add = make_shelf();
          elem_t shelf = { .p = shelf_to_add};

          while(tree_apply(tree, 1, check_shelf_tree_apply_fun, shelf_to_add->name))
            {
              free(shelf_to_add->name);
              puts("--------------------------------");
              puts("The shelf is already occupied!");
              puts("Try again with a different shelf");
              puts("--------------------------------");
              shelf_to_add->name = ask_question_shelf("Input new shelf");
            }
                                       

          list_append(((item_t*)result->p)->storage, shelf);
          print_item(*result, NULL);
          return;
            

        }
      else if(input_shelf == 'N' || input_shelf == 'n')
        {
          shelf_loop = false;
        }

      else if(input_shelf == 'E' || input_shelf == 'e')
        {
          break;
        }

      shelf_loop = false;
    }

}

item_t *make_item(char *name, char *description, int price, char *shelf_name, int amount)
{

  shelf_t *new_shelf = calloc(1, sizeof(shelf_t));
  
  new_shelf->name = strdup(shelf_name);
  new_shelf->amount = amount;

  elem_t elem = { .p = new_shelf};

  item_t *item = calloc(1, sizeof(item_t));

  item->name =strdup(name);
  item->description = strdup(description);
  item->price = price;
  item->storage = list_new(NULL, free_shelf, shelf_compare);
  list_append(item->storage, elem);

  elem_t item_elem = { .p = item};
  
  return item_elem.p;
}

item_t *input_item(tree_t *tree, char *name)
{
  elem_t result;
  elem_t elem = { .p = name};
  elem_t elem1;
  
  if(tree_has_key(tree, elem))
    {
      puts("--------------------------------");
      puts("There is already an item with this name");
      puts("--------------------------------");
      add_shelf_loop(tree, elem, &result);
      tree_get(tree, elem, &elem1);
      return result.p;
    }  
  char *description = ask_question_string("Describe the item");
  int price = ask_question_int("What does it cost?");
  char *shelf_name = ask_question_shelf("What shelf?");
  
  while(tree_apply(tree, 1, check_shelf_tree_apply_fun, shelf_name))
    {
      
      puts("--------------------------------");
      puts("The shelf is already occupied!");
      puts("Try again with a different shelf");
      puts("--------------------------------");
      shelf_name = ask_question_shelf("Input new shelf");
    }
  int shelf_amount = ask_question_int("How many items is on the shelf?");

  item_t *item =make_item(name, description, price, shelf_name, shelf_amount);
  free(name);
  free(description);
  free(shelf_name);
  return item;
  
}


void want_to_add_to_db_loop(tree_t *tree, item_t *item)
{
  bool loop = true;
  while (loop)
    {
      char input = ask_question_add_menu("Do you want to save it to the database?");
      if (input == 'Y' || input == 'y')
        {
          elem_t item_elem = { .p = item};
          puts("You chose Yes!");
          print_item(item_elem, NULL);
          tree_key_t key = { .p = strdup(item->name)};
          elem_t elem = { .p = item};
          
          tree_insert(tree, key, elem);
          puts("----------------------------------------------");
          puts("The item above is now stored in the database\n");
          puts("----------------------------------------------");
          break;

        }
      else if (input == 'N' || input == 'n')
        {
          puts("You chose No!");
          
          elem_t item_to_free = { .p = item};
          free_item(item_to_free);
          
          break;
        }
      else if (input == 'E' || input == 'e')
        {

          puts("You chose Edit!");
          item_t *edited_item = input_item(tree, strdup(item->name));
          elem_t item_name = { .p = strdup(edited_item->name)};
          elem_t item_elem = { .p = edited_item };

          
          tree_insert(tree, item_name, item_elem);

          
          elem_t item_to_free = { .p = item};
          free_item(item_to_free);
          break;
        }

    }
}

void add_item(tree_t *tree)
{
  char *name = ask_question_string("What's the item?");
  
  item_t *item = input_item(tree, name);
  want_to_add_to_db_loop(tree, item);
  
}




bool get_shelf(elem_t shelf, void *edit)
{

  if(strcmp(((shelf_t*)shelf.p)->name, ((edit_t*)edit)->name) == 0)
    
    {
      ((edit_t*)edit)->found = (shelf_t*)shelf.p;
      return true;
    }

  return false;
}


void edit_shelf(shelf_t *shelf, char *new_shelf, tree_t *tree, elem_t *item)
{

  
  if(strcmp(shelf->name, new_shelf) == 0)
    {
      char *name_amount = ask_question_string("input name to move to another shelf, amount to edit the amount, or delete to remove the item from this shelf");
      if((strcmp(name_amount, "name") == 0) || (strcmp(name_amount, "Name") == 0))
        {
          char *new_name = ask_question_string("Input the new name for the shelf: ");
          shelf_t *tmp = calloc(1, sizeof(shelf_t));
          tmp->name = new_name;   
  
          
          if(tree_apply(tree, 1, check_shelf_tree_apply_fun, new_name))
            {
              puts("----------------------------------------------------");
              puts("----------------------------------------------------");
              puts("The shelf you entered is already occupied. Try again");
              puts("----------------------------------------------------");
              puts("----------------------------------------------------");
              free(tmp);
              return edit_shelf(shelf, new_shelf, tree, item);
              
            }

          else
            {
            shelf->name = new_name;
            free(tmp);
            return;
            }
        }


      else if((strcmp(name_amount, "amount") == 0) || (strcmp(name_amount, "Amount") == 0))
        {

          int new_amount = ask_question_int("Input the new amount: ");
          shelf->amount = new_amount;

          return;

        }
      else if((strcmp(name_amount, "Delete") == 0) || (strcmp(name_amount, "delete") == 0))
        {
          elem_t shelf_edit_elem = {.p = shelf->name};
          int contains = list_contains(((item_t*)item)->storage, shelf_edit_elem); 
          list_remove(((item_t*)item)->storage, contains, false);
          return;

        }
    }

  
  return;

}





void list_db_aux(tree_t *tree, int *no_items, tree_key_t *all_keys, int *page_counter, elem_t *return_elem)
{
  int counter = *page_counter * 20;

  for(int i = 1; i <= 20 && counter < *no_items; i++)
    {
      
      printf("%d. %s\n", i, ((char**)all_keys)[counter]);
      counter = counter + 1;

    }

  char *input = ask_question_string("Choose item by index, 'n' to show next-,'p' for previous page of items or 'b' to go back to the menu");
  int input_int = atoi(input);
  

  if(*input != 'n' && *input != 'b' && *input != 'p' && input_int < 0 && input_int > 20)
    {
      printf("--------------------------------------------------\n");
      printf("--------------------------------------------------\n");
      puts("                 invalid input");
      printf("--------------------------------------------------\n");
      printf("--------------------------------------------------\n");
      list_db_aux(tree, no_items, all_keys, page_counter, return_elem);
    }
      
      
  else if(strcmp(input, "n") == 0)
    {
      *page_counter = *page_counter + 1;
      if(*page_counter * 20 >= *no_items)
        {

          *page_counter = *page_counter -1;
          puts("You are already on the last page!");

          list_db_aux(tree, no_items, all_keys, page_counter, return_elem);
        }

      list_db_aux(tree, no_items, all_keys, page_counter, return_elem);
    }

  else if(strcmp(input, "p") == 0)
    {
      *page_counter = *page_counter -1;
      if((*page_counter) < 1)
        {
          puts("There is no previous page\n");
          *page_counter = 0;
          list_db_aux(tree, no_items, all_keys, page_counter, return_elem);
        }

      list_db_aux(tree, no_items, all_keys, page_counter, return_elem);
    }

  else if(strcmp(input, "b") == 0)
    {
      return;
    }

  else if (input_int + (*page_counter * 20) > *no_items  || input_int < 1 || input_int > 20)
    {
      printf("--------------------------------------------------\n");
      printf("--------------------------------------------------\n");
      puts("invalid input");
      printf("--------------------------------------------------\n");
      printf("--------------------------------------------------\n");
      list_db_aux(tree, no_items, all_keys, page_counter, return_elem);
          
    }

      
  elem_t result = {.p = NULL};
  tree_key_t item_to_get = all_keys[input_int + (*page_counter*20)-1 ]; 
  tree_get(tree, item_to_get, &result);
  
  print_item(result, NULL);
  *return_elem = result;
  free(input);

  return; 
}


elem_t list_db(tree_t *tree, int *no_items)
{
  elem_t return_elem;
  int page_counter = 0;

  tree_key_t *all_keys = tree_keys(tree);


  list_db_aux(tree, no_items, all_keys, &page_counter, &return_elem);
  free(all_keys);
  
  return return_elem;

}

void list_db_edit_aux(tree_t *tree, int *no_items, tree_key_t *all_keys, int *page_counter, int *return_int)
{
  int counter = *page_counter * 20;



  for(int i = 1; i <= 20 && counter < *no_items; i++)
    {
          
      printf("%d. %s\n", i, ((char**)all_keys)[counter]);

      counter = counter + 1;


    }

  char *input = ask_question_string("Choose the item by index that you want to edit, 'n' to show next-,'p' for pre                                         vious page of items or 'b' to go back to the menu");
  int input_int = atoi(input);

  if(*input != 'n' && *input != 'b' && *input != 'p' && input_int < 0 && input_int > 20)
    {
      printf("--------------------------------------------------\n");
      printf("--------------------------------------------------\n");
      puts("invalid input");
      printf("--------------------------------------------------\n");
      printf("--------------------------------------------------\n");
      list_db_edit_aux(tree, no_items, all_keys, page_counter, return_int);
    }
      
  else if(strcmp(input, "n") == 0)
    {
      *page_counter = *page_counter + 1;

      if(*page_counter * 20 >= *no_items)

        {
          puts("You are already on the last page!");
          *page_counter = *page_counter -1;
          return list_db_edit_aux(tree, no_items, all_keys, page_counter, return_int);

        }

      return list_db_edit_aux(tree, no_items, all_keys, page_counter, return_int);
    }

  else if(strcmp(input, "p") == 0)
    {
      *page_counter = *page_counter -1;
      if((*page_counter) < 1)
        {
          puts("There is no previous page\n");
          *page_counter = 0;

          return list_db_edit_aux(tree, no_items, all_keys, page_counter, return_int);
        }



      return list_db_edit_aux(tree, no_items, all_keys,  page_counter, return_int);
    }

  else if(strcmp(input, "b") == 0)
    {
      *return_int = -1;
      return;
    }

      
  else if (input_int + (*page_counter * 20) > *no_items  || input_int < 1 || input_int > 20)
    {
      printf("--------------------------------------------------\n");
      printf("--------------------------------------------------\n");
      puts("Not a item in range. Choose item with index 1-20");
      printf("--------------------------------------------------\n");
      printf("--------------------------------------------------\n");
      list_db_edit_aux(tree, no_items, all_keys, page_counter, return_int);
          
    }

      
  *return_int = (atoi(input) + (*page_counter*20));
  printf("%d\n", *return_int);
  return;
}




int list_db_edit(tree_t *tree, int *no_items)
{
  int return_int = 0;
  int page_counter = 0;

  tree_key_t *all_keys = tree_keys(tree);
  if(*no_items == 0)

    {
      puts("The database is empty.");
    }

  else list_db_edit_aux(tree, no_items, all_keys, &page_counter, &return_int);
  free(all_keys);
  
  return return_int;
}



shelf_t *copy_shelf(shelf_t *shelf)
{
  shelf_t *shelf_copy = calloc(1, sizeof(shelf_t));
  shelf_copy->name = strdup(shelf->name);
  shelf_copy->amount = shelf->amount;
  return shelf_copy;
}


list_t *copy_list(list_t *list)
{

  elem_t result;
  
  list_t *new_list = list_new(NULL, NULL, NULL);
  for(int i = 0; i < list_length(list); i++)
    {
      
      list_get(list, i, &result);
      
      elem_t new_copy = { .p = copy_shelf(result.p)};
      list_insert(new_list, i, new_copy);
       
              
    }
  return new_list;
}


bool edit_db(tree_t *tree, int *size, action_t *action)
{
  bool loop = true;
  int index = list_db_edit(tree, size);


  if (index == -1)
  {
    return false;
  }
  tree_key_t *all_keys = tree_keys(tree);
  elem_t item_by_index = all_keys[index-1];
  free(all_keys);

  elem_t result;
      
  tree_get(tree, item_by_index, &result);
  action -> copy = *(item_t*)result.p;
  print_item(result, NULL);


  while(loop)

    {

      char input = ask_question_edit_menu("What do you want to edit?");

      if(input == 'D' || input == 'd')
        {
          printf("Current description: %s\n", ((item_t*)result.p)->description);
          printf("--------------------------------------------------\n");
          ((item_t*)result.p)->description = ask_question_string("New description: ");
        }
      else if(input == 'P' || input == 'p')
        {
          printf("Current price: %d\n", ((item_t*)result.p)->price);
          printf("--------------------------------------------------\n");
          ((item_t*)result.p)->price = ask_question_int("New price: ");
        }
      else if(input == 'S' || input == 's')
        {
          action->type = 4;
          
          action->copy.name = strdup(((item_t*)result.p)->name);
          free(((item_t*)result.p)->name);
          
          action->copy.description = strdup(((item_t*)result.p)->description);
          free(((item_t*)result.p)->description);
          
          action->copy.price = ((item_t*)result.p)->price;
          action->copy.storage = copy_list(((item_t*)result.p)->storage);
          list_apply(((item_t*)result.p)->storage, print_shelf, NULL);
          char *shelf_edit = ask_question_shelf("Choose a shelf to edit");
          elem_t shelf_edit_elem = {.p = shelf_edit};
          
          int contains = list_contains(((item_t*)result.p)->storage, shelf_edit_elem);
          if(contains >= 0)
            {
              edit_t edit;
              edit.name = shelf_edit_elem.p;
              edit.found = NULL;
             
              list_apply(((item_t*)result.p)->storage, get_shelf, &edit);
              
              edit_shelf(edit.found, shelf_edit_elem.p, tree, result.p);
            }
          else
            {
              printf("--------------------------------------------------\n");
              puts("The item is not on this shelf");
              printf("--------------------------------------------------\n");
              
            }


        }

      else if(input == 'Q' || input == 'q')
        {
          loop = false;
        }
    }
  return false;

}





void undo (tree_t *tree, action_t *action)
{

  switch(action->type)

  {
    

    case 1 :
      {
        break;
      }
    case 2 :

      {
        break;
      }
    case 3 :
      {
        elem_t result;
        tree_key_t item_copy = { .p = action->copy.name};
        tree_get(tree, item_copy, &result); //fetches edited item
        *((item_t*)result.p) = action->copy;
        break;
      }
    case 4 :
      {
        elem_t result;
        tree_key_t name_copy = { .p = action->copy.name};

        tree_remove(tree, name_copy, &result);

        item_t *old_item = calloc(1, sizeof(item_t));

        *old_item = action->copy;
        elem_t old_item_elem = { .p = old_item};
        
        
        
        
        tree_insert(tree, name_copy, old_item_elem);
        
                
        break;
      }
    default : 
      {
        puts("Nothing to undo!");
      }

  }

}



void event_loop(tree_t *tree, char *filename)
{

  action_t *action = calloc(1,sizeof(action_t));
  bool loop = true;

  while(loop){

  char input = ask_question_menu();

  int size = tree_size(tree);


  if(input == 'A' || input == 'a'){
    add_item(tree);
  }
  else if(input == 'B' || input == 'b'){

    if(tree_size(tree) < 1)
      {
        puts("------------------------------------------");
        puts("---------the database is empty------------");
        puts("------------------------------------------");
      }
    else
      {
    puts("------------------------------------------");
    puts("---------The tree is now balanced!--------");
    puts("------------------------------------------");

    tree_t *new_tree = tree_balance(tree);
    tree = new_tree;
      }
  }
  else if(input == 'R' || input == 'r'){
    elem_t result;
    elem_t item_to_remove = list_db(tree, &size);
    tree_key_t key = {.p = ((item_t*)item_to_remove.p)->name };
    tree_remove(tree, key, &result);
  }
  else if(input == 'E' || input == 'e'){
   action->type = 3;
   edit_db(tree, &size,action);
  }
  else if(input == 'U' || input == 'u'){
    
    undo(tree,action);
  }
   else if(input == 'L' || input == 'l'){
    if(tree_size(tree) < 1)
      {
        puts("------------------------------------------");
        puts("---------the database is empty------------");
        puts("------------------------------------------");
      }
    else
      {
    list_db(tree, &size);
      }
  }
  else if(input == 'Q' || input == 'q'){
    free(action);


    save_tree(tree, filename);
    tree_delete(tree, true, true);
    loop = false;
  }

  }

}

int main(int argc, char* argv[])
{
  
  tree_t *tree = tree_new(elem_copy_fun, free_key, free_item, tree_key_compare);
  load_tree(tree, argv[1]);
  event_loop(tree, argv[1]);
}


