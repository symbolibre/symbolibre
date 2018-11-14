#include "expression.h"
#include "Parser.h"
#include "Lexer.h"
#include <iostream>

extern ExpressionNode *root;

//YY_BUFFER_STATE yy_scan_buffer(char *base, yy_size_t size)


int main(void)
{
    while (1) {
        char buff[1024];
        fgets(buff, 1024, stdin);
        int a = lauch_lex(buff);
        if (a == 0) { // If there was no error then we evaluate our AST.
            float b = root->evaluate();
            printf("%f\n", b);
        }
    }
    return 0;
}
