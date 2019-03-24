// %require "3.2"
// %skeleton "lalr1.cc" /* -*- C++ -*- */
// %language "c++"

%{

#include <iostream>
#include <vector>
#include <string>
#include <SLL.hpp>
using giac::gen;

int yyparse(void);
int yylex(void);

int yyerror(giac::gen *, giac::context *, const char *s)
{
	std::cerr << s << std::endl;
	return 1;
}

%}

%start main

%define api.value.type union
%define parse.trace

%parse-param {giac::gen *ret_gen}
%parse-param {giac::context *ctx}

%token <const char *> INTEGER
%token <const char *> ID

%left '+'
%left '-'
%left '*'
%left '/'
%left ','

%type <giac::gen *> expr
%type <giac::gen *> id
%type <giac::gen *> args

%%

main: expr { *ret_gen = *$1; }

expr:
    INTEGER           { $$ = new gen($1, ctx); }
  | '(' expr ')'      { $$ = $2; }
  | expr '+' expr     { $$ = new gen(*$1 + *$3);        delete $1; delete $3; }
  | expr '-' expr     { $$ = new gen(*$1 - *$3);        delete $1; delete $3; }
  | expr '*' expr     { $$ = new gen(*$1 * *$3);        delete $1; delete $3; }
  | expr '/' expr     { $$ = new gen(*$1 / *$3);        delete $1; delete $3; }
  | id '(' args ')'   { $$ = new gen((*$1)(*$3, ctx));  delete $1; delete $3; }
  | id                { $$ = $1; }

id: ID                { $$ = new gen($1, ctx); }

args:
    expr              { $$ = new gen(giac::makesequence(*$1)); }
  | expr ',' expr     { $$ = new gen(giac::makesequence(*$1, *$3)); }

%%
