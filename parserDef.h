// #Group 9
// #Akshay Singhal								              2018B3A70635P
// #Archit Gupta                               2018B1A70149P
// #Punit Lohia                                2018B3A70766P
// #Saksham Pandey                             2018B3A70790P

#ifndef PARSE_DEF_
#define PARSE_DEF_

#include "lexerDef.h"
#include "NAryDef.h"
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum NonTerminal {
	program,
    	mainFunction,
	otherFunctions,
	function,
	input_par,
	output_par,
	parameter_list,
	dataType,
	primitiveDatatype,
	constructedDatatype,
	remaining_list,
	stmts,
	typeDefinitions,
	typeDefinition,
	fieldDefinitions,
	fieldDefinition,
	moreFields,
	declarations,
	declaration,
	global_or_not,
	otherStmts,
	stmt,
	assignmentStmt,
	singleOrRecId,
	funCallStmt,
	outputParameters,
	inputParameters,
	iterativeStmt,
	conditionalStmt,
	ioStmt,
	arithmeticExpression,
	expPrime,
	term,
	termPrime,
	factor,
	highPrecedenceOperators,
	lowPrecedenceOperators,
	booleanExpression,
    	var,
	logicalOp,
	relationalOp,
	returnStmt,
	optionalReturn,
	idList,
	more_ids,
	definetypestmt,
	recOrUnion
} NonTerminal;

typedef token_type Terminal;

typedef struct FirstAndFollow {
	int** FIRST;
 	int** FOLLOW;
} FirstAndFollow;

typedef union SymbolType {
    Terminal TERMINAL;
    NonTerminal NON_TERMINAL;
} SymbolType;

typedef struct Symbol {
    SymbolType TYPE;
	int IS_TERMINAL;
    struct Symbol* next;
} Symbol;

typedef struct SymbolList {
    Symbol* HEAD_SYMBOL;
	Symbol* TAIL_SYMBOL;
    int RULE_LENGTH;
} SymbolList;

typedef struct NonTerminalRuleRecords {
	int start;
	int end;
} NonTerminalRuleRecords;

typedef struct Rule {
    SymbolList* SYMBOLS;
    int RULE_NO;
} Rule;

 typedef struct Grammar {
    int GRAMMAR_RULES_SIZE;
    Rule** GRAMMAR_RULES;
} Grammar;

typedef struct ParsingTable {
	int** entries;
} ParsingTable;

#endif
