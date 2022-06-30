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

		int iNums[100];
		float fNums[100];
		int numbersLen;
		char* strings[100];
		int stringsLen;
	} type;
}

%union {type all;}    

%left '+' '-'
%left '*' '/'
%left '(' ')'
%left UMINUS

    /* Yacc definitions */
%token display NEWLINE
%token <all> INTEGERS DECIMALS CHARACTER NIDENTIFIER SIDENTIFIER INT FLOAT CHAR STRING
%type <all> type expr term factor values str print printVals

%%

/* descriptions of expected inputs corresponding actions (in C) */

/* main line */
program		:	commands														
			|	program commands												
			;

commands	:	num_statements
			|	let_statements
			|	print
			|	NEWLINE										{line++;}
			;

/* expected inputs for the variable declaration & initialization */
num_statements		:	NIDENTIFIER ':' type				{checkVarDup($1.c, $3.c);}
					|	NIDENTIFIER '=' expr				{checkVarExist($1.c, $3.i, $3.f, $3.c);}
					|	NIDENTIFIER ':' type '=' expr		{
															checkVarDup($1.c, $3.c); 
															registThisVal($1.c ,$5.i, $5.f, $5.c); 
															updateVal($1.c ,$5.i, $5.f, $5.c);
															}

let_statements		:	SIDENTIFIER ':' CHAR				{checkVarDup($1.c, $3.c);}
					|	SIDENTIFIER '=' str					{checkVarExist($1.c, $3.i, $3.f, $3.c);}
					|	SIDENTIFIER ':' CHAR '=' str		{
															checkVarDup($1.c, $3.c);
															registThisVal($1.c ,$5.i, $5.f, $5.c);
															updateVal($1.c ,$5.i, $5.f, $5.c);
															} 
					;


/* type can be either int , float or char */
type		:	INT												{$$.c = $1.c;}
			|	FLOAT											{$$.c = $1.c;}
			;

/* expected inputs for the print statement */
print		:	display ':' STRING         						{printValues($3.c);}
			|	display ':' STRING printVals 					{printStruct($3.c, $4.iNums, $4.fNums, $4.numbersLen, $4.stringsLen);}
			;

printVals	:	',' expr											{
																		$$.iNums[$$.numbersLen] = $2.i; 
																		$$.fNums[$$.numbersLen] = $2.f;
																		$$.numbersLen++;
																	}
			|	',' str   											{
																		addStr($2.c, $$.stringsLen);
																		$$.stringsLen++;
																	}
			|	printVals ',' expr									{
																		$$.iNums[$$.numbersLen] = $3.i;
																		$$.fNums[$$.numbersLen] = $3.f;
																		$$.numbersLen++;
																	}
			|	printVals ',' str									{
																		addStr($3.c, $$.stringsLen);
																		$$.stringsLen++;
																	}
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
values		:	NIDENTIFIER														{$$.i = checkThisNumVar($1.c); $$.f = checkThisNumVar($1.c);}
			|	INTEGERS														{$$.i = $1.i;}
			|	DECIMALS														{$$.f = $1.f;}
			;

/* str can be either character or variable holding the value */
str			:	SIDENTIFIER														{$$.c = checkThisCharVar($1.c);}
			|	CHARACTER														{$$.c = $1.c;}
			|	STRING															{$$.c = $1.c;}
			;

%%                    

int main (void) {
	return yyparse();
}

void yyerror (const char *s) {
	fflush(stdout);
	fprintf(stderr, "\n>>>> ERROR LINE %d: %s <<<<<\n", yylineno, s);
}