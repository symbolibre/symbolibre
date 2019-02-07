//%require "3.2"
//%skeleton "lalr1.cc" /* -*- C++ -*- */
//%language "c++"

%{

#include <iostream>
#include <vector>
#include <string>
#include "expression.hpp"

#include <giac/config.h>
#include <giac/giac.h>
/* Here we declare the prototype of our parsing, lexing and error functions */


int yyparse();
int yylex();
int yyerror(char *s);

/* yyparse() returns an integer. However, we also want to have access to the AST that we construct.
So we declare a pointer root that is going to points to the AST */
ExpressionNode *root;

%}



// We start our programm with this rule
%start Exp


// We define our different elements
%union {
  ExpressionNode *exp;  /* For the expressions. Since it is a pointer, no problem. */
  std::vector<ExpressionNode *> *vect;
  char *str;
  
}

// This destructor will empty all the stack when bison meet a syntax error, so there are no memory leakage
//%destructor {free($$); } <exp>

/* Lets inform Bison about the type of each terminal and non-terminal */
%type <exp>   Calc
%type <exp>  Exp
%type <exp>   Add
%type <exp>   Minus
%type <exp>   Times
%type <exp>   Divided
%type <exp>   Paren
%type <exp>   Sqrt
%type <exp>	Fun
%type <vect> Arglist


// We delare our tokens, and if they have a field.
// We also set priorities on rules, and if they are associative (%left means that 3+4+5 will be parsed as plus(plus(3,4),5)), we do the left operation first )
%token NUMBER
%type <str> NUMBER // NUMBER now have a field value that is used in the lexer
%token <str> FUN

%left PLUS
%left MINUS
%left TIMES
%left DIVIDED
%token SQRT
%token LPAR
%token RPAR
%token COMMA

%%

Exp: Calc { $$ = NULL; root = $1; } // We don't assign $$ to $1 because the destructor routine free elements even when the parsing works.
// It means that root would be freed before being returned, which lead to a segfault.

Calc: NUMBER { giac::context ct; std::string num($1); $$ = new NumberNode(giac::gen(num, &ct)); } // TODO : proper thing
   | Add {$$ = $1; }
   | Minus {$$ = $1; }
   | Times {$$ = $1; }
   | Divided {$$ = $1; }
   | Paren { $$ = $1; }
   | Sqrt { $$ = $1; }
   | Fun { $$ = $1; }

Paren : LPAR Calc RPAR { $$ = $2; }

Add : Calc PLUS Calc {$$ = new PlusNode(*$1,*$3);}

Minus : Calc MINUS Calc {$$ = new MinusNode(*$1,*$3); }

Times : Calc TIMES Calc {$$ = new TimesNode(*$1,*$3); }

Divided : Calc DIVIDED Calc {$$ = new FracNode(*$1,*$3); }

Sqrt: SQRT LPAR Calc RPAR { $$ = new SqrtNode(*$3); }

Fun: FUN LPAR Arglist RPAR {std::string funname($1); $$ = new FunAppNode(*$3, funname); }

Arglist: Calc { std::vector<ExpressionNode *> args; args.push_back($1); $$ = &args; }
  | Calc COMMA Arglist {std::vector<ExpressionNode *> *args = $3; args->push_back($1); $$ = args; }
  
%%


// output "sytax error"
int yyerror(char *s) {
  std::cerr << s << std::endl;
  return 1;
}
