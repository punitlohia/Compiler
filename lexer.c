// #Group 9
// #Akshay Singhal								              2018B3A70635P
// #Archit Gupta                               2018B1A70149P
// #Punit Lohia                                2018B3A70766P
// #Saksham Pandey                             2018B3A70790P

#include "lexerDef.h"
#include "hashtable.h"
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 4096


char* buffPair[2];   // Twin buffers for input
int bufferToBeLoaded = 0;
int currentBuffer = 1;
char* lexemeBegin = NULL;
char* forward = NULL;  // Acting as a lookahead pointer
int dfa_state = 0;
int lineCount = 1;
int inputExhausted = 0; // flag to check if the input has exhausted
int fp;
int nextCharacterReadAfterRetraction = 0;  // Needed to prevent a double count of \n after a retraction is performed

Table* lt;

void twinbuffer_initialisation(int f) {
    fp = f;
    buffPair[0] = (char*)malloc(BUFFER_SIZE*sizeof(char));
    buffPair[1] = (char*)malloc(BUFFER_SIZE*sizeof(char));
    lexemeBegin = NULL;
    forward = NULL;
    currentBuffer = 1;
    bufferToBeLoaded = 0;
    inputExhausted = 0;
    lineCount = 1;
    nextCharacterReadAfterRetraction = 0;
    dfa_state = 0;
}

void lexer_initialisation(int f) {
    twinbuffer_initialisation(f);
    lt = initialize_table();
}

int get_stream() {
    if(inputExhausted != 0)
        return EOF;

    memset(buffPair[bufferToBeLoaded],EOF ,BUFFER_SIZE);

    int res = read(fp,buffPair[bufferToBeLoaded],BUFFER_SIZE);
    if(res == 0 || res < BUFFER_SIZE) {
        inputExhausted = 1;
    }

    bufferToBeLoaded = 1 - bufferToBeLoaded;
    currentBuffer = 1 - currentBuffer;
    if(res == -1) {
        printf("Error: Input Buffers failed to be loaded\n");
        return -1;
    }
    return res;
}

char next_char() {
    if(lexemeBegin == NULL && forward == NULL) {

        // When get_stream is called for the first time
        int res = get_stream();
        if(res == -1) {
            return EOF;
        }
        lexemeBegin = buffPair[currentBuffer];
        forward = buffPair[currentBuffer];
        char* curr_forward = forward;
        forward++;

        // Only increment lineCount if the character read is \n and it is not being double counted due to a retraction
        if(nextCharacterReadAfterRetraction == 0 && *curr_forward == '\n') {
            lineCount++;
        }

        if(nextCharacterReadAfterRetraction == 1)
            nextCharacterReadAfterRetraction = 0;

        return *curr_forward;
    }

    char* curr_forward = forward;


    if(curr_forward - buffPair[currentBuffer] == BUFFER_SIZE-1) {
        int res = get_stream();
        if(res == -1) {
            return EOF;
        }
        forward = buffPair[currentBuffer];
    }
    else if(*curr_forward == EOF) {
        return EOF;
    }
    else
        forward++;

    if(nextCharacterReadAfterRetraction == 0 && *curr_forward == '\n') {
        lineCount++;
    }

    if(nextCharacterReadAfterRetraction == 1)
        nextCharacterReadAfterRetraction = 0;

    return *curr_forward;
}


void retract(int amt) {
    while(amt > 0) {
        --forward;
        --amt;
    }

    nextCharacterReadAfterRetraction = 1;
}

char* string_copy(char* start, char* end) {
    char* store = (char*)malloc((end-start+2)*sizeof(char));
    char* temp = start;
    int i = 0;
    // Note <= was erroneous , corrected to <
    while(temp < end) {
        store[i] = *temp;
        ++temp;
        ++i;
    }

    store[i] = '\0';
    return store;
}

int match_single_character(char ch, char chToEqual) {
    if(ch == chToEqual)
        return 1;
    return 0;
}


int match_rangeof_characters(char ch,char start, char end) {
    if(ch >= start && ch <= end)
        return 1;
    return 0;
}

