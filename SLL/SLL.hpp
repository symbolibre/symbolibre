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
 * SLL::Object - Opaque, structured mathematical objects
 * Essentially a giac::gen when using Giac, for instance.
 */
class Object: private giac::gen
{
    /** approx:
     * @param precision: Number of digits of precision
     * @return Same object, with approximated numerical entries */
    Object approx(unsigned precision);

    /** str:
     * @return Giac's string representation of the object. */
    std::string str(void);
};

/**
 * SLL::Context - Evaluation context where the value of variables is stored
 * This is essentially a map from std::string to SLL::Object.
 */
class Context: private std::map<std::string, Object>
{
public:
    /* ******************* CREATE and SET VARIABLES ******************* */

    /** newVar:
     * @param name: Name of the new variable
     * @param val:  Its value, as an object
     * The name of the variable has to be new. If you want to assign an
     * expression, evaluate it before with eval(). */
    void newVar(std::string name, Object value);

    /** setVar:
     * @param name: Name of an existing variable
     * @param val:  Its value, as an object
     * The variable has to exist. If you want to assign an expression,
     * evaluate it before with eval(). */
    void setVar(std::string name, Object value);

    /* *********************** ACCESS VARIABLES *********************** */

    Object operator[](std::string name);

private:
    /* Provides thread-safe calls to giac */
    giac::context ctx;
};

/**
 * eval: Formally evaluate a string into a structured object
 * @param formula: Text formula following the SLL grammar
 * @param context: Evaluation context
 */
Object eval(std::string const &formula, Context const &context);

/**
 * exec: Execute an SLL command that does not return a result
 * This type of commands includes variables definitions and updates.
 * @param command: Text command following the SLL grammar
 * @param conetxt: Evaluation context, may be modified by the command
 */
void exec(std::string const &command, Context &context);

}

#endif // SLL_HPP
