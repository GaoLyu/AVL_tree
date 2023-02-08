/*
   AVL tree implementation.

   Author: A. Tafliovich
   Based heavily on materials developed by F. Estrada.
*/

#include "AVL_tree.h"

/*************************************************************************
 ** Suggested helper functions
 *************************************************************************/

/* Returns the height (number of nodes on the longest root-to-leaf path) of
 * the tree rooted at node 'node'. Returns 0 if 'node' is NULL.
 */
int height(AVL_Node* node){
  if(node == NULL){
    return 0;
  }
  else{
    if(height(node.left) >= height(node.right)){
      return height(node.left) + 1;
    }
    else{
      return height(node.right) + 1;
    }
  }
}

/* Updates the height of the tree rooted at node 'node' based on the heights
 * of its children. Note: this should be an O(1) operation.
 */
void update_height(AVL_Node* node){
  if((node->left)->height >= (node->right)->height){
    node->height = (node->left)->height + 1;
  }
  else{
    node->height = (node->right)->height + 1;
  }
}

/* Returns the balance factor (height of left subtree - height of right
 * subtree) of node 'node'. Returns 0 of node is NULL.
 */
int balance_factor(AVL_Node* node){
  if(node==NULL){
    return 0;
  }
  return (node->left)->height - (node->right)->height;
}

/* Returns the result of performing the corresponding rotation in the AVL
 * tree rooted at 'node'.
 */
// single rotations: right/clockwise
AVL_Node* right_rotation(AVL_Node* node){
  AVL_Node* new_root = node->left;
  node->left = new_root->right;
  update_height(node);
  new_root->right = node;
  update_height(new_root);
  return new_root;
}
// single rotations: left/counter-clockwise
AVL_Node* left_rotation(AVL_Node* node){
  AVL_Node* new_root = node->right;
  node->right = new_root->left;
  update_height(node);
  new_root->left = node;
  update_height(new_root);
  return new_root;
}
// double rotation: right/clockwise then left/counter-clockwise
AVL_Node* right_left_rotation(AVL_Node* node){
  node->right = right_rotation(node->right);
  return left_rotation(node);
}
// double rotation: left/counter-clockwise then right/clockwise
AVL_Node* left_right_rotation(AVL_Node* node){
  node->left = left_rotation(node->left);
  return right_rotation(node);
}

/* Returns the successor node of 'node'. */
AVL_Node* successor(AVL_Node* node){
  while(successor->left != NULL){
    successor = successor->left;
  }
  return successor;
}

/* Creates and returns an AVL tree node with key 'key', value 'value', height
 * of 1, and left and right subtrees NULL.
 */
AVL_Node* create_node(int key, void* value){
  AVL_Node* node;
  node->key=key;
  node->value=value;
  node->height=1;
  node->left=NULL;
  node->right=NULL;
  return node;
}

AVL_Node* balance(AVL_Node *node){
  if(balance_factor(node) < -1){
    if(balance_factor(node->right) <= 0){
      node = left_rotation(node);
    }
    else{
      node = right_left_rotation(node);
    }
  }
  else if(balance_factor(node) > 1){
    if(balance_factor(node->left) >= 0){
      node = right_rotation(node);
    }
    else{
      node = left_right_rotation(node);
    }
  }
  return node;
}
/*************************************************************************
 ** Provided functions
 *************************************************************************/
void print_tree_inorder_(AVL_Node* node, int offset) {
  if (node == NULL) return;
  print_tree_inorder_(node->right, offset + 1);
  printf("%*s %d [%d]\n", offset, "", node->key, node->height);
  print_tree_inorder_(node->left, offset + 1);
}

void print_tree_inorder(AVL_Node* node) {
  print_tree_inorder_(node, 0);
}

void delete_tree(AVL_Node* node) {
  if (node == NULL) return;
  delete_tree(node->left);
  delete_tree(node->right);
  free(node);
}

/*************************************************************************
 ** Required functions
 ** Must run in O(log n) where n is the number of nodes in a tree rooted
 **  at 'node'.
 *************************************************************************/

AVL_Node* search(AVL_Node* node, int key) {
  if(node == NULL){
    return NULL;
  }
  else if(node->key == key){
    return node;
  }
  else if(node->key < key){
    return search(node->right, key);
  }
  else{
    return search(node->left, key);
  }
}

AVL_Node* insert(AVL_Node* node, int key, void* value) {
  AVL_Node* exist;
  if((exist = search(node, key)) != NULL){
    exist->value = value;
    return node;
  }
  else{
    AVL_Node* new=create_node(key, value);
    if(node == NULL){
      return new;
    }
    else{
      if(node->key < key){
        node->right = insert(node->right, key);
      }
      else{
        node->left = insert(node->left, key);
      }
      update_height(node);
      node = balance(node);
    }
    return node;
  }
  
}
AVL_Node* search_parent(node, successor){
  if(node == successor){
    return NULL;
  }
  if(node->left == successor){
    return node;
  }
  return search_parent(node->left, successor);
}

AVL_Node* delete(AVL_Node* node, int key) {
  if(search(node,key) == NULL){
    return node;
  }
  else{
    if(node == NULL){
      return NULL;
    }
    else if(node->key < key){
      node->right = delete(node->right, key);
    }
    else if(node->key > key){
      node->left = delete(node->left, key);
    }
    else{
      if(node->left == NULL && node->right == NULL){
        return NULL;
      }
      else if(node->left == NULL && node->right != NULL){
        return node->right;
      }
      else if(node->left != NULL && node->right == NULL){
        return node->left;
      }
      else(node->left != NULL && node->right != NULL){
        AVL_Node *successor = successor(node->right);
        AVL_Node *parent = search_parent(node->right, successor);
        if(parent != NULL){
          parent->left = successor->right;
          successor->right = node->right;
          update_height(parent);  
        }
        successor->left = node->left;
        update_height(successor);
        return balance(successor);
      }
    }
    update_height(node);
    node = balance(node);
    return node;
  }
}
