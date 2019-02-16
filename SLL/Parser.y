// %require "3.2"
// %skeleton "lalr1.cc" /* -*- C++ -*- */
// %language "c++"

%{

#include <iostream>
#include <vector>
#include <string>
#include <SLL.hpp>

static giac::context ct;

int yyparse(void);
int yylex(void);

int yyerror(giac::gen *, const char *s)
{
	std::cerr << s << std::endl;
	return 1;
}

%}

%start main

%define api.value.type union
%define parse.trace

%parse-param {giac::gen *ret_gen}

%token <const char *> INTEGER
%token <const char *> ID

%left '+'
%left '-'
%left '*'
%left '/'
%left ','

%type <giac::gen *> expr

%%

main: expr { *ret_gen = *$1; delete $1; }

expr:
    INTEGER           { $$ = new giac::gen($1, &ct); }
  | '(' expr ')'      { $$ = $2; }
  | expr '+' expr     { $$ = new giac::gen(*$1 + *$3); delete $1; delete $3; }
  | expr '-' expr     { $$ = new giac::gen(*$1 - *$3); delete $1; delete $3; }
  | expr '*' expr     { $$ = new giac::gen(*$1 * *$3); delete $1; delete $3; }
  | expr '/' expr     { $$ = new giac::gen(*$1 / *$3); delete $1; delete $3; }
  /* TODO: Function call */

/* Fun: FUN LPAR Arglist RPAR {std::string funname($1); $$ = new FunAppNode(*$3, funname); }

Arglist: Calc { std::vector<ExpressionNode *> args; args.push_back($1); $$ = &args; }
	| Calc COMMA Arglist {std::vector<ExpressionNode *> *args = $3; args->push_back($1); $$ = args; } */

%%
