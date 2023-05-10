#include "ServerBase.h"

ServerBase::ServerBase()
{ /*if (this->listen(QHostAddress::Any, 5000)) {
        qDebug() << "start";
    } else {
        qDebug() << "error";
    }*/
    this->listen(QHostAddress::Any, 5000);
}
ServerBase::~ServerBase()
{
    //Data.clear();
}
/*
inline void ServerBase::SendToClient(QTcpSocket *socket, QString str) {}

void ServerBase::incomingConnection(qintptr socketDescriptor) {}

void ServerBase::slotReadyRead() {}

void ServerBase::deleteSocket(QTcpSocket *socket) {}
*/
