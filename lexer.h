// #Group 9
// #Akshay Singhal								              2018B3A70635P
// #Archit Gupta                               2018B1A70149P
// #Punit Lohia                                2018B3A70766P
// #Saksham Pandey                             2018B3A70790P

#include "lexerDef.h"

void twinbuffer_initialisation(int f);
void lexer_initialisation(int f);
int get_stream();
char next_char();

Token* getToken();
void remove_comments(char* testCaseFile, char* cleanFile);
void print_buffers();
char* string_copy(char* start, char* end);
int stringToInteger(char* str);
float stringToFloat(char* str);
void accept();
void retract(int amt);
int match_rangeof_characters(char ch,char start, char end);
int match_single_character(char ch, char chToEqual);
Token* fill_token_values(Token* t,token_type tokenName,char* lexeme,int lineNumber,int isNumber,Value* value);
