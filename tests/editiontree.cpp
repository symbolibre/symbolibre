#include <symbolibre/editiontree.hpp>
#include <symbolibre/MathContext.hpp>
#include <QtTest>

class EditionTreeTest : public QObject
{
    Q_OBJECT

private slots:
    void serialization()
    {
        QFETCH(QString, input);
        QFETCH(bool, cursor);
        auto doc = QJsonDocument::fromJson(input.toUtf8());
        auto result = QJsonDocument(EditionTree(doc.array()).serialize(cursor));
        QVERIFY(doc.isArray());
        QCOMPARE(result.toJson(QJsonDocument::Compact), doc.toJson(QJsonDocument::Compact));
    }

    void serialization_data()
    {
        QTest::addColumn<QString>("input");
        QTest::addColumn<bool>("cursor");
        QTest::newRow("addition") << R"(["1",{"type": "op", "op": "+"},"2"])" << false;
        QTest::newRow("sqrt") << R"(["",{"type": "sqrt", "content": [["2"]]},""])" << false;
    }

    void evaluation()
    {
        QFETCH(QString, input);
        QFETCH(QString, expected);
        MathContext ctx;
        QCOMPARE(ctx.evaluate(input, false), expected.toUtf8());
    }

    void evaluation_data()
    {
        QTest::addColumn<QString>("input");
        QTest::addColumn<QString>("expected");
        QTest::newRow("addition") << R"(["1",{"type": "op", "op": "+"},"2"])" << R"(["3"])";
    }
};

QTEST_MAIN(EditionTreeTest)

#include "editiontree.moc"
