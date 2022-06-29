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

%code requires{
	typedef struct types{
		int i;
		float f;
		char* c;
	}type;
}

%union {type all;}    

%left '+' '-'
%left '*' '/'
%left '(' ')'
%left UMINUS

    /* Yacc definitions */
%token display NEWLINE
%token <all> INTEGERS DECIMALS CHARACTER IDENTIFIER NUM_SPECIFIER LET_SPECIFIER INT FLOAT CHAR 
%type <all> type expression iExpr iTerm iFactor iValues fExpr fTerm fFactor fValues str

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
					|	IDENTIFIER '=' expression		{checkVarExist($1.c, $3.i, $3.f, $3.c);}
					|	IDENTIFIER ':' type '=' expression		{checkVarDup($1.c, $3.c); 
																saveThisVal($1.c ,$5.i, $5.f, $5.c); 
																updateVal($1.c ,$5.i, $5.f, $5.c);}
					;

expression		:	iExpr
				|	fExpr
				|	str
				;

/* type can be either int , float or char */
type		:	INT																{$$ = $1;}
			|	FLOAT															{$$ = $1;}
			|	CHAR															{$$ = $1;}
			;

/* expected inputs for the print statement */
print_num		:	display ':' '"' NUM_SPECIFIER '"' ',' iExpr							{oneNumValPrint($4.c,$7.i,$7.f);}
				|	display ':' '"' NUM_SPECIFIER '"' ',' fExpr							{oneNumValPrint($4.c,$7.i,$7.f);}
				|	display ':' '"' NUM_SPECIFIER NUM_SPECIFIER '"' ',' iExpr ',' iExpr	{twoNumValPrint($4.c,$5.c,$8.i,$8.f,$10.i,$10.f);}
				|	display ':' '"' NUM_SPECIFIER NUM_SPECIFIER '"' ',' fExpr ',' fExpr	{twoNumValPrint($4.c,$5.c,$8.i,$8.f,$10.i,$10.f);}
				|	display ':' '"' NUM_SPECIFIER NUM_SPECIFIER '"' ',' iExpr ',' fExpr	{twoNumValPrint($4.c,$5.c,$8.i,$8.f,$10.i,$10.f);}
				|	display ':' '"' NUM_SPECIFIER NUM_SPECIFIER '"' ',' fExpr ',' iExpr	{twoNumValPrint($4.c,$5.c,$8.i,$8.f,$10.i,$10.f);}
				;

print_let		:	display ':' '"' LET_SPECIFIER '"' ',' str							{oneCharValPrint($4.c,$7.c);}
				|	display ':' '"' LET_SPECIFIER LET_SPECIFIER '"' ',' str	',' str		{twoCharValPrint($4.c,$5.c,$8.c,$10.c);}
				|	display ':' '"' NUM_SPECIFIER LET_SPECIFIER '"' ',' iExpr ',' str	{NumCharValPrint($4.c,$5.c,$8.i,$8.f,$10.c);}
				|	display ':' '"' LET_SPECIFIER NUM_SPECIFIER '"' ',' str	',' iExpr	{CharNumValPrint($4.c,$5.c,$8.c,$10.i,$10.f);}
				|	display ':' '"' NUM_SPECIFIER LET_SPECIFIER '"' ',' fExpr ',' str	{NumCharValPrint($4.c,$5.c,$8.i,$8.f,$10.c);}
				|	display ':' '"' LET_SPECIFIER NUM_SPECIFIER '"' ',' str	',' fExpr	{CharNumValPrint($4.c,$5.c,$8.c,$10.i,$10.f);}
				;

/*---------------INT TYPE---------------*/
iExpr    	:	iTerm															{$$.i = $1.i;}
       	    |	iExpr '+' iTerm													{$$.i = $1.i + $3.i;}
       	    |	iExpr '-' iTerm													{$$.i = $1.i - $3.i;}
       	    ;

iTerm		:	iFactor															{$$.i = $1.i;}
        	|	iTerm '*' iFactor												{$$.i = $1.i * $3.i;}		
        	|	iTerm '/' iFactor												{$$.i = $1.i / $3.i;}
        	;

iFactor		:	iValues															{$$.i = $1.i;}
			|	'(' iExpr ')'													{$$.i = $2.i;}		
			|	'-' iValues  %prec UMINUS  /* Unary minus oerator will have higher precedence*/ 	{$$.i = -$2.i;}
			;

iValues		:	IDENTIFIER														{$$.i = checkThisVar($1.c);}
			|	INTEGERS														{$$.i = $1.i;}
			;

/*---------------FLOAT TYPE---------------*/

fExpr    	:	fTerm															{$$.f = $1.f;}
       	    |	fExpr '+' fTerm													{$$.f = $1.f + $3.f;}
       	    |	fExpr '-' fTerm													{$$.f = $1.f - $3.f;}
       	    ;

fTerm		:	fFactor															{$$.f = $1.f;}
        	|	fTerm '*' fFactor												{$$.f = $1.f * $3.f;}		
        	|	fTerm '/' fFactor												{$$.f = $1.f / $3.f;}
        	;

fFactor		:	fValues															{$$.f = $1.f;}
			|	'(' fExpr ')'													{$$.f = $2.f;}		
			|	'-' fValues  %prec UMINUS  /* Unary minus oerator will have higher precedence*/ 	{$$.f = -$2.f;}
			;

fValues		:	IDENTIFIER														{$$.f = checkThisVar($1.c);}
			|	DECIMALS														{$$.f = $1.f;}
			;


/*---------------CHAR TYPE---------------*/
str			:	IDENTIFIER														{$$ = checkThisVar($1.c);}
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