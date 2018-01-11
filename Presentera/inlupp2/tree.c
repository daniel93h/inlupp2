#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "tree.h"
#include "list.h"

typedef struct node node_t;
typedef struct tree tree_t;
typedef bool(*tree_traversal_func)(node_t *elem, void *data);

struct tree
{
  element_copy_fun element_copy;
  key_free_fun key_free;
  element_free_fun elem_free;
  element_comp_fun compare;  
  node_t *root; 
  
};

struct node
{
  tree_key_t key;
  elem_t element;
  node_t *right;
  node_t *left; 
};

bool free_node(node_t *node, void* data)
{  
  free(node);
  return true;
}


node_t *node_new(tree_key_t key, elem_t element)
{
  node_t *result = calloc(1, sizeof(struct node));//Allokerar minne till en ny nod med en key och elem. 
  result->key = key;  
  result->element = element;
  result->left = NULL;
  result->right = NULL;
  return result; 
  
}

tree_t *tree_new(element_copy_fun element_copy, key_free_fun key_free, element_free_fun elem_free, element_comp_fun compare)
{
  tree_t *tree = calloc(1, sizeof(tree_t));

  tree->root = NULL;
  *tree = (tree_t)
    {
      .element_copy = element_copy,
      .key_free = key_free,
      .elem_free = elem_free,
      .compare = compare
    };
  
  return tree; 
}
void node_delete(node_t *node, bool delete_keys, bool delete_elements, key_free_fun key_free, element_free_fun elem_free)
{
  if(node == NULL)
    {
      return; 
    }
  else
    {
      node_delete(node->right, delete_keys, delete_elements, key_free, elem_free);
      node_delete(node->left, delete_keys, delete_elements, key_free, elem_free);

      if (delete_elements && elem_free) {elem_free(node->element);}

      if(delete_keys && key_free){key_free(node->key);}
      free(node);
    }

   
}

void tree_delete(tree_t *tree, bool delete_keys, bool delete_elements)
{
  node_delete(tree->root, delete_keys, delete_elements, *tree->key_free, *tree->elem_free);
  free(tree); 
}

int tree_size2(node_t *node)
{
  if(node == NULL)
    {

      return 0;
    }

  else
    {
      return(tree_size2(node->left) + 1 + tree_size2(node->right));
    }

}



int tree_size(tree_t *tree)//Returnerar en int med antalet noder i ett träd.  
{
  return tree_size2(tree->root); 
}

 

int tree_depth_aux(node_t *node)//Retunerar en int med djupet på trädet.
{
  if(node == NULL)// Om det inte finns någon nod retunera 0. 
    {
      return 0;
    }

  else
    {
      int depth_left = tree_depth_aux(node->left);
      int depth_right = tree_depth_aux(node->right);
      if(depth_left > depth_right)
        return (depth_left +1);
      else return (depth_right + 1);
    }
}


int tree_depth(tree_t *tree)
{
  return(tree_depth_aux(tree->root));//Anropar tree_depth2 med roten.  
}

void tree_balance_aux(tree_t *tree, elem_t *all_elements, tree_key_t *all_keys, int start, int stop)
{

  if(start <= stop)
    {
      int middle = ((start + stop)/ 2);
      tree_key_t key = all_keys[middle];
      elem_t elem = all_elements[middle];

      tree_insert(tree, key, elem);
      tree_balance_aux(tree, all_elements, all_keys, start, middle-1);
      tree_balance_aux(tree, all_elements, all_keys, middle+1, stop);
      
    }

  return;
  
}

tree_t *tree_balance(tree_t *tree)

{
  elem_t *all_elements = tree_elements(tree);
  tree_key_t *all_keys = tree_keys(tree);
  int size = tree_size(tree);
  tree_t *new_tree = tree_new(tree->element_copy, tree->key_free, tree->elem_free, tree->compare);
  tree_balance_aux(new_tree, all_elements, all_keys, 0, size -1);
  tree_delete(tree, false, false);
  free(all_elements);
  free(all_keys);
  return new_tree;
  
}

void tree_keys_aux(node_t *root, tree_key_t *all_keys, int *index)
{

  if(root == NULL)
    {

      return;
    }

  tree_keys_aux(root->left, all_keys, index);
  all_keys[*index] = root->key;
  *index = *index + 1;
  tree_keys_aux(root->right, all_keys, index);


  return;

}

tree_key_t *tree_keys(tree_t *tree)
{
  int index = 0;
  tree_key_t *all_keys = calloc(tree_size(tree), sizeof(tree_key_t));

  tree_keys_aux(tree->root, all_keys, &index);

  return all_keys;
}



