#pragma once

#include <QTcpServer>
#include <QTcpSocket>

class ServerBase : public QTcpServer
{
    Q_OBJECT

public:
    explicit ServerBase();
    ~ServerBase();

    QVector<QTcpSocket *> Sockets;
    // QByteArray Data;

    //void SendToClient(QTcpSocket *socket, QString str);

public slots:
    // void incomingConnection(qintptr socketDescriptor);

    // void slotReadyRead();

    //void deleteSocket(QTcpSocket *socket);
};
