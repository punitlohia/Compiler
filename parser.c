// #Group 9
// #Akshay Singhal								              2018B3A70635P
// #Archit Gupta                               2018B1A70149P
// #Punit Lohia                                2018B3A70766P
// #Saksham Pandey                             2018B3A70790P

#include "parserDef.h"
#include "lexer.h"
#include "NAry.h"
#include "stack.h"
#include <string.h>
#include<stdio.h>
#define grammar_name "GRAMMAR.txt"
#define num_terminals 60
#define num_rules 95
#define num_nonterminals 53


NonTerminalRuleRecords** nt_rule_records;
int checkIfDone[num_nonterminals] = {0};
int vectorSize = num_terminals+1;

int syntaxErrorFlag;
int lexicalErrorFlag;
Grammar* g;


char* NonTerminalID[] = {
  "program",
    "mainFunction",
"otherFunctions",
"function",
"input_par",
"output_par",
"parameter_list",
"dataType",
"primitiveDatatype",
"constructedDatatype",
"remaining_list",
"stmts",
"typeDefinitions",
"typeDefinition",
"fieldDefinitions",
"fieldDefinition",
"moreFields",
"declarations",
"declaration",
"global_or_not",
"otherStmts",
"stmt",
"assignmentStmt",
"singleOrRecId",
"funCallStmt",
"outputParameters",
"inputParameters",
"iterativeStmt",
"conditionalStmt",
"ioStmt",
"arithmeticExpression",
"expPrime",
"term",
"termPrime",
"factor",
"highPrecedenceOperators",
"lowPrecedenceOperators",
"booleanExpression",
    "var",
"logicalOp",
"relationalOp",
"returnStmt",
"optionalReturn",
"idList",
"more_ids",
"definetypestmt",
"recOrUnion",
"actualOrRedefined",
"fieldType",
"option_single_constructed",
"oneExpansion",
"moreExpansions",
"elsePart"
};

char* TerminalID[] = {
    "TK_ASSIGNOP",
    "TK_COMMENT",
    "TK_ID",
    "TK_NUM",
    "TK_RNUM",
    "TK_FIELDID",
    "TK_FUNID",
    "TK_RUID",
    "TK_WITH",
    "TK_PARAMETERS",
    "TK_END",
    "TK_WHILE",
    "TK_UNION",
    "TK_ENDUNION",
    "TK_DEFINETYPE",
    "TK_AS",
    "TK_TYPE",
    "TK_MAIN",
    "TK_GLOBAL",
    "TK_PARAMETER",
    "TK_LIST",
    "TK_SQL",
    "TK_SQR",
    "TK_INPUT",
    "TK_OUTPUT",
    "TK_INT",
    "TK_REAL",
    "TK_COMMA",
    "TK_SEM",
    "TK_COLON",
    "TK_DOT",
    "TK_ENDWHILE",
    "TK_OP",
    "TK_CL",
    "TK_IF",
    "TK_THEN",
    "TK_ENDIF",
    "TK_READ",
    "TK_WRITE",
    "TK_RETURN",
    "TK_PLUS",
    "TK_MINUS",
    "TK_MUL",
    "TK_DIV",
    "TK_CALL",
    "TK_RECORD",
    "TK_ENDRECORD",
    "TK_ELSE",
    "TK_AND",
    "TK_OR",
    "TK_NOT",
    "TK_LT",
    "TK_LE",
    "TK_EQ",
    "TK_GT",
    "TK_GE",
    "TK_NE",
    "TK_EPS",
    "TK_DOLLAR",
    "TK_ERR"
};



char* lexeme_copy(char* str) {
    int len = strlen(str);
    char* lex = (char*)malloc(sizeof(char)*(len+1));
    for(int i=0; i < len; i++)
        lex[i] = str[i];

    lex[len] = '\0';
    return lex;
}


char* symbol_append(char* str, char c) {
    int len = strlen(str);
    char* strConcat = (char*)malloc(sizeof(char)*(len+2));
    for(int i=0; i < len; i++)
        strConcat[i] = str[i];

    strConcat[len] = c;
    strConcat[len+1] = '\0';
    return strConcat;
}

int findInTerminalMap(char* str) {
    for(int i=0; i < num_terminals; i++) {
        if(strcmp(str,TerminalID[i]) == 0)
            return i;
    }

    return -1;
}


char* getTerminal(int enumId) {
    return TerminalID[enumId];
}

