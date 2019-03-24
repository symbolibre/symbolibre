#include <SLL.hpp>

int main(void)
{
    SLL::Context sll;
    sll.exec("x := sqrt(2)");

    SLL::Term t = sll.eval("x * x");
    t = sll.simplify(t);

    std::cout << sll.str(t) << "\n";
    return 0;
}
