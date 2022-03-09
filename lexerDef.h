// #Group 9
// #Akshay Singhal								              2018B3A70635P
// #Archit Gupta                               2018B1A70149P
// #Punit Lohia                                2018B3A70766P
// #Saksham Pandey                             2018B3A70790P

#ifndef LEXERDEF
#define LEXERDEF

typedef enum token_type {
    TK_ASSIGNOP,
    TK_COMMENT,
    TK_ID,
    TK_NUM,
    TK_RNUM,
    TK_FIELDID,
    TK_FUNID,
    TK_RUID,
    TK_WITH,
    TK_PARAMETERS,
    TK_END,
    TK_WHILE,
    TK_UNION,
    TK_ENDUNION,
    TK_DEFINETYPE,
    TK_AS,
    TK_TYPE,
    TK_MAIN,
    TK_GLOBAL,
    TK_PARAMETER,
    TK_LIST,
    TK_SQL,
    TK_SQR,
    TK_INPUT,
    TK_OUTPUT,
    TK_INT,
    TK_REAL,
    TK_COMMA,
    TK_SEM,
    TK_COLON,
    TK_DOT,
    TK_ENDWHILE,
    TK_OP,
    TK_CL,
    TK_IF,
    TK_THEN,
    TK_ENDIF,
    TK_READ,
    TK_WRITE,
    TK_RETURN,
    TK_PLUS,
    TK_MINUS,
    TK_MUL,
    TK_DIV,
    TK_CALL,
    TK_RECORD,
    TK_ENDRECORD,
    TK_ELSE,
    TK_AND,
    TK_OR,
    TK_NOT,
    TK_LE,
    TK_LT,
    TK_EQ,
    TK_GT,
    TK_GE,
    TK_NE,
    TK_EPS,
    TK_DOLLAR,
    TK_ERR
} token_type;

typedef union Value {
    int INT_VALUE;
    float FLOAT_VALUE;
} Value;

// If error type is 3 it denotes token identification failure
// If error type is 4 it denotes the token is identified but does not respect the constraints
// If error type is 5 it denotes identifiers are declared back to back
// If error type is 6 it denotes an unknown symbol
typedef struct Token {
    token_type TOKEN_NAME;
    char* LEXEME;
    int LINE_NO;
  
    int IS_NUMBER;

    Value* VALUE;
} Token;

#endif
