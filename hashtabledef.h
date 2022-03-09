// #Group 9
// #Akshay Singhal								              2018B3A70635P
// #Archit Gupta                               2018B1A70149P
// #Punit Lohia                                2018B3A70766P
// #Saksham Pandey                             2018B3A70790P

#ifndef HASHTABLEDEF
#define HASHTABLEDEF

#include "lexerDef.h"

typedef struct Node {
  token_type Token_Type; //Lexerdef
  char *lexeme;
  struct Node* next;
} Node;

typedef struct {
  Node *head;
} Entry;

typedef struct {
    Entry* entries;
} Table;

Table* lookUpTable;

#endif
