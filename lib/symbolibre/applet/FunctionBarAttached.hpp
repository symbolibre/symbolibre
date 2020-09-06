#ifndef SL_FUNCTIONBARATTACHED_HPP
#define SL_FUNCTIONBARATTACHED_HPP

#include <QColor>
#include <QQmlEngine>

class FunctionKeyModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString text MEMBER m_text NOTIFY textChanged)
    Q_PROPERTY(QColor color MEMBER m_color NOTIFY colorChanged)

public:
    FunctionKeyModel();

signals:
    void activated();
    void textChanged(QString text);
    void colorChanged(QColor color);

private:
    QString m_text;
    QColor m_color;
};

class FunctionBarAttached : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(FunctionBar)
    QML_UNCREATABLE("FunctionBarAttached is meant to be used as an attached property only.")
    QML_ATTACHED(FunctionBarAttached)

    Q_PROPERTY(FunctionKeyModel *f1 MEMBER m_f1 NOTIFY changed)
    Q_PROPERTY(FunctionKeyModel *f2 MEMBER m_f2 NOTIFY changed)
    Q_PROPERTY(FunctionKeyModel *f3 MEMBER m_f3 NOTIFY changed)
    Q_PROPERTY(FunctionKeyModel *f4 MEMBER m_f4 NOTIFY changed)
    Q_PROPERTY(FunctionKeyModel *f5 MEMBER m_f5 NOTIFY changed)

public:
    FunctionBarAttached(QObject *parent = nullptr);

    static FunctionBarAttached *qmlAttachedProperties(QObject *object);

signals:
    void changed();

private:
    FunctionKeyModel *m_f1, *m_f2, *m_f3, *m_f4, *m_f5;
};

#endif // SL_FUNCTIONBARATTACHED_HPP
