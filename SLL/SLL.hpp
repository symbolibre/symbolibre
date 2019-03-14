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
 * Opaque, structured mathematical object
 * Essentially a giac::gen when using Giac.
 */
class Term
{
public:
    /**
     * @param formula: SLL formula to be evaluated
     */
    Term(std::string formula);

    /**
     * @param precision: Number of digits of precision
     * @return Same object, with approximated numerical entries
     */
    Term approx(unsigned precision);

    /**
     * @return Giac's string representation of the object.
     */
    std::string str(void);

public:
    /* Underlying CAS object */
    giac::gen gen;
};

/**
 * Evaluation context where the value of variables is stored
 * This is essentially a map from std::string to SLL::Term.
 */
class Context
{
public:
    /* ******************* CREATE and SET VARIABLES ******************* */

    /**
     * @param name: Name of the new variable
     * @param value:  Its value, as an object
     * The name of the variable has to be new. If you want to assign an
     * expression, evaluate it before with eval(). */
    void newVar(std::string name, Term value);

    /**
     * @param name: Name of an existing variable
     * @param value:  Its value, as an object
     * The variable has to exist. If you want to assign an expression,
     * evaluate it before with eval(). */
    void setVar(std::string name, Term value);

    /* *********************** ACCESS VARIABLES *********************** */

    Term operator[](std::string name);

private:
    /* Mapping from variable names to actual SLL terms */
    std::map<std::string, Term> vars;
};

/**
 * Formally evaluate a string into a structured object
 * @param formula: Text formula following the SLL grammar
 * @param context: Evaluation context
 */
Term eval(std::string const &formula, Context const &context);

/**
 * Execute an SLL command that does not return a result
 * This type of commands includes variables definitions and updates.
 * @param command: Text command following the SLL grammar
 * @param context: Evaluation context, may be modified by the command
 */
void exec(std::string const &command, Context &context);

}

#endif // SLL_HPP
