/** Internal definitions for the lexer and parser. **/

#ifndef SLL_PARSER
#define SLL_PARSER

#include <vector>

/* A used variable name with its location */
struct IdLoc {
    std::string name;
    int line;
    int col;

    IdLoc(std::string name, int line, int col)
        : name(name), line(line), col(col) { }
};
/* A list of such locations */
using IdLocVec = std::vector<IdLoc>;

#include "build/Parser.tab.h"

int yyparse(void);
int yylex(void);

#endif /* SLL_PARSER */
