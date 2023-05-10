#pragma once

#include <ServerBase.h>

class AuthServer : public ServerBase
{
    Q_OBJECT
public:
    AuthServer();
    ~AuthServer();
    //QTcpSocket *socket;

    //QVector<QTcpSocket *> Sockets;
    QByteArray Data;

    void SendToClient(QTcpSocket *socket, QString str);

    QVector<QTcpSocket *> *getVector();

public slots:
    void incomingConnection(qintptr socketDescriptor);

    void slotReadyRead();

    void deleteSocket(QTcpSocket *socket);

signals:
    void signalAuthTrue(QDataStream *in, QTcpSocket *socket);
};
