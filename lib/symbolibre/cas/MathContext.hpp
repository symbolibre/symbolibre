// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef MATHCONTEXT_HPP
#define MATHCONTEXT_HPP

#include "symbolibre/editiontree.hpp"

#include <QObject>

class MathContext : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString configPath READ configPath WRITE setConfigPath NOTIFY configPathChanged)
    Q_PROPERTY(AngleMode angleMode
        READ angleMode WRITE setAngleMode NOTIFY angleModeChanged)

public:
    MathContext();

    Q_INVOKABLE QStringList persistentVariables() const;
    Q_INVOKABLE void registerPersistentVariables(const QString &cat, const QStringList &vars) const;

    const QString &configPath() const { return m_configPath; }
    void setConfigPath(const QString &path);

    giac::context * giacContext() { return &giac; }

    Q_INVOKABLE QString toGiac(const QString &json);

    giac::gen giacEvalString(const QString &expr);
    Q_INVOKABLE QString evalString(const QString &expr);
    EditionTree evalExpr(const EditionTree &etree, bool approx);
    Q_INVOKABLE QString evalExpr(const QString &json, bool approx);

    enum AngleMode { Radian, Degree };
    Q_ENUM(AngleMode)

    AngleMode angleMode() const;
    void setAngleMode(AngleMode mode);

Q_SIGNALS:
    void configPathChanged(const QString &);
    void angleModeChanged();

public:
    Q_INVOKABLE void loadState();
    Q_INVOKABLE void saveState();

private:
    giac::context giac;
    QString m_configPath;
};

#endif // MATHCONTEXT_HPP
