#ifndef CLIENT_H
#define CLIENT_H

#include <QDataStream>
#include <QBluetoothSocket>
#include <QObject>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    void connectToServer(QBluetoothAddress address, quint16 port);
    void sendData(QByteArray data);

signals:
    void connected();
    void disconnected();
    void readyRead(QByteArray data);

private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onError(QBluetoothSocket::SocketError error);

private:
    QBluetoothSocket *m_socket;
    int m_nextBlockSize;
};

#endif // CLIENT_H
