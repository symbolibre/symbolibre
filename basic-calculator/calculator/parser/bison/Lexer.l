%{
#include "expression.h"
#include "Parser.h"
#include <string.h>

%}

%option noyywrap

blanks          [ \t\n]+
integer		[0-9]+
plus		\+
minus		\-
times		\*
divide		\/
opar		\(
cpar 		\)
sqrt		sqrt

%%

{blanks}        { /* ignore */ }

{integer}	{ yylval.value = atof(yytext); return NUMBER; }
{plus}		{ return(PLUS); }
{minus}		{ return(MINUS); }
{times}		{ return(TIMES); }
{divide}	{ return(DIVIDED); }
{opar}		{ return(LPAR); }
{cpar}		{ return(RPAR); }
{sqrt}		{ return(SQRT); }

%%
// We modify the lexer so we lex by taking string input
int lauch_lex(const char* buff) {
  YY_BUFFER_STATE b = yy_scan_bytes (buff, strlen(buff));
  int a = yyparse();
  yy_delete_buffer(b);
  return(a);
}

