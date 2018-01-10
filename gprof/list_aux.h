#ifndef __list_aux_h__
#define __list_aux_h__

#include "list.h"
#include "common.h"

typedef struct link link_t;
typedef struct list list_t;

bool check_valid_index(list_t *list, int index);
bool list_insert_edge_cases(list_t *list, int index, elem_t elem);
bool list_delete_edge_cases(list_t *list, int index);

int convert_invalid_index(int index, list_t *list);
int convert_negative_index(int index, list_t *list);

void find_index(int index, link_t *current);

#endif
