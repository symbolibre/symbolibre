#include "calcsheet.hpp"
#include "SLL.hpp"
#include <iostream>
#include <QFont>
#include <string>

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
    mode = 0; // Init to exact mode
}

void CalcSheet::paint(QPainter *painter)
{
    QFont font = QFont("dejavu sans mono");
    font.setStyleHint(QFont::Monospace);
    font.setHintingPreference(QFont::PreferFullHinting);
    font.setPixelSize(FONT_SIZE);
    painter->setFont(font);

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

    //painter->setPen(Qt::red);
    //QPoint cursor = editedExpression.getCursorCoordinates();
    //std::cout << cursor.x() << ' ' << cursor.y() << std::endl;
    //painter->drawEllipse(cursor.x() + BORDERSPACE,
    //                     cursor.y() + (int) height() - eaheight + BORDERSPACE, 3, 3);
    //painter->setPen(Qt::black);
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

EditionTree evaluate(EditionTree &etree, SLL::Context &sll, int mode)
{
    SLL::Status status = sll.exec(etree.getText());
    status.value = sll.simplify(status.value);

    EditionTree shell = EditionTree();

    if (status.type == SLL::Status::RESULT) {
        std::string str;
        if (mode == 0) {
            if (SLL::giac_conv)
                copyExprAtCursor(status.value, shell); // convertion from giac version
            else
                str = sll.str(status.value); // no translation for debug
        } else {
            if (SLL::giac_conv) {
                SLL::Term approx = sll.approx(status.value, 10);
                copyExprAtCursor(approx, shell);
            } else
                str = sll.str(sll.approx(status.value, 10)); // no translation for debug
        }
        if (!SLL::giac_conv)
            shell.editStr(str);
        if (SLL::debug)
            std::cout << "--------- GOT: '";
        if (SLL::debug)
            std::cout << shell.getText() << "'" << std::endl;
    } else if (status.type == SLL::Status::SET_VARIABLE) {
        std::string msg = "variable " + status.name + " defined";
        shell.editStr(msg);
    } else if (status.type == SLL::Status::SET_FUNCTION) {
        std::string msg = "function " + status.name + " defined";
        shell.editStr(msg);
    }

    return shell;
}

void CalcSheet::parenthesize()
{
    recvInput(KeyCode::SLK_LPAREN);
    recvInput(KeyCode::SLK_RPAREN);
    recvInput(KeyCode::SLK_LEFT);
}


void CalcSheet::recvInput(int /* KeyCode::keycode */ input)
{
    if (KeyCode::SLK_A <= input && input <= KeyCode::SLK_Z)
        editedExpression.editChar('A' + input - KeyCode::SLK_A);
    else if (KeyCode::SLK_a <= input && input <= KeyCode::SLK_z)
        editedExpression.editChar('a' + input - KeyCode::SLK_a);
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

        case KeyCode::SLK_SPACE:
            editedExpression.editChar(' ');
            break;
        case KeyCode::SLK_COMMA:
            editedExpression.editChar(',');
            break;
        case KeyCode::SLK_DOT:
            editedExpression.editChar('.');
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

        case KeyCode::SLK_COLONEQ:
            editedExpression.editChar(':');
            editedExpression.editChar('=');
            break;

        case KeyCode::SLK_EXE:
            results.push_back(evaluate(editedExpression, sll, mode));
            expressions.push_back(std::move(editedExpression));
            editedExpression = EditionTree();
            break;

        case KeyCode::SLK_APPROX:
            mode = 1 - mode;
            break;

    /* New keys */ //TODO : dispatch once debugged

        case KeyCode::SLK_COS:
            editedExpression.editChar('c');
            editedExpression.editChar('o');
            editedExpression.editChar('s');
            parenthesize();
            break;

        case KeyCode::SLK_SIN:
            editedExpression.editChar('s');
            editedExpression.editChar('i');
            editedExpression.editChar('n');
            parenthesize();
            break;

        case KeyCode::SLK_TAN:
            editedExpression.editChar('t');
            editedExpression.editChar('a');
            editedExpression.editChar('n');
            parenthesize();
            break;

        case KeyCode::SLK_ACOS:
            editedExpression.editChar('a');
            editedExpression.editChar('c');
            editedExpression.editChar('o');
            editedExpression.editChar('s');
            parenthesize();
            break;

        case KeyCode::SLK_ASIN:
            editedExpression.editChar('a');
            editedExpression.editChar('s');
            editedExpression.editChar('i');
            editedExpression.editChar('n');
            parenthesize();
            break;

        case KeyCode::SLK_ATAN:
            editedExpression.editChar('a');
            editedExpression.editChar('t');
            editedExpression.editChar('a');
            editedExpression.editChar('n');
            parenthesize();
            break;

        case KeyCode::SLK_PI:
            editedExpression.editChar('p');
            editedExpression.editChar('i');
            break;

        case KeyCode::SLK_SQUARE:
            recvInput(KeyCode::SLK_POWER);
            recvInput(KeyCode::SLK_2);
            recvInput(KeyCode::SLK_RIGHT);
            break;

        case KeyCode::SLK_EXP:
            recvInput(KeyCode::SLK_e);
            recvInput(KeyCode::SLK_POWER);
            break;

        case KeyCode::SLK_LN:
            editedExpression.editChar('l');
            editedExpression.editChar('n');
            parenthesize();
            break;

        case KeyCode::SLK_LOG:
            editedExpression.editChar('l');
            editedExpression.editChar('o');
            editedExpression.editChar('g');
            parenthesize();
            break;

        case KeyCode::SLK_POW10:
            recvInput(KeyCode::SLK_1);
            recvInput(KeyCode::SLK_0);
            recvInput(KeyCode::SLK_POWER);
            break;

        default:
            std::cerr << "Unsupported key " << input << std::endl;
            break;
        }

    if (SLL::debug)
        editedExpression.ascii(true);
    emit expressionChanged();
    update();
}
