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
    /* Numerical approximation */
    Object approx(unsigned precision);
};

/* Formally evaluate a string into a structured object */
Object eval(std::string const &formula, Context &context);

/* Execute an SLL command, including variable updates or definitions, that does
   not return a result */
void exec(std::string const &command, Context &context);

class Context: private std::map<std::string, Object>
{
    /* unordered map from variables to SLL_expr */
public:
    /* ********************** CREATE and SET VARIABLES ********************** */
    /** addVar:
     * @param name: std::string  that is the name of the NEW variable
     * @param val : Object that is its value (evaluate before adding)
     * The name of the variable has to be new. */
    void newVar(std::string name, Object value);

    /** setVar:
     * @param name: std::string  that is the name of the NEW variable
     * @param val : Object that is its value (evaluate before setting)
     * The name of the variable has to exist. */
    void setVar(std::string name, Object value);

    /* ********************** ACCESS VARIABLES ********************** */
    Object getVal(std::string name);
};

}

#endif // SLL_HPP
