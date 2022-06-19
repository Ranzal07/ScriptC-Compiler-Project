%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calc.tab.h"

%}
%%
"display"			                    {return display;}
int                                     {yylval.var_type=strdup(yytext);return INT;}


"%d"                                    {yylval.i = yytext[0]; return int_spec;}
     

[-]?[0-9]+                              {yylval.i = atoi(yytext); return dig;}

[a-zA-Z][a-zA-Z0-9_]+			        {yylval.s=strdup(yytext); return identifier;}


[ \t\n]                                 ;
[-+*/=":,]           	                {return yytext[0];}
.                                       {ECHO; yyerror ("unexpected character");}

%%
int yywrap (void) {return 1;}