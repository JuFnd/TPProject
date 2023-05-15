#include "screenshots.h"
#include "ui_screenshots.h"

Screenshots::Screenshots(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Screenshots)
{
    server = new QBluetoothServer(QBluetoothServiceInfo::RfcommProtocol);
    ui->setupUi(this);
}

Screenshots::~Screenshots()
{
    delete ui;
    delete server;
}

void Screenshots::on_pushButton_clicked()
{

}

void Screenshots::acceptMessage()
{
    QBluetoothAddress address = QBluetoothAddress();
    server->listen(address);

    connect(server, &QBluetoothServer::newConnection, this, &Screenshots::handleConnection);
}

void Screenshots::handleConnection()
{
    // Accept the incoming connection
    QBluetoothSocket* socket = server->nextPendingConnection();

    // Read the incoming message
    QByteArray message = socket->readAll();

    // Do something with the message
    qDebug() << "Received message: " << message;

    // Close the socket
    socket->close();
}

