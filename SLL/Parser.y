// %require "3.2"
// %skeleton "lalr1.cc" /* -*- C++ -*- */
// %language "c++"

%{

#include <SLL.hpp>
#include <Parser.hpp>

#include <iostream>
#include <string>

#include <clocale>
#include <libintl.h>
using giac::gen;

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

void idlocset_filter(IdLocSet *set, IdLocSet *ref)
{
	std::string name = std::get<0>(*ref->begin());
	auto it = set->begin();

	while(it != set->end())
	{
		if(std::get<0>(*it) == name) it = set->erase(it);
		else ++it;
	}
}

void idlocset_check(IdLocSet *set, giac::context *ctx)
{
	for(auto it = set->begin(); it != set->end(); ++it)
	{
		std::string name = std::get<0>(*it);
		if(ctx->tabptr->count(name.c_str())) continue;

		std::cerr << std::get<1>(*it) << ":" << std::get<2>(*it) <<
			": undefined variable " << name << "\n";
	}
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

%type <IdLocSet *> atom expr id

%%

main:
    expr            { end_value(status, out, ctx);
	              idlocset_check($1, ctx); delete $1; }
  | ID COLONEQ expr { end_var(status, $1, out, ctx); free($1);
                      idlocset_check($3, ctx); delete $3; }

expr:
    atom            { $$ = $1; }
  | atom expr       { $$ = $1; $$->insert($2->begin(), $2->end()); delete $2; }
  | id arrow expr   { $$ = $3; idlocset_filter($3, $1); }

atom:
    lp expr rp      { $$ = $2; }
  | id              { $$ = $1; }
  | C               { $$ = new IdLocSet(); out += $1; }

lp: '('             { out += '('; }
rp: ')'             { out += ')'; }
arrow: ARROW        { out += "->"; }

id: ID              { $$ = new IdLocSet();
	              $$->insert(IdLoc($1, @1.first_line, @1.first_column));
                      out += $1; free($1); }

%%

__attribute__((constructor))
void configure_gettext(void)
{
	setlocale(LC_ALL, "");
	bindtextdomain("functions", "lang");
	textdomain("functions");
}
