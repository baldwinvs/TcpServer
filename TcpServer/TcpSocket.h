#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QTcpSocket>

class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpSocket(qintptr socketDescriptor, QObject *parent = Q_NULLPTR);

signals:
    void sendData(QByteArray);

public slots:
    void onReadyRead();
    void onDisconnect();
    void setOutputSocket(TcpSocket* socket);
    void writeData_(const QByteArray &data);

private:
    long long sock_desc;
 };

#endif // TCPSOCKET_H
