%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



#define YYERROR_VERBOSE 1

extern int yylex();
extern void yyerror (const char *s);
extern yylineno;

%}

%code requires{
	#include "scriptc-tools.c"
}

%union {type all;}    

%left '+' '-'
%left '*' '/'
%left '(' ')'
%left UMINUS

    /* Yacc definitions */
%token display NEWLINE EQUALS
%token <all> INTEGERS DECIMALS CHARACTER IDENTIFIER NUM_SPECIFIER LET_SPECIFIER INT FLOAT CHAR 
%type <all> type expr term factor values str

%%

/* descriptions of expected inputs corresponding actions (in C) */

/* main line */
program		:	commands														
			|	program commands												
			;

commands	:	var_statements
			|	print_num
			|	print_let
			|	NEWLINE															{line++;}
			;

/* expected inputs for the variable declaration & initialization */
var_statements		:	IDENTIFIER ':' type				{checkVarDup($1.c, $3.c);}
					|	IDENTIFIER '=' expr		{checkVarExist($1.c, $3.i, $3.f, $3.c);}
					|	IDENTIFIER ':' type '=' expr		{checkVarDup($1.c, $3.c); 
															saveThisVal($1.c ,$5.i, $5.f, $5.c); 
															updateVal($1.c ,$5.i, $5.f, $5.c);}
					|	IDENTIFIER ':' CHAR				{checkVarDup($1.c, $3.c);}
					|	IDENTIFIER EQUALS str		{checkVarExist($1.c, $3.i, $3.f, $3.c);}
					|	IDENTIFIER ':' CHAR '=' str		{checkVarDup($1.c, $3.c);} 
					;


/* type can be either int , float or char */
type		:	INT																{$$.c = $1.c;}
			|	FLOAT															{$$.c = $1.c;}
			;

/* expected inputs for the print statement */
print_num		:	display ':' '"' NUM_SPECIFIER '"' ',' expr							{oneNumValPrint($4.c,$7.i,$7.f);}
				|	display ':' '"' NUM_SPECIFIER NUM_SPECIFIER '"' ',' expr ',' expr	{twoNumValPrint($4.c,$5.c,$8.i,$8.f,$10.i,$10.f);}
				;	
				
print_let		:	display ':' '"' LET_SPECIFIER '"' ',' str							{oneCharValPrint($4.c,$7.c);}
				|	display ':' '"' LET_SPECIFIER LET_SPECIFIER '"' ',' str	',' str		{twoCharValPrint($4.c,$5.c,$8.c,$10.c);}
				|	display ':' '"' NUM_SPECIFIER LET_SPECIFIER '"' ',' expr ',' str	{NumCharValPrint($4.c,$5.c,$8.i,$8.f,$10.c);}
				|	display ':' '"' LET_SPECIFIER NUM_SPECIFIER '"' ',' str	',' expr	{CharNumValPrint($4.c,$5.c,$8.c,$10.i,$10.f);}
				;

/* expected inputs for the arithmetic statement */
expr    	:	term															{$$.i = $1.i;			$$.f = $1.f;}
       	    |	expr '+' term													{$$.i = $1.i + $3.i;	$$.f = $1.f + $3.f;}
       	    |	expr '-' term													{$$.i = $1.i - $3.i;	$$.f = $1.f - $3.f;}
       	    ;

term		:	factor															{$$.i = $1.i;			$$.f = $1.f;}
        	|	term '*' factor													{$$.i = $1.i * $3.i;	$$.f = $1.f * $3.f;}		
        	|	term '/' factor													{$$.i = $1.i / $3.i;	$$.f = $1.f / $3.f;}
        	;

factor		:	values															{$$.i = $1.i;			$$.f = $1.f;}
			|	'(' expr ')'													{$$.i = $2.i;			$$.f = $2.f;}		
			|	'-' values  %prec UMINUS  /* Unary minus oerator will have higher precedence*/ 	{$$.i = -$2.i; 			$$.f = -$2.f;}
			;

/* values can be either int or float or variable holding the value */
values		:	IDENTIFIER														{$$.i = checkThisVar($1.c); $$.f = checkThisVar($1.c);}
			|	INTEGERS														{$$.i = $1.i;}
			|	DECIMALS														{$$.f = $1.f;}
			;

/* str can be either character or variable holding the value */
str			:	IDENTIFIER														{$$.c = checkThisVar($1.c);}
			|	CHARACTER														{$$.c = $1.c;}
			;

%%                    

int main (void) {
	return yyparse();
}

void yyerror (const char *s) {
	fflush(stdout);
	fprintf(stderr, "\n>>>> ERROR LINE %d: %s <<<<<\n", yylineno, s);
}