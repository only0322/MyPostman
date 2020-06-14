#include "mypostman.h"
#include "ui_mypostman.h"
#include "common.h"

MyPostman::MyPostman(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MyPostman)
{
    setFixedSize(1280,800);

    ui->setupUi(this);
    TableViewInit(); //初始化表头
    m_accessManager = new QNetworkAccessManager(this);
    connect(m_accessManager,SIGNAL(finished(QNetworkReply *)),this,SLOT(httpReply(QNetworkReply*)));
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


    QNetworkRequest request;

    request.setUrl(QUrl(BaseUrl));
    request.setRawHeader("User-Agent",
                         "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_6) "
                         "AppleWebKit/537.36 (KHTML, like Gecko)"
                         " Chrome/78.0.3904.108 Safari/537.36");
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

    QByteArray postData = QJsonDocument(obj1).toJson(); //组给前端的参数


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
