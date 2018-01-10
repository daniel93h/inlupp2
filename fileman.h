#ifndef __filhanterare_h__
#define __filhanterare_h__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "list.h"
#include "tree.h"


typedef struct item item_t;
typedef struct shelf shelf_t;

extern item_t *make_item(char *name, char *description, int price, char *shelf_name, int amount);

bool save_item_to_file(tree_key_t key, elem_t elem, void *file_pointer);//Sparar en vara till fil.

bool write_file(tree_t *tree, FILE *fptr);


tree_t *read_file(tree_t *tree, FILE *fptr);

tree_t *load_tree(tree_t *tree, const char *filename);//Laddar databas(tree)

void save_tree(tree_t *tree, const char *filename);

#endif
