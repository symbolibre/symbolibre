// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CONFIGRW_H
#define CONFIGRW_H


#include <QObject>
#include <QProcess>
#include <QVariant>
#include <QJsonObject>

class ConfigRW : public QObject
{
    Q_OBJECT
public:
    explicit ConfigRW(QObject *parent = nullptr);

    ConfigRW(QString path);

    Q_INVOKABLE QVariant read(QString key);
    Q_INVOKABLE void write(QString key, QVariant value);


signals:

public slots:
};



#endif // CONFIGRW_H
