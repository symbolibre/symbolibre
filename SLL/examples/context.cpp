#include <SLL.hpp>

int main(void)
{
    SLL::Context sll;

    SLL::Term t = sll.eval("2+sqrt(7)");
    sll.set("x", t);

    SLL::Term u = sll.eval("2 * x");
    u = sll.simplify(u);

    std::cout << sll.str(u) << "\n";
    return 0;
}
