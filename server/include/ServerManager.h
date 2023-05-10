#pragma once

#include <QTcpServer>
#include <QTcpSocket>
#include <QSqlDatabase>
#include <QSqlQuery>

class ServerManager : public QTcpServer
{
    Q_OBJECT
public:
    ServerManager();

    void start();

public slots:
    void incomingConnection(qintptr socketDescriptor);

    void slotReadyRead();

    void SendToClient(QTcpSocket *socket, QString result);

    void SendToClients(QString result);

    void insertUserDB(QString login_, QString password);

    bool authorizationUserDB(QString login, QString password);

    void removeSocket(QString login);

private:
    //AuthServer *auth;
    //MessageServer *ms;
    QHash<QString, QTcpSocket *> table;
    QVector<QTcpSocket *> Sockets;
    QByteArray Data;
    QSqlDatabase db;
    QSqlQuery *query;
};
