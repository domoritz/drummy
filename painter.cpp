#include "painter.h"

Painter::Painter(QObject *parent) :
        QObject(parent)
{
    map.fetch();
}

void Painter::init() {
    counter = 2;
    barcounter = 0;

    // fetch mapping
    map.fetch();

    newLine();
}

void Painter::setTextEdit(QTextEdit *textEdit) {
    te = textEdit;
};

void Painter::newLine() {
    te->append("   |");
    foreach(MappingItem mapping, *map.getByNumberOrderedMapping()) {
        te->append(QString("%1|").arg(mapping.name,-3));
    }

    up(map.count);
    eol();
}

// key pressed in main window
void Painter::keyPressed(QKeyEvent *event) {
    // get charater of keyevent
    QChar c = event->text()[0];

    MappingItem *item = map.getMappingForKey(c);

    if (item->active && item) {
        qDebug() << c;

        down(item->number+1);

        te->textCursor().deletePreviousChar();
        te->insertPlainText(item->character);

        up(item->number+1);
    }
}

void Painter::tick() {

    // if 1 bar reached draw a line
    if (counter == 10) {
        counter = 2;
        barcounter++;
        // horizontal line
        for (int i = 0; i < map.count+1; i++) {
            te->insertPlainText("|");
            down(1);
        }
        up(map.count);
    }

    // after 4 bars, new line
    if (barcounter == 4) {
        barcounter = 0;
        end();
        te->append("");
        end();
        newLine();
    }

    //paint a new column
    te->insertPlainText(counter%2 ? "+" : QString::number(counter/2));
    counter++;

    foreach(MappingItem mapping, *map.getByNumberOrderedMapping()) {
        down(1);
        if (mapping.active) {
            te->insertPlainText("-");
        } else {
            te->insertPlainText(" ");
        }
    }

    up(map.count);
}

void Painter::up(int n) {
    cursor = te->textCursor();

    cursor.movePosition(QTextCursor::Up,QTextCursor::MoveAnchor,n);

    te->setTextCursor(cursor);
}

void Painter::down(int n) {
    cursor = te->textCursor();

    cursor.movePosition(QTextCursor::Down,QTextCursor::MoveAnchor,n);

    te->setTextCursor(cursor);
}

void Painter::left(int n) {
    cursor = te->textCursor();

    cursor.movePosition(QTextCursor::Left,QTextCursor::MoveAnchor,n);

    te->setTextCursor(cursor);
}

void Painter::eol() {
    cursor = te->textCursor();

    cursor.movePosition(QTextCursor::EndOfLine);

    te->setTextCursor(cursor);
}

void Painter::end() {
    cursor = te->textCursor();

    cursor.movePosition(QTextCursor::Down,QTextCursor::MoveAnchor,map.count+1);
    cursor.movePosition(QTextCursor::EndOfLine);

    te->setTextCursor(cursor);
}
