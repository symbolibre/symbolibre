#include <string>
#include "ParserInterface.hpp"

std::string evaluate(std::string input);
/* Take a SLL string as input and return the string corresponding to the result
TODO : temporary, will shift to edition tree */

double approximate(std::string fun_body, std::string antecedent);
/* Take a function expression and an antecedant and return an approximated evaluation
of it
TODO : temporary, for graph viewer development */
