#include <symbolibre/editiontree.hpp>
#include <symbolibre/cas/MathContext.hpp>
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
        QTest::newRow("addition")
            << R"(["1",{"type": "op", "op": "+"},"2"])" << false;
        QTest::newRow("sqrt")
            << R"(["",{"type": "sqrt", "content": [["2"]]},""])" << false;
        QTest::newRow("frac")
            << R"(["",{"type": "frac", "content": [["6"],["8"]]},""])" << false;
        QTest::newRow("frac_plus")
            << R"(["",{"type": "frac", "content": [["5"],
                ["7", {"type": "op", "op": "+"}, "5"]]},""])" << false;
        QTest::newRow("cos")
            << R"(["cos", {"type": "lparen"}, "0", {"type": "rparen"}, ""])" << false;
        QTest::newRow("int")
            << R"(["",{"content":[["x²",{"op":"+","type":"op"},"1"],[""]],"type":"int"},""])" << false;
    }

    void evaluation()
    {
        QFETCH(QString, input);
        QFETCH(QString, expected);
        MathContext ctx;
        QJsonDocument out = QJsonDocument::fromJson(ctx.evalExpr(input, false).toUtf8());
        QCOMPARE(out, QJsonDocument::fromJson(expected.toUtf8()));
    }

    void evaluation_data()
    {
        QTest::addColumn<QString>("input");
        QTest::addColumn<QString>("expected");
        QTest::newRow("addition")
            << R"(["1",{"type": "op", "op": "+"},"2"])"
            << R"(["3"])";
        QTest::newRow("sqrt")
            << R"(["",{"type": "sqrt", "content": [["2"]]},""])"
            << R"(["",{"type": "sqrt", "content": [["2"]]},""])";
        QTest::newRow("frac")
            << R"(["",{"type": "frac", "content": [["6"],["8"]]},""])"
            << R"(["",{"type": "frac", "content": [["3"],["4"]]},""])";
        QTest::newRow("frac_plus")
            << R"(["",{"type": "frac", "content": [["5"],
                ["7", {"type": "op", "op": "+"}, "5"]]},""])"
            << R"(["",{"type": "frac", "content": [["5"],["12"]]},""])";
        QTest::newRow("cos")
            << R"(["cos", {"type": "lparen"}, "0", {"type": "rparen"}, ""])"
            << R"(["1"])";
        QTest::newRow("int")
            << R"(["",{"content":[["x²",{"op":"+","type":"op"},"1"],[""]],"type":"int"},""])"
            << R"(["",{"content":[["x",{"content":[["3"]],"type":"pow"},"",
                {"op":"+","type":"op"},"3",{"op":"*","type":"op"},"x"],["3"]],"type":"frac"},""])";
    }
};

QTEST_MAIN(EditionTreeTest)

#include "editiontree.moc"
