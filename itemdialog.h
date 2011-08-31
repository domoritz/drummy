#ifndef ITEMDIALOG_H
#define ITEMDIALOG_H

#include <QDialog>
#include <QTreeWidgetItem>
#include "prefsrec.h"

namespace Ui {
    class ItemDialog;
}

class ItemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ItemDialog(PrefsRec *parent = 0);
    ~ItemDialog();

protected:
    QTreeWidgetItem *item;

private:
    Ui::ItemDialog *ui;

private slots:
    void on_buttonBox_rejected();
    void on_buttonBox_accepted();

public slots:
    void setItem(QTreeWidgetItem *item, int column = -1);
};

#endif // ITEMDIALOG_H
