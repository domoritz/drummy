/**
 * Draw/ paint the bars
 * ------------
 * This will create nice looking tag bars likte this one
 *

   |1 + 2 + 3 + 4 + |1 + 2 + 3 + 4 + |
Hh |----xx-------x--|x----------x----|
S  |----------o---o-|---o----o-------|
B  |o-------o----o--|--------o-----o-|

 * It reacts to key presses and ticks
 *
 */

#include "painter.h"
#include <QScrollBar>

Painter::Painter(QObject *parent) :
        QObject(parent)
{
    // avoid text input before first tick
    ticked = false;
}

void Painter::init() {
    counter = 6;
    barcounter = 0;

    // fetch mapping
    map.fetch();

    mainCursor = te->textCursor();

    // if te is empty (no previous recording), init painting
    if (te->toPlainText().isEmpty()) {
        drawInitLine();
    } else {
        mainCursor.movePosition(QTextCursor::Start);
        mainCursor.movePosition(QTextCursor::Right,QTextCursor::MoveAnchor,4);

        QTextCursor moveCursor;
        moveCursor = mainCursor;

        moveCursor.movePosition(QTextCursor::Left);
    }
}

void Painter::setTextEdit(QTextEdit *textEdit) {
    te = textEdit;
};

// draws the first line
void Painter::drawInitLine()
{
    QTextCursor moveCursor;
    moveCursor = mainCursor;

    moveCursor.insertText("   |\n");

    mainCursor.movePosition(QTextCursor::Up);
    mainCursor.movePosition(QTextCursor::Right,QTextCursor::MoveAnchor,4);

    foreach(MappingItem mapping, *map.getByNumberOrderedAndEnabledMapping()) {
        moveCursor.insertText(QString("%1|\n").arg(mapping.shortName,-3));
    }

    te->setTextCursor(mainCursor);
}

// key pressed in main window
void Painter::keyPressed(QKeyEvent *event)
{
    if (ticked) {
        // get charater of keyevent
        QChar c = event->text()[0];

        MappingItem *item = map.getMappingForKey(c);

        if (item->active && item->enabled && item) {
            qDebug() << c;

            QTextCursor moveCursor;
            moveCursor = mainCursor;

            moveCursor.beginEditBlock();

            moveCursor.movePosition(QTextCursor::Down,QTextCursor::MoveAnchor,map.getNumberInEnabledMapping(item->id)+1);
            moveCursor.deletePreviousChar();
            moveCursor.insertText(item->character);

            moveCursor.endEditBlock();
        }
    }
}

void Painter::tick()
{
    ticked = true;

    te->setTextCursor(mainCursor);

    QTextCursor moveCursor;
    moveCursor = mainCursor;

    highlight(false);

    // if 1 bar reached draw a line
    if (counter == 22) {
        counter = 6;
        barcounter++;

        // vertical line
        if (mainCursor.atBlockEnd()) {
            mainCursor.insertText("|");
            for (int i = 0; i < map.activeCount(); i++) {
                moveCursor.movePosition(QTextCursor::Down);
                moveCursor.insertText("|");
            }
        } else {
            mainCursor.movePosition(QTextCursor::Right);
        }

        moveCursor = mainCursor;


        // after 4 bars, new line
        if (barcounter == settings.value("barsperline",4).toInt()) {
            barcounter = 0;

            mainCursor.movePosition(QTextCursor::Down,QTextCursor::MoveAnchor,map.activeCount()+1);
            mainCursor.movePosition(QTextCursor::EndOfLine);

            if (mainCursor.atEnd()) {
                mainCursor.insertText("\n\n");
                drawInitLine();
            } else {
                mainCursor.movePosition(QTextCursor::Down,QTextCursor::MoveAnchor,2);
                mainCursor.movePosition(QTextCursor::Right,QTextCursor::MoveAnchor,4);
            }
            moveCursor = mainCursor;

            //scroll down
            te->moveCursor(QTextCursor::End);
            te->ensureCursorVisible();

            te->setTextCursor(mainCursor);
        }
    }

    //paint a new column header
    if (mainCursor.atBlockEnd()) {
        mainCursor.insertText(counter%4==0 ? "+" : (counter%2?" ":QString::number(counter/4)));
    } else {
        mainCursor.movePosition(QTextCursor::Right);
    }
    counter++;

    int column;
    foreach(MappingItem mapping, *map.getByNumberOrderedAndEnabledMapping()) {
        moveCursor.movePosition(QTextCursor::Down);
        column = moveCursor.columnNumber();

        if (mapping.active) {
            if (!moveCursor.atBlockEnd()) {
                moveCursor.deleteChar();
                moveCursor.insertText("-");
            } else {
                moveCursor.insertText("-");
            }
        } else {
            if (moveCursor.atBlockEnd()) {
                moveCursor.insertText(" ");
            }
        }

        moveCursor.movePosition(QTextCursor::StartOfLine);
        moveCursor.movePosition(QTextCursor::Right,QTextCursor::MoveAnchor,column);
    }

    highlight(true);
}

/**
 * Highlight a column by underlining the characters in the column
 */
void Painter::highlight(bool enabled)
{
    QTextCursor moveCursor;
    moveCursor = mainCursor;

    for(int i = 0; i<map.activeCount()+1 ;i++) {
        moveCursor.movePosition(QTextCursor::Left,QTextCursor::KeepAnchor);

        QTextCharFormat format;
        //format.setFontWeight(enabled ? QFont::Bold : QFont::Normal);
        format.setFontUnderline(enabled);

        moveCursor.mergeCharFormat(format);

        moveCursor.movePosition(QTextCursor::Right,QTextCursor::MoveAnchor);

        moveCursor.movePosition(QTextCursor::Down);
    }
}
