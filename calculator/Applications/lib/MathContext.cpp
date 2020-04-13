#include "MathContext.hpp"
#include "SLL.hpp"
#include <QDebug>
#include <QJsonDocument>
#include <iostream>
#include <string>

MathContext::MathContext() : QObject(), sll()
{

}

QString MathContext::toGiac(const QString &json)
{
    auto doc = QJsonDocument::fromJson(json.toUtf8());
    if (!doc.isArray()) {
        qDebug() << "bad editiontree json given to toGiac()";
        return "";
    }
    return QString::fromStdString(EditionTree(doc.array()).getText());
}

EditionTree MathContext::evaluate(const EditionTree &etree, bool approx)
{
    SLL::Status status = sll.exec(etree.getText());
    status.value = sll.simplify(status.value);

    EditionTree shell = EditionTree();

    if (status.type == SLL::Status::RESULT) {
        std::string str;
        if (!approx) {
            if (SLL::giac_conv)
                copyExprAtCursor(status.value, shell); // convertion from giac version
            else
                str = sll.str(status.value); // no translation for debug
        } else {
            if (SLL::giac_conv) {
                SLL::Term approx = sll.approx(status.value, 10);
                copyExprAtCursor(approx, shell);
            } else
                str = sll.str(sll.approx(status.value, 10)); // no translation for debug
        }
        if (!SLL::giac_conv)
            shell.editStr(str);
        if (SLL::debug)
            std::cout << "--------- GOT: '";
        if (SLL::debug)
            std::cout << shell.getText() << "'" << std::endl;
    } else if (status.type == SLL::Status::SET_VARIABLE) {
        std::string msg = "variable " + status.name + " defined";
        shell.editStr(msg);
    } else if (status.type == SLL::Status::SET_FUNCTION) {
        std::string msg = "function " + status.name + " defined";
        shell.editStr(msg);
    }

    return shell;
}

QString MathContext::evaluate(const QString &json, bool approx)
{
    auto doc = QJsonDocument::fromJson(json.toUtf8());
    if (!doc.isArray()) {
        qDebug() << "bad editiontree json given to evaluate()";
        return "";
    }
    const EditionTree et(doc.array());
    EditionTree result = evaluate(et, approx);
    doc.setArray(result.serialize(true));
    return doc.toJson();
}
