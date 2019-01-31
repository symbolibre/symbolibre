#ifndef SLL_HPP
#define SLL_HPP


namespace SLL
{

enum Error
{ NO_ERROR, NAME_EXISTS, NAME_UNKNOWN,};

class Expr
{
public:
    /* empty */

    Error formalEval(Expr target);

    Error approximationEval(Expr, unsigned precision);
};

Expr runCommand(std::string const &command, Context &context);

class Context
{
    /* unordered map from variables to SLL_expr */
public:
    /* ********************** CREATE and SET VARIABLES ********************** */
    /** addVar:
     * @param name: std::string  that is the name of the NEW variable
     * @param val : Expr that is its value
     * The name of the variable has to be new. */
    Error newVar(std::string name, Expr value);

    /** setVar:
     * @param name: std::string  that is the name of the NEW variable
     * @param val : Expr that is its value
     * The name of the variable has to exist. */
    Error setVar(std::string name, Expr value);

    /* ********************** ACCESS VARIABLES ********************** */
    Error getVal(std::string name, Expr &res);

};


}


#endif // SLL_HPP
