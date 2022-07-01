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
		int dType;
		int iValues;
		float fValues;
		char* cValues;
	} type;
}

%union {type all;}    

%left '+' '-'
%left '*' '/'
%left '(' ')'
%left UMINUS

    /* Yacc definitions */
%token display NEWLINE
%token <all> INTEGERS DECIMALS CHARACTER IDENTIFIER INT FLOAT CHAR STRING
%type <all> type expr term factor values str print printVals ID
%glr-parser
%%

/* descriptions of expected inputs corresponding actions (in C) */

/* main line */
program		:	commands														
			|	program commands												
			;

commands	:	num_statements
			|	let_statements
			|	print
			|	NEWLINE									{line++;}
			;

/* expected inputs for the variable declaration & initialization */
num_statements		:	ID ':' type						{checkVarDup($1.c, $3.c);}
					|	ID '=' expr						{checkVarExist($1.c, $3.i, $3.f, $3.c);}
					|	ID ':' type '=' expr			{
															checkVarDup($1.c, $3.c); 
															registThisVal($1.c ,$5.i, $5.f, $5.c); 
															updateVal($1.c ,$5.i, $5.f, $5.c);
														}

let_statements		:	ID ':' CHAR						{checkVarDup($1.c, $3.c);}
					|	ID '=' str						{checkVarExist($1.c, $3.i, $3.f, $3.c);}
					|	ID ':' CHAR '=' str				{
															checkVarDup($1.c, $3.c);
															registThisVal($1.c ,$5.i, $5.f, $5.c);
															updateVal($1.c ,$5.i, $5.f, $5.c);
														} 
					|	ID ':' CHAR '=' ID				{
															checkVarDup($1.c,$3.c); 
															char* cValues = getStringFromId($5.c);
															registThisVal($1.c ,$5.i, $5.f, cValues);
															updateVal($1.c ,$5.i, $5.f, cValues);
														}
					;


/* type can be either int , float or char */
type		:	INT										{$$.c = $1.c;}
			|	FLOAT									{$$.c = $1.c;}
			;

/* expected inputs for the print statement */
print		:	display ':' STRING         				{printValues($3.c);}
			|	display ':' STRING printVals 			{printStruct($3.c, $4.iNums, $4.fNums, $4.numbersLen, $4.stringsLen);}
			;

printVals	:	',' expr								{
															$$.iNums[$$.numbersLen] = $2.i; 
															$$.fNums[$$.numbersLen] = $2.f;
															$$.numbersLen++;
														}
			|	',' str   								{
															addStr($2.c, $$.stringsLen);
															$$.stringsLen++;
														}
			|	',' ID   								{ 
															checkVar($2.c);
															if($2.dType == 1){
																$$.iNums[$$.numbersLen] = $2.iValues;
																$$.numbersLen++;
															}
															else if($2.dType == 2){
																$$.fNums[$$.numbersLen] = $2.fValues;
																$$.numbersLen++;
															}
															if($2.dType == 3){
																addStr($2.cValues, $$.stringsLen);
																$$.stringsLen++;
															} 
														}															
			|	printVals ',' expr						{
															$$.iNums[$$.numbersLen] = $3.i;
															$$.fNums[$$.numbersLen] = $3.f;
															$$.numbersLen++;
														}
			|	printVals ',' str						{
															addStr($3.c, $$.stringsLen);
															$$.stringsLen++;
														}
			|	printVals ',' ID   						{ 
															checkVar($3.c);
															if($3.dType == 1){
																$$.iNums[$$.numbersLen] = $3.iValues;
																$$.numbersLen++;
															}
															else if($3.dType == 2){
																$$.fNums[$$.numbersLen] = $3.fValues;
																$$.numbersLen++;
															}
															if($3.dType == 3){
																addStr($3.cValues, $$.stringsLen);
																$$.stringsLen++;
															} 
														}
			;
			

/* expected inputs for the arithmetic statement */

expr    	:	term									{
															$$.i = $1.i;								
															$$.f = $1.f;
														}
       	    |	expr '+' term							{
															$$.i = $1.i + $3.i;						
															$$.f = $1.f + $3.f;
														}
			|	ID '+' factor							{
															$$.i = checkThisNumVar($1.c) + $3.i;		
															$$.f = checkThisNumVar($1.c) + $3.f;
														}
        	|	factor '+' ID							{
															$$.i = $1.i + checkThisNumVar($3.c);		
															$$.f = $1.f + checkThisNumVar($3.c);
														}
       	    |	expr '-' term							{
															$$.i = $1.i - $3.i;						
															$$.f = $1.f - $3.f;
														}
			|	ID '-' factor							{
															$$.i = checkThisNumVar($1.c) - $3.i;		
															$$.f = checkThisNumVar($1.c) - $3.f;
														}
        	|	factor '-' ID							{
															$$.i= $1.i - checkThisNumVar($3.c);		
															$$.f= $1.f - checkThisNumVar($3.c);
														}
       	    ;

term		:	factor									{
															$$.i = $1.i;								
															$$.f = $1.f;
														}
        	|	term '*' factor							{
															$$.i = $1.i * $3.i;								
															$$.f = $1.f * $3.f;
														}
        	|	term '/' factor							{
															$$.i = $1.i / $3.i;						
															$$.f = $1.f / $3.f;
														}
			|	ID '*' factor							{
															$$.i = checkThisNumVar($1.c) * $3.i;		
															$$.f = checkThisNumVar($1.c) * $3.f;
														}
        	|	factor '*' ID							{
															$$.i = $1.i * checkThisNumVar($3.c);		
															$$.f = $1.f * checkThisNumVar($3.c);
														}
        	|	ID '/' factor							{
															$$.i = checkThisNumVar($1.c) / $3.i;			
															$$.f = checkThisNumVar($1.c) / $3.f;
														}
        	|	factor '/' ID							{
															$$.i = $1.i / checkThisNumVar($3.c);		
															$$.f = $1.f / checkThisNumVar($3.c);
														}
        	;

factor		:	values									{
															$$.i = $1.i;
															$$.f = $1.f;
														}
			|	'(' expr ')'							{	$$.i = $2.i;
															$$.f = $2.f;
														}		
			|	'-' values  %prec UMINUS   				{	$$.i = -$2.i; 								
															$$.f = -$2.f;
														}
			;/* Unary minus oerator will have higher precedence*/

/* values can be either int or float or variable holding the value */
values		:	INTEGERS								{$$.i = $1.i;}
			|	DECIMALS								{$$.f = $1.f;}
			;

/* str can be either character or variable holding the value */
str			:	CHARACTER								{$$.c = $1.c;}
			|	STRING									{$$.c = $1.c;}
			;

ID			:	IDENTIFIER								{													
															$$.c = $1.c;
															if(getType($1.c)==1) {
																$$.iValues = checkThisNumVar($1.c);
																$$.dType = 1;
															} 	
															else if(getType($1.c)==2) {
																$$.fValues = checkThisNumVar($1.c);
																$$.dType = 2;
															}
															else if(getType($1.c)==3) {										
																$$.cValues = checkThisCharVar($1.c);
																$$.dType = 3;
															}
														}
%%                    

int main (void) {
	return yyparse();
}

void yyerror (const char *s) {
	fflush(stdout);
	fprintf(stderr, "\n>>>> ERROR LINE %d: %s <<<<<\n", yylineno, s);
}