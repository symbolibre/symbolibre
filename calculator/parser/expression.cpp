#include <map>
#include <list>
#include "expression.hpp"
#include <cmath>

// Here we implement the methods of the classes given in expression.h
// If class A has a method f, we will use the syntax A::f to refer to the method.


// Atomic elements

NumberNode::NumberNode(giac::gen value) : num(value) {}

giac::gen NumberNode::evaluate()
{
    return num;
}

EmptyNode::EmptyNode(giac::gen value) : num(value) {}
giac::gen EmptyNode::evaluate()
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
giac::gen PlusNode::evaluate()
{
    giac::gen left_num, right_num;
    left_num  = left->evaluate();
    right_num = right->evaluate();
    return (left_num + right_num);
}

TimesNode::TimesNode(ExpressionNode &L, ExpressionNode &R) : OperatorNode(L, R) {}
giac::gen TimesNode::evaluate()
{
    giac::gen left_num, right_num;
    left_num  = left->evaluate();
    right_num = right->evaluate();
    return (left_num * right_num);
}

MinusNode::MinusNode(ExpressionNode &L, ExpressionNode &R) : OperatorNode(L, R) {}
giac::gen MinusNode::evaluate()
{
    giac::gen left_num, right_num;
    left_num  = left->evaluate();
    right_num = right->evaluate();
    return (left_num - right_num);
}

FracNode::FracNode(ExpressionNode &L, ExpressionNode &R) : OperatorNode(L, R) {}
giac::gen FracNode::evaluate()
{
    giac::gen left_num, right_num;
    left_num  = left->evaluate();
    right_num = right->evaluate();
    return (left_num / right_num);
}



// Unary functions

UnaryNode::UnaryNode(ExpressionNode &C) : child(&C) {}

UnaryMinusNode::UnaryMinusNode(ExpressionNode &C): UnaryNode(C) {}
giac::gen UnaryMinusNode::evaluate()
{
    return (-child->evaluate());
}

SqrtNode::SqrtNode(ExpressionNode &C): UnaryNode(C) {}
giac::gen SqrtNode::evaluate()
{
    giac::context ct; // TODO
    return (giac::sqrt(child->evaluate(), &ct));
}

FunAppNode::FunAppNode(std::vector<ExpressionNode *> args, std::string s) : arglist(args), name(s) {}
giac::gen FunAppNode::evaluate()
{
    giac::context ct;
    return (giac::gen("0", &ct)); // TODO
}


// Function Application


