#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "list.h"
#include "tree.h"
#include "common.h"
#include <assert.h>



typedef struct list list_t;
typedef struct link link_t;



struct list
{
  link_t *first;
  link_t *last;
  size_t size;
  element_free_fun free;
  element_comp_fun compare;
  element_copy_fun copy;  
};



struct link
{
  elem_t elem;
  link_t *next;
};

static
elem_t list_no_copy(elem_t element)
{
  return element;
}



link_t *link_new(elem_t element, link_t *next)
{
  link_t *link = calloc(1 ,sizeof(link_t));

  if (link)
    {
      *link = (link_t) { .elem = element, .next = next };
    }

  return link;

}

size_t list_size(list_t *list)
{
  return list->size;
}


  
list_t *list_new(element_copy_fun copy, element_free_fun free, element_comp_fun compare)
{
  list_t *list = calloc(1, sizeof(list_t));


  if (list)
    {
      *list = (list_t) {
        .copy = (copy) ? copy : list_no_copy,
        .free = free,
        .compare = compare,
        .size = 0
      };
    }

  return list;

}

int adjust_index(int index, int size)
{
  // If the size is 0 and index < 0 to prevent seg fault
  if(index < 0 && size == 0)
    {
      return 0;
    }
 
  /// Negative indexes count from the back
  else if (index < 0)
    {
      return size + index;
    }
  
  /// Index too big, adjust to after last element
  else if (index > size)
    {
      return size;
    }

  /// Index OK
  return index;
}


link_t **list_find(list_t *list, int index)
{
  link_t **cursor = &(list->first);

  int adjusted_index = 
    (0 <= index && index < list->size) ? index : adjust_index(index, list->size);

  for (int i = 0; i < adjusted_index && *cursor; ++i)
    {
      cursor = &(*cursor)->next;
    }

  return cursor;
}



void list_insert(list_t *list, int index, elem_t element)
{
  int adjusted_index =
    (0 <= index && index < list->size) ? index : adjust_index(index, list->size +1);


  link_t **cursor = list_find(list, adjusted_index);

  if (*cursor || adjusted_index == list->size)
    {
      
      *cursor = link_new(list->copy(element), *cursor);
      ++list->size;
    }
  
  

}

void list_append(list_t *list, elem_t element)
{
  list_insert(list, -1, element);
}

void list_prepend(list_t *list, elem_t element)
{
  list_insert(list, 0, element);
}




void list_remove(list_t *list, int index, bool delete)
{
  
  int adjusted_index =
    (0 <= index && index < list->size) ? index : adjust_index(index, list->size);
  
  link_t **cursor = list_find(list, adjusted_index);

  if (*cursor)
    {
      link_t *to_remove = *cursor;
      *cursor = (*cursor)->next;

      if (delete)
        {
          list->free(to_remove->elem);
        }
      free(to_remove);

      --list->size;
     
    }
  else
    {
      return;
    }
}

void list_delete(list_t *list, bool delete)
{
  while ((list->size) > 0)
    {
      list_remove(list, 0, delete);
    }
  assert(list->size == 0);

  free(list);
}

bool list_get(list_t *list, int index, elem_t *result)
{
  int adjusted_index =
    (0 <= index && index < list->size) ? index : adjust_index(index, list->size);
  
  link_t **cursor = list_find(list, adjusted_index);
  
  if (*cursor)
    {
      *result = (*cursor)->elem;
      return true;
    }

  return false; 
   

}


bool list_first(list_t *list, elem_t *result)
{
  if(list_get(list, 0, result))
    {
      return true;
    }
  
  return false;
}

bool list_last(list_t *list, elem_t *result)
{
 if(list_get(list, -1, result))
    {
      return true;
    }
  
  return false;
}


int list_length(list_t *list)
{
  return list->size;
}



bool list_apply_aux(link_t *link, elem_apply_fun fun, void *data, bool *result)
{
  if(link == NULL)
    {
      return result;
    }
  
  if(fun((*link).elem, data))
    {

      *result = true;
      
    }
  list_apply_aux(link->next, fun, data, result);
  return result;
}




bool list_apply(list_t *list, elem_apply_fun fun, void *data)
{
  bool result = false;
  return list_apply_aux(list->first, fun, data, &result);
}


int list_contains(list_t *list, elem_t elem)
{
  link_t *cursor = list->first;
  
  if(list->compare != NULL)
    {
      for (int i = 0; i < list->size && cursor; ++i)
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

  for (int i = 0; i < list->size && cursor; ++i)
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
