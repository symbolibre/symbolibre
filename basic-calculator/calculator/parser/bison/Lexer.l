%{
#include "expression.h"
#include "Parser.h"
#include <string.h>

%}

%option noyywrap

blanks          [ \t\n]+
entier		[0-9]+
plus		\+
moins		\-
fois		\*
divise		\/
ouvrir		\(
fermer		\)

%%

{blanks}        { /* ignore */ }

{entier}	{ yylval.value = atof(yytext); return NUMBER; }
{plus}		{ return(PLUS); }
{moins}		{ return(MINUS); }
{fois}		{ return(TIMES); }
{divise}	{ return(DIVIDED); }
{ouvrir}	{ return(LPAR); }
{fermer}	{ return(RPAR); }

%%
// We modify the lexer so we lex by taking string input
int lauch_lex(const char* buff) {
  YY_BUFFER_STATE b = yy_scan_bytes (buff, strlen(buff));
  int a = yyparse();
  yy_delete_buffer(b);
  return(a);
}

