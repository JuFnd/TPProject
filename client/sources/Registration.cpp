#include "Registration.h"
#include "ui_Registration.h"

Registration::Registration(QTcpSocket *socket_)
    : QWidget(nullptr)
    , socket(socket_)
    , ui(new Ui::Registration)
{
    ui->setupUi(this);
}

Registration::~Registration()
{
    delete ui;
    qDebug() << "~Registration";
}

void Registration::slotReadyRead()
{
    Data.clear();
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_5);
    if (in.status() == QDataStream::Ok) {
        QString login;
        QString password;
        in >> login >> password;
    }
}

void Registration::sendToServer(QString login_, QString password_)
{
    Data.clear();
    QString command = "registration";
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_5);
    out << command << login_ << password_;
    socket->write(Data);
    ui->login->clear();
    ui->password->clear();
}

void Registration::on_pushButton_pressed()
{
    sendToServer(ui->login->text(), ui->password->text());
}
