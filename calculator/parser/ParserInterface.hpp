#include "expression.hpp"
#include "Parser.h"
#include "Lexer.h"

extern ExpressionNode *root;

ExpressionNode *get_ast(std::string input);
