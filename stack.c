// #Group 9
// #Akshay Singhal								              2018B3A70635P
// #Archit Gupta                               2018B1A70149P
// #Punit Lohia                                2018B3A70766P
// #Saksham Pandey                             2018B3A70790P

#include "stack.h"
#include "NAry.h"

StackNode* createStack(NaryTreeNode* ntn) {
    StackNode* st_temp = (StackNode*)malloc(sizeof(StackNode));
    st_temp->TREE_NODE = ntn;
    st_temp->next = NULL;
    return st_temp;
}

void pop(Stack* s) {
    StackNode* head = s->HEAD;

    if(head == NULL)
        return;

    s->HEAD = s->HEAD->next;
    s->node_count--;
}

NaryTreeNode* top(Stack* s) {
    if(s->HEAD == NULL)
        return NULL;
    else
        return s->HEAD->TREE_NODE;
}

void push(Stack* s,NaryTreeNode* ntn) {
    StackNode* st_temp = createStack(ntn);
    StackNode* head = s->HEAD;

    if(head == NULL) {
        s->HEAD = st_temp;
        s->node_count++;
        return;
    }

    st_temp->next = head;
    s->HEAD = st_temp;
    s->node_count++;
    return;
}


void pushTree(Stack* s,NaryTreeNode* ntn) {
    if(ntn == NULL)
        return;
    pushTree(s,ntn->next);
    push(s,ntn);
}

Stack* initialiseStack(ParseTree* pt) {
    Stack* s = (Stack*)malloc(sizeof(Stack));
    s->node_count = 0;
    s->HEAD = NULL;

    SymbolType sType;
    sType.TERMINAL = TK_DOLLAR;
    NaryTreeNode* ntn = create_node(1,sType,NULL);
    push(s,ntn);
    push(s,pt->root);
    return s;
}