char* getNonTerminal(int enumId) {
    return NonTerminalID[enumId];
}

int findInNonTerminalMap(char* str) {
    for(int i=0; i < num_nonterminals; i++) {
        if(strcmp(str,NonTerminalID[i]) == 0)
            return i;
    }

    return -1;
}

// initialises the grmmar structure object g
int grammar_initialisation() {

    g = (Grammar*)malloc(sizeof(Grammar));
    // printf("Testing 1");
    g->GRAMMAR_RULES_SIZE = num_rules+1;
    g->GRAMMAR_RULES = (Rule**)malloc(sizeof(Rule*)*g->GRAMMAR_RULES_SIZE);
    g->GRAMMAR_RULES[0] = NULL;
}

ParsingTable* parsing_table_initialisation() {
    ParsingTable* pt = (ParsingTable*)malloc(sizeof(ParsingTable));
    pt->entries = (int**)malloc(num_nonterminals*sizeof(int*));
    for(int i=0; i < num_nonterminals; i++) {

        pt->entries[i] = (int*)calloc(num_terminals,sizeof(int));
    }
    return pt;
}



Symbol* symbol_initialisation(char* symbol) {

    Symbol* s = (Symbol*)malloc(sizeof(Symbol));
        int idNonTerminal, idTerminal;
        idNonTerminal = findInNonTerminalMap(symbol);
        if(idNonTerminal != -1) {
            s->TYPE.NON_TERMINAL = idNonTerminal;
            s->IS_TERMINAL = 0;
        }
        else {
            idTerminal = findInTerminalMap(symbol);
            if(idTerminal != -1) {
                s->TYPE.TERMINAL = idTerminal;
                s->IS_TERMINAL = 1;
            }
        }

    s->next = NULL;

    return s;
}


void CheckIfDone_initialisation() {
    for(int i=0; i < num_nonterminals; i++)
        checkIfDone[i] = 0;
}

SymbolList* symbol_list_initialisation() {
    SymbolList* sl = (SymbolList*)malloc(sizeof(SymbolList));
    sl->HEAD_SYMBOL = NULL;
    sl->TAIL_SYMBOL = NULL;
    sl->RULE_LENGTH = 0;
    return sl;
}

Rule* rule_initialisation(SymbolList* sl, int ruleCount) {
    Rule* r = (Rule*)malloc(sizeof(Rule));
    r->SYMBOLS = sl;
    r->RULE_NO = ruleCount;
    return r;
}

NonTerminalRuleRecords** nonterminal_records_initialisation() {
    NonTerminalRuleRecords** nt_rule_records = (NonTerminalRuleRecords**)malloc(sizeof(NonTerminalRuleRecords*)*num_nonterminals);
    return nt_rule_records;
}


FirstAndFollow* firstandfollowset_initialisation() {
    FirstAndFollow* fafl = (FirstAndFollow*)malloc(sizeof(FirstAndFollow));

    fafl->FIRST = (int**)malloc(sizeof(int*)*num_nonterminals);
    fafl->FOLLOW = (int**)malloc(sizeof(int*)*num_nonterminals);


    for(int i=0; i < num_nonterminals; i++) {
        fafl->FIRST[i] = (int*)calloc(vectorSize,sizeof(int));
        fafl->FOLLOW[i] = (int*)calloc(vectorSize,sizeof(int));
    }

    return fafl;

}

void calcuating_firstset(int** firstVector, int enumId) {

    int start = nt_rule_records[enumId]->start;
    int end = nt_rule_records[enumId]->end;
    int producesNull = 0;
    // printf("Testing 2");
    for(int i=start; i <= end; i++) {
        Rule* r = g->GRAMMAR_RULES[i];
        Symbol* s = r->SYMBOLS->HEAD_SYMBOL;
        Symbol* trav = s;
        Symbol* nextSymbol = trav->next;
        int ruleYieldsEpsilon = 1;
        while(nextSymbol != NULL) {


            if(nextSymbol->IS_TERMINAL == 1) {
                if(nextSymbol->TYPE.TERMINAL != TK_EPS) {
                    ruleYieldsEpsilon = 0;
                    firstVector[enumId][nextSymbol->TYPE.TERMINAL] = 1;
                }
                break;
            }


            if(checkIfDone[nextSymbol->TYPE.NON_TERMINAL] == 0) {
                calcuating_firstset(firstVector,nextSymbol->TYPE.NON_TERMINAL);
            }

            for(int j=0; j < vectorSize; j++) {
                if(firstVector[nextSymbol->TYPE.NON_TERMINAL][j] == 1)
                    firstVector[s->TYPE.NON_TERMINAL][j] = 1;
            }

            if(firstVector[nextSymbol->TYPE.NON_TERMINAL][TK_EPS] == 0) {
                ruleYieldsEpsilon = 0;
                break;
            }

            nextSymbol = nextSymbol->next;
        }

        if(ruleYieldsEpsilon)
            producesNull = 1;
    }

    if(producesNull)
        firstVector[enumId][TK_EPS] = 1;
    else
        firstVector[enumId][TK_EPS] = 0;

    checkIfDone[enumId] = 1;

}

