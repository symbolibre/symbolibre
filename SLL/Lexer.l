%{
#include <SLL.hpp>
#include "Parser.h"
#include <string.h>

%}

%option noyywrap
%option nounput

blank		[ \t\n]+
integer		[0-9]+
id		[A-Za-z_]+

%%

{blank}    ;

{integer}  { yylval.INTEGER = yytext; return INTEGER; }
"+"        { return '+'; }
"-"        { return '-'; }
"*"        { return '*'; }
"/"        { return '/'; }
"("        { return '('; }
")"        { return ')'; }
{id}       { yylval.ID = yytext; return ID; }
","        { return ','; }
.          { std::cerr << "unknown token near '" << yytext << "' " << strlen(yytext) << "\n";return 0; }

%%

extern int yydebug;

namespace SLL {

// We modify the lexer so we lex by taking string input
int parse(const char *input, giac::gen *gen)
{
	#ifdef NDEBUG
	yydebug = 1;
	#endif

	YY_BUFFER_STATE buf = yy_scan_bytes(input, strlen(input));
	int x = yyparse(gen);
	yy_delete_buffer(buf);

	return x;
}

}
