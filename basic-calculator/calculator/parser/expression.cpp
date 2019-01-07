#include <map>
#include <list>
#include "expression.hpp"
#include <cmath>

// Here we implement the methods of the classes given in expression.h
// If class A has a method f, we will use the syntax A::f to refer to the method.


// Atomic elements

NumberNode::NumberNode(float value) : num(value) {}

float NumberNode::evaluate()
{
    return num;
}

EmptyNode::EmptyNode(float value) : num(value) {}
float EmptyNode::evaluate()
{
    printf("Oh an empty node\n");
    return num;
}




// Operators nodes


// Initialisation of any operator node
OperatorNode::OperatorNode(ExpressionNode &L, ExpressionNode &R) : left(&L), right(&R) {}

// Evaluation of each operator node
// We need to redefine the constructor, but this is done in a generic way so this is modular
PlusNode::PlusNode(ExpressionNode &L, ExpressionNode &R) : OperatorNode(L, R) {}
float PlusNode::evaluate()
{
    float left_num, right_num;
    left_num  = left->evaluate();
    right_num = right->evaluate();
    return (left_num + right_num);
}

TimesNode::TimesNode(ExpressionNode &L, ExpressionNode &R) : OperatorNode(L, R) {}
float TimesNode::evaluate()
{
    float left_num, right_num;
    left_num  = left->evaluate();
    right_num = right->evaluate();
    return (left_num * right_num);
}

MinusNode::MinusNode(ExpressionNode &L, ExpressionNode &R) : OperatorNode(L, R) {}
float MinusNode::evaluate()
{
    float left_num, right_num;
    left_num  = left->evaluate();
    right_num = right->evaluate();
    return (left_num - right_num);
}

FracNode::FracNode(ExpressionNode &L, ExpressionNode &R) : OperatorNode(L, R) {}
float FracNode::evaluate()
{
    float left_num, right_num;
    left_num  = left->evaluate();
    right_num = right->evaluate();
    return (left_num / right_num);
}



// Unary functions

UnaryNode::UnaryNode(ExpressionNode &C) : child(&C) {}

UnaryMinusNode::UnaryMinusNode(ExpressionNode &C): UnaryNode(C) {}
float UnaryMinusNode::evaluate()
{
    return (-child->evaluate());
}

SqrtNode::SqrtNode(ExpressionNode &C): UnaryNode(C) {}
float SqrtNode::evaluate()
{
    return (sqrt(child->evaluate()));
}

FunAppNode::FunAppNode(std::vector<ExpressionNode *> args, std::string s) : name(s), arglist(args) {}
float FunAppNode::evaluate()
{
    return (0);
}


// Function Application


