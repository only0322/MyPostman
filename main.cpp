#include "mypostman.h"
#include "common.h"
#include <QApplication>

void initDB();
int main(int argc, char *argv[])
{
#ifdef _QT_NO_DEBUG
    QString dbname = "./MyPostman.db";
#elif defined (Q_OS_WIN)
    QString dbname = "./MyPostman.db";
#elif defined (Q_OS_MACOS)
    QString dbname = "/Users/hideyoshi/Desktop/codes/MyPostman/MyPostman.db";
#else
    QString dbname = "./MyPostman.db";
#endif

    QApplication a(argc, argv);
    QSqlDatabase database;
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(dbname);
    if (!database.open())
    {
        qDebug() << "Error: Failed to connect database." << database.lastError();
    }
    else
    {
        qDebug() << "Succeed to connect database." ;
    }
    MyPostman w;
    w.show();


    return a.exec();
}


void initDB()
{
    QString createUser = "create table user";
}
