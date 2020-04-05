#include "serialization.hpp"

#include "EditionArea.hpp"
#include "Flow.hpp"
#include "Frac.hpp"
#include "InternalEditionNode.hpp"
#include "Operator.hpp"
#include "Paren.hpp"
#include "Power.hpp"
#include "Root.hpp"
#include "Sigma.hpp"

#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>

#include <memory>


QJsonArray serializeFlow(const Flow &flow)
{
    QJsonArray children;
    for (auto &node : flow.flow) {
        if (auto area = dynamic_cast<EditionArea *>(node.get()))
            children.append(QJsonValue(QString::fromStdString(area->getText())));

        else if (auto op = dynamic_cast<Operator *>(node.get()))
            children.append(QJsonObject({qMakePair(QString("name"), QString("op")), qMakePair(QString("op"), QString::fromStdString(op->getText()))}));

        else if (auto paren = dynamic_cast<Paren *>(node.get()))
            children.append(QJsonObject({qMakePair(QString("name"), QString(paren->getParenType() == LPAREN ? "lparen" : "rparen"))}));

        else if (auto intNode = dynamic_cast<InternalEditionNode *>(node.get()))
            children.append(serializeInternalNode(*intNode));

        else
            qDebug() << "serialization of unknown edition node";
    }
    return children;
}

QJsonObject serializeInternalNode(const InternalEditionNode &node)
{
    QJsonObject json;
    json["name"] = node.getNodeType();

    QJsonArray children;
    for (auto &child : const_cast<InternalEditionNode &>(node).getChildren()) {
        children.append(serializeFlow(*child));
    }
    json["content"] = children;
    return json;
}

EditionNode *deserializeInternalNode(QJsonObject node)
{
    const auto name = node["name"].toString();

    if (name == "op") {
        const auto op = node["op"].toString();
        if (op == "*")
            return new Operator('*', "×");
        else if (op == "+")
            return new Operator('+', "+");
        else if (op == "-")
            return new Operator('-', "-");
        else {
            qDebug() << "deserialization of unknown operator";
            return nullptr;
        }
    }

    else if (name == "lparen")
        return new Paren(LPAREN);

    else if (name == "rparen")
        return new Paren(RPAREN);

    InternalEditionNode *ret;
    if (name == "frac")
        ret = new Frac;
    else if (name == "pow")
        ret = new Power;
    else if (name == "sqrt")
        ret = new Root;
    else if (name == "sum")
        ret = new Sigma;
    else {
        qDebug() << "deserialization of unknown internal node";
        return nullptr;
    }

    int i = 0;
    const auto content = node["content"].toArray();
    for (auto flowNode : content) {
        *ret->getChildren()[i] = deserializeFlow(flowNode.toArray());
        ++i;
    }

    return ret;
}

Flow deserializeFlow(const QJsonArray &json)
{
    Flow flow;
    bool isEditionArea = true; // every other child should be an edition area
    for (auto child : json) {
        if (child.isString()) {
            if (!isEditionArea) {
                qDebug() << "deserialization of ill-formed flow";
                return Flow();
            }
            flow.flow.push_back(std::make_unique<EditionArea>(child.toString().toStdString()));
            isEditionArea = false;
            continue;
        }

        if (isEditionArea || !child.isObject()) {
            qDebug() << "deserialization of ill-formed flow";
            return Flow();
        }
        isEditionArea = true;

        auto obj = child.toObject();
        auto node = deserializeInternalNode(obj);

        if (!node)
            return Flow();

        flow.flow.push_back(std::unique_ptr<EditionNode>(node));
    }

    flow.edited_node = --flow.flow.end();
    return flow;
}
