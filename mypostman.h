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

    void getUserList();     //获取用户列表





private slots:
    void on_btn_exit_clicked();

    void on_btn_send_clicked();

    void httpReply(QNetworkReply * reply);

    void on_btn_add_clicked();

    void on_tableView_Params_doubleClicked(const QModelIndex &index);

    void on_tableView_Body_doubleClicked(const QModelIndex &index);

    void on_tableView_Headers_doubleClicked(const QModelIndex &index);

    void on_btn_clear_clicked();

    void on_btn_delete_clicked();

    void on_btn_changeUser_clicked();

    void on_btn_addNewUser_clicked();

    void insertHis();       //记录操作历史

    void getHistory();      //获取操作历史



    void on_listWidget_history_clicked(const QModelIndex &index);

private:
    Ui::MyPostman *ui;
    QNetworkAccessManager *m_accessManager;

    QStandardItemModel * ParamModel;
    QStandardItemModel * BodyModel;
    QStandardItemModel * HeaderModel;
    AddNew * addNew;
    QString currentUser;
    QStringList UserList;
};
#endif // MYPOSTMAN_H