void tree_elements_aux(node_t *root, elem_t *all_elements, int *index)
{
  if(root == NULL)
    {
      return; 
    }
  all_elements[*index] = root->element;
  *index = *index + 1;
  tree_elements_aux(root->left, all_elements, index);
  tree_elements_aux(root->right, all_elements, index);

  return;
}

elem_t *tree_elements(tree_t *tree)
{
  int index = 0;
  elem_t *all_elements = calloc(tree_size(tree), sizeof(elem_t));
  tree_elements_aux(tree->root, all_elements, &index);
  return all_elements;
}




bool tree_insert_aux(node_t *node, tree_key_t key, elem_t elem, tree_t *tree)
{
  
  if(tree->compare)
    {
      int i = (*tree->compare)(key, node->key);
        
      if(i == 0)
        {
          return false;
        }
      else if(i < 0)
        {
          if(node->left == NULL)
            {
              node->left = node_new(key, (*tree->element_copy)(elem));
              return true;
            }
          else
            {
              tree_insert_aux(node->left, key, elem, tree);
          
            }
        }
      else if((*tree->compare)(key, node->key) >  0)
            {
              if(node->right == NULL)
                {
                  node->right = node_new(key, (*tree->element_copy)(elem));
                  return true;
                }
              else 
                {
                  tree_insert_aux(node->right, key, elem, tree);
                }
            }
    }

  
  else
    {
      if(key.i == node->key.i)
        {

          return false;
        }


      else if((key.i < node->key.i))
        {
          if(node->left == NULL)
            {
              node->left = node_new(key, elem);
              return true;
            }
          return tree_insert_aux(node->left, key, elem, tree);

        }
      else if(key.i > node->key.i)
        {
          if(node->right == NULL)
            {
              node->right = node_new(key, elem);
              return true;
            }
          return tree_insert_aux(node->right, key, elem, tree);
        }
            

    }
  return false;
}


bool tree_insert(tree_t *tree, tree_key_t key, elem_t elem)
{
 if(tree->root == NULL)
    {
      tree->root = node_new(key, elem);
    }
 bool result = tree_insert_aux(tree->root, key, elem, tree);

 
 return result;

}




bool find_key_aux(node_t *node, tree_key_t key, tree_t *tree)
{
  if(node == NULL)
    {
      return false;
    }
  if(tree->compare)
    {
      if((*tree->compare)(node->key, key) == 0)
        {
          return true;
        }
      
      else if((*tree->compare)(node->key, key) < 0)
        {
          find_key_aux(node->left, key, tree);
        }
      
      else
        {
          find_key_aux(node->right, key, tree);
        }
    }
 
  else
    {
      if(key.i == node->element.i)
        {
          return true;
        }

      else if(key.i < node->element.i)
        {
          find_key_aux(node->left, key, tree);
        }

      else
        {
           find_key_aux(node->right, key, tree);
        }
    }
  return false;
}

bool tree_has_key(tree_t *tree, tree_key_t key)
{
 return find_key_aux(tree->root, key, tree);

}

bool get_elem_aux(node_t *node, tree_key_t key, elem_t *result, element_comp_fun compare)
{
  if(node == NULL)
  {
    return false;
  }
   if(compare)
    {
      if(compare(node->key, key) == 0)
        {
          *result = node->element;
          return true;
        }
      else if(compare(node->key, key) > 0)
        {
          return  get_elem_aux(node->left, key, result, compare);
        }

      else 
        {
          return get_elem_aux(node->right, key, result, compare);
        }
    }
  else
    {
      if(node->element.i == key.i)
        {    
          *result = node->element;
          return true;
        }
      else if(node->element.i > key.i)
        {
          return get_elem_aux(node->left, key, result, compare);

        }

      else
        {

          return get_elem_aux(node->right, key, result, compare);
        }

    }

  return false;

}


bool tree_get(tree_t *tree, tree_key_t key, elem_t *result)
{

  bool exist = get_elem_aux(tree->root, key, result, *tree->compare);
  return exist;

}

node_t **find_node_aux(node_t **node, tree_key_t key,  element_comp_fun compare)
{
  //assert(node == NULL); 
  
  if(compare)
    {
      if(compare((*node)->key, key) == 0)
        {
         
          return node;
        }
      else if(compare((*node)->key, key) < 0)
        {
          return find_node_aux(&(*node)->right, key, compare);
        }

      else 
        {
          return find_node_aux(&(*node)->left, key, compare);
        }
    }

  else
    {
      if((*node)->element.i == key.i)
        {
          return node ; 
        }
      else if ((*node)->element.i < key.i)
        {
          return find_node_aux(&(*node)->right, key, compare);
        }
      else
        {
          return find_node_aux(&(*node)->left, key, compare);
        }
       
  
    }
}

