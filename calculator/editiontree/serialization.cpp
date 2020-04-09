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


QJsonArray serializeFlow(const Flow &flow, bool cursor)
{
    QJsonArray children;
    for (auto &node : flow.flow) {
        if (auto area = dynamic_cast<EditionArea *>(node.get())) {
            if (cursor && flow.edited_node->get() == node.get())
                children.append(QJsonObject({
                qMakePair(QString("type"), QString("selected_text")),
                qMakePair(QString("text"), QString::fromStdString(area->getText())),
                qMakePair(QString("cursor"), area->getCursorPos())
            }));
            else
                children.append(QJsonValue(QString::fromStdString(area->getText())));
        }

        else if (auto op = dynamic_cast<Operator *>(node.get()))
            children.append(QJsonObject({
            qMakePair(QString("type"), QString("op")),
            qMakePair(QString("op"), QString::fromStdString(op->getText()))}));

        else if (auto paren = dynamic_cast<Paren *>(node.get()))
            children.append(QJsonObject({
            qMakePair(
                QString("type"),
                QString(paren->getParenType() == LPAREN ? "lparen" : "rparen"))
        }));

        else if (auto intNode = dynamic_cast<InternalEditionNode *>(node.get())) {
            children.append(serializeInternalNode(*intNode, cursor && flow.edited_node->get() == node.get()));
        }

        else
            qDebug() << "serialization of unknown edition node";
    }
    return children;
}

QJsonObject serializeInternalNode(const InternalEditionNode &node, bool cursor)
{
    QJsonObject json;
    json["type"] = node.getNodeType();

    QJsonArray children;
    for (auto &child : const_cast<InternalEditionNode &>(node).children) {
        children.append(serializeFlow(child, cursor && &child == node.getActiveChild()));
    }
    json["content"] = children;
    if (cursor)
        json["cursor"] = node.active_child_idx;

    return json;
}

EditionNode *deserializeInternalNode(QJsonObject node)
{
    const auto name = node["type"].toString();

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
        ret->children[i] = deserializeFlow(flowNode.toArray());
        ++i;
    }
    ret->active_child_idx = node["cursor"].toInt();

    return ret;
}

Flow deserializeFlow(const QJsonArray &json)
{
    Flow flow;
    flow.flow.clear();
    flow.edited_node = flow.flow.end();

    bool isEditionArea = true; // every other child should be an edition area
    for (auto child : json) {
        if (isEditionArea) {
            if (child.isString()) {
                flow.flow.push_back(std::make_unique<EditionArea>(child.toString().toStdString()));
            }

            else if (child.isObject() && child.toObject()["type"] == "selected_text") {
                const auto text = child.toObject()["text"].toString().toStdString();
                const auto cursor_pos = child.toObject()["cursor"].toInt();
                flow.flow.push_back(std::make_unique<EditionArea>(text, cursor_pos));
                flow.edited_node = --flow.flow.end();
            }

            else {
                qDebug() << "deserialization of ill-formed flow";
                return Flow();
            }

            isEditionArea = false;
            continue;
        }

        if (!child.isObject()) {
            qDebug() << "deserialization of ill-formed flow";
            return Flow();
        }
        isEditionArea = true;

        const auto obj = child.toObject();
        auto node = deserializeInternalNode(obj);

        if (!node)
            return Flow();

        flow.flow.push_back(std::unique_ptr<EditionNode>(node));

        if (!obj["cursor"].isUndefined())
            flow.edited_node = --flow.flow.end();
    }

    if (flow.flow.size() % 2) {
        qDebug() << "deserialization of ill-formed flow";
        return Flow();
    }

    if (flow.edited_node == flow.flow.end())
        --flow.edited_node;

    return flow;
}
