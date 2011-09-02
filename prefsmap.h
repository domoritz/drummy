#ifndef PREFSMAP_H
#define PREFSMAP_H

#include <QWidget>

namespace Ui {
    class PrefsMap;
}

class PrefsMap : public QWidget
{
    Q_OBJECT

public:
    explicit PrefsMap(QWidget *parent = 0);
    ~PrefsMap();

private:
    Ui::PrefsMap *ui;
};

#endif // PREFSMAP_H
