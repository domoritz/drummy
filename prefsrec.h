#ifndef PREFSREC_H
#define PREFSREC_H

#include <QWidget>

namespace Ui {
    class PrefsRec;
}

class PrefsRec : public QWidget
{
    Q_OBJECT

public:
    explicit PrefsRec(QWidget *parent = 0);
    ~PrefsRec();

private:
    Ui::PrefsRec *ui;
};

#endif // PREFSREC_H
