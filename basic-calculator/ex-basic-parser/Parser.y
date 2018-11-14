%{
#include <iostream>
#include "expression.h"
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
  float       value;  /* For the lexical analyser. NUMBER tokens */
}

// This destructor will empty all the stack when bison meet a syntax error, so there are no memory leakage
%destructor {free($$); } <exp>

/* Lets inform Bison about the type of each terminal and non-terminal */
%type <exp>   Calc
%type <exp>  Exp
%type <exp>   Add
%type <exp>   Minus
%type <exp>   Times
%type <exp>   Divided
%type <exp>   Paren


// We delare our tokens, and if they have a field.
// We also set priorities on rules, and if they are associative (%left means that 3+4+5 will be parsed as plus(plus(3,4),5)), we do the left operation first )
%token NUMBER
%type <value> NUMBER // NUMBER now have a field value that is used in the lexer

%left PLUS
%left MINUS
%left TIMES
%left DIVIDED
%token LPAR
%token RPAR

%%

Exp: Calc { $$ = NULL; root = $1; } // We don't assign $$ to $1 because the destructor routine free elements even when the parsing works.
// It means that root would be freed before being returned, which lead to a segfault.

Calc: NUMBER { $$ = new NumberNode($1); }
   | Add {$$ = $1; }
   | Minus {$$ = $1; }
   | Times {$$ = $1; }
   | Divided {$$ = $1; }
   | Paren { $$ = $1; }

Paren : LPAR Calc RPAR { $$ = $2; }

Add : Calc PLUS Calc {$$ = new PlusNode(*$1,*$3); }

Minus : Calc MINUS Calc {$$ = new MinusNode(*$1,*$3); }

Times : Calc TIMES Calc {$$ = new TimesNode(*$1,*$3); }

Divided : Calc DIVIDED Calc {$$ = new FracNode(*$1,*$3); }
%%

/*
int main(void) {
  char buff[1024];
  while (1) {
    //fgets(buffer, 1024, stdin);
    int a = yyparse();
    if (a==0) { // If there was no error then we evaluate our AST.
          float b = root->evaluate();
	  printf("%f\n",b);
	  free(root);
    }
  }
  return 0;
}*/


// output "sytax error"
int yyerror(char *s) {
        printf("%s\n",s);
	return 1;
}
