// #Group 9
// #Akshay Singhal								              2018B3A70635P
// #Archit Gupta                               2018B1A70149P
// #Punit Lohia                                2018B3A70766P
// #Saksham Pandey                             2018B3A70790P

#include "hashtable.h"
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#define NUMBER_KEYWORD 28


int calc_hash(char* str){
  unsigned long hash = 5381;
  int c;
  while (c = *str++)
        hash = ((hash << 5) + hash) + c;
  return (hash%NUMBER_KEYWORD);
}

void add_entry(Table *t,token_type ty,char *lexeme){
  int hash = calc_hash(lexeme);
  t->entries[hash].head=insert_list(t->entries[hash].head,ty,lexeme);
}

Node* look_up(Table *t,char *lexeme){
  int hash=calc_hash(lexeme);
  Node* temp=t->entries[hash].head;
  while(temp!=NULL){
    if(strcmp(lexeme,temp->lexeme)==0)
      return temp;
    temp=temp->next;
  }
  return NULL;
}

Table* initialize_table(){
  Table* t = (Table*)malloc(sizeof(Table));
  t->entries = (Entry*)malloc(NUMBER_KEYWORD*sizeof(Entry));

  for(int i=0;i<NUMBER_KEYWORD;i++)
    t->entries[i].head=NULL;

    add_entry(t,TK_WITH,"with");
    add_entry(t,TK_PARAMETERS,"parameters");
    add_entry(t,TK_END,"end");
    add_entry(t,TK_WHILE,"while");
    add_entry(t,TK_UNION,"union");
    add_entry(t,TK_ENDUNION,"endunion");
    add_entry(t,TK_DEFINETYPE,"definetype");
    add_entry(t,TK_AS,"as");
    add_entry(t,TK_TYPE,"type");
    add_entry(t,TK_MAIN,"_main");
    add_entry(t,TK_GLOBAL,"global");
    add_entry(t,TK_PARAMETER,"parameter");
    add_entry(t,TK_LIST,"list");
    add_entry(t,TK_INPUT,"input");
    add_entry(t,TK_OUTPUT,"output");
    add_entry(t,TK_INT,"int");
    add_entry(t,TK_REAL,"real");
    add_entry(t,TK_ENDWHILE,"endwhile");
    add_entry(t,TK_IF,"if");
    add_entry(t,TK_THEN,"then");
    add_entry(t,TK_ENDIF,"endif");
    add_entry(t,TK_READ,"read");
    add_entry(t,TK_WRITE,"write");
    add_entry(t,TK_RETURN,"return");
    add_entry(t,TK_CALL,"call");
    add_entry(t,TK_RECORD,"record");
    add_entry(t,TK_ENDRECORD,"endrecord");
    add_entry(t,TK_ELSE,"else");

    return t;
}

Node* insert_list(Node* node, token_type ty, char* lexeme) {

    if(node == NULL) {
        Node* n  = (Node*)malloc(sizeof(Node));
        n->lexeme = lexeme;
        n->Token_Type = ty;
        n->next = NULL;
        return n;
    }

    Node* n = (Node*)malloc(sizeof(Node));
    n->lexeme = lexeme;
    n->Token_Type = ty;
    n->next = node;
    return n;
}

int search_list(Node* node, char* lexeme) {
    Node* temp = node;
    while(temp != NULL) {
        if(strcmp(lexeme,temp->lexeme))
            return 1;
        temp = temp->next;
    }
    return 0;
}

int print_list(Node* node){
  Node* temp = node;
  int length = 0;
  if(node == NULL) {
      printf("This Slot is Empty\n\n");
      return 0;
  }

  while(temp != NULL) {
      printf("Keyword: %s \n" ,temp->lexeme);
      temp = temp->next;
      length++;
  }
  printf("\n\n");
  return length;
}

/*
void print_hash_table(Table *t){
  int empty = 0;
   int collisions = 0;
   for(int i=0; i < NUMBER_KEYWORD; i++) {
       int len = print_list(t->entries[i].head);
       if(len == 0)
           empty++;
       if(len > 1)
           collisions += len-1;
   }

   // Aim for as less a load factor as possible, JAVA 10 specs says 0.75

   printf("\n");
   printf("Calculating laod-factor\n");
   printf("%f\n" ,((float)empty)/NUMBER_KEYWORD);

   // Aim for as less collision as possible

   printf("\n");
   printf("Calculating total collisions\n");
   printf("%d\n" , collisions);
}
*/
