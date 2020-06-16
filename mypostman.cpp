#include "mypostman.h"
#include "ui_mypostman.h"
#include "common.h"

MyPostman::MyPostman(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MyPostman)
{
    //setFixedSize(1280,800);

    ui->setupUi(this);
    TableViewInit(); //初始化表头
    getUserList();   //获取用户列表
    getHistory();    //获取操作历史
    m_accessManager = new QNetworkAccessManager(this);
    connect(m_accessManager,SIGNAL(finished(QNetworkReply *)),this,SLOT(httpReply(QNetworkReply*)));
    connect(m_accessManager,SIGNAL(finished(QNetworkReply *)),this,SLOT(insertHis()));

    connect(m_accessManager,SIGNAL(finished(QNetworkReply *)),this,SLOT(getHistory()));
}

MyPostman::~MyPostman()
{
    delete ui;
}


void MyPostman::on_btn_exit_clicked()
{
    close();
}

void MyPostman::on_btn_send_clicked()
{
    QString type = ui->comboBox_prot->currentText();
    QString BaseUrl = ui->comboBox_httpType->currentText();
    BaseUrl += ui->lineEdit_request->text();   //获取URL


    if(type == "POST")
    {
        RequestPOST(BaseUrl);
    }
    else if (type == "GET")
    {
        RequestGET(BaseUrl);
    }



}

void MyPostman::RequestPOST(QString BaseUrl)
{
    using namespace DJY;

    QNetworkRequest request;

    request.setUrl(QUrl(BaseUrl));
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    QJsonObject obj1;
    int count = ui->tableView_Params->model()->rowCount();

    for(int i=0;i<count;i++)
    {
        QModelIndex name = ParamModel->index(i,0,QModelIndex());
        QModelIndex value = ParamModel->index(i,1,QModelIndex());
        QString strName = name.data().toString();
        QString strValue = value.data().toString();
        if(strName == "" || strName == nullptr)
        {
            continue;   //空数据不发给服务器
        }
        obj1.insert(strName,strValue);
    }
    QJsonDocument docu;
    docu.setObject(obj1);
    QByteArray postData = docu.toJson(QJsonDocument::Compact); //组给服务器的参数
    qDebug()<<postData<<__LINE__;

    m_accessManager->post(request,postData);
    qDebug()<<"postData = "<<postData;
}

void MyPostman::RequestGET(QString BaseUrl)
{
    QNetworkRequest request;
    int count = ui->tableView_Params->model()->rowCount();

    for(int i=0;i<count;i++)
    {
        QModelIndex name = ParamModel->index(i,0,QModelIndex());
        QModelIndex value = ParamModel->index(i,1,QModelIndex());
        QString strName = name.data().toString();
        QString strValue = value.data().toString();
        if(strName == "" || strName == nullptr)
        {
            continue;   //空数据不发给服务器
        }
        if(i==0)
        {
            BaseUrl += "?";

        }
        else
        {
            BaseUrl += "&";
        }
        BaseUrl += strName + "=" + strValue;
    }


    request.setUrl(QUrl(BaseUrl));
    qDebug()<<"GET: BaseUrl = "<<BaseUrl;
    m_accessManager->get(request);


}

void MyPostman::httpReply(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();
        qDebug()<<bytes;
        QString string = QString::fromUtf8(bytes);

        ui->textEdit_result->setText(string.toUtf8());
    }
    else
    {
        qDebug()<<"handle errors here";
        QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        //statusCodeV是HTTP服务器的相应码，reply->error()是Qt定义的错误码，可以参考QT的文档
        qDebug( "请求发生错误 code: %d %d\n", statusCodeV.toInt(),static_cast<int>(reply->error()));
        qDebug()<<(reply->errorString());
        ui->textEdit_result->setText(qPrintable(reply->errorString()));
    }
    reply->deleteLater();

}

//初始化TableView的表头
void MyPostman::TableViewInit()
{
    ParamModel = new QStandardItemModel;
    BodyModel = new QStandardItemModel;
    HeaderModel = new QStandardItemModel;
    QStringList head;
    head<<("key")<<("value");
    ParamModel->setHorizontalHeaderLabels(head);
    BodyModel->setHorizontalHeaderLabels(head);
    HeaderModel->setHorizontalHeaderLabels(head);

    QList<QStandardItem *>item;
    QStandardItem * item1 = new QStandardItem("");
    QStandardItem * item2 = new QStandardItem("");
    item<<item1<<item2;

    ui->tableView_Body->setModel(BodyModel);
    ui->tableView_Body->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    BodyModel->appendRow(item);

    ui->tableView_Params->setModel(ParamModel);
    ui->tableView_Params->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ParamModel->appendRow(item);

    ui->tableView_Headers->setModel(HeaderModel);
    ui->tableView_Headers->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    HeaderModel->appendRow(item);

}



