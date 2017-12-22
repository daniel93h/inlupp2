#include "tree.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tree_aux.h"

typedef elem_t tree_key_t;
typedef element_free_fun key_free_fun;
typedef struct tree tree_t;
typedef struct node node_t;

struct node
{
  elem_t key;
  elem_t elem;
  struct node *left;
  struct node *right;
};
typedef struct node node_t;

struct tree 
{ 
  node_t *root;
  element_copy_fun element_copy;
  key_free_fun key_free;
  element_free_fun elem_free;
  element_comp_fun elem_comp;
};

int tree_depth_aux(node_t *node)
{
  // Base case: reaching bottom node
  if (node == NULL)
    {
      return 0;
    }
  
  int depth_r = tree_depth_aux(node->right);
  int depth_l = tree_depth_aux(node->left);
  if (depth_r > depth_l)
    {
      return 1 + depth_r;
    }
  else
    {
      return 1 + depth_l;
    }
}

bool node_is_balanced(node_t *node){
  if(abs(tree_depth_aux(node->left)-tree_depth_aux(node->right))>1){
    return false;
    }
  if(tree_depth_aux(node->left)>tree_depth_aux(node->right)){
    return node_is_balanced(node->left);
  }
  else if(node->right != NULL){
    return node_is_balanced(node->right);
  }
  return true;
}

bool tree_is_balanced(tree_t *tree){
  if(tree->root == NULL){
    return true;
  }
  return node_is_balanced(tree->root);
}

node_t *new_node(elem_t key, elem_t elem)
{
  node_t *new = malloc(sizeof(node_t));
  new->left = NULL;
  new->right = NULL;
  new->key = key;
  new->elem = elem;
  return new;
  }

bool node_insert(tree_t *tree, node_t *current, tree_key_t key, elem_t elem){
  if(tree->elem_comp(key, current->key)== 0){
    return false;
  }
  if(tree->elem_comp(key, current->key)> 0){
    if(current->right == NULL){
      node_t *node = new_node(key,elem);
      current->right = node;
      return true;
    }
    else
      {
        node_insert(tree,current->right,key,elem);
      }
  }else{
    if(current->left == NULL){
      node_t *node = new_node(key,elem);
      current->left = node;
      return true;
    }else{
      node_insert(tree, current->left,key,elem);
    }
  }return true;
}

bool tree_unbalanced_insert(tree_t *tree, tree_key_t key, elem_t elem){
  if(tree->root == NULL){
  node_t *node = new_node(key, elem);
  tree->root = node;
  return true;
  }
  node_t *current = tree->root;
  return node_insert(tree, current, key, elem);


////////////Fortsätt här//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void balance_tree_from_array_aux(tree_t *new_tree, tree_key_t *keys, elem_t *elements, int length){
  int mid = length/2;
  if(length < 1){
    return;
  }

  tree_unbalanced_insert(new_tree, keys[mid], elements[mid]);

  
}

tree_t *balance_tree_from_array(tree_t *tree){
  tree_t *new_tree = tree_new(tree->element_copy, tree->key_free, tree->elem_free, tree->elem_comp);
  tree_key_t *keys = tree_keys(tree);
  elem_t *elements = tree_elements(tree);
  int size = tree_size(tree);
  balance_tree_from_array_aux(new_tree, keys, elements,size);
  return new_tree;
}

tree_t *tree_balance(tree_t *tree){
  if(tree_is_balanced(tree)){
    return tree;
  }
  else
    {
      tree_t *balanced_tree = balance_tree_from_array(tree);
      return balanced_tree;
    }
}
