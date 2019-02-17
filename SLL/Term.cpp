#include <SLL.hpp>

/* Provides thread-safe calls to giac */
static giac::context ct;

namespace SLL
{

/**
 * Internal SLL parser
 */
int parse(const char *formula, giac::gen *gen);

Term::Term(std::string formula)
{
    parse(formula.c_str(), &this->gen);
}

std::string Term::str(void)
{
    return this->gen.print(NULL);
}

}
