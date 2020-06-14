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

    void RequestPOST(QString BaseUrl);
    void RequestGET(QString BaseUrl);

    void TableViewInit(); //初始化表头


    void TableClear();      //清空tableview的数据

private slots:
    void on_btn_exit_clicked();

    void on_btn_send_clicked();

    void httpReply(QNetworkReply * reply);

    void on_btn_add_clicked();

    void on_tableView_Params_doubleClicked(const QModelIndex &index);

    void on_tableView_Body_doubleClicked(const QModelIndex &index);

    void on_tableView_Headers_doubleClicked(const QModelIndex &index);

private:
    Ui::MyPostman *ui;
    QNetworkAccessManager *m_accessManager;

    QStandardItemModel * ParamModel;
    QStandardItemModel * BodyModel;
    QStandardItemModel * HeaderModel;
};
#endif // MYPOSTMAN_H
