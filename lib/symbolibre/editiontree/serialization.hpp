// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SERIALIZATION_HPP
#define SERIALIZATION_HPP

#include "Flow.hpp"

#include <QJsonObject>
#include <QJsonArray>

class EditionNode;
class InternalEditionNode;

QJsonArray serializeFlow(const Flow &flow, bool cursor);
QJsonObject serializeInternalNode(const InternalEditionNode &node, bool cursor);

Flow deserializeFlow(const QJsonArray &json);
EditionNode *deserializeInternalNode(QJsonObject node);

#endif // SERIALIZATION_HPP