void fill_firstset(int** firstVector, Grammar* g) {

    for(int i=0; i < num_nonterminals; i++) {
        if(checkIfDone[i] == 0)
            calcuating_firstset(firstVector,i);
    }
}

void fill_followset(int** followVector, int** firstVector, Grammar* g) {


    for(int i=1; i <= num_rules; i++) {
        Rule* r = g->GRAMMAR_RULES[i];
        Symbol* head = r->SYMBOLS->HEAD_SYMBOL;
        Symbol* trav = head->next;
        int epsilonIdentifier = 0;
        // printf("Testing 3");
        while(trav != NULL) {

            if(trav->IS_TERMINAL == 0) {
                Symbol* followTrav = trav->next;
                while(followTrav != NULL) {
                    if(followTrav->IS_TERMINAL == 1 && followTrav->TYPE.TERMINAL != TK_EPS) {
                        followVector[trav->TYPE.NON_TERMINAL][followTrav->TYPE.TERMINAL] = 1;
                        break;
                    }
                    else {

                        for(int j=0; j < vectorSize; j++)
                            if(firstVector[followTrav->TYPE.NON_TERMINAL][j] == 1 && j != TK_EPS)
                                followVector[trav->TYPE.NON_TERMINAL][j] = 1;

                        if(firstVector[followTrav->TYPE.NON_TERMINAL][TK_EPS] == 0)
                            break;

                    }
                    followTrav = followTrav->next;
                }


                if(trav->next == NULL || (followTrav == NULL)) {
                    for(int j=0; j < vectorSize; j++)
                        if(followVector[head->TYPE.NON_TERMINAL][j] == 1 && j != TK_EPS)
                            followVector[trav->TYPE.NON_TERMINAL][j] = 1;
                }

            }


            trav = trav->next;
        }
    }
}

// the follow set is filled till it stabilises
void followset_filling_to_stable(int** followVector, int** firstVector, Grammar* g) {
    int** prevFollowVector = (int**)malloc(num_nonterminals*sizeof(int*));
    // printf("Testing 4");
    for(int i=0; i < num_nonterminals; i++) {
        prevFollowVector[i] = (int*)calloc(vectorSize,sizeof(int));
    }

    followVector[program][TK_DOLLAR] = 1;
    prevFollowVector[program][TK_DOLLAR] = 1;

    while(1) {

        fill_followset(followVector,firstVector,g);
        int stable = 1;

        for(int i=0; i < num_nonterminals; i++) {
            for(int j=0; j < vectorSize; j++) {
                if(prevFollowVector[i][j] != followVector[i][j])
                    stable = 0;
            }
        }

        if(stable)
            break;

        for(int i=0; i < num_nonterminals; i++) {
            for(int j=0; j < vectorSize; j++)
                prevFollowVector[i][j] = followVector[i][j];
        }
    }
}

// Uses the previous functions to compute the first and follow sets for the non-terminals of the grammar
FirstAndFollow* compute_firstandfollow_sets(Grammar* g) {
    FirstAndFollow* fafl = firstandfollowset_initialisation();
    fill_firstset(fafl->FIRST,g);
    followset_filling_to_stable(fafl->FOLLOW,fafl->FIRST,g);
    return fafl;
}


