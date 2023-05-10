#include "ServerManager.h"

ServerManager::ServerManager()
{
    if (this->listen(QHostAddress::Any, 5000)) {
        qDebug() << "start";
    } else {
        qDebug() << "error";
    }

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./testDB.db");
    if (db.open()) {
        qDebug() << "SQL open";
    } else {
        qDebug() << "SQL no open";
    }

    query = new QSqlQuery(db);
    if (!db.tables().contains("USERS")) {
        query->exec(
            "CREATE TABLE USERS (id INT AUTO_INCREMENT PRIMARY KEY, login TEXT, password TEXT);");
    }
}

void ServerManager::start() {}

void ServerManager::incomingConnection(qintptr socketDescriptor)
{
    QString login;
    Data.clear();
    QTcpSocket *socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);
    //Sockets.push_back(socket);
    //table.insert(socket, "NULL");
    connect(socket, &QTcpSocket::readyRead, this, &ServerManager::slotReadyRead);
    //connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    connect(socket, &QTcpSocket::disconnected, this, [this, login] { removeSocket(login); });

    qDebug() << "client connected";
}

void ServerManager::slotReadyRead()
{
    QString command;
    QString login;
    QString password;
    QString str;
    Data.clear();
    QTcpSocket *socket = static_cast<QTcpSocket *>(sender());
    //disconnect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    //connect(socket, &QTcpSocket::disconnected, this, [this, login] { removeSocket(login); });
    QDataStream in(socket);

    in.setVersion(QDataStream::Qt_6_5);
    if (in.status() == QDataStream::Ok) {
        qDebug() << "Read...";
        in >> command;
        qDebug() << command;
        if (command == "auth") {
            in >> login >> password;
            if (table.value(login)) {
                qDebug() << "Error. You are already logged in";
                return;
            }
            if (authorizationUserDB(login, password)) {
                SendToClient(socket, "yes");
                table.insert(login, socket);
                //table[socket] = login;
            } else {
                SendToClient(socket, "no1");
            }
        } else if (command == "message") {
            in >> str;
            qDebug() << str << "SERRVER";
            //SendToClient(socket, str);
            SendToClients(str);
        } else if (command == "registration") {
            in >> login >> password;
            insertUserDB(login, password);
        }
    } else {
        qDebug() << "DataStream error";
    }
}

void ServerManager::SendToClient(QTcpSocket *socket, QString str)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_5);
    out << str;
    socket->write(Data);
}

void ServerManager::SendToClients(QString str)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_5);
    out << str;
    qDebug() << str << "SendToClients";
    qDebug() << table.size() << "SendToClients SIZE";

    for (auto it = table.begin(); it != table.end(); it++) {
        it.value()->write(Data);
    }
    //for (int i = 0; i < table.size(); i++) {
    //    table[i]->write(Data);
    //}
}

void ServerManager::insertUserDB(QString login_, QString password_)
{
    QSqlQuery query1;
    qDebug() << login_ << " " << password_;
    query1.prepare("INSERT INTO USERS (login, password) VALUES (:login, :password)");
    query1.bindValue(":login", login_);
    query1.bindValue(":password", password_);
    if (query1.exec()) {
        qDebug() << "insertUserDB successfull";
    } else {
        qDebug() << "insertUserDB error";
    }
}

bool ServerManager::authorizationUserDB(QString login_, QString password_)
{
    query->prepare("SELECT count(*) FROM USERS WHERE login = :login and password=:password");
    query->bindValue(":login", login_);
    query->bindValue(":password", password_);
    if (query->exec() && query->next()) {
        int count = query->value(0).toInt();
        if (count > 0) {
            qDebug() << "authorizationUserDB succesfull";
            return true;
        }
    }

    qDebug() << "authorizationUserDB error";
    return false;
}

void ServerManager::removeSocket(QString login)
{
    qDebug() << "Remove socket..";
    //int index = Sockets.indexOf(socket);
    //Sockets[index]->deleteLater();
    //Sockets.remove(index);
    //table[login]->deleteLater();
    table.remove(login);
}
