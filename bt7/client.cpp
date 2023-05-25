#include "client.h"
#include "bluetoothformatimage.h"

#include <QtCore/qmetaobject.h>

Client::Client(QObject *parent)
    :   QObject(parent)
{
}

Client::~Client()
{
    stopClient();
}

//! [startClient]
void Client::startClient(const QBluetoothServiceInfo &remoteService)
{
    if (socket)
        return;

    // Connect to service
    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
    qDebug() << "Create socket";
    socket->connectToService(remoteService);
    qDebug() << "ConnectToService done";

    connect(socket, &QBluetoothSocket::readyRead, this, &Client::readSocket);
    connect(socket, &QBluetoothSocket::connected, this, QOverload<>::of(&Client::connected));
    connect(socket, &QBluetoothSocket::disconnected, this, &Client::disconnected);
    connect(socket, QOverload<QBluetoothSocket::SocketError>::of(&QBluetoothSocket::error),
            this, &Client::onSocketErrorOccurred);

}
//! [startClient]

//! [stopClient]
void Client::stopClient()
{
    delete socket;
    socket = nullptr;
}
//! [stopClient]

//! [readSocket]
void Client::readSocket()
{
    if (!socket)
        return;

    while (socket->bytesAvailable() > 0) {
        QByteArray data = socket->readAll();
        QImage image;
        image.loadFromData(data, "PNG");
        emit messageReceived(socket->peerName(), image);
    }
}
//! [readSocket]

//! [sendMessage]
void Client::sendMessage(const QImage &message)
{
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    message.save(&buffer, "PNG"); // save image to buffer in PNG format
    socket->write(ba); // write the buffer to the socket
}
//! [sendMessage]

void Client::onSocketErrorOccurred(QBluetoothSocket::SocketError error)
{
    if (error == QBluetoothSocket::NoSocketError)
        return;

    QMetaEnum metaEnum = QMetaEnum::fromType<QBluetoothSocket::SocketError>();
    QString errorString = socket->peerName() + QLatin1Char(' ')
                          + metaEnum.valueToKey(error) + QLatin1String(" occurred");

    emit socketErrorOccurred(errorString);
}

//! [connected]
void Client::connected()
{
    emit connected(socket->peerName());
}
//! [connected]
