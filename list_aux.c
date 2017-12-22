#include "common.h"
#include "list.h"
#include "list_aux.h"

#include <stdlib.h>

struct list
{
  element_free_fun free;
  element_comp_fun compare;
  element_copy_fun copy;
  link_t *first;
  link_t *last;
  int length;
};

struct link
{
  elem_t elem;
  link_t *next;
};

void find_index(int index, link_t *current)
{
  int index_count=0;
  while(index_count < index)
    {
      current = current->next;
      ++index_count;
    }
  return;
}

int convert_negative_index(int index, list_t *list)
{
  int i = list_length(list);
  index = i+1+index;
  return index;
}

int convert_invalid_index(int index, list_t *list)
{
  if(index>list_length(list))
    {
      index = -1;
    }
  if(index<0)
    {
      index = convert_negative_index(index, list);
      if(index<0)
        {
          index = 0;
        }
    }
  return index;
}

bool check_valid_index(list_t *list, int index)
{
 if(index>list_length(list))
    {
      return false;
    }
 if(convert_negative_index(index,list) < 0)
   {
     return false;
   }
 return true;
   
   
}

bool list_insert_edge_cases(list_t *list,int index, elem_t elem)
{
  if(index==0)
    {
      list_prepend(list,elem);
      return true;
    }
  if(index==-1)
    {
      list_append(list,elem);
      return true;
    }
  if(index>list_length(list))
    {
      list_append(list,elem);
      return true;
    }
  if(index<0)
    {
      int i = list_length(list);
      index = i+1+index;
      if(index<0)
        {
          list_prepend(list,elem);
          return true;
        }
    }
  return false;
}
