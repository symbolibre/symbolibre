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
.          { std::cerr << "unknown token near '" << yytext << "'\n";return 0; }

%%

namespace SLL {

// We modify the lexer so we lex by taking string input
giac::gen *parse(const char *input)
{
	giac::gen *result;

	YY_BUFFER_STATE buf = yy_scan_bytes(input, strlen(input));
	int x = yyparse(&result);
	yy_delete_buffer(buf);

	if(x && result) delete result;
	return (x) ? NULL : result;
}

}
