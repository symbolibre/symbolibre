#include <SLL.hpp>

namespace SLL
{

/* Internal SLL parser */
int parse(const char *formula, giac::gen *gen, giac::context *ctx);

Term Context::parse(std::string formula)
{
    giac::gen g;
    SLL::parse(formula.c_str(), &g, &this->ctx);
    return g;
}

Term Context::eval(std::string formula)
{
    std::cout << "SLL: " << formula << "\n";
    return giac::eval(parse(formula), &this->ctx);
}

Term Context::simplify(Term t)
{
    return giac::_simplify(t, &this->ctx);
}

std::string Context::str(Term t)
{
    return t.print(NULL);
}

void Context::set(std::string name, Term value, bool replace)
{
    /* TODO: replace is currently ignored */
    giac::gen var(name, &this->ctx);
    sto(value, var, &this->ctx);
}

Term Context::get(std::string name)
{
    return this->eval(name);
}

} /* namespace SLL */
