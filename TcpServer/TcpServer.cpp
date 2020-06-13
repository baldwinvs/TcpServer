#include "TcpServer.h"

#include <QDebug>

TcpServer::TcpServer(QObject* parent)
    : QTcpServer(parent)
{

}

void TcpServer::startServer()
{
    const quint16 port = 60002;
    if(!listen(QHostAddress("192.168.0.10"), port)) {
        qDebug() << "Could not start server";
    }
    else {
        qDebug() << "Listening on" << serverAddress().toString() << ":" << serverPort();
    }
}

void TcpServer::incomingConnection(qintptr handle)
{
    if(SocketList.size() <= MAX_CLIENTS) {
        qDebug() << "Connecting to socket descriptor [" << handle << "]";

        auto socket = new TcpSocket(handle);
        connect(socket, &TcpSocket::disconnected, this, &TcpServer::onClientDisconnect);

        SocketList.push_back(socket);

        auto thread {new QThread(this)};
        connect(thread, &QThread::finished, this, &TcpServer::onThreadFinished);

        ThreadList.push_back(thread);
        socket->moveToThread(thread);

        if(SocketList.size() > 1) {

            auto iter {SocketList.begin()};
            const auto endIter {SocketList.end()};

            //this is not a good loop
            //everytime a new client is added, the entirety of the client list gets connected to every other client
            //given 8 total clients, each client would have a total of 28 connections rather than 7 connections
            for(; iter != endIter; ++iter) {
                auto s_iter {SocketList.begin()};
                for(; s_iter != endIter; ++s_iter) {
                    if(s_iter != iter) {
                        connect(*iter, SIGNAL(sendData(QByteArray)), *s_iter, SLOT(writeData_(QByteArray)), Qt::QueuedConnection);
                    }
                }
            }
        }
        thread->start();
    }
    else {
        qDebug() << "maximum number of clients";
    }
}

void TcpServer::onClientDisconnect()
{
    TcpSocket* socket {dynamic_cast<TcpSocket*>(sender())};
    SocketList.removeOne(socket);

    delete socket;
}

void TcpServer::onThreadFinished()
{
    QThread* thread {dynamic_cast<QThread*>(sender())};
    ThreadList.removeOne(thread);

    delete thread;
}
