%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern int yylex();
void yyerror (char *strError);	

int int_inc=0;
char typ[10]="nothing";
char symbols[1000];
int typ_inc = 0;
int val_inc = 0;
struct{
	char* var_typ[100][52];
	char* var_val[100][52];
} sym;
int intVal(char* symbol);
float floVal(char* symbol);
char storeIntTyp(char* symbol, char* int_type);
void storeIntVal(char* symbol, int int_val);
void storeFloVal(char* symbol, float flo_val);
%}

%union {int i; float f; char* s; char* var_type;}         /* Yacc definitions */
%start main
%token display int_spec
%token <i> dig
%token <s> identifier
%token <var_type> INT
%type <var_type> TYPE
%type <i> int_exp int_term 
%type <s> main assignment print

%%

/* descriptions of expected inputs     corresponding actions (in C) */

/* main line */
main        : assignment		                                                  {;}
            | print	                                                              {;}
            | main assignment	                                                  {;}
		    | main print	                                                      {;}
            ;

/* expected inputs for variable declaration */
assignment  : identifier ':' TYPE                                                 {storeIntTyp($1,$3);}
            | identifier '=' int_exp                                              {storeIntVal($1,$3);}
            | identifier ':' TYPE '=' int_exp                                     {storeIntVal($1,$5);}
			;

TYPE		: INT	{$$ = "int";}
			;

/* expected inputs for print statement */
print       : display ':' dig													  {printf("%d\n", $3);}
			| display ':' '"' int_spec '"' ',' int_exp		                      {printf("%d\n", $7);}
			| display ':' '"' int_spec int_spec	'"' ',' int_exp ',' int_exp       {printf("%d%d\n", $8, $10);}
  		    ;

/* int_exp can be either int_term or arithmetic expression*/
int_exp    	: int_term                                                            {$$ = $1;}
       	    | int_exp '+' int_term                                                {$$ = $1 + $3;}
       	    | int_exp '-' int_term                                                {$$ = $1 - $3;}
            | int_exp '*' int_term                                                {$$ = $1 * $3;}
       	    | int_exp '/' int_term                                                {$$ = $1 / $3;}
       	    ;

/* float_exp can be either float_term or arithmetic expression*/

/* int_term can be either digits or indentifier's int values*/
int_term    : dig                                                                 {$$ = $1;}
            | identifier 			                                              {$$ = intVal($1);} 
            ;

/* float_term can be either digits or indentifier's float values*/            

%%                    
/* C code */

/* returns int value to the identifier */

char storeIntTyp(char* symbol, char* int_type){
	int bucket = *symbol;
	strcpy(sym.var_typ[int_inc],symbols[bucket]);
	printf("%s",sym.var_typ[int_inc]);
	return symbols[bucket];
}

int intVal(char* symbol)
{
	int bucket = *symbol;
	return symbols[bucket];
}

/* returns float value to the identifier */

/* stores the int value into the identifier */
void storeIntVal(char* symbol, int int_val)
{
	int bucket = *symbol;
	symbols[bucket] = int_val;
}

/* stores the float value into the identifier */


/* stores the string value into the identifier */


int main (void) {
	/* init symbol table */
	int i;
	for(i=0; i<52; i++) {
		symbols[i] = 0;
	}
	return yyparse();
}

void yyerror (char *s) {fprintf (stderr, "%s\n", s);} 