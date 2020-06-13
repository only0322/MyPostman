#ifndef MYPOSTMAN_H
#define MYPOSTMAN_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MyPostman; }
QT_END_NAMESPACE

class MyPostman : public QMainWindow
{
    Q_OBJECT

public:
    MyPostman(QWidget *parent = nullptr);
    ~MyPostman();

private:
    Ui::MyPostman *ui;
};
#endif // MYPOSTMAN_H
