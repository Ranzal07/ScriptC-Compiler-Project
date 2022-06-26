%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "scriptc-tools.c"

extern int yylex();
void yyerror (char *strError);	

%}
%union {int i; float f; char* s;}     

    /* Yacc definitions */
%token <s> display IDENTIFIER SPECIFIER NEWLINE INT FLOAT <i> INTEGERS <f> DECIMALS
%type <f> expr term factor values
%type <s> type
%%

/* descriptions of expected inputs corresponding actions (in C) */

/* main line */
program		:	statements														
			|	program statements												
			;

/* expected inputs for the variable declaration & initialization */
statements	:	IDENTIFIER ':' type												{checkVarDup($1,$3);}
			|	IDENTIFIER '=' expr												{checkVarExist($1,$3);}
			|	IDENTIFIER ':' type '=' expr									{checkVarDup($1,$3); checkVarExist($1,$5);}
			|	print
			|	NEWLINE															{line++;}
			;

/* type can be either INT or FLOAT */
type		:	INT																{$$ = $1;}
			|	FLOAT															{$$ = $1;}
			;

/* expected inputs for the print statement */
print		:	display ':' '"' INTEGERS '"'									{printf("%d",$4);}
			|	display ':' '"' DECIMALS '"'									{printf("%f",$4);}
			|	display ':' '"' SPECIFIER '"' ',' expr							{oneValPrint($4,$7);}
			|	display ':' '"' SPECIFIER SPECIFIER '"' ',' expr ',' expr		{twoValPrint($4,$5,$8,$10);}
			;

/* expected inputs for the arithmetic statement */
expr    	:	term															{$$ = $1;}
       	    |	expr '+' term													{$$ = $1 + $3;}
       	    |	expr '-' term													{$$ = $1 - $3;}
       	    ;

term		: factor															{$$ = $1;}
        	| term '*' factor													{$$ = $1 * $3;}		
        	| term '/' factor													{$$ = $1 / $3;}
        	;

factor		: values															{$$ = $1;}
			| '(' expr ')'														{$$ = $2;}		
			;

/* term can be either int or float or variable holding the value */
values		:	IDENTIFIER														{$$ = checkThisVar($1);}
			|	INTEGERS														{$$=$1;}
			|	DECIMALS														{$$=$1;}
			;

%%                    

int main (void) {
	return yyparse();
}
void yyerror (char *s) {fprintf (stderr, "%s\n", s);}