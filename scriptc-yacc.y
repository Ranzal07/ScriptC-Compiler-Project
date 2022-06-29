%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "scriptc-tools.c"

#define YYERROR_VERBOSE 1

extern int yylex();
extern void yyerror (const char *s);
extern yylineno;

%}
%union {int i; float f; char* s; char* c;}    

%left '+' '-'
%left '*' '/' '%'
%left '(' ')'
%right negative

    /* Yacc definitions */
%token EQUALS
%token <s> display IDENTIFIER <s> NUM_SPECIFIER NEWLINE INT CHAR FLOAT <i> INTEGERS <f> DECIMALS <c> CHARACTER LET_SPECIFIER
%type <f> expr term factor values 
%type <s> type str
%%

/* descriptions of expected inputs corresponding actions (in C) */

/* main line */
program		:	commands														
			|	program commands												
			;

commands	:	numVar_statements
			|	letVar_statements
			|	numPrint_statements
			|	letPrint_statements
			|	NEWLINE															{line++;}
			;

/* expected inputs for the variable declaration & initialization */
numVar_statements	:	IDENTIFIER ':' type										{checkVarDup($1,$3);}
					|	IDENTIFIER '=' expr										{checkNumVarExist($1,$3);}
					|	IDENTIFIER ':' type '=' expr							{checkVarDup($1,$3); saveThisNumVal($1,$5); updateNumVal($1,$5);}
					;

letVar_statements	:	IDENTIFIER ':' CHAR										{checkVarDup($1,$3);}
					|	IDENTIFIER EQUALS str									{checkCharVarExist($1,$3);}
					|	IDENTIFIER ':' CHAR '=' str								{checkVarDup($1,$3); saveThisCharVal($1,$5); updateCharVal($1,$5);}
					;

/* type can be either INT or FLOAT */
type		:	INT																{$$ = $1;}
			|	FLOAT															{$$ = $1;}
			;

/* expected inputs for the print statement */
numPrint_statements		:	display ':' '"' NUM_SPECIFIER '"' ',' expr							{oneNumValPrint($4,$7);}
						|	display ':' '"' NUM_SPECIFIER NUM_SPECIFIER '"' ',' expr ',' expr	{twoNumValPrint($4,$5,$8,$10);}
						;

letPrint_statements		:	display ':' '"' LET_SPECIFIER '"' ',' str							{oneCharValPrint($4,$7);}
						|	display ':' '"' LET_SPECIFIER LET_SPECIFIER '"' ',' str	',' str		{twoCharValPrint($4,$5,$8,$10);}
						|	display ':' '"' NUM_SPECIFIER LET_SPECIFIER '"' ',' expr ',' str	{NumCharValPrint($4,$5,$8,$10);}
						|	display ':' '"' LET_SPECIFIER NUM_SPECIFIER '"' ',' str	',' expr	{CharNumValPrint($4,$5,$8,$10);}
						;

/* expected inputs for the arithmetic statement */
expr    	:	term															{$$ = $1;}
       	    |	expr '+' term													{$$ = $1 + $3;}
       	    |	expr '-' term													{$$ = $1 - $3;}
       	    ;

term		:	factor															{$$ = $1;}
        	|	term '*' factor													{$$ = $1 * $3;}		
        	|	term '/' factor													{$$ = $1 / $3;}
        	;

factor		:	values															{$$ = $1;}
			|	'(' expr ')'													{$$ = $2;}		
			|	'-' values  %prec negative    									{$$ = -$2;}		//		accepting negative numbers
			;

/* values can be either int or float or variable holding the value */
values		:	IDENTIFIER														{$$ = checkThisNumVar($1);}
			|	INTEGERS														{$$ = $1;}
			|	DECIMALS														{$$ = $1;}
			;

/* str can be either character or variable holding the value */
str			:	IDENTIFIER														{$$ = checkThisCharVar($1);}
			|	CHARACTER														{$$ = $1;}
			;

%%                    

int main (void) {
	return yyparse();
}

void yyerror (const char *s) {
	fflush(stdout);
	fprintf(stderr, "\n>>>> ERROR LINE %d: %s <<<<<\n", yylineno, s);
}