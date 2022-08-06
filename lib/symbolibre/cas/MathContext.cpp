// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "MathContext.hpp"

#include <iostream>
#include <string>

#include <QDebug>
#include <QDir>
#include <QJsonDocument>

#include "../util/FileSystemSingleton.hpp"


MathContext::MathContext() : QObject(), giac(), m_configPath(Fs::readWriteDataDir())
{

}

void MathContext::setConfigPath(const QString &path)
{
    if (path != m_configPath) {
        m_configPath = path;
        emit configPathChanged(path);
    }
}

QStringList MathContext::persistentVariables() const
{
    QDir varsDir(m_configPath + "/vars");
    QStringList varFiles(varsDir.entryList(QDir::Files, QDir::Name));

    QStringList allVars;

    for (const auto &varFile : varFiles) {

        QFile file(varsDir.absoluteFilePath(varFile));
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "unable to read JSON data for math variables from file " << varFile;
            continue;
        }

        auto doc = QJsonDocument::fromJson(file.readAll());
        auto vars = doc.object()["variables"].toArray();

        for (const auto &var : vars) {
            if (var.isString()) {
                allVars.append(var.toString());
            } else {
                qWarning() << "invalid JSON math variable data";
            }
        }
    }

    return allVars;
}

void MathContext::registerPersistentVariables(const QString &cat, const QStringList &vars) const
{
    Fs::createDir(m_configPath + "/vars");
    QFile file(m_configPath + "/vars/" + cat + ".json");
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        qWarning() << "unable to write persistent math variables file to " << m_configPath;
        return;
    }
    auto array = QJsonArray::fromStringList(vars);
    QJsonObject root;
    root["variables"] = array;
    file.write(QJsonDocument(root).toJson());
}

void MathContext::loadState()
{
    QFile file(m_configPath + "/cas_state.json");
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qWarning() << "no saved CAS state found";
        return;
    }
    auto doc = QJsonDocument::fromJson(file.readAll());
    if (!doc.object()["variables"].isObject()) {
        qWarning() << "unable to read CAS state file";
        return;
    }
    auto variables = doc.object()["variables"].toObject();
    for (auto it = variables.constBegin(); it != variables.constEnd(); ++it) {
        giacEvalString(it.key() + " := " + it.value().toString());
    }
}

void MathContext::saveState()
{
    QFile file(m_configPath + "/cas_state.json");
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        qWarning() << "unable to write CAS state file";
        return;
    }
    QJsonObject variables;
    for (const auto &v : persistentVariables()) {
        variables[v] = QString::fromStdString(giacEvalString(v).print());
    }
    QJsonObject root;
    root["variables"] = variables;
    file.write(QJsonDocument(root).toJson());
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
