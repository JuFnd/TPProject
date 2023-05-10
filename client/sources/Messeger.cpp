#include "Messeger.h"
#include "./ui_Messeger.h"

Messeger::Messeger(QTcpSocket *socket_)
    : QWidget(nullptr)
    , socket(socket_)
    , ui(new Ui::Messeger)

{
    ui->setupUi(this);
}

Messeger::~Messeger()
{
    delete ui;
}

void Messeger::on_lineEdit_returnPressed()
{
    QString text = ui->lineEdit->text();
    if (text == "") {
        return;
    }
    SendToServer(ui->lineEdit->text());
}

void Messeger::on_pushButton_clicked()
{
    QString text = ui->lineEdit->text();
    if (text == "") {
        return;
    }
    SendToServer(ui->lineEdit->text());
}

void Messeger::slotReadyRead()
{
    Data.clear();
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_5);
    if (in.status() == QDataStream::Ok) {
        QString str;
        in >> str;
        ui->textBrowser->append(str);

    } else {
        ui->textBrowser->append("read error..");
    }
}

void Messeger::SendToServer(QString str)
{
    Data.clear();
    QString command = "message";
    qDebug() << str << " client";
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_5);
    out << command << str;
    socket->write(Data);
    ui->lineEdit->clear();
}
