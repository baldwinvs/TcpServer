#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "TcpSocket.h"

#include <QTcpServer>
#include <QThread>
#include <QList>

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject* parent = Q_NULLPTR);

    void startServer();

signals:
    void sendSocketThread(TcpSocket*);

protected:
    void incomingConnection(qintptr handle) Q_DECL_OVERRIDE;

private:
    enum {
        MAX_CLIENTS = 8
    };
    QList<TcpSocket*> SocketList;
    QList<QThread*> ThreadList;

private slots:
    void onClientDisconnect();
    void onThreadFinished();
};

#endif // TCPSERVER_H
