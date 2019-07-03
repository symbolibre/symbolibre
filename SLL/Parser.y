// %require "3.2"
// %skeleton "lalr1.cc" /* -*- C++ -*- */
// %language "c++"

%{

#include <SLL.hpp>
#include "Parser.h"

#include <iostream>
#include <string>
#include <set>

#include <clocale>
#include <libintl.h>
using giac::gen;

/* Set of declared variables with locations */
struct identifier {
	char *name;
	int line;
	int col;
};
typedef std::set<struct identifier> IdList;

int yyparse(void);
int yylex(void);

int yyerror(SLL::Status *, giac::context *, std::string &, char const *s)
{
	std::cerr << s << "\n";
	return 1;
}

void end_value(SLL::Status *status, std::string formula, giac::context *ctx)
{
	status->type = SLL::Status::RESULT;
	status->name = "";
	status->value = giac::gen(formula, ctx);
}

void end_var(SLL::Status *status, std::string name, std::string formula,
	giac::context *ctx)
{
	status->type = SLL::Status::SET_VARIABLE;
	status->name = name;
	status->value = giac::gen(formula, ctx);
}

%}

%start main
%locations

%define api.value.type union
%define parse.trace

%parse-param {SLL::Status *status}
%parse-param {giac::context *ctx}
%parse-param {std::string &out}

%token <char *> ID
%token <int> C
%token COLONEQ
%token ARROW

//%type <IdList> expr

%%

main:
    expr                   { end_value(status, out, ctx); }
  | ID COLONEQ expr        { end_var(status, $1, out, ctx); free($1); }

expr:
    atom
  | atom expr

atom:
    leftp expr rightp
  | id leftp expr rightp
  | id
  | C                      { out += $1; }

leftp: '('                 { out += '('; }
rightp: ')'                { out += ')'; }

id: ID                     { out += $1; free($1); }

%%

__attribute__((constructor))
void configure_gettext(void)
{
	setlocale(LC_ALL, "");
	bindtextdomain("functions", "lang");
	textdomain("functions");
}
