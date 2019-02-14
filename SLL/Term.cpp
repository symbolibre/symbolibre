#include <SLL.hpp>

/* Provides thread-safe calls to giac */
static giac::context ct;

namespace SLL
{

Term::Term(std::string formula)
{
    this->gen = giac::gen(formula.c_str(), &ct);
}

std::string Term::str(void)
{
    return this->gen.print(NULL);
}

}
