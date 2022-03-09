// #Group 9
// #Akshay Singhal								              2018B3A70635P
// #Archit Gupta                               2018B1A70149P
// #Punit Lohia                                2018B3A70766P
// #Saksham Pandey                             2018B3A70790P


#include "hashtabledef.h"

int calc_hash(char* str);
void add_entry(Table* t,token_type ty, char* lexeme);
Node* look_up(Table* t,char* lexeme);
Table* initialize_table();
void print_hash_table(Table* t);

Node* insert_list(Node* node, token_type ty, char* lexeme);
int search_list(Node* node, char* lexeme);
int print_list(Node* node);
