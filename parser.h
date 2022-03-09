// #Group 9
// #Akshay Singhal								              2018B3A70635P
// #Archit Gupta                               2018B1A70149P
// #Punit Lohia                                2018B3A70766P
// #Saksham Pandey                             2018B3A70790P

#include "parserDef.h"

int grammar_initialisation();
Grammar* obtaining_grammar();
FirstAndFollow* compute_firstandfollow_sets (Grammar* g);
void fill_firstset(int** firstVector, Grammar* g);
ParsingTable* parsing_table_initialisation();
void ParseTable_creation(FirstAndFollow* fafl, ParsingTable* pt);
ParseTree* source_code_parsing(char *testcaseFile, ParsingTable* pTable,FirstAndFollow* fafl);
void print_ParseTree(ParseTree* pt, char* outfile);
void helper_function_print_parsetree(NaryTreeNode * pt, FILE* f);
void calcuating_firstset(int** firstVector, int enum_id);
void fill_followset(int** followBitVector, int ** firstSet, Grammar* g);
void followset_filling_to_stable(int** followVector, int** firstVector, Grammar* g);


void CheckIfDone_initialisation();

int findInNonTerminalMap(char* str);
int findInTerminalMap(char* str);

NonTerminalRuleRecords** nonterminal_records_initialisation();
Rule* rule_initialisation(SymbolList* sl, int ruleCount);

Symbol* symbol_initialisation(char* symbol);
void SymbolList_append(SymbolList* ls, Symbol* s);
SymbolList* symbol_list_initialisation();


char* symbol_append(char* str, char c);
char* lexeme_copy(char* str);
char* getTerminal(int enumId);
char* getNonTerminal(int enumId);

void print_Rule(Rule* r);
void print_GrammarStructure();
void print_NonTerminalRule_Records();
void print_FirstSets(FirstAndFollow* fafl);
void printSymbol(Symbol* ls);
void print_FollowSets(FirstAndFollow* fafl);
void print_ParseTable(ParsingTable* pt);
int getErrorStatus();
