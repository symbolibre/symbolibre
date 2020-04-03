#ifndef SERIALIZATION_HPP
#define SERIALIZATION_HPP

#include "Flow.hpp"

#include <QJsonObject>
#include <QJsonArray>

class EditionNode;
class InternalEditionNode;

QJsonArray serializeFlow(const Flow &flow);
QJsonObject serializeInternalNode(const InternalEditionNode &node);

Flow deserializeFlow(const QJsonArray &json);
EditionNode *deserializeInternalNode(QJsonObject node);

#endif // SERIALIZATION_HPP
