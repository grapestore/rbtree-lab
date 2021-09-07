#include "rbtree.h"
#include <stdlib.h>
#include <stdio.h>


node_t _NIL = {
    .color = RBTREE_BLACK,
    .key = 0,
    .parent = NULL,
    .left = NULL,
    .right = NULL
};
node_t *NIL = &_NIL;

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(sizeof(rbtree), 1);
  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

void left_Rotate(rbtree* root,node_t* node){
  node_t* child = node->right;
  node->right = child->left;
  if(node->right != NULL){
      node->right->parent = node;
  }
  child->parent = node->parent;
  if(node->parent == NULL){
      (root->root->left) = child;
  }
  else if(node == node->parent->left){
      node->parent->left = child;
  }
  else{
      node->parent->right = child;
  }
  child->left = node;
  node->parent = child; 
}

void right_Rotate(rbtree* root,node_t* node){
  node_t* child = node->left;
  node->left = child->right;
  if(child->right != NULL){
      child->right->parent = node;
  }
  child->parent = node->parent;
  if(child->parent == NULL){
      (root->root->left) = child;
  }
  else if(node == node->parent->left){
      node->parent->left = child;
  }
  else{
      node->parent->right = child;
  }
  child->right = node;
  node->parent = child;
}

void Fixed(rbtree *root, node_t* node){
  while(node != root->root->left && node->parent->color == RBTREE_RED){
    node_t* temp;
    
    if(node->parent == node->parent->parent->right){
      temp = node->parent->parent->left;
    }
    else{
      temp = node->parent->parent->right;
    }
    if(temp==NULL || temp->color != RBTREE_RED){
      node_t* temp_color = (node_t *)calloc(sizeof(node_t), 1);
        //LL
      if(node->parent == node->parent->parent->left && node == node->parent->left){
    
          temp_color->color = node->parent->color;
          node->parent->color = node->parent->parent->color;
          node->parent->parent->color = temp_color->color;
          right_Rotate(root,node->parent->parent);
      }
        //LR
      else if(node->parent == node->parent->parent->left && node == node->parent->right){

          temp_color->color = node->color;
          node->color = node->parent->parent->color;
          node->parent->parent->color = temp_color->color;
          left_Rotate(root,node->parent);
          right_Rotate(root,node->parent->parent);
      }
        //RR
      else if(node->parent == node->parent->parent->right && node == node->parent->right){

          temp_color->color = node->parent->color;
          node->parent->color = node->parent->parent->color;
          node->parent->parent->color = temp_color->color;
          left_Rotate(root,node->parent->parent);
      }
        //RL
      else if(node->parent == node->parent->parent->right && node == node->parent->left){

          temp_color->color = node->color;
          node->color = node->parent->parent->color;
          node->parent->parent->color = temp_color->color;
          right_Rotate(root, node->parent);
          left_Rotate(root, node->parent->parent);
      }
      free(temp_color);
    }
    else if(temp!=NULL && temp->color == RBTREE_RED){
      node->parent->color = RBTREE_BLACK;
      temp->color = RBTREE_BLACK;
      node->parent->parent->color = RBTREE_RED;
      node = node->parent->parent;
      }
    }
  (root->root->left)->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  
  // root->root 노드들
    node_t *new = (node_t*)malloc(sizeof(node_t));
    new->key = key;
    new->left = new->right = new->parent = NULL;
    node_t *curr = (t->root);
    node_t *pre_curr = NULL;
    if(t->root == NULL){
        new->color = RBTREE_BLACK;
        t->root = new;
        curr = new;
        new->right = NIL;
        new->left = NIL;
    }
    else{
        while(curr != NIL){
            pre_curr = curr;
            if(new->key > curr->key){
                curr = curr->right;
            }
            else{
                curr = curr->left;
            }
        }
        new->parent = pre_curr;
        if(new->key > pre_curr->key){
            pre_curr->right = new;
        }
        else{
            pre_curr->left = new;
        }
        new->color = RBTREE_RED;
        new->right = NIL;
        new->left = NIL;
        Fixed(t, new);
    }
  return curr;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *curr = (t->root);
  node_t *pre_curr = NULL;
  while(curr != NIL){
    pre_curr = curr;
    if(curr->key > key){
      curr = curr->left;
      }
    else if(curr->key < key){
      curr = curr->right;
      }
    else{
      return curr;
    }
  }
  return curr;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

void delete_fixed(rbtree* t, node_t* x){
    node_t* sibling = NULL;
    if(x->color == RBTREE_RED){
        x->color = RBTREE_BLACK;
        return;
    }
    if(x == x->parent->left){
        sibling = x->parent->right;
        if(sibling->color == RBTREE_RED){
            sibling->color = RBTREE_BLACK;
            sibling->left->color = RBTREE_RED;
            left_Rotate(t, sibling->parent);
            sibling = x->parent->right;
        }
        if(sibling->left->color ==RBTREE_BLACK && sibling->right->color == RBTREE_BLACK){
            sibling->color = RBTREE_RED;
            x->parent->color = RBTREE_BLACK;
            return;
        }
        else if(sibling->left->color == RBTREE_RED && sibling->right->color == RBTREE_BLACK){
            node_t temp;
            temp.color = sibling->color;
            sibling->color = sibling->left->color;
            sibling->left->color = temp.color;
            right_Rotate(t, sibling);
            sibling = x->parent->right;
        }
        if(sibling->right->color == RBTREE_RED){
            sibling->color = x->parent->color;
            x->parent->color = RBTREE_BLACK;
            sibling->right->color = RBTREE_BLACK;
            left_Rotate(t, x->parent);
        }
        t->root->color = RBTREE_BLACK;
        return;
    }
    else{
        sibling = x->parent->left;
        if(sibling->color == RBTREE_RED){
            sibling->color = RBTREE_BLACK;
            sibling->right->color = RBTREE_RED;
            right_Rotate(t, sibling->parent);
            sibling = x->parent->left;
        }
        if(sibling->left->color ==RBTREE_BLACK && sibling->right->color == RBTREE_BLACK){
            sibling->color = RBTREE_RED;
            x->parent->color = RBTREE_BLACK;
            return;
        }
        else if(sibling->left->color == RBTREE_RED && sibling->right->color == RBTREE_BLACK){
            node_t temp;
            temp.color = sibling->color;
            sibling->color = sibling->left->color;
            sibling->left->color = temp.color;
            right_Rotate(t, sibling);
            sibling = x->parent->right;
        }
        if(sibling->right->color == RBTREE_RED){
            sibling->color = x->parent->color;
            x->parent->color = RBTREE_BLACK;
            sibling->right->color = RBTREE_BLACK;
            right_Rotate(t, x->parent);
        }
        t->root->color = RBTREE_BLACK;
        return;
    }

}


int rbtree_erase(rbtree *t, node_t *p){
    // p -> 변경 p->key
    node_t* target = rbtree_find(t,p->key);
    node_t* closer = NULL;

    // closer 자리에 올라올 애
    node_t* fixed_x = NULL;
    // 차일드가 둘다 있을때
    if(target->left != NIL && target->right != NIL){
        // closer(successor)를 찾는 코드
        closer = target->right;
        while(closer!=NIL && closer->left!=NIL){
            closer = closer->left;
        }
        target->key = closer->key;
        fixed_x = closer->right;
        fixed_x->parent = closer->parent;
        if(closer == closer->parent->right){closer->parent->right = closer->right;}
        else if(closer == closer->parent->left){closer->parent->left = closer->right;}
    }
    // child가 left 하나만 있는 경우
    else if(target->left != NIL || target->right != NIL){
        if(target->left != NIL){
            closer = target->left;
            target->key = closer->key;
            fixed_x = closer;
            fixed_x->parent = closer->parent;
            if(closer == closer->parent->right){closer->parent->right = closer->right;}
            else if(closer == closer->parent->left){closer->parent->left = closer->right;}
        }
    }
    // child가 없는 경우
    else{
        if(target == target->parent->right){target->parent->right = NIL;}
        else{target->parent->left = NIL;}
    }
    if(closer != NULL && closer->color == RBTREE_BLACK){delete_fixed(t, fixed_x);}
    free(closer);
    return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}

