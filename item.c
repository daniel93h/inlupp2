//Items have their own file to avoid circular dependencies.
#include "list.h"
#include <stdlib.h>

struct item
{
  char *name;
  char *desc;
  int price;
  list_t *shelves;
};
typedef struct item item_t;


/*
  Makes an item with specified properties, where shelf list is a single shelf
  with specified amount.
*/
item_t *make_item(char *name, char *desc, int price, char *shelf_name, int amount)
{
  shelf_t shelf = {.name=shelf_name, amount=amount};
  list_t *shelf_list = list_new();
  list_append(shelf_list, shelf);
  
  item_t *new_item = malloc(sizeof(item_t));
  new_item->name=name;
  new_item->desc=desc;
  new_item->price=price;
  new_item->shelves=shelf_list;
  return new_item;
}
