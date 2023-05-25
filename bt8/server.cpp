#include <QDataStream>
#include "server.h"

Server::Server(QObject *parent) : QObject(parent), m_nextBlockSize(0) {
    m_server = new QBluetoothServer(QBluetoothServiceInfo::RfcommProtocol, this);
    connect(m_server, &QBluetoothServer::newConnection, this, &Server::onNewConnection);
    bool result = m_server->listen(QBluetoothAddress(), 1);
    if (!result) {
        qWarning() << "Can't listen on RFCOMM port";
        delete m_server;
        m_server = nullptr;
        return;
    }
}


void Server::onNewConnection() {
    QBluetoothSocket *socket = m_server->nextPendingConnection();
    qDebug() << "New connection!";
    connect(socket, &QBluetoothSocket::readyRead, this, &Server::onReadyRead);
    connect(socket, &QBluetoothSocket::disconnected, socket, &QBluetoothSocket::deleteLater);

    if (!m_clients.contains(socket)) {
        m_clients.append(socket);
    }
}

void Server::onReadyRead() {
    QBluetoothSocket *socket = qobject_cast<QBluetoothSocket*>(sender());
    if (!socket)
        return;

    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_3);

    while (true) {
        if (!m_nextBlockSize) {
            if (socket->bytesAvailable() < sizeof(int))
                return;
            in >> m_nextBlockSize;
        }

        if (socket->bytesAvailable() < m_nextBlockSize)
            return;

        QByteArray data;
        in >> data;

        sendMessage(data, socket);

        m_nextBlockSize = 0;
    }
}

void Server::sendMessage(QByteArray message, QBluetoothSocket *socket)
{
    for (auto client : m_clients) {
        if (client != socket) { // exclude the client that sent the message
            client->write(message);
        }
    }
}

void Server::onError(QBluetoothSocket::SocketError error)
{
    qWarning() << "Socket error:" << error;
}
