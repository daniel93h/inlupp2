#ifndef __tree_aux_h__
#define __tree_aux_h__
#include "tree.h"

bool tree_is_balanced(tree_t *tree);
tree_t *tree_balance(tree_t *tree);
node_t *new_node(elem_t key, elem_t elem);
int tree_depth_aux(node_t *node);


#endif
