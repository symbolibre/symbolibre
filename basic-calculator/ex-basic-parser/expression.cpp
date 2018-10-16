#include <map>
#include <list>
#include "expression.h"

// Here we implement the methods of the classes given in expression.h
// If class A has a method f, we will use the syntax A::f to refer to the method.


// Atomic elements

number_node::number_node(float value) : num(value) {}

float number_node::evaluate()
{
    return num;
}

empty_node::empty_node(float value) : num(value) {}
float empty_node::evaluate()
{
    printf("Oh an empty node\n");
    return num;
}





// Operators nodes


// Initialisation of any operator node
operator_node::operator_node(ExpressionNode &L, ExpressionNode &R) : left(&L), right(&R) {}

// Evaluation of each operator node
// We need to redefine the constructor, but this is done in a generic way so this is modular
plus_node::plus_node(ExpressionNode &L, ExpressionNode &R) : operator_node(L, R) {}
float plus_node::evaluate()
{
    float left_num, right_num;
    left_num  = left->evaluate();
    right_num = right->evaluate();
    return (left_num + right_num);
}

times_node::times_node(ExpressionNode &L, ExpressionNode &R) : operator_node(L, R) {}
float times_node::evaluate()
{
    float left_num, right_num;
    left_num  = left->evaluate();
    right_num = right->evaluate();
    return (left_num * right_num);
}

minus_node::minus_node(ExpressionNode &L, ExpressionNode &R) : operator_node(L, R) {}
float minus_node::evaluate()
{
    float left_num, right_num;
    left_num  = left->evaluate();
    right_num = right->evaluate();
    return (left_num - right_num);
}

frac_node::frac_node(ExpressionNode &L, ExpressionNode &R) : operator_node(L, R) {}
float frac_node::evaluate()
{
    float left_num, right_num;
    left_num  = left->evaluate();
    right_num = right->evaluate();
    return (left_num / right_num);
}



// Unary functions

unary_node::unary_node(ExpressionNode &C) : child(&C) {}

unary_minus_node::unary_minus_node(ExpressionNode &C): unary_node(C) {}
float unary_minus_node::evaluate()
{
    return (-child->evaluate());
}