void ParseTable_creation(FirstAndFollow* fafl, ParsingTable* pt) {

    for(int i=1; i <= num_rules; i++) {
        Rule* r = g->GRAMMAR_RULES[i];
        int lhsNonTerminal = r->SYMBOLS->HEAD_SYMBOL->TYPE.NON_TERMINAL;

        Symbol* rhsHead = r->SYMBOLS->HEAD_SYMBOL->next;
        Symbol* trav = rhsHead;
        int epsilonGenerated = 1;
        // printf("Testing 5");
        while(trav != NULL) {
            // Terminal encountered in RHS => It cannot generate epsilon, break!
            if(trav->IS_TERMINAL == 1 && trav->TYPE.TERMINAL != TK_EPS) {
                epsilonGenerated = 0;
                pt->entries[lhsNonTerminal][trav->TYPE.TERMINAL] = r->RULE_NO;
                break;
            }
            else if(trav->IS_TERMINAL == 1 && trav->TYPE.TERMINAL == TK_EPS) {
                // No action
                epsilonGenerated = 1;
                break;
            }
            else {


                for(int j=0; j < num_terminals; j++) {
                    if(fafl->FIRST[trav->TYPE.NON_TERMINAL][j] == 1)
                        pt->entries[lhsNonTerminal][j] = r->RULE_NO;
                }


                if(fafl->FIRST[trav->TYPE.NON_TERMINAL][TK_EPS] == 0) {
                    epsilonGenerated = 0;
                    break;
                }
            }

            trav = trav->next;
        }


        if(epsilonGenerated) {
            for(int j=0; j < num_terminals; j++) {
                if(fafl->FOLLOW[lhsNonTerminal][j] == 1)
                    pt->entries[lhsNonTerminal][j] = r->RULE_NO;
            }
        }
    }

}

void SymbolList_append(SymbolList* ls, Symbol* s) {
    Symbol* h = ls->HEAD_SYMBOL;
    if(h == NULL) {
        ls->HEAD_SYMBOL = s;
        ls->TAIL_SYMBOL = s;
        ls->RULE_LENGTH = 1;
        return;
    }

    ls->TAIL_SYMBOL->next = s;
    ls->TAIL_SYMBOL = s;
    ls->RULE_LENGTH += 1;
}

//Extracts the grammar rule from the file
Grammar* obtaining_grammar() {

    int ruleCount = 1;
    int fd = open(grammar_name,O_RDONLY);
    char c;
    int actualRead;
    char* symbol = "";

    int symbolsRead = 0;
    int noOfLinesofNonTerminal = 0;
    Symbol* currentNonTerminal = NULL;
    SymbolList* sl = NULL;
    // printf("Testing 6");
    grammar_initialisation();
    nt_rule_records = nonterminal_records_initialisation();
    CheckIfDone_initialisation();


    while((actualRead = read(fd,&c,sizeof(char))) != 0) {

        if(c == EOF) {
            break;
        }

        if(c == ' ') {
            symbolsRead++;
            Symbol* s = symbol_initialisation(symbol);
            SymbolList_append(sl,s);

            if(symbolsRead == 1 ) {

                if(currentNonTerminal == NULL) {
                    nt_rule_records[s->TYPE.NON_TERMINAL] = (NonTerminalRuleRecords*)malloc(sizeof(NonTerminalRuleRecords));
                    nt_rule_records[s->TYPE.NON_TERMINAL]->start = 1;
                }
                else if(currentNonTerminal != NULL && currentNonTerminal->TYPE.NON_TERMINAL != s->TYPE.NON_TERMINAL) {
                    nt_rule_records[currentNonTerminal->TYPE.NON_TERMINAL]->end = ruleCount-1;
                    nt_rule_records[s->TYPE.NON_TERMINAL] = (NonTerminalRuleRecords*)malloc(sizeof(NonTerminalRuleRecords));
                    nt_rule_records[s->TYPE.NON_TERMINAL]->start = ruleCount;
                }
                currentNonTerminal = s;
            }

            symbol = "";
        }

        else if(c == '\n') {
            Symbol* s = symbol_initialisation(symbol);
            SymbolList_append(sl,s);
            Rule* r = rule_initialisation(sl,ruleCount);
            g->GRAMMAR_RULES[ruleCount] = r;
            ruleCount++;
            symbolsRead=0;
            symbol = "";
        }
        else {
            if(symbolsRead == 0){
                sl = symbol_list_initialisation();
            }

            symbol = symbol_append(symbol,c);
        }


    }

    nt_rule_records[currentNonTerminal->TYPE.NON_TERMINAL]->end = ruleCount-1;

    return g;
}


