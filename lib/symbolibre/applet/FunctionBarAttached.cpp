#include "FunctionBarAttached.hpp"

#include <QGuiApplication>
#include <QPalette>

FunctionKeyModel::FunctionKeyModel() : QObject(),
    m_text(), m_color(QGuiApplication::palette().mid().color())
{

}

FunctionBarAttached::FunctionBarAttached(QObject *parent) : QObject(parent),
    m_f1(nullptr), m_f2(nullptr), m_f3(nullptr), m_f4(nullptr), m_f5(nullptr),
    m_combine(false)
{

}

FunctionKeyModel *FunctionBarAttached::getF(int key) const
{
    switch (key) {
    case 1: return m_f1;
    case 2: return m_f2;
    case 3: return m_f3;
    case 4: return m_f4;
    case 5: return m_f5;
    default: return nullptr;
    }
}

void FunctionBarAttached::setF(int key, FunctionKeyModel *model)
{
    switch (key) {
    case 1: m_f1 = model; break;
    case 2: m_f2 = model; break;
    case 3: m_f3 = model; break;
    case 4: m_f4 = model; break;
    case 5: m_f5 = model; break;
    }
}

bool FunctionBarAttached::combine() const
{
    return m_combine;
}

void FunctionBarAttached::setCombine(bool combine)
{
    bool changes = m_combine != combine;
    m_combine = combine;

    if (changes)
        emit changed();
}

FunctionBarAttached *FunctionBarAttached::qmlAttachedProperties(QObject *object)
{
    return new FunctionBarAttached(object);
}
