#ifndef MATHCONTEXT_HPP
#define MATHCONTEXT_HPP

#include "symbolibre/editiontree.hpp"
#include <giac/config.h>
#include <giac/giac.h>

#include <QObject>

class MathContext : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int angleMode
        READ angleMode WRITE setAngleMode NOTIFY angleModeChanged)

public:
    MathContext();

    giac::context * giacContext() { return &giac; }

    Q_INVOKABLE QString toGiac(const QString &json);

    giac::gen giacEvalString(const QString &expr);
    Q_INVOKABLE void evalString(const QString &expr);
    EditionTree evalExpr(const EditionTree &etree, bool approx);
    Q_INVOKABLE QString evalExpr(const QString &json, bool approx);

    int angleMode() const;
    void setAngleMode(int mode);

Q_SIGNALS:
    void angleModeChanged();

private:
    giac::context giac;
};

#endif // MATHCONTEXT_HPP
