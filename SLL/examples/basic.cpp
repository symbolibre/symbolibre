#include <SLL.hpp>
#include <iostream>

int main(void)
{
    SLL::Context sll;

    SLL::Term t = sll.eval("2 * 8 + 1 + sqrt(2)");
    SLL::Term u = sll.eval("sqrt(2) * sqrt(2)");
    u = sll.simplify(u);

    std::cout << sll.str(t) << "\n";
    std::cout << sll.str(u) << "\n";
    return 0;
}
