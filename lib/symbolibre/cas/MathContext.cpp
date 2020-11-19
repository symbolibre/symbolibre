// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

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
    giac::gen f;
    std::string err = "";

    try {
        f = giac::eval(giac::gen(expr.toStdString(), &giac), &giac);
        err = giac::parser_error(&giac);
    }
    catch (std::exception &e) {
        err = std::string(e.what());
    }

    if (err != "") {
        f = giac::string2gen(err, false);
        f.subtype = -1;
        // Clear parser error for next operation
        giac::parser_error("", &giac);
    }

    return f;
}

QString MathContext::evalString(const QString &expr)
{
    giac::gen f = giacEvalString(expr);

    // Only forward error messages
    if (f.type == giac::_STRNG && f.subtype == -1)
        return QString::fromStdString(*f.ref_STRNGptr());

    return "";
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

MathContext::AngleMode MathContext::angleMode() const
{
    return static_cast<AngleMode>(giac::angle_mode(&giac));
}

void MathContext::setAngleMode(AngleMode mode)
{
    if (mode == angleMode())
        return;

    giac::angle_mode(mode, &giac);
    emit angleModeChanged();
}
