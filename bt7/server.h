#ifndef SERVER_H
#define SERVER_H

#include <QObject>

#include "bluetoothformatimage.h"

#include <QtBluetooth/qbluetoothaddress.h>
#include <QtBluetooth/qbluetoothserviceinfo.h>

QT_FORWARD_DECLARE_CLASS(QBluetoothServer)
QT_FORWARD_DECLARE_CLASS(QBluetoothSocket)

QT_USE_NAMESPACE

    //! [declaration]
class Server : public QObject
{
    Q_OBJECT

public:
    explicit Server(QObject *parent = nullptr);
    ~Server();

    void startServer(const QBluetoothAddress &localAdapter = QBluetoothAddress());
    void stopServer();

public slots:
    void sendMessage(QByteArray data);

signals:
    void messageReceived(const QString &sender, const QByteArray &message);
    void clientConnected(const QString &name);
    void clientDisconnected(const QString &name);

private slots:
    void clientConnected();
    void clientDisconnected();
    void readSocket();

private:
    QBluetoothServer *rfcommServer = nullptr;
    QBluetoothServiceInfo serviceInfo;
    QList<QBluetoothSocket *> clientSockets;
};
//! [declaration]

#endif // SERVER_H
