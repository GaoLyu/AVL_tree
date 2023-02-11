#include "AVL_tree.h"

#define ninf -100000
#define pinf 100000

/* Returns the height (number of nodes on the longest root-to-leaf path) of
 * the tree rooted at node 'node'. Returns 0 if 'node' is NULL.
 */
int height(AVL_Node* node){
  if(node == NULL){
    return 0;
  }
  else{
    if(height(node->left) >= height(node->right)){
      return height(node->left) + 1;
    }
    else{
      return height(node->right) + 1;
    }
  }
}

/* Updates the height of the tree rooted at node 'node' based on the heights
 * of its children. Note: this should be an O(1) operation.
 */
void update_height(AVL_Node* node){
  if(node->left == NULL && node->right == NULL){
    node->height = 1;
  }
  else if(node->left != NULL && node->right == NULL){
    node->height = 1 + (node->left)->height;
  }
  else if(node->left == NULL && node->right != NULL){
    node->height = 1 + (node->right)->height;
  }
  else{
    if((node->left)->height >= (node->right)->height){
      node->height = ((node->left)->height) + 1;
    }
    else{
      node->height = ((node->right)->height) + 1;
    }
  }
}

/* Returns the balance factor (height of left subtree - height of right
 * subtree) of node 'node'. Returns 0 if node is NULL.
 */
int balance_factor(AVL_Node* node){
  if(node==NULL){
    return 0;
  }
  else if(node->left == NULL && node->right == NULL){
    return 0;
  }
  else if(node->left != NULL && node->right == NULL){
    return (node->left)->height;
  }
  else if(node->left == NULL && node->right != NULL){
    return -((node->right)->height);
  }
  else{
    return (node->left)->height - (node->right)->height;
  }
}


/* Returns the successor node of 'node'. */
AVL_Node* successor(AVL_Node* node){
  AVL_Node* successor = node;
  while(successor->left != NULL){
    successor = successor->left;
  }
  return successor;
}

/* Updates rightmost, leftmost, min, max in node.
 */
void update_most(AVL_Node* node){
  //for rightmost: compare node->left and node->left->rightmost
  if(node->left==NULL){
    node->rightmost=ninf;
    node->min=node->key;
  }
  else{
    node->rightmost=node->left->max;
    node->min=node->left->min;
  }
  if(node->right==NULL){
    node->leftmost=pinf;
    node->max=node->key;
  }
  else{
    node->leftmost=node->right->min; 
    node->max=node->right->max; 
  }
}

/* Updates minimun difference in node.
 */
void update_diff(AVL_Node* node){
  int n_r=pinf;
  int n_l=pinf;
  int n=pinf;
  int l_m=pinf;
  int r_m=pinf;
  int c=pinf;
  if(node->rightmost!=ninf){
    n_r=node->key - node->rightmost;
  }
  if(node->leftmost!=pinf){
    n_l=-node->key+node->leftmost;
  }
  if(node->left!=NULL){
    l_m=node->left->diff;
  }
  if(node->right!=NULL){
    r_m=node->right->diff;
  }
  if(n_r<n_l){
    n=n_r;
  }
  else{
    n=n_l;
  }
  if(l_m<r_m){
    c=l_m;
  }
  else{
    c=r_m;
  }
  if(c<n){
    node->diff=c;
  }
  else{
    node->diff=n;
  }
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
  update_most(node);
  update_diff(node);
  update_most(new_root);
  update_diff(new_root);
  return new_root;
}

// single rotations: left/counter-clockwise
AVL_Node* left_rotation(AVL_Node* node){
  AVL_Node* new_root = node->right;
  node->right = new_root->left;
  update_height(node);
  new_root->left = node;
  update_height(new_root);
  update_most(node);
  update_diff(node);
  update_most(new_root);
  update_diff(new_root);
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

/* Creates and returns an AVL tree node with key 'key', value 'value', height
 * of 1, left and right subtrees NULL, rightmost ninf, leftmost pinf,
 * min 'key', max 'key', diff pinf.
 */
AVL_Node* create_node(int key, void* value){
  AVL_Node* node = malloc(sizeof(AVL_Node));
  node->key=key;
  node->value=value;
  node->height=1;
  node->left=NULL;
  node->right=NULL;
  node->rightmost=ninf;
  node->leftmost=pinf;
  node->min=key;
  node->max=key;
  node->diff=pinf;
  return node;
}

/* Balances the tree rooted at node and 
 * returns the root of the resulting tree.
 */
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

void print_tree_inorder_(AVL_Node* node, int offset) {
  if (node == NULL) return;
  print_tree_inorder_(node->right, offset + 1);
  //printf("%*s %d [%d]                     rightmost: %d, leftmost: %d, diff: %d\n", offset*2, "", node->height, node->key, node->rightmost, node->leftmost, node->diff);
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
  if(node == NULL){
    return create_node(key, value);
  }
  else{
    if(node->key < key){
      node->right = insert(node->right, key, value);
    }
    else if(node->key > key){
      node->left = insert(node->left, key, value);
    }
    else{
      node->value = value;
    }
    update_height(node);
    update_most(node);
    update_diff(node);
    node = balance(node);
    return node;
  }
}

AVL_Node* delete(AVL_Node* node, int key) {
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
      free(node);
      return NULL;
    }
    else if(node->left == NULL && node->right != NULL){
      AVL_Node* new=node->right;
      free(node);
      return new;
    }
    else if(node->left != NULL && node->right == NULL){
      AVL_Node* new=node->left;
      free(node);
      return new;
    }
    else{
      AVL_Node *succ = successor(node->right);
      node->key=succ->key;
      node->value=succ->value;
      update_most(node);
      update_diff(node);
      node->right=delete(node->right,succ->key);
    }
  }
  update_height(node);
  update_most(node);
  update_diff(node);
  node = balance(node);
  return node;
}

void min_difference(AVL_Node* node, int pair[2]){
  if(height(node)<=1){
    pair[0]=-1;
    pair[1]=-1;
  }
  else{
    int n_r=pinf;
    int n_l=pinf;
    int r_m=pinf;
    int l_m=pinf;

    if(node->rightmost!=ninf){
      n_r=node->key - node->rightmost;
    }
    if(node->leftmost!=pinf){
      n_l=-node->key+node->leftmost;
    }
    if(node->left!=NULL){
      l_m=node->left->diff;
    }
    if(node->right!=NULL){
      r_m=node->right->diff;
    }

    if(node->diff==n_r){
      pair[0]=node->key;
      pair[1]=node->rightmost;
    }
    else if(node->diff==n_l){
      pair[0]=node->key;
      pair[1]=node->leftmost;
    }
    else if(node->diff==r_m){
      min_difference(node->right,pair);
    } 
    else if(node->diff==l_m){
      min_difference(node->left,pair);
    }
  }
}