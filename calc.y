%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


extern int yylex();
void yyerror (char *strError);	
int line=1;
int var_inc=0;
int stat_inc=0;

typedef struct indentifiers{
	char var[100];
	char typ[100];
	int ival;
	float fval;
} identifier;

char stat_storage[100];
float symbols[1000];
identifier id[100];

void checkNumType(char* symbol);
void saveDecVar(char* symbol, char* type, float flo_val);
void checkDupVar(char* symbol, char* type);
void checkDupVar2(char* symbol, char* type, float flo_val);
void checkDecVar(char* symbol, float flo_val);

float getFloVal(char* symbol);

void storeFloVal(char* symbol, char* type, float flo_val);
void oneValPrint(char* specifier, float symbol);
void twoValPrint(char* specifier, char* specifier2, float symbol, float symbol2);
%}

%union {int i; float f; char* s;}     
    /* Yacc definitions */
%token display <s> SPECIFIER INT FLOAT <s> IDENTIFIER <i> INTEGERS <f> DECIMALS
%type <f> expr term
%type <s> type
%%

/* descriptions of expected inputs     corresponding actions (in C) */

/* main line */
program     : statements {line++;};
			| program statements {line++;};
			;

statements  : IDENTIFIER ':' type                                               {checkDupVar($1,$3);}
			| IDENTIFIER '=' expr                                  			    {checkDecVar($1,$3); checkNumType($1);}
			| IDENTIFIER ':' type '=' expr                                  	{checkDupVar2($1,$3,$5);}
			| print
			;

type		: INT 	{$$ = $1;}
			| FLOAT {$$ = $1;}
			;

print		: display ':' '"' INTEGERS '"'										{printf("%d",$4);}
			| display ':' '"' DECIMALS '"'										{printf("%f",$4);}
			| display ':' '"' SPECIFIER '"' ',' expr							{oneValPrint($4,$7);}
			| display ':' '"' SPECIFIER SPECIFIER '"' ',' expr ',' expr			{twoValPrint($4,$5,$8,$10);}
			;

/* int_exp can be either int_term or arithmetic expression*/
expr    	: term 																	{$$ = $1;}
       	    | expr '+' term                                                		  {$$ = $1 + $3;}
       	    | expr '-' term                                               		 {$$ = $1 - $3;}
            | expr '*' term                                                		{$$ = $1 * $3;}
       	    | expr '/' ter                                               			 {$$ = $1 / $3;}
       	    ;

term		: IDENTIFIER                                                        {$$ = getFloVal($1);}
			| INTEGERS                                               		    {$$=$1;}
			| DECIMALS                                               		    {$$=$1;}
			;

/* float_exp can be either float_term or arithmetic expression*/

/* int_term can be either digits or indentifier's int values*/


/* float_term can be either digits or indentifier's float values*/     


%%                    
/* C code */

/* returns int value to the identifier */

void checkDupVar(char* symbol, char* type){
	int i;
	int flag=0;
	for(i=0;i<var_inc;i++){
		if(strcmp(id[i].var,symbol)==0){
			flag=1;
			break;
		}
	}
	if(flag==1){
		printf("\nERROR LINE %d: '%s' already declared!",line,symbol);
		stat_inc++;
	}
	else{
		printf("\nLINE %d: Correct Variable Declaration!",line);
		stat_inc++;
	}
}

void checkDupVar2(char* symbol, char* type, float flo_val){
	int i;
	int flag=0;
	for(i=0;i<var_inc;i++){
		if(strcmp(id[i].var,symbol)==0){
			flag=1;
			break;
		}
	}
	if(flag==1){
		printf("\nERROR LINE %d: '%s' already declared!",line,symbol);
		stat_inc++;
	}
	else{
		printf("\nLINE %d: Correct Variable Declaration!",line);
		stat_inc++;
		saveDecVar(symbol,type,flo_val);
		storeFloVal(symbol,type,flo_val);
	}
}



