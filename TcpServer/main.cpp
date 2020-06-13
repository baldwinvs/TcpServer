#include <QCoreApplication>
#include <qglobal.h>

#include "TcpServer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TcpServer server;
    server.startServer();

    return a.exec();
}