node_t **get_node(tree_t *tree, tree_key_t key)
{

  return find_node_aux(&tree->root, key, tree->compare);

}
node_t * find_min(node_t *node)//Hitta minimum key-value i trädet och retunera detta. 
{
   node_t *cursor = node;
  while(cursor -> left != NULL)
    {
      cursor = cursor->left; 
    }
  return cursor;
  }

bool tree_remove(tree_t *tree, tree_key_t key, elem_t *result){

  node_t **node = get_node(tree, key);
  node_t *nodetoremove = *node;//utgå från root??
  if (*node == NULL)
    {
    return false;
    }
  if((*node)->left == NULL && (*node)->right == NULL)//Om noden inte har några barn. 
    {
      *result = (*node)->element;
      free(nodetoremove);
       *node = NULL; 
      return true;
    }
  else if((*node)-> left == NULL)//om noden har ett barn. 
    {
      *result = (*node)->element;
      *node = (*node) ->right;
      free(nodetoremove);
        return true;
    }
  else if((*node)->right == NULL)//Om noden har ett barn. 
    {
      *result = (*node)->element; 
      *node = (*node)->left;
      free(nodetoremove);
        return true; 
    }
  else 
    {
      node_t *tmp =  find_min((*node)->right);
      *result = (*node)->element;
      tmp->left = (*node)->left;
      *node = tmp;
      free(nodetoremove); 
      }
  return false; 
}


void tree_apply_aux(node_t *root, enum tree_order order, key_elem_apply_fun fun, void *data, bool *result)
{

  if(root == NULL)
    {
      return;
    }
  else if(order == 0)
    {
      tree_apply_aux(root->left, order, fun, data, result);
      if(fun(root->key, root->element, data))
        {
          *result = true;
        }
      tree_apply_aux(root->right, order, fun, data, result);
       
    }
  else if(order == -1)
    {
      if(fun(root->key, root->element, data))
        {
          *result = true;
        }
      tree_apply_aux(root->left, order, fun, data, result);
      tree_apply_aux(root->right, order, fun, data, result);
      return;
    }
  else if(order == 1)
    {
      
      tree_apply_aux(root->left, order, fun, data, result);
      tree_apply_aux(root->right, order, fun, data, result);
      if(fun(root->key, root->element, data))
        {
          *result = true;
        }
      return;
    }
  
}

bool tree_apply(tree_t *tree, enum tree_order order, key_elem_apply_fun fun, void *data)
{
  bool result = false;
  tree_apply_aux(tree->root, order, fun, data, &result);
  return result;
}



void tree_apply_node_aux(node_t *node, tree_traversal_func fun, void *data, bool *result)
{
   if(node == NULL)
     {
       return;
     }
    
   tree_apply_node_aux(node->left, fun, data, result);
   tree_apply_node_aux(node->right, fun, data, result);

   if(fun(node, data))
     {
       *result = true;
     }
 
}

bool tree_apply_node(tree_t *tree, tree_traversal_func fun, void *data)
{
  bool result = false;
  if(tree)
    {
      tree_apply_node_aux(tree->root, fun, data, &result);
      return result;
    }
  return result;
}

void delete_all_nodes(tree_t *tree)
{
  tree_apply_node(tree, free_node, NULL);
  return;   
}


/*
int main(void)
{
     elem_t elem1 = { .i =1};
     elem_t elem2 = { .i =2};
     elem_t elem3 = { .i =3};
     elem_t elem4 = { .i =4};
     elem_t elem5 = { .i =5};
     elem_t elem6 = { .i =6};
    
     tree_t *new_tree = tree_new(NULL, NULL, NULL, NULL);
  
     tree_insert(new_tree, elem1, elem1);
     tree_insert(new_tree, elem2, elem2);
     tree_insert(new_tree, elem3, elem3);
     tree_insert(new_tree, elem4, elem4);
     tree_insert(new_tree, elem5, elem5);
     //tree_insert(new_tree, elem6, elem6);

     
     // node_t *node = get_node(new_tree, elem4);
     //printf("%d", node->element.i); //Seg.fault??
     

     


     //
     //  tree_key_t *all_keys = tree_keys(new_tree);
  //
     //  printf("%d, %d, %d, %d, %d", all_keys[0].i, all_keys[1].i, all_keys[2].i, all_keys[3].i, all_keys[4].i);
  //  
     //  tree_balance(new_tree);
  //
     //  printf("\n%d\n", new_tree->root->element.i);
  //
     //  return 0;
  //  
  }


*/
