%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scriptc-yacc.tab.h"
%}

%option yylineno

%%
"display"                           {return display;}
"int"                               {yylval.all.c = strdup(yytext); return INT;}
"float"                             {yylval.all.c = strdup(yytext); return FLOAT;}
"char"                              {yylval.all.c = strdup(yytext); return CHAR;}


                             
[0-9]+                              {yylval.all.i = atof(yytext); return INTEGERS;}
[0-9]+\.[0-9]+                      {yylval.all.f = atof(yytext); return DECIMALS;}       
[n][a-zA-Z_]+                       {yylval.all.c = strdup(yytext); return NIDENTIFIER;}
[s][a-zA-Z_]+                       {yylval.all.c = strdup(yytext); return SIDENTIFIER;}

\"([^"]|\\\")*\"                    {
                                        yylval.all.c = strdup(yytext+1);
                                        if (yylval.all.c[yyleng-2] != '"')
                                            yyerror("improperly terminated string");
                                        else
                                            yylval.all.c[yyleng-2] = 0;
                                        return STRING;
                                    }


\'.*\'                              {
                                          yylval.all.c = strdup(yytext+1);
                                          if (yylval.all.c[yyleng-2] != '\'')
                                              yyerror("improperly terminated string");
                                          else
                                              yylval.all.c[yyleng-2] = 0;
                                          return CHARACTER;
                                    }


\n                                  {return NEWLINE;}

[ \t\n]                             ;
[-+*/=":,]                        {return yytext[0];}
.                                   {ECHO; yyerror ("unexpected character");}

%%
int yywrap (void) {return 1;}