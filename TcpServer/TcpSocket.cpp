#include "TcpSocket.h"
#include "TcpServer.h"

#include <QDebug>
#include <QHostAddress>

TcpSocket::TcpSocket(qintptr socketDescriptor, QObject* parent)
    : QTcpSocket(parent)
{
    if(!setSocketDescriptor(socketDescriptor)) {
        emit error();
    }
    sock_desc = socketDescriptor;
    connect(this, SIGNAL(readyRead()), this, SLOT(onReadyRead()), Qt::DirectConnection);
    connect(this, SIGNAL(disconnected()), this, SLOT(onDisconnect()));

    qDebug() << "[" << socketDescriptor << "]" << "Client connected";
    qDebug() << "[" << socketDescriptor << "]" << "Loc Addr " << localAddress();
    qDebug() << "[" << socketDescriptor << "]" << "Loc Port " << localPort();
    qDebug() << "[" << socketDescriptor << "]" << "Peer Addr" << peerAddress();
    qDebug() << "[" << socketDescriptor << "]" << "Peer Addr" << peerPort();
    qDebug();
}

void TcpSocket::writeData_(const QByteArray &data)
{
    // will write on server side window
    qDebug() << "Write to" << "[" << sock_desc << "]" << data;
    write(data);
}

void TcpSocket::onReadyRead()
{
    // get the information
    QByteArray data {readAll()};

    // will write on server side window
    qDebug() << "Data from" << " [" << sock_desc << "]" << data;

    emit sendData(data);
}

void TcpSocket::onDisconnect()
{
    qDebug() << "[" << sock_desc << "]" << "Disconnected";

    deleteLater();
}

void TcpSocket::setOutputSocket(TcpSocket* const socket)
{
    if(this != socket) {
        connect(this, SIGNAL(sendData(QByteArray)), socket, SLOT(writeData(QByteArray)));
    }
}
