// #Group 9
// #Akshay Singhal								              2018B3A70635P
// #Archit Gupta                               2018B1A70149P
// #Punit Lohia                                2018B3A70766P
// #Saksham Pandey                             2018B3A70790P

#include "NAryDef.h"
#include "parser.h"

int parseTreeNodeCount;
int parseTreeMemory;


NaryTreeNode* create_non_leaf_node(int enumId) {
    NaryTreeNode* ntn = (NaryTreeNode*)malloc(sizeof(NaryTreeNode));
    ntn->IS_LEAF_NODE = 0;
    ntn->NODE_TYPE.NL.ENUM_ID = enumId;
    ntn->NODE_TYPE.NL.NUMBER_CHILDREN = 0;
    ntn->next = NULL;
    ntn->NODE_TYPE.NL.child = NULL;
    parseTreeMemory = parseTreeMemory +  sizeof(NaryTreeNode);
    return ntn;
}

NaryTreeNode* create_leaf_node(int enumId) {
    NaryTreeNode* ntn = (NaryTreeNode*)malloc(sizeof(NaryTreeNode));
    ntn->IS_LEAF_NODE = 1;
    ntn->NODE_TYPE.L.ENUM_ID = enumId;
    ntn->next = NULL;
    parseTreeMemory = parseTreeMemory +  sizeof(NaryTreeNode);
    return ntn;
}

ParseTree* parsetree_initialisation() {
    parseTreeNodeCount = *((int*)malloc(sizeof(int)));
    parseTreeMemory = *((int*)malloc(sizeof(int)));
    ParseTree* pt = (ParseTree*)malloc(sizeof(ParseTree));
    pt->root = create_non_leaf_node(program);
    pt->root->parent = NULL;
    return pt;
}

NaryTreeNode* create_node(int isTerminal, SymbolType type,NaryTreeNode* parent) {

    NaryTreeNode* ntn;
    if(isTerminal == 1) {
        ntn = create_leaf_node(type.TERMINAL);
        ntn->parent = parent;
    }
    else {
        ntn = create_non_leaf_node(type.NON_TERMINAL);
        ntn->parent = parent;
    }

    parseTreeNodeCount += 1;
    return ntn;
}


void addRuleToParseTree(NaryTreeNode* ntn, Rule* r) {

    if(ntn->IS_LEAF_NODE == 1) {
        printf("TERMINALS CANNOT HAVE CHILDREN! \n");
        return;
    }

    int numberChild = 0;
    Symbol* trav = r->SYMBOLS->HEAD_SYMBOL->next;
    NaryTreeNode* childHead = NULL;
    NaryTreeNode* childTrav = NULL;
    while(trav != NULL) {
        if(childHead == NULL) {
            childHead = create_node(trav->IS_TERMINAL,trav->TYPE,ntn);
            childTrav = childHead;
        }
        else {
            childTrav->next = create_node(trav->IS_TERMINAL,trav->TYPE,ntn);
            childTrav = childTrav->next;
        }
        numberChild++;
        trav = trav->next;
    }

    ntn->NODE_TYPE.NL.RULE_NO = r->RULE_NO;
    ntn->NODE_TYPE.NL.child = childHead;
    ntn->NODE_TYPE.NL.NUMBER_CHILDREN = numberChild;
}

void print_nary_tree(NaryTreeNode* nt) {
    if(nt->IS_LEAF_NODE == 1) {
        printf("%s " ,getTerminal(nt->NODE_TYPE.L.ENUM_ID));
        return;
    }

    printf("%s\n" , getNonTerminal(nt->NODE_TYPE.NL.ENUM_ID));

    NaryTreeNode* childTrav = nt->NODE_TYPE.NL.child;
    while(childTrav != NULL) {

        if(childTrav->IS_LEAF_NODE == 1)
            printf("%s " ,getTerminal(childTrav->NODE_TYPE.L.ENUM_ID));
        else
            printf("%s " ,getNonTerminal(childTrav->NODE_TYPE.NL.ENUM_ID));

        childTrav = childTrav->next;
    }

    printf("\n");

    childTrav = nt->NODE_TYPE.NL.child;
    while(childTrav != NULL) {
        if(childTrav->IS_LEAF_NODE == 0)
            print_nary_tree(childTrav);
        childTrav = childTrav->next;
    }
}


int get_parsetree_node_count() {
    return parseTreeNodeCount;
}

int get_parsetree_memory() {
    return parseTreeMemory;
}


void print_tree(ParseTree* pt) {
    NaryTreeNode* nt = pt->root;
    print_nary_tree(nt);
}
