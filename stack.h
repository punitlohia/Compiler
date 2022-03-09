// #Group 9
// #Akshay Singhal								              2018B3A70635P
// #Archit Gupta                               2018B1A70149P
// #Punit Lohia                                2018B3A70766P
// #Saksham Pandey                             2018B3A70790P

#ifndef STACK
#define STACK
#include "NAryDef.h"

typedef struct StackNode {
    NaryTreeNode* TREE_NODE;
    struct StackNode* next;
} StackNode;

typedef struct Stack {
    StackNode* HEAD;
    int node_count;
} Stack;

StackNode* createStack(NaryTreeNode* ntn);

void push(Stack* s,NaryTreeNode* ntn);
NaryTreeNode* top(Stack* s);
Stack* initialiseStack(ParseTree* pt);
void pushTree(Stack* s,NaryTreeNode* ntn);
void pop(Stack* s);

#endif
