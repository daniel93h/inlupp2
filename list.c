#include "common.h"
#include "list.h"
#include "list_aux.h"
#include "tree.h"

#include <stdlib.h>

typedef struct list list_t;
typedef struct link link_t;

struct link
{
  elem_t elem;
  link_t *next;
};

struct list
{
  element_free_fun free;
  element_comp_fun compare;
  element_copy_fun copy;
  link_t *first;
  link_t *last;
  int length;
};

/*
  if (list->free) {
  list->free(current->elem);
  }
  free(current)
*/

elem_t list_no_copy(elem_t elem)
{
  return elem;
}

void list_no_free(elem_t elem)
{
  return;
}

list_t *list_new(element_copy_fun copy, element_free_fun free, element_comp_fun compare)
{
  list_t *new_list = calloc(1, sizeof(list_t));
  new_list->copy = copy ? copy : list_no_copy;
  new_list->free = free ? free : list_no_free;
  new_list->compare = compare;
  new_list->length = 0;
  return new_list;
}

void list_insert(list_t *list, int index, elem_t elem)
{
  if(list_insert_edge_cases(list,index,elem))
    {
      return;
    }
  if(index<0)
    {
      index = convert_negative_index(index, list);
    }
  link_t *current = list->first;
  find_index(index,current);
  link_t *new_element = calloc(1,sizeof(link_t));
  new_element->elem = elem;
  if(list->first == NULL)
    {
      list->first = new_element;
      list->last = new_element;
    }
  else
    {
      new_element->next = current;
      current = new_element;
    }
  list->length++;
}

void list_append(list_t *list, elem_t elem)
{
  link_t *new_elem = calloc(1, sizeof(link_t));
  new_elem->elem = elem;
  if(list->first == NULL)
    {
      list->first = new_elem;
      list->last = new_elem;
    }
  else
    {
      list->last->next = new_elem;
      list->last = new_elem;
    }
  list->length++;
}

void list_prepend(list_t *list, elem_t elem)
{
  link_t *new_elem = calloc(1, sizeof(link_t));
  new_elem->elem = elem;
  if(list->first == NULL)
    {
      list->first = new_elem;
      list->last = new_elem;
    }
  else
    {
      new_elem->next = list->first->next;
      list->first = new_elem;
    }
  list->length++;
}

void list_remove(list_t *list, int index, bool delete)
{
  index = convert_invalid_index(index,list);
  link_t *current = list->first;
  find_index(index,current);
  if(delete)
    {
      list->free(current->elem);
      free(current);
    }
  list->length--;
}

bool list_get(list_t *list, int index, elem_t *result)
{
  if(check_valid_index(list,index))
    {
      link_t *current=list->first;
      index = convert_invalid_index(index,list);
      find_index(index,current);
      *result = current->elem;
      return true;
    }
  return false;
}

bool list_first(list_t *list, elem_t *result)
{
  list_get(list, 0, result);
  return true;
}

bool list_last(list_t *list, elem_t *result)
{
  list_get(list, -1, result);
  return true;
}

int list_length_iter(list_t *list)
{
  int count = 0;
  link_t *current = list->first;
  while(current != NULL)
    {
      count++;
      current = current->next;
    }
  return count;
}


int list_length(list_t *list)
{  
  return list->length;
}

void list_delete(list_t *list, bool delete)
{
  list_remove(list,0,delete);
  if(list_length(list)!=0)
    {
      list_delete(list,delete);
    }
  free(list);
}

bool list_apply(list_t *list, elem_apply_fun fun, void *data)
{
  bool apply = false;
  link_t *current = list->first;
  while(current->next != NULL)
    {
    apply = fun(current->elem, data);
    current = current->next;
    if(apply != false)
      {
        return apply;
      }
    }
  return true;
}

int list_contains(list_t *list, elem_t elem)
{
  link_t *cursor = list->first;
  if(list->compare != NULL)
    {
      for (int i = 0; i < list->length && cursor; ++i)
        {
          if(list->compare((cursor)->elem, elem) != 0)
            {
              cursor = cursor->next;     
            }
          else
            {
              return i;
            }
        }
      return -1;
    }
  for (int i = 0; i < list->length && cursor; ++i)
    {
      if(cursor->elem.i == elem.i)
        {
          return i;
        }
      else
        {
          cursor = cursor->next;
        }
    }
  return -1; 
}

