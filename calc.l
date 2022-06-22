%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calc.tab.h"

%}

%%
"display"			                    {return display;}

"int"                                   {return INT;}
"float"                                 {return FLOAT;}

"%d"|"%f"                               {yylval.s = strdup(yytext); return SPECIFIER;}
                             
     
[-]?[0-9]+                              {yylval.i = atoi(yytext); return INTEGERS;}
[-]?[0-9]+\.[0-9]+                      {yylval.f = atof(yytext); return DECIMALS;}       

[a-zA-Z][a-zA-Z0-9]+			        {yylval.s = strdup(yytext); return IDENTIFIER;}



[ \t\n]                                 ;
[-+*/=":,]           	                {return yytext[0];}
.                                       {ECHO; yyerror ("unexpected character");}

%%
int yywrap (void) {return 1;}