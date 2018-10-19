#ifndef PRETTYPRINTER_H
#define PRETTYPRINTER_H

#include <QWidget>

class PrettyPrinter : public QWidget
{
    Q_OBJECT

public:

    PrettyPrinter(QString top_entry, QString bot_entry, QWidget *parent = 0);
    QString top;
    QString bot;

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // PRETTYPRINTER_H
