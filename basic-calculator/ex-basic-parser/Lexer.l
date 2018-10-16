%{
#include "expression.h"
#include "Parser.h"
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

