#ifndef SONGINFO_H
#define SONGINFO_H

#include <QWidget>
#include <QString>

namespace Ui {
    class SongInfo;
}

class SongInfo : public QWidget
{
    Q_OBJECT

public:
    explicit SongInfo(QWidget *parent = 0);
    ~SongInfo();

private:
    Ui::SongInfo *ui;


public slots:
    QString stringifyInfos();
    void clearUi();

};

#endif // SONGINFO_H
