#include "mypostman.h"
#include "ui_mypostman.h"

MyPostman::MyPostman(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MyPostman)
{
    ui->setupUi(this);
}

MyPostman::~MyPostman()
{
    delete ui;
}

