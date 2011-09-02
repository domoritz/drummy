#ifndef PREVIEWDIALOG_H
#define PREVIEWDIALOG_H

#include <QDialog>
#include <QString>
#include <QSettings>

namespace Ui {
    class PreviewDialog;
}

class PreviewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreviewDialog(QWidget *parent = 0);
    ~PreviewDialog();

private:
    Ui::PreviewDialog *ui;
    QSettings settings;

public slots:
    void setText(QString text);
    void readFont();
};

#endif // PREVIEWDIALOG_H
