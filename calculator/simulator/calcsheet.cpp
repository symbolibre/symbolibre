#include "calcsheet.hpp"
#include "SLL.hpp"
#include <iostream>
#include <QFont>
#include <string>

#include <giac/config.h>
#include <giac/giac.h>

QColor _SL_LIGHT_GRAY(230, 230, 230);
QColor _SL_DARK_GRAY(96, 96, 96);

/*static __attribute__((constructor)) void _(void)
{
    _SL_LIGHT_GRAY.red = ...;
}*/

CalcSheet::CalcSheet(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    editedExpression = EditionTree();
    cursorPos = -1;
}

void CalcSheet::paint(QPainter *painter)
{
    QFont font = QFont("dejavu sans mono");
    font.setStyleHint(QFont::Monospace);
    font.setHintingPreference(QFont::PreferFullHinting);
    font.setPixelSize(FONT_SIZE);
    painter->setFont(font);
    //painter->rotate(5);

    recomputeDimensions(painter, false);

    paintEditedArea(painter);
    paintList(painter);
    return;
}

qreal CalcSheet::editedAreaHeight(void)
{
    return std::max(2 * BORDERSPACE + editedExpression.getHeight(), DEFAULT_EDITED_AREA_SIZE);
}

void CalcSheet::paintEditedArea(QPainter *painter)
{
    int eaheight = editedAreaHeight();
    painter->setPen(_SL_DARK_GRAY);
    painter->drawRect(0, height() - eaheight, width() - 1, eaheight);

    painter->setPen(Qt::red);
    QPoint cursor = editedExpression.getCursorCoordinates();
    std::cout << cursor.x() << ' ' << cursor.y() << std::endl;
    painter->drawEllipse(cursor.x() + BORDERSPACE,
                         cursor.y() + (int) height() - eaheight + BORDERSPACE, 3, 3);
    painter->setPen(Qt::black);
    editedExpression.draw(BORDERSPACE, (int) height() - eaheight + BORDERSPACE,
                          *painter, true);

    return;
}

void CalcSheet::paintList(QPainter *painter)
{
    if (expressions.empty())
        return;

    auto resIt  = --results.end();
    auto exprIt = --expressions.end();

    painter->setBrush(QBrush(_SL_LIGHT_GRAY));
    int y = height() - editedAreaHeight() + BACKSPACE - BORDERSPACE - 1;
    int color_parity = 1;
    do {
        y -=  BACKSPACE + resIt->getHeight();
        if (color_parity) {
            painter->setPen(_SL_LIGHT_GRAY);
            painter->drawRect(0, y  - BACKSPACE / 2, width(), BACKSPACE + resIt->getHeight());
            painter->setPen(Qt::black);
        }
        resIt->draw(width() - resIt->getWidth() - BORDERSPACE, y, *painter, false);

        y -=  BACKSPACE + exprIt->getHeight();
        if (color_parity) {
            painter->setPen(_SL_LIGHT_GRAY);
            painter->drawRect(0, y  - BACKSPACE / 2, width(), BACKSPACE + exprIt->getHeight());
            painter->setPen(Qt::black);
        }
        exprIt->draw(BORDERSPACE, y, *painter, false);
        if (exprIt == expressions.begin())
            break;
        resIt --;
        exprIt --;
        color_parity ^= 1;
    } while (y >= 0);

    painter->setBrush(QBrush(Qt::white));

    return;
}

void CalcSheet::recomputeDimensions(QPainter *painter, bool hard)
{
    editedExpression.computeDimensions(*painter);

    if (results.empty())
        return;

    auto resIt  = --results.end();
    auto exprIt = --expressions.end();
    if (hard)
        do {
            resIt->computeDimensions(*painter);
            exprIt->computeDimensions(*painter);
        } while (exprIt -- != expressions.begin() && resIt -- != results.begin());
    else
        resIt->computeDimensions(*painter);

    return;
}


EditionTree pseudo_evaluation(EditionTree &etree)
{
    giac::context ctx;
    SLL::Term term(etree.getText());

    EditionTree shell = EditionTree();
    //copyExprAtCursor(term, shell); // convertion from giac version
    std::string str = term.str();
    shell.editStr(str);
    std::cout << "--------- GOT: '";
    std::cout << shell.getText() << "'" << std::endl;
    return shell;
}

void CalcSheet::recvInput(int /* KeyCode::keycode */ input)
{
    std::string lol = "un deux trois lol !";
    //SLL::Context empty_context;

    if (KeyCode::SLK_A <= input && input <= KeyCode::SLK_Z)
        editedExpression.editChar('A' + input - KeyCode::SLK_A);
    else if (KeyCode::SLK_0 <= input && input <= KeyCode::SLK_9)
        editedExpression.editChar('0' + input - KeyCode::SLK_0);
    else
        switch (input) {
        /* Mathematical Operands */
        case KeyCode::SLK_PLUS:
            editedExpression.editOperator('+', QString("+"));
            break;
        case KeyCode::SLK_MINUS:
            editedExpression.editOperator('-', QString("-"));
            break;
        case KeyCode::SLK_TIMES:
            editedExpression.editOperator('*', QString("×"));
            break;
        case KeyCode::SLK_DIVIDE:
            editedExpression.editFrac();
            break;
        case KeyCode::SLK_LPAREN:
            editedExpression.editParen(LPAREN);
            break;
        case KeyCode::SLK_RPAREN:
            editedExpression.editParen(RPAREN);
            break;
        case KeyCode::SLK_SQUAREROOT:
            editedExpression.editRoot();
            break;
        case KeyCode::SLK_POWER:
            editedExpression.editPower();
            break;
        case KeyCode::SLK_SIGMA:
            editedExpression.editSigma();
            break;

        /* Arrows */
        case KeyCode::SLK_UP:
            if (!editedExpression.editMoveUp())
                std::cerr << "Cannot move UP" << std::endl;
            break;
        case KeyCode::SLK_DOWN:
            if (!editedExpression.editMoveDown())
                std::cerr << "Cannot move DOWN" << std::endl;
            break;
        case KeyCode::SLK_RIGHT:
            if (!editedExpression.editMoveRight())
                std::cerr << "Cannot move RIGHT" << std::endl;
            break;
        case KeyCode::SLK_LEFT:
            if (!editedExpression.editMoveLeft())
                std::cerr << "Cannot move LEFT" << std::endl;
            break;

        /* Special keys */
        case KeyCode::SLK_DEL:
            editedExpression.editDelete();
            break;
        case KeyCode::SLK_CLEAR:
            editedExpression.editClear();
            break;

        case KeyCode::SLK_EXE:
            //lol = evaluate(editedExpression.getText());
            results.push_back(pseudo_evaluation(editedExpression));
            //results.push_back(EditionTree(lol));
            expressions.push_back(std::move(editedExpression));
            //results.push_back(editedExpression);
            editedExpression = EditionTree();
            break;

        default:
            std::cerr << "Unsupported key " << input << std::endl;
            break;
        }
    editedExpression.ascii(true);
    emit expressionChanged();
    update();
}
