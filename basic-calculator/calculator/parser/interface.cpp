#include "interface.hpp"
#include "Parser.h"
#include "Lexer.h"
#include <iostream>
#include <cstring>

extern ExpressionNode *root;

ExpressionNode *get_ast(std::string input)
{
    int a = lauch_lex(input.data());
    return root;
}


std::string evaluate(std::string input)
{
    int a = lauch_lex(input.data());
    /*if (a==0) {
        // If there was no error then we evaluate our AST.
        float b = root->evaluate();
        return std::to_string(b);
    }*/
    if (a == 0) { // no error, sending to sage
        std::cout << input << std::endl;
        //std::cerr << "Input sent, waiting output" << std::endl;
        /* Here sage computes something */
        std::string result;
        std::getline(std::cin, result);
        return result;
    }
    return std::string("Erreur syntaxe");
}
