// #Group 9
// #Akshay Singhal								              2018B3A70635P
// #Archit Gupta                               2018B1A70149P
// #Punit Lohia                                2018B3A70766P
// #Saksham Pandey                             2018B3A70790P

#include "NAryDef.h"


void addRuleToParseTree(NaryTreeNode* ntn, Rule* r);
void print_tree(ParseTree* pt);
void print_nary_tree(NaryTreeNode* nt);
NaryTreeNode* create_non_leaf_node(int enumId);
NaryTreeNode* create_node(int isTerminal, SymbolType type,NaryTreeNode* parent);
ParseTree* parsetree_initialisation();
NaryTreeNode* create_leaf_node(int enumId);

int get_parsetree_node_count();
int get_parsetree_memory();
