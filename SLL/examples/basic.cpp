#include <SLL.hpp>
#include <iostream>

int main(void)
{
    SLL::Term term("2 * 8 + 1");
    std::cout << term.str() << std::endl;
    return 0;
}
