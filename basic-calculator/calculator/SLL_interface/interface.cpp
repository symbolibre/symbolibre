#include "interface.hpp"
#include <iostream>
#include <cstring>
#include <sstream>

std::string evaluate(std::string input)
{
    ExpressionNode *expr = get_ast(input);
    if (expr == NULL) {
        return (std::string("Syntax error"));
    } else {
        giac::context ct; // TODO
        std::ostringstream result;
        giac::gen e = expr->evaluate();
        result << e << " = " << _evalf(e, &ct) << std::endl;
        return (result.str());
    }
}
