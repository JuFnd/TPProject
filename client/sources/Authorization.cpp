#include "Authorization.h"
#include "ui_Authorization.h"

Authorization::Authorization(QTcpSocket *socket_)
    : QWidget(nullptr)
    , socket(socket_)
    , ui(new Ui::Authorization)
{
    ui->setupUi(this);
}

Authorization::~Authorization()
{
    delete ui;
    qDebug() << "~Authorization";
}

void Authorization::on_pushButton_pressed()
{
    sendToServer(ui->login->text(), ui->password->text());
}

void Authorization::slotReadyRead()
{
    Data.clear();
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_5);
    if (in.status() == QDataStream::Ok) {
        QString str;
        in >> str;
        if (str == "yes") {
            emit signalAuthTrue();

            qDebug() << "the user is logged in";

        } else {
            qDebug() << "user is not logged in";
        }

    } else {
        ui->error->setText("error");
    }
}

void Authorization::sendToServer(QString login, QString password)
{
    Data.clear();
    QString command = "auth";
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_5);
    out << command << login << password;
    socket->write(Data);
    ui->login->clear();
    ui->password->clear();
}

void Authorization::on_pushButton_2_clicked()
{
    emit signalRegistration();
}
