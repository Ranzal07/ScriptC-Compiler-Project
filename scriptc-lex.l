%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scriptc-yacc.tab.h"
%}

%option yylineno

%%
"display"                           {return display;}

"int"                               {yylval.s = strdup(yytext); return INT;}
"float"                             {yylval.s = strdup(yytext); return FLOAT;}
"char"                              {yylval.s = strdup(yytext); return CHAR;}
%d|%f                               {yylval.s = strdup(yytext); return NUM_SPECIFIER;}
%c|%s                               {yylval.c = strdup(yytext); return LET_SPECIFIER;}
                             
[-]?[0-9]+                          {yylval.i = atof(yytext); return INTEGERS;}
[-]?[0-9]+\.[0-9]+                  {yylval.f = atof(yytext); return DECIMALS;}       
[a-zA-Z_]+                          {yylval.s = strdup(yytext); return IDENTIFIER;}

\'.*\'                              {
                                          yylval.c = strdup(yytext+1);
                                          if (yylval.c[yyleng-2] != '\'')
                                              yyerror("improperly terminated string");
                                          else
                                              yylval.c[yyleng-2] = 0;
                                          return CHARACTER;
                                    }

\n                                  {return NEWLINE;}


[ \t\n]                             ;
[-+*/=":,()]                        {return yytext[0];}
.                                   {ECHO; yyerror ("unexpected character");}

%%
int yywrap (void) {return 1;}