void accept() {
    lexemeBegin = forward;
}

void print_buffers() {
    char c;

    while((c = next_char()) != EOF) {
        printf("%c " ,c);
    }
    printf("\n");
}


Token* fill_token_values(Token* t,token_type tokenName,char* lexeme,int lineNumber,int isNumber,Value* value) {
    t->TOKEN_NAME = tokenName;
    t->LINE_NO = lineNumber;
    t->IS_NUMBER = isNumber;
    t->LEXEME = lexeme;
    t->VALUE =value;
    return t;
}

int stringToInteger(char* str) {
    int num;
    sscanf(str,"%d",&num);
    return num;
}

float stringToFloat(char* str) {
    float f;
    sscanf(str,"%f",&f);
    return f;
}

// removing comments and printing the code on the console
void remove_comments(char* testCaseFile, char* cleanFile) {
    int tcf = open(testCaseFile,O_RDONLY);
    twinbuffer_initialisation(tcf);
    int check = 0;
    char c;
    while((c = next_char()) != EOF) {

        switch(check) {
            case 0: {
                if(c == ' ' || c == '\v' || c == '\f'  || c == '\t' || c == '\r') {
                    write(1,&c,1);
                    check = 0;
                }
                else if(c == '%') {
                    check = 3;
                }
                else if(c == '\n') {
                    write(1,&c,1);
                    check = 0;
                }
                else {
                    write(1,&c,1);
                    check = 2;
                }
                break;
            }
            case 2: {
                write(1,&c,1);
                if(c == '\n')
                    check = 0;
                break;
            }
            case 3: {
                if(c == '\n') {
                    write(1,&c,1);
                    check = 0;
                }
                break;
            }
        }

    }
    close(tcf);
}

