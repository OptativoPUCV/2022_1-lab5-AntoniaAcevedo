#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap));
    new->lower_than = lower_than;
    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
  TreeNode * new;
  tree -> current = tree -> root;
  while (tree -> current != NULL){
    if (tree -> lower_than(key,tree -> current -> pair -> key) == 1){
      if (tree -> current -> left == NULL)
        {
          new = createTreeNode(key, value);
          tree -> current -> left = new;
          new -> parent = tree -> current;
          tree -> current = tree -> current -> left;
          return;
        }
        tree -> current = tree -> current -> left;
      }
      else if(is_equal(tree, key,tree -> current -> pair -> key) == 1) return;
      else
      {
        if (tree -> current -> right == NULL){
          new = createTreeNode(key, value);
          tree -> current -> right = new;
          new -> parent = tree -> current;
          tree -> current = tree -> current -> right;
          return;
        }
        tree -> current = tree -> current -> right;
      }
  }
  if (tree -> root == NULL){
    new = createTreeNode(key, value);
    tree -> root = new;
    return;
  }
}

TreeNode * minimum(TreeNode * x){

  while (x -> left != NULL)
    {
      x= x -> left;
    }
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
  if (node -> left == NULL && node -> right == NULL)
  {
    if (node == node -> parent -> left)
    {
      node -> parent -> left = NULL;
      free(node);
    }
    else
    {
      node -> parent -> right = NULL;
      free(node);
    }
    return;
  }
  else if (node -> left != NULL && node -> right != NULL)
  {
    TreeNode* aux = minimum(node -> right);
    node -> pair = aux -> pair;
    removeNode(tree, aux);
    return;
  }  
  else if (node -> left != NULL && node -> right == NULL)
  {
    if (node == node -> parent -> left)
    {
      node -> parent -> left = node -> left;
      node -> left -> parent = node -> parent;
      free(node);
    }
    else
    {
      node -> parent -> right = node -> left;
      node -> left -> parent = node -> parent;
      free(node);
    }
    return;
  }
  else
  {
    if (node == node -> parent -> left)
    {
      node -> parent -> left = node -> right;
      node -> right -> parent = node -> parent;
      free(node);
    }
    else
    {
      node -> parent -> right = node -> right;
      node -> right -> parent = node -> parent;
      free(node);
    }
    return;
  } 
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
  TreeNode* aux = tree -> root ;
  while (aux != NULL){
    if (is_equal(tree,aux -> pair -> key ,key)){
      tree -> current = aux;
      return aux -> pair;
    }
    if (tree-> lower_than(aux -> pair -> key ,key)){
      aux= aux -> right;
    }
    else{ 
     aux= aux -> left;
    }
    }
  return NULL;
}
  


Pair * upperBound(TreeMap * tree, void* key) {
  TreeNode * nodo;
  tree -> current = tree -> root;
  nodo = NULL;
  while (tree -> current != NULL){
    if (tree -> lower_than(key,tree -> current -> pair -> key) == 1 ){
      nodo = tree -> current;
      if(tree -> current -> left == NULL) break;
      tree -> current = tree -> current -> left;
    }
    else if (is_equal(tree, key,tree -> current -> pair -> key) == 1){
      nodo = tree -> current;
      break;
    }
    else{
      if(tree -> current -> right == NULL) break;
      tree -> current = tree -> current -> right;
    }
  }
  if (nodo == NULL) return NULL;
  return nodo -> pair;
}

Pair * firstTreeMap(TreeMap * tree) {
  TreeNode* nodo = (TreeNode*) calloc (1, sizeof(TreeNode));
  nodo = tree -> root;
  if (tree == NULL || tree -> root == NULL) return NULL;
  while (nodo -> left != NULL)
  {
    nodo= nodo -> left;
  }
  return nodo -> pair;
}

Pair * nextTreeMap(TreeMap * tree) {
  TreeNode * aux = tree -> current;
  if(tree -> current == NULL) return NULL;
  if (aux -> right == NULL){
    while (aux -> parent != NULL){
        if (aux == NULL) return NULL;
        else if (tree->lower_than(aux -> parent -> pair -> key,tree -> current -> pair -> key) == 1){
          aux = aux -> parent;
        }
        else if (tree -> lower_than(tree -> current -> pair -> key, aux -> parent -> pair -> key) == 1){
          aux = aux -> parent;
          tree -> current = aux;
          return aux -> pair;
        }
        else return aux->pair;
      }
    }
  else {
    tree -> current = minimum(tree -> current -> right);
    return tree -> current -> pair;
    }   
  return NULL;
}
