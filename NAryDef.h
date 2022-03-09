// #Group 9
// #Akshay Singhal								              2018B3A70635P
// #Archit Gupta                               2018B1A70149P
// #Punit Lohia                                2018B3A70766P
// #Saksham Pandey                             2018B3A70790P

#ifndef NARY
#define NARY

#include "lexerDef.h"
#include "parserDef.h"

typedef struct NaryTreeNode NaryTreeNode;

typedef struct NonLeafNode {
    int ENUM_ID; // The symbol's identifier
    int NUMBER_CHILDREN;
    int RULE_NO;
    NaryTreeNode* child;
} NonLeafNode;

typedef struct LeafNode {
    int ENUM_ID;
    Token* TK; //Populated when the input is parsed
} LeafNode;

typedef union NodeType {
    NonLeafNode NL;
    LeafNode L;
} NodeType;

typedef struct NaryTreeNode {
    NodeType NODE_TYPE;
    int IS_LEAF_NODE;
    struct NaryTreeNode* parent;
    struct NaryTreeNode* next; 

} NaryTreeNode;

typedef struct ParseTree {
    NaryTreeNode* root;
} ParseTree;

#endif
