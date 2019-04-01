#include <SLL.hpp>
#include <iostream>

int main(void)
{
    SLL::Context sll;

    SLL::Term t = sll.eval("2 * 8 + 1 + sqrt(2)");
    SLL::Term u = sll.eval("sqrt(2) * sqrt(2)");
    u = sll.simplify(u);
    SLL::Term v = sll.eval("2^7 - 3^3");

    std::cout << sll.str(t) << "\n";
    std::cout << sll.str(u) << "\n";
    std::cout << sll.str(v) << "\n";
    return 0;
}
