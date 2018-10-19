%{
#include <iostream>
#include "expression.h"
/* Here we declare the prototype of our parsing, lexing and error functions */
int yyparse();
int yylex();
int yyerror(char *s);

/* yypares() returns an integer. However, we also want to have access to the AST that we construct.
So we declare a pointer root that is going to points to the AST */
ExpressionNode *root;

using namespace std;

%}

// We start one an expression
%start Exp


// We define our different elements
%union {
  ExpressionNode *exp;  /* For the expressions. Since it is a pointer, no problem. */
  float       value;  /* For the lexical analyser. NUMBER tokens */
}

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

/* Since we create new nodes along the computation, there is going to be some memory leackage if the parser trigger an exception.
There is also going to be a segfault.
Please don't mind it */

Exp: Calc { $$ = $1; root = $$; }

Calc: NUMBER { $$ = new number_node($1); }
   | Add {$$ = $1; }
   | Minus {$$ = $1; }
   | Times {$$ = $1; }
   | Divided {$$ = $1; }
   | Paren { $$ = $1; }
   | Empty { $$ = new empty_node(0); }
  
Empty: %empty

Paren : LPAR Calc RPAR { $$ = $2; }
   

Add : Calc PLUS Calc {$$ = new plus_node(*$1,*$3); }

Minus : Calc MINUS Calc {$$ = new minus_node(*$1,*$3); }

Times : Calc TIMES Calc {$$ = new times_node(*$1,*$3); }

Divided : Calc DIVIDED Calc {$$ = new frac_node(*$1,*$3); }
%%

// This is still really ugly, don't mind it
int yyerror(char *s) {
	printf("yyerror : %s\n",s);
	return 1;
}

int main(void) {
	yyparse();
	float b = root->evaluate(); // Couldn't use cout. I don't know why yet. Don't mind it.
	printf("%f\n",b);
	return 0;
}