void checkNumType(char* symbol)
{
	int i;
	int flag=0;
	for(i=0;i<var_inc;i++){
		if(strcmp(id[i].var,symbol)==0 && strcmp(id[i].typ,"int")==0){
			flag=1;
			break;
		}
		else if(strcmp(id[i].var,symbol)==0 && strcmp(id[i].typ,"float")==0){
			flag=2;
			break;
		}
	}
	if(flag==1){
		/*sprintf(stat_storage[stat_inc], "\nERROR LINE %d: '%s' cannot convert into float!",line,symbol);*/
		stat_inc++;
	}
	else if(flag==2){
		/*sprintf(stat_storage[stat_inc], "\nERROR LINE %d: '%s' cannot convert into int!",line,symbol);*/
		stat_inc++;
	}
	else{
		/*sprintf(stat_storage[stat_inc], "\nLINE %d: Correct Initialization!",line);*/
		stat_inc++;
	}
}

void oneValPrint(char* specifier, float symbol){
	int num;
	char spec[5];
	strcpy(spec,specifier);
	if(strcmp(spec,"%d")==0){
		num=symbol;
		printf("\n%d",num);
	}
	else{
		printf("\n%g",symbol);
	}
}

void twoValPrint(char* specifier, char* specifier2, float symbol, float symbol2){
	int num;
	int num2;
	char spec[5];
	char spec2[5];
	strcpy(spec,specifier);
	strcpy(spec2,specifier2);
	if(strcmp(spec,"%d")==0){
		if(strcmp(spec2,"%d")==0){
			num=symbol;
			num2=symbol2;
			printf("\n%d%d",num,num2);
		}
		else{
			num=symbol;
			printf("\n%d%g",num,symbol2);
		}
	}
	else if(strcmp(spec,"%f")==0){
		if(strcmp(spec2,"%f")==0){
			printf("\n%g%g",symbol,symbol2);
		}
		else{
			num2=symbol2;
			printf("\n%g%d",symbol,num2);
		}
	}
}


void saveDecVar(char* symbol, char* type, float flo_val){
	int to_int;
	char sym[100];
	char vtype[10];
	strcpy(sym,symbol);
	strcpy(vtype,type);
	if(strcmp(vtype,"int")==0){
		to_int = flo_val;
		strcpy(id[var_inc].var, sym);
		strcpy(id[var_inc].typ, vtype);
		id[var_inc].ival = to_int;
		var_inc++;
	}
	else if(strcmp(vtype,"int")==0){
		strcpy(id[var_inc].var, sym);
		strcpy(id[var_inc].typ, vtype);
		id[var_inc].fval = flo_val;
		var_inc++;
	}
}

void checkDecVar(char* symbol, float flo_val){
	int i;
	int flag = 0;
	for(i=0;i<var_inc;i++){
		if(strcmp(id[i].var,symbol)==0){
			flag = 1;
			break;
		}
	}
	if(flag==1){
		printf("\nLINE %d: Correct Variable Initialization!",line);
		stat_inc++;
		
	} else {
		printf("\nERROR LINE %d: '%s' undeclared!",line,symbol);
		stat_inc++;
	}
}

int getCurTok(char* token)
{
	int idx = -1;
    idx = *token;
    return idx;
} 


/* returns float value to the identifier */
float getFloVal(char* symbol)
{	
	int bucket = getCurTok(symbol);
	return symbols[bucket];
}

/* stores the int value into the identifier */


/* stores the float value into the identifier */
void storeFloVal(char* symbol, char* type, float flo_val)
{
	int to_int;
	char vtype[10];
	strcpy(vtype,type);
	int bucket = getCurTok(symbol);
	symbols[bucket] = flo_val;

}

/* stores the string value into the identifier */
void stat_print(){
	int i;
	for(i=0;i<stat_inc;i++){
		printf("%s",stat_storage[i]);
	}
}

int main (void) {
	/* init symbol table */
	int i;
	for(i=0; i<52; i++) {
		symbols[i] = 0;
	}
	return yyparse();
}

void yyerror (char *s) {fprintf (stderr, "%s\n", s);} 