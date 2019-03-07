#ifndef PROCESS_H
#define PROCESS_H

#include <QObject>

/*
 * This class represents an object able to interpret or compile
 * and execute code, depending on the language. It provides a
 * QML interface to make it usable in QML files, and is designed
 * to be as user friendly as possible. No knowledge of any command
 * in command-line, or of any detailed understanding of how some
 * programming languages are compiled and executed, is required.
 * It is also designed to be easily embedded within a QML window,
 * in which the user inputs can be read and forwarded to a programm,
 * and in which the outputs and errors of this programm are printed.
 */

class Process : public QObject
{
    Q_OBJECT
public:
    explicit Process(QObject *parent = nullptr);

signals:

public slots:
};

#endif // PROCESS_H
