/* hufftr.h */

#pragma once
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <assert.h>

enum 
{
    Leaf,
    Node
};

typedef unsigned char int8;
typedef unsigned short int int16;
typedef unsigned int int32;
typedef unsigned long long int int64;

struct s_leaf;
typedef struct s_leaf leaf;
struct s_node;
typedef struct s_node node;
typedef union u_tree tree;

struct s_leaf
{
    int64 freq;        // frequency
    int8 kind;
    tree *up;         // upstrem
    int8 c;          // character
    
};

struct s_node
{
    int64 freq;
    int8 kind;
    tree *up;
    tree *left, *right;  // downstrem leg
    
};

union u_tree  
{
    struct s_node n;
    struct s_leaf l;
};

#define $c (char *)
#define $1 (int8 *)
#define $2 (int16)
#define $4 (int32)
#define $8 (int64)
#define $v (void *)
#define $i (int)
#define $t (tree *)
#define $el (leaf *)
#define $n (node *)

#define alloc(x)     malloc($i (x))
#define destroy(x)   free(x)
#define show(x)      show_($t (x), $1 # x)

// constructurs
node *mknode(tree*, tree*);
leaf *mkleaf(int8);

void show_(tree*, int8*);
void conn(tree*, bool, tree*);
void zero(int8*, int16);
int main(void);