//清空tableview的数据，不包括表头
void MyPostman::TableClear()
{
    ui->tableView_Body->clearSpans();
    ui->tableView_Params->clearSpans();
    ui->tableView_Headers->clearSpans();
}

void MyPostman::on_btn_add_clicked()
{
    int index = ui->tabWidget->currentIndex();

    QList<QStandardItem *>item;
    QStandardItem * item1 = new QStandardItem("");
    QStandardItem * item2 = new QStandardItem("");
    item<<item1<<item2;




    if(index == 0)  //Params
    {
        ParamModel->appendRow(item);
        ui->tableView_Params->repaint();
    }
    else if(index == 1)  //Body
    {
        BodyModel->appendRow(item);
        ui->tableView_Body->repaint();
    }
    else if(index == 2)  //Headers
    {
        HeaderModel->appendRow(item);
        ui->tableView_Headers->repaint();
    }

}

void MyPostman::on_tableView_Params_doubleClicked(const QModelIndex &index)
{
    //判断下是不是最后一个列
    int num = index.row();      //这是当前选中的行号
    QString str = index.data().toString();
    int Count = ui->tableView_Params->model()->rowCount();
    if(num == Count-1&& (str != "" || str != nullptr))
    {
        QList<QStandardItem *>item;
        QStandardItem * item1 = new QStandardItem("");
        QStandardItem * item2 = new QStandardItem("");
        item<<item1<<item2;
        ParamModel->appendRow(item);
        ui->tableView_Params->repaint();
    }



}

void MyPostman::on_tableView_Body_doubleClicked(const QModelIndex &index)
{
    //判断下是不是最后一个列
    int num = index.row();      //这是当前选中的行号
    int Count = ui->tableView_Body->model()->rowCount();
    QString str = index.data().toString();
    if(num == Count-1 && (str != "" || str != nullptr))
    {
        QList<QStandardItem *>item;
        QStandardItem * item1 = new QStandardItem("");
        QStandardItem * item2 = new QStandardItem("");
        item<<item1<<item2;
        BodyModel->appendRow(item);
        ui->tableView_Body->repaint();
    }
}

void MyPostman::on_tableView_Headers_doubleClicked(const QModelIndex &index)
{
    //判断下是不是最后一个列
    int num = index.row();      //这是当前选中的行号
    int Count = ui->tableView_Headers->model()->rowCount();
    QString str = index.data().toString();
    if(num == Count-1 &&  (str != "" || str != nullptr))
    {
        QList<QStandardItem *>item;
        QStandardItem * item1 = new QStandardItem("");
        QStandardItem * item2 = new QStandardItem("");
        item<<item1<<item2;
        HeaderModel->appendRow(item);
        ui->tableView_Headers->repaint();
    }
}

void MyPostman::on_btn_clear_clicked()
{
    int index = ui->tabWidget->currentIndex();
    if(index == 0)  //Params
    {
        ParamModel->clear();
        ui->tableView_Params->setModel(ParamModel);
        ui->tableView_Params->repaint();
    }
    else if(index == 1)  //Body
    {
        BodyModel->clear();
        ui->tableView_Params->setModel(BodyModel);
        ui->tableView_Body->repaint();
    }
    else if(index == 2)  //Headers
    {
        HeaderModel->clear();
        ui->tableView_Params->setModel(HeaderModel);
        ui->tableView_Headers->repaint();
    }
    TableViewInit();    //重新初始化表头

}

void MyPostman::on_btn_delete_clicked()
{
    int index = ui->tabWidget->currentIndex();

    if(index == 0)  //Params
    {
        int count = ui->tableView_Params->currentIndex().row();
        int rows = ParamModel->rowCount();
        if(rows == 1)
        {
            return;
        }
        ParamModel->removeRow(count);
        ui->tableView_Params->repaint();

    }
    else if(index == 1)  //Body
    {
        int count = ui->tableView_Body->currentIndex().row();
        int rows = BodyModel->rowCount();
        if(rows == 1)
        {
            return;
        }
        BodyModel->removeRow(count);
        ui->tableView_Body->repaint();
    }
    else if(index == 2)  //Headers
    {
        int count = ui->tableView_Headers->currentIndex().row();
        int rows = HeaderModel->rowCount();
        if(rows == 1)
        {
            return;
        }
        HeaderModel->removeRow(count);
        ui->tableView_Headers->repaint();
    }
}