Token* getToken() {

    dfa_state = 0;
    char c = 1;
    Token* t = (Token*)malloc(sizeof(Token));
    int errorType; // set before going to dead/error state 59

    while(1) {
        if(c == EOF)
            return NULL;


        switch(dfa_state) {
            case 0: {
                c = next_char();
                if(match_single_character(c,'<')) {
                    dfa_state = 12;
                }
                else if(match_single_character(c,'#')) {
                    dfa_state = 19;
                }
                else if(match_rangeof_characters(c,'b','d')) {
                    dfa_state = 40;
                }
                else if(match_single_character(c,'a') || match_rangeof_characters(c,'e','z')) {
                    dfa_state = 38;
                }
                else if(match_single_character(c,'_')) {
                    dfa_state = 22;
                }
                else if(match_single_character(c,'[')) {
                    dfa_state = 18;
                }
                else if(match_single_character(c,']')) {
                    dfa_state = 16;
                }
                else if(match_single_character(c,',')) {
                    dfa_state = 25;
                }
                else if(match_single_character(c,';')) {
                    dfa_state = 26;
                }
                else if(match_single_character(c,':')) {
                    dfa_state = 27;
                }
                else if(match_single_character(c,'.')) {
                    dfa_state = 28;
                }
                else if(match_single_character(c,'(')) {
                    dfa_state = 29;
                }
                else if(match_single_character(c,')')) {
                    dfa_state = 30;
                }
                else if(match_single_character(c,'+')) {
                    dfa_state = 34;
                }
                else if(match_single_character(c,'-')) {
                    dfa_state = 33;
                }
                else if(match_single_character(c,'*')) {
                    dfa_state = 31;
                }
                else if(match_single_character(c,'/')) {
                    dfa_state = 32;
                }
                else if(match_single_character(c,'~')) {
                    dfa_state = 35;
                }
                else if(match_single_character(c,'!')) {
                    dfa_state = 5;
                }
                else if(match_single_character(c,'>')) {
                    dfa_state = 7;
                }
                else if(match_single_character(c,'=')) {
                    dfa_state = 10;
                }
                else if(match_single_character(c,'@')) {
                    dfa_state = 3;
                }
                else if(match_single_character(c,'&')) {
                    dfa_state = 1;
                }
                else if(match_single_character(c,'%')) {
                    dfa_state = 36;
                }
                else if(match_rangeof_characters(c,'0','9')) {
                    dfa_state = 43;
                }
                else if(match_single_character(c,' ') || match_single_character(c,'\t') || match_single_character(c,'\v')) {
                    lexemeBegin++;
                    dfa_state = 0;
                }
                else if(match_single_character(c,'\n')) {
                    lexemeBegin++;
                    dfa_state = 0;
                }
                else if(match_single_character(c,EOF)) {
                    return NULL;
                }
                else {
                    printf("Line %d : Cannot recognize character %c \n" ,lineCount,c);
                    errorType = 6;
                    dfa_state = 59;
                }
                break;
            }
            case 1: {
                c = next_char();
                if(c == '&') {
                    dfa_state = 2;
                }
                else {
                    char* pattern = string_copy(lexemeBegin, forward-sizeof(char));
                    printf("Line %d : Cannot recognize pattern %s, Were you tring for &&& ?\n" ,lineCount,pattern);
                    free(pattern);
                    errorType = 3;
                    dfa_state = 59;

                    retract(1);
                }
                break;
            }
            case 2: {
                c = next_char();
                if(c == '&') {
                    dfa_state = 57;
                }
                else {
                    char* pattern = string_copy(lexemeBegin, forward-sizeof(char));
                    printf("Line %d : Cannot recognize pattern %s, Were you tring for &&& ?\n" ,lineCount,pattern);
                    free(pattern);
                    errorType = 3;
                    dfa_state = 59;

                    retract(1);
                }
                break;
            }
            case 3: {
                c = next_char();
                if(c == '@') {
                    dfa_state = 4;
                }
                else {
                    char* pattern = string_copy(lexemeBegin, forward-sizeof(char));
                    printf("Line %d : Cannot recognize pattern %s, Were you tring for @@@ ?\n" ,lineCount,pattern);
                    free(pattern);
                    errorType = 3;
                    dfa_state = 59;

                    retract(1);
                }
                break;
            }
            case 4: {
                c = next_char();
                if(c == '@') {
                    dfa_state = 58;
                }
                else {
                    char* pattern = string_copy(lexemeBegin,forward);
                    printf("Line %d : Cannot recognize pattern %s, Were you tring for @@@ ?\n" ,lineCount,pattern);
                    free(pattern);
                    errorType = 3;
                    dfa_state = 59;

                    retract(1);
                }
                break;
            }
            case 5: {
                c = next_char();
                if(c == '=') {
                    dfa_state = 6;
                }
                else {
                    // Throw Lexical error
                    char* pattern = string_copy(lexemeBegin, forward-sizeof(char));
                    printf("Line %d : Cannot recognize pattern %s, Were you tring for != ?\n" ,lineCount,pattern);
                    free(pattern);
                    errorType = 3;
                    dfa_state = 59;

                    retract(1);
                }
                break;
            }
            case 6: {
                char* lex = string_copy(lexemeBegin,forward);
                fill_token_values(t,TK_NE,lex,lineCount,0,NULL);
                accept();
                return t;
                break;
            }
            case 7: {
                c = next_char();
                if(c == '=') {
                    dfa_state = 8;
                }
                else {
                    dfa_state = 9;
                }
                break;
            }
            case 8: {
                char* lex = string_copy(lexemeBegin,forward);
                fill_token_values(t,TK_GE,lex,lineCount,0,NULL);
                accept();
                return t;
                break;
            }
            case 9: {
                retract(1);
                char* lex = string_copy(lexemeBegin,forward);
                if(c == '\n')
                    fill_token_values(t,TK_GT,lex,lineCount-1,0,NULL);
                else
                    fill_token_values(t,TK_GT,lex,lineCount,0,NULL);
                accept();
                return t;
                break;
            }
            case 10: {
                c = next_char();
                if(c == '=') {
                    dfa_state = 11;
                }
                else {
                    char* pattern = string_copy(lexemeBegin, forward-sizeof(char));
                    printf("Line %d : Cannot recognize pattern %s, Were you tring for == ?\n" ,lineCount,pattern);
                    free(pattern);
                    errorType = 3;
                    dfa_state = 59;

                    retract(1);
                }
                break;
            }
            case 11: {
                char* lex = string_copy(lexemeBegin,forward);
                fill_token_values(t,TK_EQ,lex,lineCount,0,NULL);
                accept();
                return t;
                break;
            }
            case 12: {
                c = next_char();
                if(c == '-') {
                    dfa_state = 13;
                }
                else if(c == '=') {
                    dfa_state = 14;
                }
                else {
                    dfa_state = 15;
                }
                break;
            }
            case 13: {
                c = next_char();
                if(c == '-') {
                    dfa_state = 17;
                }
                else {
                    char* pattern = string_copy(lexemeBegin, forward-sizeof(char));
                    printf("Line %d : Cannot recognize pattern %s, Were you tring for <--- ?\n" ,lineCount,pattern);
                    free(pattern);
                    errorType = 3;
                    dfa_state = 59;

                    retract(1);
                }
                break;
            }
            case 14: {
                char* lex = string_copy(lexemeBegin,forward);
                fill_token_values(t,TK_LE,lex,lineCount,0,NULL);
                accept();
                return t;
                break;
            }
            case 15: {
                retract(1);
                char* lex = string_copy(lexemeBegin,forward);

                if(c == '\n')
                    fill_token_values(t,TK_LT,lex,lineCount-1,0,NULL);
                else
                    fill_token_values(t,TK_LT,lex,lineCount,0,NULL);

                accept();
                return t;
                break;
            }
            case 16: {
                char* lex = string_copy(lexemeBegin,forward);
                fill_token_values(t,TK_SQR,lex,lineCount,0,NULL);
                accept();
                return t;
                break;
            }
            case 17: {
                c = next_char();
                if(c == '-') {
                    dfa_state = 60;
                }
                else {
                    char* pattern = string_copy(lexemeBegin, forward-sizeof(char));
                    printf("Line %d : Cannot recognize pattern %s, Were you tring for <--- ?\n" ,lineCount,pattern);
                    free(pattern);
                    errorType = 3;
                    dfa_state = 59;

                    retract(1);
                }
                break;
            }
            case 18: {
                char* lex = string_copy(lexemeBegin,forward);
                fill_token_values(t,TK_SQL,lex,lineCount,0,NULL);
                accept();
                return t;
                break;
            }
            case 19: {
                c = next_char();
                if(match_rangeof_characters(c,'a','z')) {
                    dfa_state = 20;
                }
                else {
                    char* pattern = string_copy(lexemeBegin, forward-sizeof(char));
                    printf("Line %d : Cannot recognize pattern %s, Were you tring for a record ID ?\n" ,lineCount,pattern);
                    free(pattern);
                    errorType = 3;
                    dfa_state = 59;

                    retract(1);
                }
                break;
            }
            case 20: {
                c = next_char();
                while(match_rangeof_characters(c,'a','z'))
                    c = next_char();

                dfa_state = 21;
                break;
            }
            case 21: {
                retract(1);
                char* lex = string_copy(lexemeBegin,forward);
                if(c == '\n')
                    fill_token_values(t,TK_RUID,lex,lineCount-1,0,NULL);
                else
                    fill_token_values(t,TK_RUID,lex,lineCount,0,NULL);
                accept();
                return t;
                break;
            }
            case 22: {
                c = next_char();
                if(match_rangeof_characters(c,'a','z') || match_rangeof_characters(c,'A','Z')) {
                    dfa_state = 23;
                }
                else {
                    char* pattern = string_copy(lexemeBegin, forward-sizeof(char));
                    printf("Line %d : Cannot recognize pattern %s, Were you tring for a function ID ?\n" ,lineCount,pattern);
                    free(pattern);
                    errorType = 3;
                    dfa_state = 59;

                    retract(1);
                }
                break;
            }
            case 23: {
                c = next_char();
                while(match_rangeof_characters(c,'a','z') || match_rangeof_characters(c,'A','Z'))
                    c = next_char();

                if(match_rangeof_characters(c,'0','9')) {
                    dfa_state = 51;
                }
                else {
                    dfa_state = 24;
                }
                break;
            }
            case 24: {
                retract(1);
                char* lex = string_copy(lexemeBegin,forward);
                Node* n = look_up(lt,lex);
                if(n == NULL) {
                    if(c == '\n')
                        fill_token_values(t,TK_FUNID,lex,lineCount-1,0,NULL);
                    else
                        fill_token_values(t,TK_FUNID,lex,lineCount,0,NULL);
                }
                else {
                    if(c == '\n')
                        fill_token_values(t,n->Token_Type,lex,lineCount-1,0,NULL);
                    else
                        fill_token_values(t,n->Token_Type,lex,lineCount,0,NULL);
                }
                accept();
                return t;
                break;
            }
            case 25: {
                char* lex = string_copy(lexemeBegin,forward);
                fill_token_values(t,TK_COMMA,lex,lineCount,0,NULL);
                accept();
                return t;
                break;
            }
            case 26: {
                char* lex = string_copy(lexemeBegin,forward);
                fill_token_values(t,TK_SEM,lex,lineCount,0,NULL);
                accept();
                return t;
                break;
            }
            case 27: {
                char* lex = string_copy(lexemeBegin,forward);
                fill_token_values(t,TK_COLON,lex,lineCount,0,NULL);
                accept();
                return t;
                break;
            }
            case 28: {
                char* lex = string_copy(lexemeBegin,forward);
                fill_token_values(t,TK_DOT,lex,lineCount,0,NULL);
                accept();
                return t;
                break;
            }
            case 29: {
                char* lex = string_copy(lexemeBegin,forward);
                fill_token_values(t,TK_OP,lex,lineCount,0,NULL);
                accept();
                return t;
                break;
            }
            case 30: {
                char* lex = string_copy(lexemeBegin,forward);
                fill_token_values(t,TK_CL,lex,lineCount,0,NULL);
                accept();
                return t;
                break;
            }
            case 31: {
                char* lex = string_copy(lexemeBegin,forward);
                fill_token_values(t,TK_MUL,lex,lineCount,0,NULL);
                accept();
                return t;
                break;
            }
            case 32: {
                char* lex = string_copy(lexemeBegin,forward);
                fill_token_values(t,TK_DIV,lex,lineCount,0,NULL);
                accept();
                return t;
                break;
            }
            case 33: {
                char* lex = string_copy(lexemeBegin,forward);
                fill_token_values(t,TK_MINUS,lex,lineCount,0,NULL);
                accept();
                return t;
                break;
            }
            case 34: {
                char* lex = string_copy(lexemeBegin,forward);
                fill_token_values(t,TK_PLUS,lex,lineCount,0,NULL);
                accept();
                return t;
                break;
            }
            case 35: {
                char* lex = string_copy(lexemeBegin,forward);
                fill_token_values(t,TK_NOT,lex,lineCount,0,NULL);
                accept();
                return t;
                break;
            }
            case 36: {
                c = next_char();
                while(c != '\n' && c != EOF) {
                    c = next_char();
                }
                dfa_state = 37;
                break;
            }
            case 37: {
                char* lex = string_copy(lexemeBegin,forward-1);

                if(c == '\n')
                    fill_token_values(t,TK_COMMENT,lex,lineCount-1,0,NULL);
                else
                    fill_token_values(t,TK_COMMENT,lex,lineCount,0,NULL);
                accept();

                return t;
                break;
            }
            case 38: {
                c = next_char();
                while(match_rangeof_characters(c,'a','z'))
                    c = next_char();

                dfa_state = 39;
                break;
            }
            case 39: {
                retract(1);
                char* lex = string_copy(lexemeBegin,forward);
                Node* n = look_up(lt,lex);
                if(n == NULL) {
                    if(c == '\n')
                        fill_token_values(t,TK_FIELDID,lex,lineCount-1,0,NULL);
                    else
                        fill_token_values(t,TK_FIELDID,lex,lineCount,0,NULL);
                }
                else {
                    if(c == '\n')
                        fill_token_values(t,n->Token_Type,lex,lineCount-1,0,NULL);
                    else
                        fill_token_values(t,n->Token_Type,lex,lineCount,0,NULL);
                }
                accept();
                return t;
                break;
            }
            case 40: {
                c = next_char();
                if(match_rangeof_characters(c,'2','7')) {
                    dfa_state = 41;
                }
                else if(match_rangeof_characters(c,'a','z')) {
                    dfa_state = 38;
                }
                else {
                    dfa_state = 39;
                }
                break;
            }
            case 41: {
                c = next_char();
                while(match_rangeof_characters(c,'b','d'))
                    c = next_char();

                if(match_rangeof_characters(c,'2','7'))
                    dfa_state = 45;
                else {
                    dfa_state = 42;
                }
                break;
            }
            case 42: {
                retract(1);
                char* lex = string_copy(lexemeBegin,forward);
                if(c == '\n')
                    fill_token_values(t,TK_ID,lex,lineCount-1,0,NULL);
                else
                    fill_token_values(t,TK_ID,lex,lineCount,0,NULL);

                accept();
                return t;
                break;
            }
            case 43: {
                c = next_char();
                while(match_rangeof_characters(c,'0','9'))
                    c = next_char();

                if(c == '.') {
                    dfa_state = 47;
                }
                else {
                    dfa_state = 44;
                }
                break;
            }
            case 44: {
                retract(1);
                char* lex = string_copy(lexemeBegin,forward);
                Value* val = malloc(sizeof(Value));
                val->INT_VALUE = stringToInteger(lex);
                if(c == '\n')
                    fill_token_values(t,TK_NUM,lex,lineCount-1,1,val);
                else
                    fill_token_values(t,TK_NUM,lex,lineCount,1,val);
                accept();
                return t;
                break;
            }
            case 45: {
                c = next_char();
                while(match_rangeof_characters(c,'2','7'))
                    c = next_char();

                if(!match_rangeof_characters(c,'2','7') && !match_rangeof_characters(c,'b','d')) {
                    dfa_state = 46;
                }
                else {
                    printf("Line %d : two identifers are not allowed back to back without a break ?\n" ,lineCount);
                    errorType = 5;
                    dfa_state = 59;
                }
                break;
            }
            case 46: {
                retract(1);
                int identifierLength = forward - lexemeBegin + 1;
                if(identifierLength < 2) {
                    printf("Line %d : Identifier length is less than 2\n" , lineCount);
                    errorType = 4;
                    dfa_state = 59;
                }
                else if(identifierLength > 20) {
                    printf("Line %d : Identifier length is more than 20\n" ,lineCount);
                    errorType = 4;
                    dfa_state = 59;
                }
                else {
                    char* lex = string_copy(lexemeBegin,forward);
                    if(c == '\n')
                        fill_token_values(t,TK_ID,lex,lineCount-1,0,NULL);
                    else
                        fill_token_values(t,TK_ID,lex,lineCount,0,NULL);
                    accept();
                    return t;
                }
                break;
            }
            case 47: {
                c = next_char();
                if(match_rangeof_characters(c,'0','9')) {
                    dfa_state = 48;
                }
                else {
                    char* pattern = string_copy(lexemeBegin, forward-sizeof(char));
                    printf("Line %d : Cannot recognize pattern %s, Were you tring for a real number ?\n" ,lineCount,pattern);
                    free(pattern);
                    errorType = 3;
                    dfa_state = 59;

                    retract(1);
                }
                break;
            }
            case 48: {
                c = next_char();
                if(match_rangeof_characters(c,'0','9')) {
                    dfa_state = 49;
                }
                else {
                    // Throw lexical
                    char* pattern = string_copy(lexemeBegin, forward-sizeof(char));
                    printf("Line %d : Cannot recognize pattern %s, Were you tring for a real number ?\n" ,lineCount,pattern);
                    free(pattern);
                    errorType = 3;
                    dfa_state = 59;

                    retract(1);
                }
                break;
            }
            case 49: {
                c = next_char();
                if(c == 'E')
                    dfa_state = 53;
                else
                    dfa_state = 50;

                break;
            }
            case 50: {
                retract(1);
                char* lex = string_copy(lexemeBegin,forward);
                fill_token_values(t,TK_RNUM,lex,lineCount,0,NULL);
                accept();
                return t;
                break;

            }
            case 51: {
                c = next_char();
                while(match_rangeof_characters(c,'0','9'))
                    c = next_char();

                dfa_state = 52;

                break;
            }
            case 52: {
                retract(1);
                int identifierLength = forward - lexemeBegin + 1;
                if(identifierLength > 30) {
                    printf("Line %d : Function identifier length exceeds 30 characters\n" ,lineCount);
                    errorType = 4;
                    dfa_state = 59;
                }
                else {
                    char* lex = string_copy(lexemeBegin,forward);
                    if(c == '\n')
                        fill_token_values(t,TK_FUNID,lex,lineCount,0,NULL);
                    else
                        fill_token_values(t,TK_FUNID,lex,lineCount-1,0,NULL);
                    accept();
                    return t;
                }
                break;
            }
            case 53: {
                c = next_char();
                if(c == '+' || c =='-')
                    dfa_state = 54;
                else if(match_rangeof_characters(c,'0','9'))
                    dfa_state = 55;
                else {
                    char* pattern = string_copy(lexemeBegin, forward-sizeof(char));
                    printf("Line %d : Cannot recognize pattern %s, Were you tring for a real number ?\n" ,lineCount,pattern);
                    free(pattern);
                    errorType = 3;
                    dfa_state = 59;

                    retract(1);

                }
                break;
            }
            case 54: {
                c = next_char();
                if(match_rangeof_characters(c,'0','9'))
                    dfa_state = 55;
                else {
                    char* pattern = string_copy(lexemeBegin, forward-sizeof(char));
                    printf("Line %d : Cannot recognize pattern %s, Were you tring for a real number ?\n" ,lineCount,pattern);
                    free(pattern);
                    errorType = 3;
                    dfa_state = 59;

                    retract(1);

                }
                break;
            }
            case 55: {
                c = next_char();
                if(match_rangeof_characters(c,'0','9'))
                    dfa_state = 56;
                else {
                    char* pattern = string_copy(lexemeBegin, forward-sizeof(char));
                    printf("Line %d : Cannot recognize pattern %s, Were you tring for a real number ?\n" ,lineCount,pattern);
                    free(pattern);
                    errorType = 3;
                    dfa_state = 59;

                    retract(1);

                }
                break;
            }
            case 56 :{
                char* lex = string_copy(lexemeBegin,forward);
                fill_token_values(t,TK_RNUM,lex,lineCount,0,NULL);
                accept();
                return t;
                break;
            }
            case 57 :{
                char* lex = string_copy(lexemeBegin,forward);
                fill_token_values(t,TK_AND,lex,lineCount,0,NULL);
                accept();
                return t;
                break;
            }
            case 58 :{
                char* lex = string_copy(lexemeBegin,forward);
                fill_token_values(t,TK_OR,lex,lineCount,0,NULL);
                accept();
                return t;
                break;
            }
            case 59 : {
                char* lex = string_copy(lexemeBegin,forward);

                if(errorType == 3 && c == '\n')
                    fill_token_values(t,TK_ERR,lex,lineCount-1,errorType,NULL);
                else
                    fill_token_values(t,TK_ERR,lex,lineCount,errorType,NULL);
                accept();

                return t;

                break;
            }
            case 60 : {
                char* lex = string_copy(lexemeBegin,forward);
                fill_token_values(t,TK_ASSIGNOP,lex,lineCount,0,NULL);
                accept();
                return t;
                break;
            }
        }
    }

}
