#include "painter.h"

Painter::Painter(QObject *parent) :
    QObject(parent)
{
    map.fetch();
}

void Painter::init() {
    // fetch mapping
    map.fetch();

    te->append("   |");
    foreach(MappingItem mapping, map.getByNumberOrderedMapping()) {
        te->append(QString("%1|").arg(mapping.name,-3));
    }
    te->cursor().setPos(3,0);
}

void Painter::setTextEdit(QTextEdit *textEdit) {
    te = textEdit;
};

// key pressed in main window
void Painter::keyPressed(QKeyEvent *event) {
    // get charater of keyevent
    QChar c = event->text()[0];

    qDebug() << c;

    te->insertPlainText(map.getMappingForKeyIfActive(c)->character);
}
