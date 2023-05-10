#include "MessageServer.h"

MessageServer::MessageServer()
{

}

MessageServer::~MessageServer()
{
    //delete socket;
    Data.clear();
}

void MessageServer::SendToClient(QString str)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_5);
    out << str;
    //socket->write(Data);

    for (int i = 0; i < Sockets.size(); i++) {
        Sockets[i]->write(Data);
    }
}

void MessageServer::signalAuthTrue(QDataStream *in, QTcpSocket *socket)
{
    Data.clear();
    in->setVersion(QDataStream::Qt_6_5);
    if (in->status() == QDataStream::Ok) {
        qDebug() << "read...";
        QString command;
        QString str;

        *in >> command >> str;
        if (str == "message") {
            qDebug() << str;
            SendToClient(str);
        } else {
            qDebug() << "message error";
        }

    } else {
        qDebug() << "DataStream error";
    }
}

void MessageServer::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket *socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket,
            &QTcpSocket::readyRead,
            static_cast<MessageServer *>(this),
            &MessageServer::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, [this, socket]() { deleteSocket(socket); });
    Sockets.push_back(socket);

    qDebug() << "client connected";
}

void MessageServer::slotReadyRead()
{
    Data.clear();
    QTcpSocket *socket = static_cast<QTcpSocket *>(sender());
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_5);
    if (in.status() == QDataStream::Ok) {
        qDebug() << "read...";
        QString command;
        QString str;

        in >> command >> str;
        if (str == "message") {
            qDebug() << str;
            SendToClient(str);
        } else {
            qDebug() << "message error";
        }

    } else {
        qDebug() << "DataStream error";
    }
}

void MessageServer::deleteSocket(QTcpSocket *socket)
{
    //Sockets.erase(std::remove(Sockets.begin(), Sockets.end(), socket), Sockets.end());
    //socket->deleteLater();
}
