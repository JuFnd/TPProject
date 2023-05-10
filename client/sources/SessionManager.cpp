#include <SessionManager.h>

SessionManager::SessionManager() {}

SessionManager::~SessionManager()
{
    delete w;
    delete auth;
    delete reg;

    socket->disconnectFromHost();
    delete socket;
}

void SessionManager::start()
{
    socket = new QTcpSocket(this);
    socket->connectToHost("127.0.0.1", 5000);

    auth = new Authorization(socket);
    auth->show();
    connect(auth, &Authorization::signalAuthTrue, this, &SessionManager::slotAuthTrue);
    connect(socket, &QTcpSocket::readyRead, auth, &Authorization::slotReadyRead);
    connect(auth, &Authorization::signalRegistration, this, &SessionManager::onRegistration);
}

void SessionManager::slotAuthTrue()
{
    disconnect(socket, &QTcpSocket::readyRead, auth, &Authorization::slotReadyRead);
    auth->close();
    w = new Messeger(socket);
    w->show();
    connect(socket, &QTcpSocket::readyRead, w, &Messeger::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
}

void SessionManager::onRegistration()
{
    disconnect(socket, &QTcpSocket::readyRead, auth, &Authorization::slotReadyRead);
    auth->close();

    reg = new Registration(socket);
    reg->show();
    connect(socket, &QTcpSocket::readyRead, reg, &Registration::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
}
