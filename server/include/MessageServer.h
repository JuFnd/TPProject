#pragma once

#include <ServerBase.h>

class MessageServer : public ServerBase
{
    Q_OBJECT
public:
    MessageServer();
    ~MessageServer();

    //QVector<QTcpSocket *> Sockets;
    QByteArray Data;

    void SendToClient(QTcpSocket *socket, QString str);

public slots:
    void incomingConnection(qintptr socketDescriptor);

    void slotReadyRead();

    void deleteSocket(QTcpSocket *socket);

    void SendToClient(QString str);

    void signalAuthTrue(QDataStream *in, QTcpSocket *socket);
};