void helper_function_print_parsetree(NaryTreeNode * pt, FILE* f) {

    if(pt == NULL)
        return;
      // printf("Testing 7");
    if(pt->IS_LEAF_NODE == 1) {
        int tokenEnumID = pt->NODE_TYPE.L.ENUM_ID;
        char lexeme[30];
        for(int i=0; i < 29; i++)
            lexeme[i] = ' ';
        lexeme[29] = '\0';

        if(tokenEnumID != TK_EPS) {
            for(int i=0; i < strlen(pt->NODE_TYPE.L.TK->LEXEME); i++)
                lexeme[i] = pt->NODE_TYPE.L.TK->LEXEME[i];
        }
        else {
            char* str = "EPSILON";
            for(int i=0; i < strlen(str); i++)
                lexeme[i] = str[i];
        }

        int lineNumber;
        int isNumber;
        int valueIfInt;
        float valueIfFloat;
        if(tokenEnumID != TK_EPS) {
            lineNumber = pt->NODE_TYPE.L.TK->LINE_NO;
            isNumber = pt->NODE_TYPE.L.TK->IS_NUMBER;
            if(isNumber == 1)
                valueIfInt = pt->NODE_TYPE.L.TK->VALUE->INT_VALUE;
            else if(isNumber == 2)
                valueIfFloat = pt->NODE_TYPE.L.TK->VALUE->FLOAT_VALUE;
        }
        else {
            lineNumber = -1;
        }

        char tokenName[20];
        for(int i=0; i < 19; i++)
            tokenName[i] = ' ';
        tokenName[19] = '\0';

        char* obtainedTokenName = getTerminal(pt->NODE_TYPE.L.ENUM_ID);

        for(int i=0; i < strlen(obtainedTokenName); i++) {
            tokenName[i] = obtainedTokenName[i];
        }

        char parent[30];
        for(int i=0; i < 29; i++)
            parent[i] = ' ';

        parent[29] = '\0';
        char* obtainedParent = getNonTerminal(pt->parent->NODE_TYPE.NL.ENUM_ID);
        for(int i=0; i < strlen(obtainedParent); i++)
            parent[i] = obtainedParent[i];

        char* isLeafNode = "yes";
        char* currentSymbol = "----";

        if(tokenEnumID == TK_EPS || isNumber == 0)
            fprintf(f,"%s      %d    %s %s %s %s %s\n" ,lexeme,lineNumber,tokenName,"----   ",parent,isLeafNode,currentSymbol);
        else if(isNumber == 1)
            fprintf(f,"%s      %d    %s %d          %s %s %s\n" ,lexeme,lineNumber,tokenName,valueIfInt,parent,isLeafNode,currentSymbol);
        else
            fprintf(f,"%s      %d    %s %f    %s %s %s\n" ,lexeme,lineNumber,tokenName,valueIfFloat,parent,isLeafNode,currentSymbol);

    }
    // printf("Testing 8");
    else {
        NaryTreeNode* trav = pt->NODE_TYPE.NL.child;

        if(trav!=NULL) {
            helper_function_print_parsetree(pt->NODE_TYPE.NL.child, f);
            trav = trav->next;
        }

        char lexeme[30];
        for(int i=0; i < 29; i++)
            lexeme[i] = ' ';
        lexeme[29] = '\0';
        lexeme[0] = '-'; lexeme[1] = '-'; lexeme[2] = '-'; lexeme[3] = '-';

        int lineNumber = -1;
        char* tokenName = "-----------        ";
        char* valueIfNumber = "----   ";
        char* currentSymbol = getNonTerminal(pt->NODE_TYPE.NL.ENUM_ID);
        char parent[30];
        for(int i=0; i < 29; i++)
            parent[i] = ' ';

        parent[29] = '\0';

        char* obtainedParent;
        if(pt->parent != NULL)
            obtainedParent = getNonTerminal(pt->parent->NODE_TYPE.NL.ENUM_ID);
        else
            obtainedParent = "NULL";

        for(int i=0; i < strlen(obtainedParent); i++)
            parent[i] = obtainedParent[i];

        char* isLeafNode = "no";

        fprintf(f,"%s      %d    %s %s %s %s %s\n" ,lexeme,lineNumber,tokenName,valueIfNumber,parent,isLeafNode,currentSymbol);


        while(trav!=NULL){
            helper_function_print_parsetree(trav,f);
            trav = trav->next;
        }

    }
}


