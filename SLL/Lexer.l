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

blank		[ \t]+
integer		[0-9]+
id		[A-Za-z_]+

%%

{blank}    ;
\n         yylineno++, col = 1;

{integer}  { yylval.INTEGER = strdup(yytext); return INTEGER; }
"+"        { return '+'; }
"-"        { return '-'; }
"*"        { return '*'; }
"^"        { return '^'; }
"/"        { return '/'; }
"("        { return '('; }
")"        { return ')'; }
{id}       { yylval.ID = strdup(yytext); return ID; }
","        { return ','; }
":="       { return COLONEQ; }
.          { std::cerr << "unknown token near '" << yytext << "' " << strlen(yytext) << "\n"; return 0; }

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

	YY_BUFFER_STATE buf = yy_scan_bytes(input, strlen(input));
	int x = yyparse(status, ctx);
	yy_delete_buffer(buf);

	return x;
}

} /* namespace SLL */
