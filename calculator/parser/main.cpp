#include "interface.hpp"
#include <iostream>
#include <string>


int main(void)
{
    while (1) {
        giac::context ct;
        std::string input;
        std::getline(std::cin, input);
        ExpressionNode *a;
        a = get_ast(input);
        giac::gen e = a->evaluate();
        std::cout << e << " = " << _evalf(e, &ct) << std::endl;
    }
    return 0;
}
