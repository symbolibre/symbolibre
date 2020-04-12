#ifndef MATHCONTEXT_HPP
#define MATHCONTEXT_HPP

#include <EditionTree.hpp>
#include "SLL.hpp"

#include <QObject>

class MathContext : public QObject
{
    Q_OBJECT

public:
    MathContext();

    EditionTree evaluate(const EditionTree &etree, bool approx);
    Q_INVOKABLE QString evaluate(const QString &json, bool approx);

private:
    SLL::Context sll;
};

#endif // MATHCONTEXT_HPP
