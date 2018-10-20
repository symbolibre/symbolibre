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
    //  bool cursor_node;

public: /* methods */
    virtual void ascii(int shift, bool contains_cursor);

    virtual void drop_cursor(movedir dir); /* specific to cursor repositioning */
    /* Administrative : two methods that tells if the cursor has space
     * to move right/left. */
    virtual bool empty(void);
    virtual bool reachedRIGHT(void);
    virtual bool reachedLEFT(void);

    /* All edition methods return 'true' if they were able to do the edition
     * locally, and return 'false' if it has to be done above in the e.t.
     * The cursor is always either on a text node or between two non-text
     * nodes. */
    virtual bool editMOVERIGHT(void);
    virtual bool editMOVELEFT(void);
    virtual bool editMOVEUP(void);
    virtual bool editMOVEDOWN(void);

    /* About some delete actions */
    virtual bool editDELETE(void); /* A first violent delete. */
    virtual bool editDIGIT(int digit);
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
    void ascii(int shift, bool contains_cursor) override;
    void drop_cursor(movedir dir)               override;
    bool editDIGIT(int digit)                   override;

    /* Administrative : */
    bool empty(void) override;
    bool reachedRIGHT(void) override;
    bool reachedLEFT(void) override;

    /* About moving the cursor */
    bool editMOVERIGHT(void) override;
    bool editMOVELEFT(void) override;
    bool editMOVEUP(void) override;
    bool editMOVEDOWN(void) override;

    /* About deleting */
    bool editDELETE(void)    override;
};

class TextNode : public EditionTree
/* Text nodes are leaf, thus do not contains any EditionTree as children */
{
protected:
    std::string text;

public:
    TextNode();
    void ascii(int shift, bool contains_cursor) override;
    void drop_cursor(movedir dir)               override;

    /* Administrative : */
    bool empty(void) override;
    bool reachedRIGHT(void) override;
    bool reachedLEFT(void) override;

    /* About moving the cursor */
    bool editMOVERIGHT(void) override;
    bool editMOVELEFT(void) override;
    bool editMOVEUP(void) override;
    bool editMOVEDOWN(void) override;

    /* About deleting */
    bool editDELETE(void) override;

    bool editDIGIT(int digit) override;
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
