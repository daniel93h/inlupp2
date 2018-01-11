#ifndef __lager_h__
#define __lager_h__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "list.h"
#include "tree.h"

typedef struct item item_t;
typedef struct shelf shelf_t;
typedef struct action action_t;
typedef struct edit edit_t;

bool print_shelf(elem_t shelf, void *data);
void print_item(elem_t item, void *data);
int tree_key_compare(tree_key_t key, tree_key_t data);
int shelf_compare(elem_t shelf, elem_t data);
elem_t elem_copy_fun(elem_t elem);
void free_key(tree_key_t key);
void free_item(elem_t elem); 
void free_shelf(elem_t elem);
shelf_t *make_shelf();
bool check_shelf_tree_apply_fun(tree_key_t key, elem_t elem, void *data);
void add_shelf_loop(tree_t *tree, elem_t elem, elem_t *result);
item_t *make_item(char *name, char *description, int price, char *shelf_name, int amount);
item_t *input_item(tree_t *tree, char *name);
void want_to_add_to_db_loop(tree_t *tree, item_t *item);
void add_item(tree_t *tree);
bool get_shelf(elem_t shelf, void *edit);
void edit_shelf(shelf_t *shelf, char *new_shelf, tree_t *tree, elem_t *item);
void list_db_aux(tree_t *tree, int *no_items, tree_key_t *all_keys, int *page_counter, elem_t *return_elem);
elem_t list_db(tree_t *tree, int *no_items);
void list_db_edit_aux(tree_t *tree, int *no_items, tree_key_t *all_keys, int *page_counter, int *return_int);
int list_db_edit(tree_t *tree, int *no_items);
shelf_t *copy_shelf(shelf_t *shelf);
list_t *copy_list(list_t *list);
bool edit_db(tree_t *tree, int *size, action_t *action);
void undo (tree_t *tree, action_t *action); 
void event_loop(tree_t *tree, char* filename); 


#endif