ParseTree* source_code_parsing(char *testcaseFile, ParsingTable* pTable, FirstAndFollow* fafl) {

    int f = open(testcaseFile,O_RDONLY);

    lexer_initialisation(f);
    ParseTree* pt = parsetree_initialisation();
    Stack* st = initialiseStack(pt);

    syntaxErrorFlag = *((int*)malloc(sizeof(int)));
    lexicalErrorFlag = *((int*)malloc(sizeof(int)));

    syntaxErrorFlag = 0;
    lexicalErrorFlag = 0;
    Token* missedToken = NULL;
    Token* inputToken = getToken();
    // printf("Testing 9");
    while(1) {

        if(inputToken == NULL)
            break;

        if(inputToken->TOKEN_NAME == TK_COMMENT) {
            inputToken = getToken();
            continue;
        }

        if(inputToken->TOKEN_NAME == TK_ERR) {
            lexicalErrorFlag = 1;
        }

        NaryTreeNode* stackTop = top(st);

        if(stackTop->IS_LEAF_NODE == 1) {

            if(inputToken->TOKEN_NAME == stackTop->NODE_TYPE.L.ENUM_ID) {

                stackTop->NODE_TYPE.L.TK = (Token*)malloc(sizeof(Token));
                stackTop->NODE_TYPE.L.TK->LEXEME = lexeme_copy(inputToken->LEXEME);
                stackTop->NODE_TYPE.L.TK->LINE_NO = inputToken->LINE_NO;
                stackTop->NODE_TYPE.L.TK->TOKEN_NAME = inputToken->TOKEN_NAME;
                stackTop->NODE_TYPE.L.TK->IS_NUMBER = inputToken->IS_NUMBER;
                stackTop->NODE_TYPE.L.TK->VALUE = inputToken->VALUE;

                pop(st);
                inputToken = getToken();
                continue;
            }
            else {

                syntaxErrorFlag = 1;

                if(inputToken->TOKEN_NAME != TK_ERR)
                    printf("Line %d : The token %s for the lexeme %s does not match with the expected token %s\n" ,inputToken->LINE_NO,getTerminal(inputToken->TOKEN_NAME),inputToken->LEXEME,getTerminal(stackTop->NODE_TYPE.L.ENUM_ID));




                if(inputToken->TOKEN_NAME == TK_ERR) {

                    stackTop->NODE_TYPE.L.TK = (Token*)malloc(sizeof(Token));
                    stackTop->NODE_TYPE.L.TK->LEXEME = inputToken->LEXEME;
                    stackTop->NODE_TYPE.L.TK->LINE_NO = inputToken->LINE_NO;
                    stackTop->NODE_TYPE.L.TK->TOKEN_NAME = stackTop->NODE_TYPE.L.ENUM_ID;
                    stackTop->NODE_TYPE.L.TK->IS_NUMBER = 0;
                    stackTop->NODE_TYPE.L.TK->VALUE = NULL;
                    inputToken = getToken();
                    pop(st);
                }
                else {
                    stackTop->NODE_TYPE.L.TK = (Token*)malloc(sizeof(Token));
                    stackTop->NODE_TYPE.L.TK->LEXEME = "ERROR_MISSED_LEXEME";
                    stackTop->NODE_TYPE.L.TK->LINE_NO = inputToken->LINE_NO;
                    stackTop->NODE_TYPE.L.TK->TOKEN_NAME = stackTop->NODE_TYPE.L.ENUM_ID;
                    stackTop->NODE_TYPE.L.TK->IS_NUMBER = 0;
                    stackTop->NODE_TYPE.L.TK->VALUE = NULL;
                    missedToken = inputToken;
                    pop(st);
                }


                continue;

            }
        }
        else {

            int ruleNumber = pTable->entries[stackTop->NODE_TYPE.NL.ENUM_ID][inputToken->TOKEN_NAME];

            if(ruleNumber != 0) {

                Rule* r = g->GRAMMAR_RULES[ruleNumber];
                addRuleToParseTree(stackTop,r);

                pop(st);

                NaryTreeNode* childNode = stackTop->NODE_TYPE.NL.child;

                if(childNode->IS_LEAF_NODE == 1 && childNode->NODE_TYPE.L.ENUM_ID == TK_EPS);
                else
                    pushTree(st,childNode);

            }
            else {
                syntaxErrorFlag = 1;

                if(inputToken->TOKEN_NAME == TK_ERR) {
                    inputToken = getToken();
                    continue;
                }

                if(fafl->FIRST[stackTop->NODE_TYPE.NL.ENUM_ID][TK_EPS] == 1) {
                    pop(st);
                    continue;
                }

                if(inputToken != missedToken)
                    printf("Line %d : The token %s for the lexeme %s does not match with the Non Terminal %s\n" ,inputToken->LINE_NO,getTerminal(inputToken->TOKEN_NAME),inputToken->LEXEME,getNonTerminal(stackTop->NODE_TYPE.NL.ENUM_ID));

                while(inputToken != NULL && fafl->FOLLOW[stackTop->NODE_TYPE.NL.ENUM_ID][inputToken->TOKEN_NAME] == 0) {
                    inputToken = getToken();
                }

                if(inputToken == NULL)
                    break;

                else {
                    pop(st);
                    continue;
                }

            }


        }
    }

    NaryTreeNode* stackTop = top(st);
    if(lexicalErrorFlag == 0 && syntaxErrorFlag == 0 && stackTop->IS_LEAF_NODE == 1 && stackTop->NODE_TYPE.L.ENUM_ID == TK_DOLLAR) {
        printf("\n \nSuccessfully Parsed the whole Input Source Code\n");
    }
    else {
        printf("\n \nParsing was unsuccesful\n");
    }

    close(f);

    return pt;
}


