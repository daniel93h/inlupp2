#ifndef __item_h__
#define __item_h__

#include "list.h"

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
item_t *make_item(char *name, char *desc, int price, char *shelf_name, int amount);

#endif
