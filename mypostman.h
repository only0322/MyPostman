#ifndef MYPOSTMAN_H
#define MYPOSTMAN_H

#include <QMainWindow>
#include "common.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MyPostman; }
QT_END_NAMESPACE

class MyPostman : public QMainWindow
{
    Q_OBJECT

public:
    MyPostman(QWidget *parent = nullptr);
    ~MyPostman();

private slots:
    void on_btn_exit_clicked();

    void on_btn_send_clicked();

    void httpReply(QNetworkReply * reply);

private:
    Ui::MyPostman *ui;
    QNetworkAccessManager *m_accessManager;
};
#endif // MYPOSTMAN_H
