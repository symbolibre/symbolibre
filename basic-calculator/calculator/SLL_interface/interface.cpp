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
        giac::context ct; // TODO, take context into account
        std::ostringstream result;
        giac::gen e = expr->evaluate();
        result << e << " = " << _evalf(e, &ct) << std::endl;
        return (result.str());
    }
}

/* This is ugly but it will disappear */
double approximate(std::string fun_body, std::string antecedent)
{
    std::string giac_fun_input;
    giac_fun_input.append("f(x) := ");
    giac_fun_input.append(fun_body);
    std::string giac_eval_input;
    giac_eval_input.append("f(");
    giac_eval_input.append(antecedent);
    giac_eval_input.append(")");
    giac::context ct;
    eval(gen(giac_fun_input, &ct), &ct);
    return (_evalf(giac_eval_input));
}


