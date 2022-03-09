// #Group 9
// #Akshay Singhal								              2018B3A70635P
// #Archit Gupta                               2018B1A70149P
// #Punit Lohia                                2018B3A70766P
// #Saksham Pandey                             2018B3A70790P


#include "hashtable.h"
#include "lexer.h"
// #include "time.c"
#include "parser.h"
#include "NAry.h"
#include <time.h>

int main(int argc, char* argv[]) {
    int Choice;

    if(argc != 3) {
        printf("Number of arguments are not sufficient \n");
        return 0;
    }

    printf("\n\n");
    printf("=========================Compiler Project : Group 9\n");

    printf("Enter 0 to exit\n");
    printf("Enter 1 to remove comments from the source code\n");
    printf("Enter 2 to print the list of tokens created by the lexer\n");
    printf("Enter 3 to print the parse tree\n");
    printf("Enter 4 to print the time taken by the CPU to parse the source code\n");

    while(1) {

        scanf("%d", &Choice);

        switch(Choice) {
                case 0: {
                    return 0;
                    break;
                }
			case 1 :{
			printf("Removing comments from the source code\n");
			printf("The clean code is as below :\n");
			remove_comments(argv[1],argv[1]);
			printf("\nRemoval of comments was successfull\n");
			break;
			}
                case 2: {
                    printf(" Printing the token list along with the lexemes and line numbers: \n");
                    printf("========\n");
                    int f = open(argv[1],O_RDONLY);
                    lexer_initialisation(f);
                    Token* t;
                    while((t = getToken()) != NULL) {
                        printf("%s\t\t%s\t\tLine No. %d \n", getTerminal(t->TOKEN_NAME),t->LEXEME,t->LINE_NO);
                    }
                    close(f);

                    printf("\nFinished printing\n");
                    break;
                }
                case 3: {
                    printf("Parsing the input source code: \n");

                    printf("==========\n");
                    Grammar* g = obtaining_grammar();
                    FirstAndFollow* fafl = compute_firstandfollow_sets(g);
                    ParsingTable* pTable = parsing_table_initialisation();
                    ParseTable_creation(fafl,pTable);
                    ParseTree* pt = source_code_parsing(argv[1],pTable,fafl);
                    print_ParseTree(pt,argv[2]);
                    printf("\nFinished parsing \n");
                    break;
                }

			case 4 :{
			clock_t start_time, end_time;
			double total_CPU_time, total_CPU_time_in_seconds;
			start_time = clock();

			int f = open(argv[1],O_RDONLY);
                    lexer_initialisation(f);
                    Token* t;
                    while((t = getToken()) != NULL) {
                        printf("%s        %s        %d \n", getTerminal(t->TOKEN_NAME),t->LEXEME,t->LINE_NO);
                    }
                    close(f);

			Grammar* g = obtaining_grammar();
                    FirstAndFollow* fafl = compute_firstandfollow_sets(g);
                    ParsingTable* pTable = parsing_table_initialisation();
                    ParseTable_creation(fafl,pTable);
                    // // Lexer and parser are both invoked inside source_code_parsing
                    ParseTree* pt = source_code_parsing(argv[1],pTable,fafl);
                    print_ParseTree(pt,argv[2]);

			end_time = clock();

			total_CPU_time = (double) (end_time - start_time);
			total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;

			printf("\n Printing the time taken by the CPU\n");
			printf("\n The total_CPU_time is :%f\n",total_CPU_time);
			printf("\n The total_CPU_time_in_seconds is :%f\n ",total_CPU_time_in_seconds);
			break;
			}
                default: {
                    printf("The given input number is not correct.\nPlease Try Again\n");
                    continue;
                }

        }

    }

}
