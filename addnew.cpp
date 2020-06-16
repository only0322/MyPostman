#include "addnew.h"
#include "ui_addnew.h"
#include "common.h"
AddNew::AddNew(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AddNew)
{
    ui->setupUi(this);
    setFixedSize(400,300);
}

AddNew::~AddNew()
{
    delete ui;
}

void AddNew::on_btn_confirm_clicked()
{
    QSqlQuery sql_query;
    QString User = ui->lineEdit_user->text();
    QDateTime now = QDateTime::currentDateTime();
    QString nowTime = now.toString("yyyyMMddhhmmss");
    QString createUser = "insert into User(UserName,PassWord,UpdateTime) values('"+User+"','',"+nowTime+")";
    qDebug()<<"创建用户的sql为"<<createUser;
    if(!sql_query.exec(createUser))
    {
        qDebug() << sql_query.lastError();
        this->close();
    }
    else
    {
        qDebug() << "created a new user!";
        this->close();
    }
}

void AddNew::on_btn_return_clicked()
{
    this->close();
}
