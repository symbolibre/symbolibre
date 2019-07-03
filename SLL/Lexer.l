%{
#include <SLL.hpp>
#include "Parser.h"
#include <string.h>

/* Location tracking information */
static int col;

#define YY_USER_ACTION					\
	yylloc.first_line = yylineno;			\
	yylloc.first_column = col;			\
	yylloc.last_line = yylineno;			\
	yylloc.last_column = (col += yyleng) - 1;

%}

%option noyywrap
%option nounput

%%

"("         return '(';
")"         return ')';
":="        return COLONEQ;
"->"        return ARROW;
[A-Za-z_]+  { yylval.ID = strdup(yytext); return ID; }
[ \t]+      ;
\n          yylineno++, col = 1;
.           { yylval.C = yytext[0]; return C; }

%%

extern int yydebug;

namespace SLL {

// We modify the lexer so we lex by taking string input
int parse(const char *input, SLL::Status *status, giac::context *ctx)
{
	#ifdef NDEBUG
	yydebug = 1;
	#endif

	/* Initialize location tracking */
	col = 1;

	std::string result;
	result.reserve(1024);

	YY_BUFFER_STATE buf = yy_scan_bytes(input, strlen(input));
	int x = yyparse(status, ctx, result);
	yy_delete_buffer(buf);

	return x;
}

} /* namespace SLL */
