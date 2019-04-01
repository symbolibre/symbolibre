// %require "3.2"
// %skeleton "lalr1.cc" /* -*- C++ -*- */
// %language "c++"

%{

#include <iostream>
#include <vector>
#include <string>
#include <SLL.hpp>
#include <cstdlib>
using giac::gen;

int yyparse(void);
int yylex(void);

int yyerror(SLL::Status *, giac::context *, const char *s)
{
	std::cerr << s << std::endl;
	return 1;
}

void end_value(SLL::Status *status, giac::gen value)
{
	status->type = SLL::Status::RESULT;
	status->name = "";
	status->value = value;
}

void end_var(SLL::Status *status, std::string name, giac::gen value)
{
	status->type = SLL::Status::SET_VARIABLE;
	status->name = name;
	status->value = value;
}

%}

%start main

%define api.value.type union
%define parse.trace

%parse-param {SLL::Status *status}
%parse-param {giac::context *ctx}

%token <char *> INTEGER
%token <char *> ID
%token COLONEQ

%left '+'
%left '-'
%left '*'
%left '/'
%right '^'

%type <giac::gen *> expr
%type <giac::gen *> id
%type <giac::gen *> args

%%

main:
    expr              { end_value(status, *$1);    delete $1; }
  | ID COLONEQ expr   { end_var(status, $1, *$3);  delete $3; free($1); }

expr:
    INTEGER           { $$ = new gen($1, ctx); free($1); }
  | '(' expr ')'      { $$ = $2; }
  | expr '+' expr     { $$ = new gen(*$1 + *$3);        delete $1; delete $3; }
  | '-' expr          { $$ = new gen(gen("0", ctx) - *$2);		   delete $2; }
  | expr '-' expr     { $$ = new gen(*$1 - *$3);        delete $1; delete $3; }
  | expr '*' expr     { $$ = new gen(*$1 * *$3);        delete $1; delete $3; }
  | expr '/' expr     { $$ = new gen(*$1 / *$3);        delete $1; delete $3; }
  | expr '^' expr     { $$ = new gen(gen("pow", ctx)
                                     (gen(giac::makesequence(*$1, *$3)), ctx));
                        delete $1; delete $3; }
  | id '(' args ')'   { $$ = new gen((*$1)(*$3, ctx));  delete $1; delete $3; }
  | id                { $$ = $1; }

id: ID                { $$ = new gen($1, ctx); free($1); }

args:
    expr              { $$ = new gen(giac::makesequence(*$1)); }
  | expr ',' expr     { $$ = new gen(giac::makesequence(*$1, *$3)); }

%%
