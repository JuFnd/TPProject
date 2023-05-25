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
        QByteArray imageData = socket->readAll();
        if (!imageData.isEmpty()) {
            QImage image;
            image.loadFromData(imageData, "JPEG");
            emit imageReceived(socket->peerName(), image);
        }
    }
}
//! [readSocket]

//! [sendMessage]
void Client::sendMessage(const QImage &image)
{
    QByteArray imageData;
    QBuffer buffer(&imageData);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "JPEG");

    if (socket) {
        qint64 bytesToSend = std::min(static_cast<qint64>(socket->bytesAvailable()), qint64(imageData.size()));
        socket->write(imageData.constData(), bytesToSend);
    }
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
