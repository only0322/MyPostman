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

    qDebug()<<"BaseUrl"<<BaseUrl;
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

    QByteArray postData; //组给前端的参数

    postData.append("myname=djy");

    m_accessManager->post(request,postData);
}

void MyPostman::RequestGET(QString BaseUrl)
{
    QNetworkRequest request;



    request.setUrl(QUrl(BaseUrl));
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


    ui->tableView_Body->setModel(BodyModel);
    ui->tableView_Body->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableView_Params->setModel(ParamModel);
    ui->tableView_Params->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableView_Headers->setModel(HeaderModel);
    ui->tableView_Headers->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

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
