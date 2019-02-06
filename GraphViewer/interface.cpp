#include "interface.h"

#include <iostream>

/* Pas beau du tout mais en attendant d'avoir SLL, je fais avec ce que je peux */

double approximate(std::string fun_body, std::string antecedent)
{
    giac::context ct;
    double result;

    giac::gen e("f(x) := " + fun_body, &ct);
    e = eval(e, 1, &ct);
    giac::gen f("f(" + antecedent + ")", &ct);
    f = evalf(f, 1, &ct);
    result = f._DOUBLE_val;
    return result;
}
