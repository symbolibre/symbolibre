#include "ParserInterface.hpp"

ExpressionNode *get_ast(std::string input)
{
    int a = lauch_lex(input.data());
    if (a == -1) {
        return NULL;
    } else {
        return root;
    }
}
