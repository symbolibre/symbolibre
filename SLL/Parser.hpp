/** Internal definitions for the lexer and parser. **/

#ifndef SLL_PARSER
#define SLL_PARSER

#include <set>

/* A used variable name with its location (line, column) */
typedef std::tuple<std::string, int, int> IdLoc;
/* A set of such locations */
typedef std::set<IdLoc> IdLocSet;

#include "build/Parser.tab.h"

int yyparse(void);
int yylex(void);

#endif /* SLL_PARSER */
