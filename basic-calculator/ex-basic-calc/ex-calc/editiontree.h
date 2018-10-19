#ifndef EDITIONTREE_H
#define EDITIONTREE_H

#include <iostream>
#include <memory>
#include <list>
#include <string>

/* There are two components : flows and nodes.
 *
 *
 * TODO : explain */

enum movedir { MNONE, MRESET, MLEFT, MRIGHT, MUP, MDOWN };
enum nodetype { NONE, ROOT, FLOW, TEXT };

class EditionTree /* Edition tree represents a set of class, and should
                                     * not be used alone. */
{
public: /* attributes */
    nodetype ntype;

    int width;
    int height;
    int center_height;

    int cursor_pos;
    EditionTree *cursor_node;

public: /* methods */
    virtual void ascii(int shift);

    virtual void drop_cursor(movedir dir); /* specific to cursor repositioning */

    /* All edition methods returns a pointer to the node containing the cursor,
     * or NULL if the cursor must change of node. */
    virtual EditionTree *editMOVE(movedir dir);
    virtual EditionTree *editDIGIT(int digit);
    //virtual void editDELETE(void); TODO
};

class FlowNode : public EditionTree
/* These are generic nodes - containing one single list of consecutive other nodes */
{
private:
    std::list<std::unique_ptr<EditionTree>> flow;
    std::list<std::unique_ptr<EditionTree>>::iterator itcursor_pos;
    bool between;

public:
    FlowNode(nodetype arg_ntype = FLOW);
    void ascii(int shift) override;
    void drop_cursor(movedir dir)            override;
    EditionTree *editDIGIT(int digit)  override;
    EditionTree *editMOVE(movedir dir) override;
};

class TextNode : public EditionTree
/* Text nodes are leaf, thus do not contains any EditionTree as children */
{
protected:
    std::string text;

public:
    TextNode();
    void ascii(int shift) override;
    void drop_cursor(movedir dir)      override;
    EditionTree *editMOVE(movedir dir) override;
    EditionTree *editDIGIT(int digit)  override;
};

/*
class FracNode : public EditionTree
{
    protected:
        FlowNode numerator;
        FlowNode denominator;

    public:
        FracNode(void);
}; */

#endif // EDITIONTREE_H
