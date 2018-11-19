#include "expression.h"
#include "Parser.h"
#include "Lexer.h"
#include <iostream>
#include <cstring>

extern ExpressionNode *root;

std::string evaluate(std::string input)
{
    int a = lauch_lex(input.data());
    if (a == 0) { // If there was no error then we evaluate our AST.
        float b = root->evaluate();
        return std::to_string(b);
    }
    return std::string("Erreur syntaxe");
}
