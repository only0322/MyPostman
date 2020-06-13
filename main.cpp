#include "mypostman.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyPostman w;
    w.show();
    return a.exec();
}
