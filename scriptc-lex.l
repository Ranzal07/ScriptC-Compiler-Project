%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scriptc.tab.h"

%}

%%
"display"                           {return display;}

"int"                               {yylval.s = strdup(yytext); return INT;}
"float"                             {yylval.s = strdup(yytext); return FLOAT;}
"%d"|"%"                           {yylval.s = strdup(yytext); return SPECIFIER;}
                             
[-]?[0-9]+                          {yylval.i = atoi(yytext); return INTEGERS;}
[-]?[0-9]+\.[0-9]+                  {yylval.f = atof(yytext); return DECIMALS;}       
[a-zA-Z_]+                          {yylval.s = strdup(yytext); return IDENTIFIER;}



[ \t\n]                             ;
[-+*/=":,]                          {return yytext[0];}
.                                   {ECHO; yyerror ("unexpected character");}

%%
int yywrap (void) {return 1;}