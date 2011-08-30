#ifndef PAINTER_H
#define PAINTER_H

#include <QObject>
#include <QTextEdit>
#include <QTextCursor>
#include <QtDebug>
#include <QSettings>
#include <QKeyEvent>
#include "mappings.h"

class Painter : public QObject
{
    Q_OBJECT
public:
    explicit Painter(QObject *parent = 0);

protected:
    Mappings map;
    QTextEdit *te;

signals:

public slots:
    void setTextEdit(QTextEdit *te);
    void keyPressed(QKeyEvent *event);
    void init();
};

#endif // PAINTER_H
