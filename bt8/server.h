#ifndef SERVER_H
#define SERVER_H

#include <QBluetoothServer>
#include <QBluetoothSocket>
#include <QObject>

class Server : public QObject {
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);

private slots:
    void onNewConnection();
    void onReadyRead();
    void onError(QBluetoothSocket::SocketError error);

private:
    QBluetoothServer *m_server;
    QList<QBluetoothSocket*> m_clients;
    int m_nextBlockSize;

    void sendMessage(QByteArray message, QBluetoothSocket *socket);
};

#endif // SERVER_H
