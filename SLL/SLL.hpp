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

/**
 * Type of abstract math terms
 */
typedef giac::gen Term;

/**
 * Answers that can be given when a command is executed
 */
struct Status {
    enum {
        RESULT,     /**< A term is produced as result */
        SET_VARIABLE,   /**< A variable is defined */
        SET_FUNCTION,   /**< A function is defined */
    } type;

    /** Name of the variable or function just defined */
    std::string name;

    /** Result term, value of set variable or function */
    SLL::Term value;
};

/**
 * Evaluation context where the value of variables is stored
 */
class Context
{
public:
    /**
     * @param formula: Formula to be evaluated
     * @return Evaluated SLL::Term
     *
     * Evaluates the formula in context. The formula must be a math term,
     * not a command with side-effects such as an assigment.
     * This function is a special case of exec() and returns the [value]
     * attribute of the SLL::Status object produced by exec().
     */
    SLL::Term eval(std::string formula);

    /**
     * @param command: Formula or command to be evaluated
     * @return An SLL::Status variant
     *
     * Evaluates the formula in context. The formula has access to all the
     * variables and functions defined through set(). It can either be an
     * expression to evaluated, or a command to define some variable or
     * function using the ":=" syntax.
     */
    SLL::Status exec(std::string command);

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
