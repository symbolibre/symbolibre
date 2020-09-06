#include "FunctionBarAttached.hpp"

#include <QGuiApplication>
#include <QPalette>

FunctionKeyModel::FunctionKeyModel() : QObject(),
    m_text(), m_color(QGuiApplication::palette().mid().color())
{

}

FunctionBarAttached::FunctionBarAttached(QObject *parent) : QObject(parent),
    m_f1(nullptr), m_f2(nullptr), m_f3(nullptr), m_f4(nullptr), m_f5(nullptr)
{

}

FunctionBarAttached *FunctionBarAttached::qmlAttachedProperties(QObject *object)
{
    return new FunctionBarAttached(object);
}
