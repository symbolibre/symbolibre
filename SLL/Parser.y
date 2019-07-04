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

void idlocvec_filter(IdLocVec *vec, IdLocVec *ref)
{
	std::string name = (*ref)[0].name;
	auto it = vec->begin();

	while(it != vec->end())
	{
		if((*it).name == name) it = vec->erase(it);
		else ++it;
	}
}

void idlocvec_check(IdLocVec *vec, giac::context *ctx)
{
	for(auto it = vec->begin(); it != vec->end(); ++it)
	{
		if(ctx->tabptr->count(it->name.c_str())) continue;

		std::cerr << it->line << ":" << it->col <<
			": undefined variable " << it->name << "\n";
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

%type <IdLocVec *> atom expr id

%%

main:
    expr            { end_value(status, out, ctx);
	              idlocvec_check($1, ctx); delete $1; }
  | ID COLONEQ expr { end_var(status, $1, out, ctx); free($1);
                      idlocvec_check($3, ctx); delete $3; }

expr:
    atom            { $$ = $1; }
  | atom expr       { $$ = $1; $$->insert($$->end(), $2->begin(), $2->end());
                      delete $2; }
  | id arrow expr   { $$ = $3; idlocvec_filter($3, $1); }

atom:
    lp expr rp      { $$ = $2; }
  | id              { $$ = $1; }
  | C               { $$ = new IdLocVec(); out += $1; }

lp: '('             { out += '('; }
rp: ')'             { out += ')'; }
arrow: ARROW        { out += "->"; }

id: ID              { $$ = new IdLocVec();
	              $$->push_back(IdLoc($1, @1.first_line, @1.first_column));
                      out += $1; free($1); }

%%

__attribute__((constructor))
void configure_gettext(void)
{
	setlocale(LC_ALL, "");
	bindtextdomain("functions", "lang");
	textdomain("functions");
}
