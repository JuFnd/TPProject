#include "AuthServer.h"

AuthServer::AuthServer() {}

AuthServer::~AuthServer() {}

void AuthServer::SendToClient(QTcpSocket *socket, QString str)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_5);
    out << str;
    socket->write(Data);
}

QVector<QTcpSocket *> *AuthServer::getVector()
{
    return &Sockets;
}

void AuthServer::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket *socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket,
            &QTcpSocket::readyRead,
            static_cast<AuthServer *>(this),
            &AuthServer::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, [this, socket]() { deleteSocket(socket); });
    Sockets.push_back(socket);

    qDebug() << "client connected";
}

void AuthServer::slotReadyRead()
{
    Data.clear();
    QTcpSocket *socket = static_cast<QTcpSocket *>(sender());
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_5);
    if (in.status() == QDataStream::Ok) {
        qDebug() << "read...";
        QString command;
        QString login;
        QString password;
        in >> command >> login >> password;
        //qDebug() << login << " " << password;
        if (command == "auth") {
            if (login == "1" && password == "1") {
                // qDebug() << "successfull_yes";
                SendToClient(socket, "yes");
            } else {
                // qDebug() << "error_no";
                SendToClient(socket, "no");
            }
        } else {
            emit signalAuthTrue(&in, socket);
        }
    } else {
        qDebug() << "DataStream error";
    }
}

void AuthServer::deleteSocket(QTcpSocket *socket)
{
    //  Sockets.erase(std::remove(Sockets.begin(), Sockets.end(), socket), Sockets.end());
    // socket->deleteLater();
}
