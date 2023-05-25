#include "server.h"

#include <QtBluetooth/qbluetoothserver.h>
#include <QtBluetooth/qbluetoothsocket.h>
#include <QtGlobal>

//! [Service UUID]
static const QLatin1String serviceUuid("e8e10f95-1a70-4b27-9ccf-02010264e9c8");
//! [Service UUID]

Server::Server(QObject *parent)
    :   QObject(parent)
{
}

Server::~Server()
{
    stopServer();
}

void Server::startServer(const QBluetoothAddress& localAdapter)
{
    if (rfcommServer)
        return;

    //! [Create the server]
    rfcommServer = new QBluetoothServer(QBluetoothServiceInfo::RfcommProtocol, this);
    connect(rfcommServer, &QBluetoothServer::newConnection,
            this, QOverload<>::of(&Server::clientConnected));
    bool result = rfcommServer->listen(localAdapter);
    if (!result) {
        qWarning() << "Cannot bind server to" << localAdapter.toString();
        return;
    }
    //! [Create the server]

    //serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceRecordHandle, (uint)0x00010010);

    QBluetoothServiceInfo::Sequence profileSequence;
    QBluetoothServiceInfo::Sequence classId;
    classId << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::SerialPort));
    classId << QVariant::fromValue(quint16(0x100));
    profileSequence.append(QVariant::fromValue(classId));
    serviceInfo.setAttribute(QBluetoothServiceInfo::BluetoothProfileDescriptorList,
                             profileSequence);

    classId.clear();
    classId << QVariant::fromValue(QBluetoothUuid(serviceUuid));
    classId << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::SerialPort));

    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceClassIds, classId);

    //! [Service name, description and provider]
    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceName, tr("Bt Server"));
    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceDescription,
                             tr("bluetooth server"));
    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceProvider, true   );
    //! [Service name, description and provider]

    //! [Service UUID set]
    serviceInfo.setServiceUuid(QBluetoothUuid(serviceUuid));
    //! [Service UUID set]

    //! [Service Discoverability]
    QBluetoothServiceInfo::Sequence publicBrowse;
    publicBrowse << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::PublicBrowseGroup));
    serviceInfo.setAttribute(QBluetoothServiceInfo::BrowseGroupList,
                             publicBrowse);
    //! [Service Discoverability]

    //! [Protocol descriptor list]
    QBluetoothServiceInfo::Sequence protocolDescriptorList;
    QBluetoothServiceInfo::Sequence protocol;
    protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::L2cap));
    protocolDescriptorList.append(QVariant::fromValue(protocol));
    protocol.clear();
    protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::Rfcomm))
             << QVariant::fromValue(quint8(rfcommServer->serverPort()));
    protocolDescriptorList.append(QVariant::fromValue(protocol));
    serviceInfo.setAttribute(QBluetoothServiceInfo::ProtocolDescriptorList,
                             protocolDescriptorList);
    //! [Protocol descriptor list]

    //! [Register service]
    serviceInfo.registerService(localAdapter);
    //! [Register service]
}

//! [stopServer]
void Server::stopServer()
{
    // Unregister service
    serviceInfo.unregisterService();

    // Close sockets
    qDeleteAll(clientSockets);

    // Close server
    delete rfcommServer;
    rfcommServer = nullptr;
}
//! [stopServer]

//! [sendMessage]
void Server::sendMessage(const QImage &image)
{
    QByteArray imageData;
    QBuffer buffer(&imageData);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "JPEG");

    for (QBluetoothSocket *socket : qAsConst(clientSockets)) {
        qint64 bytesToSend = std::min(static_cast<qint64>(socket->bytesAvailable()), qint64(imageData.size()));
        socket->write(imageData.constData(), bytesToSend);
    }
}
//! [sendMessage]

//! [clientConnected]
void Server::clientConnected()
{
    QBluetoothSocket *socket = rfcommServer->nextPendingConnection();
    if (!socket)
        return;

    connect(socket, &QBluetoothSocket::readyRead, this, &Server::readSocket);
    connect(socket, &QBluetoothSocket::disconnected, this, QOverload<>::of(&Server::clientDisconnected));
    clientSockets.append(socket);
    emit clientConnected(socket->peerName());
}
//! [clientConnected]

//! [clientDisconnected]
void Server::clientDisconnected()
{
    QBluetoothSocket *socket = qobject_cast<QBluetoothSocket *>(sender());
    if (!socket)
        return;

    emit clientDisconnected(socket->peerName());

    clientSockets.removeOne(socket);

    socket->deleteLater();
}
//! [clientDisconnected]

//! [readSocket]
void Server::readSocket()
{
    QBluetoothSocket *socket = qobject_cast<QBluetoothSocket *>(sender());
    if (!socket)
        return;

    // Read image data from socket
    QByteArray imageData;
    while (socket->bytesAvailable()) {
        imageData.append(socket->read(std::min(static_cast<int>(socket->bytesAvailable()), 1024)));
    }

    // Create QImage from image data
    QImage receivedImage;
    if (!receivedImage.loadFromData(imageData)) {
        qDebug() << "Failed to load image data";
        return;
    }

    // Emit signal with received image
    emit messageReceived(socket->peerName(), receivedImage);
}
//! [readSocket]
