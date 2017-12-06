#include "common.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "tree.h"

struct node
{
  elem_t key;
  elem_t elem;
  struct node *left;
  struct node *right;
};
typedef struct node node_t;

/*
  struct representing a binary search tree
  root is root node
  size is number of nodes in tree
*/
struct tree
{
  element_copy_fun elem_copy;
  key_free_fun key_free;
  element_free_fun elem_free;
  element_comp_fun elem_comp_fun;
  node_t *root;
};

typedef elem_t tree_key_t;
typedef element_free_fun key_free_fun;
typedef struct tree tree_t;




/*
  Helper for get_parent_pointer
  Searches recursively for the correct placement for node with given key.

  Takes a pointer to parent's node-pointer instead of a tree.
  (i.e pointer to (node->left) or (node->right))
*/
node_t **get_parents_pointer_r(node_t **parents_pointer, elem_t *key, element_comp_fun compare)
{
  if (*parents_pointer == NULL)  // Parent node points to NULL, key is not in tree.
    {
      return parents_pointer;
    }
  

  // We can determine if node is in left or right subtree
  // by comparing the given key with the current node's key
  int comp = compare(*key, (*parents_pointer)->key);
  
  if (comp < 0)
    {
      return get_parents_pointer_r( &((*parents_pointer)->left), key, compare);
    }
  else if (comp > 0)
    {
      return get_parents_pointer_r( &((*parents_pointer)->right), key, compare);
    }
  else //  Node's key was same as key, return
    {
      return parents_pointer;
    }
}

/*
  If key exists, returns pointer to parent's pointer to node.
  If key does not exists, returns pointer to parent's pointer 
  which would point to node if it existed.

  (i.e it returns a pointer to (node->left) or (node->right) of parent node.)

  Can be used when wanting to change children of parent (parent->left or parent->right)
  for example when inserting/removing nodes.
*/
node_t **get_parents_pointer(tree_t *tree, elem_t key)
{
  return get_parents_pointer_r(&(tree->root), &key, tree->elem_comp_fun);
}



tree_t *tree_new(element_copy_fun element_copy, key_free_fun key_free, element_free_fun elem_free, element_comp_fun compare)
{
  tree_t *temp = malloc(sizeof(tree_t));
  temp->root = NULL;
  temp->elem_copy = element_copy;
  temp->key_free = key_free;
  temp->elem_free = elem_free;
  temp->elem_comp_fun = compare;
  return temp;
}

void tree_delete(tree_t *tree, bool delete_keys, bool delete_elements)
{
  return;
}

int tree_size_r(node_t *node)
{
  if(node==NULL)
    {
      return 0;
    }
  int left_count = 1 + tree_size_r(node->left);
  int right_count = 1 + tree_size_r(node->right);
  return left_count + right_count;
}

int tree_size(tree_t *tree)
{
  return tree_size_r(tree->root);
}

/*
  Helper function of tree_depth. Takes a node instead of a tree,
  allowing recursion.
  Recursively checks depth of node by counting 1 for each step it
  can recurse downwards.
  
  Returns depth of the tree with given node as root
*/
int tree_depth_r(node_t *node)
{
  // Base case: reaching bottom node
  if (node == NULL)
    {
      return 0;
    }
  
  int depth_r = tree_depth_r(node->right);
  int depth_l = tree_depth_r(node->left);
  if (depth_r > depth_l)
    {
      return 1 + depth_r;
    }
  else
    {
      return 1 + depth_l;
    }
}


int tree_depth(tree_t *tree)
{
  return tree_depth_r(tree->root);
}


// Returns a new node with given key and element and with no children.
node_t *new_node(elem_t key, elem_t elem)
{
  node_t *new = malloc(sizeof(node_t));
  new->left = NULL;
  new->right = NULL;
  new->key = key;
  new->elem = elem;
  return new;
}


bool tree_insert(tree_t *tree, tree_key_t key, elem_t elem)
{
  node_t **node_p_p = get_parents_pointer(tree, key);
  if (*node_p_p == NULL) // Parent's pointer is NULL => node does not exist already.
    {
      if(tree->elem_copy==NULL)
        {
          *node_p_p = new_node(key, elem);
          return true;
        }
      else
        {
          *node_p_p = new_node(key, tree->elem_copy(elem));
          return true;
        }
    }
  else
    {
      return false;
    }
}

bool tree_has_key(tree_t *tree, tree_key_t key)
{
  node_t *node_pointer = *get_parents_pointer(tree,key);
  return (node_pointer != NULL);
}

bool tree_get(tree_t *tree, tree_key_t key, elem_t *result)
{
  node_t *node_pointer = *get_parents_pointer(tree, key);
  if(node_pointer == NULL)
    {
      return false;
    }
  result = &node_pointer->elem;
  return true;
}

bool tree_remove(tree_t *tree, tree_key_t key, elem_t *result)
{
  return true;
}


/*
  Recursively does an in-order search, adding all the keys to result
  In-order search results in keys being added in ascending order
*/
void tree_keys_r(node_t *node, tree_key_t **result)
{
  if (node != NULL)
    {
      tree_keys_r(node->left, result);
      
      **result = node->key;
      ++(*result); //Need to increment pointer, since we added an element

      tree_keys_r(node->right, result);
    }
}


tree_key_t *tree_keys(tree_t *tree)
{
  tree_key_t *key_array = malloc(tree_size(tree)*sizeof(tree_key_t));
  tree_key_t *cursor= key_array;
  tree_keys_r(tree->root, &cursor);
  return key_array;
}

void tree_elements_r(node_t *node, elem_t **result)
{
  if (node != NULL)
    {
      tree_elements_r(node->left, result);
      
      **result = node->elem;
      ++(*result); //Need to increment pointer, since we added an elem

      tree_elements_r(node->right, result);
    }
}

elem_t *tree_elements(tree_t *tree)
{
  elem_t *elem_array = malloc(tree_size(tree)*sizeof(tree_key_t));
  elem_t *cursor= elem_array;
  tree_elements_r(tree->root, &cursor);
  return elem_array;
}

bool tree_apply(tree_t *tree, enum tree_order order, key_elem_apply_fun fun, void *data)
{
  return true;
}
<<<<<<< HEAD

/*
int main()
{
=======
/*
  int main()
  {
>>>>>>> 8758dd58d6dfa15124061e60598df96d77d57870
  tree_t *test= tree_new(NULL, NULL, NULL, NULL);
  printf("%d\n", tree_size(test));
  puts("hej");
  return 0;
<<<<<<< HEAD
}
*/
=======
  }*/

>>>>>>> 8758dd58d6dfa15124061e60598df96d77d57870