void MyPostman::on_btn_changeUser_clicked()
{
    currentUser = ui->comboBox_User->currentText();
    QDateTime now = QDateTime::currentDateTime();
    QString nowTime = now.toString("yyyyMMddhhmmss");
    QMessageBox::about(nullptr,"提示","切换用户成功，当前用户为:"+currentUser);
    QString sql_update = "update User set UpdateTime = '"+nowTime+"' where UserName = '"+currentUser+"'";
    qDebug()<<"更新用户的sql为"<<sql_update;
    QSqlQuery sqlQuery;
    if(!sqlQuery.exec(sql_update))
    {
        qDebug() << sqlQuery.lastError();

        QMessageBox::warning(nullptr,"提示","系统错误，请联系开发者!");
    }

    getHistory();

}

void MyPostman::on_btn_addNewUser_clicked()
{
    addNew = new AddNew;
    addNew->show();
}


//获取用户列表
void MyPostman::getUserList()
{
    QString getSql = "select UserName from User order by UpdateTime Desc";
    QSqlQuery sqlQuery;
    UserList.clear();
    if(!sqlQuery.exec(getSql))
    {
        qDebug() << sqlQuery.lastError();

        currentUser = nullptr;
    }
    else
    {
        qDebug() << "getUserList success";
        int count =0;
        while(sqlQuery.next())
        {
            UserList<<sqlQuery.value(0).toString();

            count++;
        }
        ui->comboBox_User->insertItems(count,UserList);
        ui->comboBox_User->repaint();
        currentUser = ui->comboBox_User->currentText();
    }
}


//记录操作历史
void MyPostman::insertHis()
{
    QString Url = ui->lineEdit_request->text(); //URL
    QString result = ui->textEdit_result->toPlainText();    //请求结果
    QDateTime now = QDateTime::currentDateTime();
    QString nowTime = now.toString("yyyy-MM-dd hh:mm:ss");
    int httpType = ui->comboBox_httpType->currentIndex();
    int prop = ui->comboBox_prot->currentIndex();
    QString ID = nowTime+"-"+ui->comboBox_prot->currentText()+"-"+ui->comboBox_httpType->currentText()+Url;     //拼接唯一键
    QString insert_sql = "insert into History(ID,User,ParamData,BodyData,HeaderData,Result,Type,HttpType,Url,CreateTime) "
                         "values('"+ID+"','"+currentUser+"','','','','"+result+"',"
            +QString::number(httpType)+","+QString::number(prop)+",'"+Url+"','"+nowTime+"')";
    qDebug()<<"插入历史记录的sql为"<<insert_sql;
    QSqlQuery sqlQuery;
    if(!sqlQuery.exec(insert_sql))
    {
        qDebug() << sqlQuery.lastError();

    }
    else
    {
        qDebug() << "记录日志成功";

    }

}

void MyPostman::getHistory()
{
    QString sql_his = "select ID,CreateTime from History where User = '"+currentUser+"' order by CreateTime Desc limit 100 ";
    QSqlQuery sqlQuery;
    ui->listWidget_history->clear();
    if(!sqlQuery.exec(sql_his))
    {
        qDebug() << sqlQuery.lastError();

        QMessageBox::warning(nullptr,"提示","系统错误，请联系开发者!");
    }
    else
    {
        qDebug() << "getHistory success";

        while(sqlQuery.next())
        {
            ui->listWidget_history->addItem(sqlQuery.value(0).toString());


        }

    }
    ui->listWidget_history->repaint();
}

void MyPostman::on_listWidget_clicked(const QModelIndex &index)
{
    qDebug()<<"卧槽咋没反应啊";
    QString ID = index.data().toString();
    QString get_sql = "select ParamData,BodyData,HeaderData,Result,Type,HttpType,Url"
                      " from History where ID = '"+ID+"'";
    qDebug()<<"关联操作记录的sql为"<<get_sql;
    QSqlQuery sqlQuery;
    if(!sqlQuery.exec(get_sql))
    {
        qDebug() << sqlQuery.lastError();

        QMessageBox::warning(nullptr,"提示","系统错误，请联系开发者!");
    }
    else
    {
        qDebug() << "getHistory success";
        ui->comboBox_prot->setCurrentIndex(sqlQuery.value(4).toInt());
        ui->comboBox_httpType->setCurrentIndex(sqlQuery.value(5).toInt());
        ui->textEdit_result->setText(sqlQuery.value(3).toString());
        ui->lineEdit_request->setText(sqlQuery.value(6).toString());
        ui->comboBox_prot->repaint();
        ui->comboBox_httpType->repaint();
        ui->textEdit_result->repaint();
        ui->lineEdit_request->repaint();

    }

}




