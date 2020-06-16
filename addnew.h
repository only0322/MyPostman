#ifndef ADDNEW_H
#define ADDNEW_H

#include <QMainWindow>

namespace Ui {
class AddNew;
}

class AddNew : public QMainWindow
{
    Q_OBJECT

public:
    explicit AddNew(QWidget *parent = nullptr);
    ~AddNew();

private slots:
    void on_btn_confirm_clicked();

    void on_btn_return_clicked();

private:
    Ui::AddNew *ui;
};

#endif // ADDNEW_H
