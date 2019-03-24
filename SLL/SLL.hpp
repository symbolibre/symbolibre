#ifndef SLL_HPP
#define SLL_HPP

#include <giac/config.h>
#include <giac/giac.h>

namespace SLL
{

struct Exception: public std::exception {
    enum Error {
        NAME_EXISTS,
        NAME_UNKNOWN,
        ZERO_DIVISION,
    };
};

typedef giac::gen Term;

/**
 * Evaluation context where the value of variables is stored
 */
class Context
{
public:
    /**
     * @param formula: Formula to be evaluated
     * Evaluates the formula in context. The formula has access to all the
     * variables and functions defined through set().
     */
    Term eval(std::string formula);

    /**
     * @param formula: Formula to parse
     * Parses the formula but does not evaluate it.
     */
    Term parse(std::string formula);

    /**
     * Simplify a term.
     */
    Term simplify(Term t);

    /**
     * Numerically approximates a term.
     * @param precision: Number of digits of precision
     * @return Same object, with approximated numerical entries
     */
    Term approx(Term t, unsigned precision);

    /**
     * Produces a human-readable string representation.
     * @return Giac's string representation of the object
     */
    std::string str(Term t);

    /**
     * @param name: Name of the variable to store
     * @param value: Value of the variable as an evaluated Term
     * @param replace: If [false] and [name] is set, raise an exception.
     * If you want to assign an expression, evaluate it before with
     * eval().
     */
    void set(std::string name, Term value, bool replace = true);

    /**
     * @param name: Variable name (must be set)
     * Raises an exception if the variable is not set.
     */
    Term get(std::string name);

private:
    /* Giac evaluation context */
    giac::context ctx;
};

} /* namespace SLL */

#endif // SLL_HPP
