#ifndef PASSWORDDIALOG_H
#define PASSWORDDIALOG_H

#include <QDialog>
#define PASSWORD "123"

namespace Ui {
class passwordDialog;
}

class passwordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit passwordDialog(QWidget *parent = 0);
    ~passwordDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::passwordDialog *ui;

signals:
    void checkPassword(bool);
};

#endif // PASSWORDDIALOG_H
