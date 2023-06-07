#include <QCoreApplication>
#include "mytcpserver_ex.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    MyTcpServer server;

    return a.exec();
}
