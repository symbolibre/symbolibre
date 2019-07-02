#include <SLL.hpp>

int main(void)
{
    SLL::Context sll;

    SLL::Term t = sll.eval("x+y-\n\n   z\n*3");
    t = sll.simplify(t);

    std::cout << sll.str(t) << "\n";
    return 0;
}
