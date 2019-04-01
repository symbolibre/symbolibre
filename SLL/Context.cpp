#include <SLL.hpp>

namespace SLL
{

/* Internal SLL parser */
int parse(const char *formula, SLL::Status *st, giac::context *ctx);

Status Context::exec(std::string command)
{
    Status st;
    SLL::parse(command.c_str(), &st, &this->ctx);

    st.value = giac::eval(st.value, &this->ctx);

    /* Handling of side effects */
    if (st.type == Status::SET_VARIABLE) {
        this->set(st.name, st.value);
    }

    return st;
}

Term Context::eval(std::string formula)
{
    Status st = this->exec(formula);
    /* TODO: raise exception if status is not [RESULT] */
    return st.value;
}

Term Context::simplify(Term t)
{
    return giac::_simplify(t, &this->ctx);
}

Term Context::approx(Term t, unsigned int precision)
{
    return giac::_evalf(t, &this->ctx); // TODO : precision
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
