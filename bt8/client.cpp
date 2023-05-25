#include "client.h"


Client::Client(QObject *parent) : QObject(parent), m_nextBlockSize(0), m_socket(nullptr)
{
}

void Client::connectToServer(QBluetoothAddress address, quint16 port)
{
    m_socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this);
    connect(m_socket, &QBluetoothSocket::connected, this, &Client::onConnected);
    connect(m_socket, &QBluetoothSocket::disconnected, this, &Client::onDisconnected);
    connect(m_socket, &QBluetoothSocket::readyRead, this, &Client::onReadyRead);
    connect(m_socket, static_cast<void(QBluetoothSocket::*)(QBluetoothSocket::SocketError)>(&QBluetoothSocket::error),
            this, &Client::onError);
    m_socket->connectToService(address, port);
}

void Client::sendData(QByteArray data)
{
    QDataStream out(m_socket);
    out.setVersion(QDataStream::Qt_5_3);

    // Serialize the data and send a packet including the size of the serialized data
    QByteArray serializedData;
    QDataStream stream(&serializedData, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_3);
    stream << data;
    out << (int)serializedData.size();
    out.writeRawData(serializedData.data(), serializedData.size());
}

void Client::onConnected()
{
    emit connected();
}

void Client::onDisconnected()
{
    emit disconnected();
}

void Client::onReadyRead()
{
    QDataStream in(m_socket);
    in.setVersion(QDataStream::Qt_5_3);

    while (true) {
        if (!m_nextBlockSize) {
            if (m_socket->bytesAvailable() < sizeof(int))
                return;
            in >> m_nextBlockSize;
        }

        if (m_socket->bytesAvailable() < m_nextBlockSize)
            return;

        QByteArray data;
        in >> data;

        emit readyRead(data);

        m_nextBlockSize = 0;
    }
}

void Client::onError(QBluetoothSocket::SocketError error)
{
    qWarning() << "Socket error:" << error << "-" << m_socket->errorString();
}
