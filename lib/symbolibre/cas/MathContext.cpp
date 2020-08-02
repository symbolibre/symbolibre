#include "MathContext.hpp"
#include <QDebug>
#include <QJsonDocument>
#include <iostream>
#include <string>

MathContext::MathContext() : QObject(), giac()
{

}

QString MathContext::toGiac(const QString &json)
{
    auto doc = QJsonDocument::fromJson(json.toUtf8());
    if (!doc.isArray()) {
        qDebug() << "bad editiontree json given to toGiac()";
        return "";
    }
    return EditionTree(doc.array()).getText();
}

giac::gen MathContext::giacEvalString(const QString &expr)
{
    return giac::eval(giac::gen(expr.toStdString(), &giac), &giac);
}

void MathContext::evalString(const QString &expr)
{
    giacEvalString(expr);
}

EditionTree MathContext::evalExpr(const EditionTree &etree, bool approx)
{
    /* TODO: Move this parameters to a suitable place */
    constexpr bool pretty_print = true;
    constexpr bool debug = false;

    /* TODO: Distinguish result, variable definition and function definition to
       produce different messages (this was previously done by SLL) */
    auto value = giacEvalString(etree.getText());
    value = giac::_simplify(value, &giac);

    EditionTree shell = EditionTree();

    /* For a variable definition, we would have...
         shell.editStr("variable " + name + " defined");
       Currently Giac returns the value */

    std::string str;
    if (!approx) {
        if (pretty_print)
            copyExprAtCursor(value, shell);
        else
            str = value.print(NULL);
    } else {
        /* TODO: Approximation precision */
        auto approx = giac::_evalf(value, &giac);

        if (pretty_print)
            copyExprAtCursor(approx, shell);
        else
            str = approx.print(NULL);
    }

    if (!pretty_print)
        shell.editStr(str);
    if (debug)
        std::cout << "--------- GOT: '";
    if (debug)
        std::cout << shell.getText().toStdString() << "'" << std::endl;

    return shell;
}

QString MathContext::evalExpr(const QString &json, bool approx)
{
    auto doc = QJsonDocument::fromJson(json.toUtf8());
    if (!doc.isArray()) {
        qDebug() << "bad editiontree json given to evaluate()";
        return "";
    }
    const EditionTree et(doc.array());
    EditionTree result = evalExpr(et, approx);
    doc.setArray(result.serialize(false));
    return doc.toJson(QJsonDocument::Compact);
}
