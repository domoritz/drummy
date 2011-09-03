#ifndef PAINTER_H
#define PAINTER_H

#include <QObject>
#include <QTextEdit>
#include <QTextCursor>
#include <QtDebug>
#include <QSettings>
#include <QKeyEvent>
#include "mappings.h"
#include <QTextDocument>

class Painter : public QObject
{
    Q_OBJECT
public:
    explicit Painter(QObject *parent = 0);

protected:
    Mappings map;
    QTextEdit *te;
    QTextDocument *document;
    QTextCursor mainCursor;
    short int counter;
    short int barcounter;
    QSettings settings;

private:
    bool ticked;

signals:

public slots:
    void setTextEdit(QTextEdit *te);
    void drawInitLine();
    void keyPressed(QKeyEvent *event);
    void init();
    void tick();
    void highlight(bool enabled);

};

#endif // PAINTER_H
