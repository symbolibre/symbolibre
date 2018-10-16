#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <memory>

/* Resources
Tutorial here http://web.eecs.utk.edu/~bvz/teaching/cs461Sp11/notes/parse_tree/
I took inspiration from the files here in order to have a minimal example.
Of course, the stucture of our calculator is going to evolve */

/* Here we define our classes, and which object they contains. Their methods are declared here, but defined in expression.cpp */



/* We create a general class for any Node. Then, any kind of node will heritate from this class.
Since nodes in our example will only consist in operations or numbers, this is the only overall class we need */
class ExpressionNode
{
public :
    /* Here we declare the method evaluate. This method is virtual since any subclass will have its own version (one version for plus
    , one for minus, one for a number, etc) */
    virtual float evaluate() = 0;
};



// Atomic elements

class number_node : public ExpressionNode
{
protected:
    float num;

public:
    number_node(float value); // This is the class constructor: given a float value it creates a number_node element
    virtual float evaluate() override; // Look at expression.cpp for the implementation
};

// For empty tokens (we parse incomplete expressions)
class empty_node : public ExpressionNode
{
protected:
    float num;

public:
    empty_node(float value); // This is the class constructor: given a float value it creates a number_node element
    virtual float evaluate() override; // Look at expression.cpp for the implementation
};



// Binary operators

/*We create a generic class for all operators in order to factorise code
An operator node consists in two children, and the operation which is going to be performed on both children */
class operator_node : public ExpressionNode
{
protected: // Protected here, not private, because we want childs to be able to access to these elements
    std::unique_ptr<ExpressionNode> left;
    std::unique_ptr<ExpressionNode> right;
public:
    operator_node(ExpressionNode &L, ExpressionNode &R); // class initialisator
};


// Each operator is a subclass of the big class operato_node
class plus_node : public operator_node
{
public:
    plus_node(ExpressionNode &L, ExpressionNode &R);
    virtual float evaluate() override;
};

class times_node : public operator_node
{
public:
    times_node(ExpressionNode &L, ExpressionNode &R);
    virtual float evaluate() override;
};

class minus_node : public operator_node
{
public:
    minus_node(ExpressionNode &L, ExpressionNode &R);
    virtual float evaluate() override;
};

class frac_node : public operator_node
{
public:
    frac_node(ExpressionNode &L, ExpressionNode &R);
    virtual float evaluate() override;
};



// Unary operators (only minus for now, but there will be log, integration, etc)

class unary_node : public ExpressionNode
{
protected:
    ExpressionNode *child;
public:
    unary_node(ExpressionNode &C);
};

class unary_minus_node : public unary_node
{
public:
    unary_minus_node(ExpressionNode &C);
    virtual float evaluate() override;
};

#endif // EXPRESSION_H