void print_SymbolList(SymbolList* ls) {
    Symbol* trav = ls->HEAD_SYMBOL;
    while(trav != NULL) {
        if(trav->IS_TERMINAL == 1)
            printf("%s " ,TerminalID[trav->TYPE.TERMINAL]);
        else
            printf("%s " ,NonTerminalID[trav->TYPE.NON_TERMINAL]);

        trav = trav->next;
    }
}

// prints the parse tree into a separate file
void print_ParseTree(ParseTree* pt, char* outfile) {

    FILE* f;

    if(outfile == NULL)
        f = stdout;
    else
        f = fopen(outfile,"wb");

    if(f == NULL) {
        printf("Error opening the outfile\n");
        return;
    }
    // printf("Testing 10");
    helper_function_print_parsetree(pt->root,f);

    if(f != stdout)
        fclose(f);

}

void print_Rule(Rule* r) {

    if(r == NULL) {
        printf("-------------------------\n");
        return;
    }
    printf("\n");
    SymbolList* ls = r->SYMBOLS;
    print_SymbolList(ls);

    printf("\n");
}


void print_NonTerminalRule_Records() {
    for(int i=0; i < num_nonterminals; i++) {
        NonTerminalRuleRecords* temp = nt_rule_records[i];
        printf("Rules for Non terminal %s start from %d and end at %d\n" ,NonTerminalID[i],temp->start,temp->end);
    }
}


void print_GrammarStructure() {
    for(int i=0; i < g->GRAMMAR_RULES_SIZE; i++) {
        Rule* r = g->GRAMMAR_RULES[i];
        print_Rule(r);
    }
}


void print_FirstSets(FirstAndFollow* fafl) {
    int** firstVector = fafl->FIRST;
    for(int i=0; i < num_nonterminals; i++) {
        printf("First set for non terminals %s are ====> " ,NonTerminalID[i]);

        for(int j=0; j < vectorSize; j++) {
            if(firstVector[i][j] == 1)
                printf("%s " , TerminalID[j]);
        }

        printf("\n");
    }
}



void print_ParseTable(ParsingTable* pt) {
    for(int i=0; i < num_nonterminals; i++) {
        printf("%s\n" ,NonTerminalID[i]);
        for(int j=0; j < num_terminals; j++) {
            for(int k=0; k < 10; k++)
                printf(" ");

            printf("%s ==> " ,TerminalID[j]);
            if(pt->entries[i][j] != 0) {
                print_Rule(g->GRAMMAR_RULES[pt->entries[i][j]]);
            }
            else
                printf("NULL\n");
        }
    }
}

void print_FollowSets(FirstAndFollow* fafl) {
    int** followVector = fafl->FOLLOW;
    for(int i=0; i < num_nonterminals; i++) {
        printf("Follow set for non terminals %s are ====> " ,NonTerminalID[i]);

        for(int j=0; j < vectorSize; j++) {
            if(followVector[i][j] == 1)
                printf("%s " , TerminalID[j]);
        }

        printf("\n");
    }
}

int getErrorStatus() {
    return (lexicalErrorFlag || syntaxErrorFlag);
}
