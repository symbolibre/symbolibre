#include <SLL.hpp>
#include <iostream>

int main(void)
{
    SLL::Term term("x -> 2 * x + 1");
    std::cout << term.str() << std::endl;
    return 0;
}
