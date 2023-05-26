#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "client.h"
#include "remoteselector.h"
#include "server.h"

#include <QPixmap>
#include <QLabel>

#include <QtCore/qdebug.h>

#include <QtBluetooth/qbluetoothdeviceinfo.h>
#include <QtBluetooth/qbluetoothlocaldevice.h>
#include <QtBluetooth/qbluetoothuuid.h>


static const QLatin1String serviceUuid("e8e10f95-1a70-4b27-9ccf-02010264e9c8");

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    //! [Construct UI]
    ui->setupUi(this);

    connect(ui->connectButton, &QPushButton::clicked, this, &MainWindow::connectClicked);
    connect(ui->sendButton, &QPushButton::clicked, this, &MainWindow::sendClicked);
    //! [Construct UI]

    localAdapters = QBluetoothLocalDevice::allDevices();
    if (localAdapters.count() < 2) {
        qDebug() << "localAdapters 0";
    } else {
        //we ignore more than two adapters
        connect(ui->firstAdapter, &QRadioButton::clicked, this, &MainWindow::newAdapterSelected);
        connect(ui->secondAdapter, &QRadioButton::clicked, this, &MainWindow::newAdapterSelected);
        QBluetoothLocalDevice adapter(localAdapters.at(0).address());
        adapter.setHostMode(QBluetoothLocalDevice::HostDiscoverable);
    }

    //! [Create Chat Server]
    server = new Server(this);
    connect(server, QOverload<const QString &>::of(&Server::clientConnected),
            this, &MainWindow::clientConnected);
    connect(server, QOverload<const QString &>::of(&Server::clientDisconnected),
            this,  QOverload<const QString &>::of(&MainWindow::clientDisconnected));
    connect(server, &Server::messageReceived,
            this,  &MainWindow::showMessage);
    connect(this, &MainWindow::sendMessage, server, &Server::sendMessage);
    server->startServer();
    //! [Create Server]

    //! [Get local device name]
    localName = QBluetoothLocalDevice().name();
    //! [Get local device name]
}

MainWindow::~MainWindow()
{
    qDeleteAll(clients);
    delete server;
}


//! [clientConnected clientDisconnected]
void MainWindow::clientConnected(const QString &name)
{
    qDebug() << name << "connect";
}

void MainWindow::clientDisconnected(const QString &name)
{
    qDebug() << name << "disconnect";
}
//! [clientConnected clientDisconnected]

//! [connected]
void MainWindow::connected(const QString &name)
{
    qDebug() << name << "connect";
}
//! [connected]

void MainWindow::newAdapterSelected()
{
    const int newAdapterIndex = adapterFromUserSelection();
    if (currentAdapterIndex != newAdapterIndex) {
        server->stopServer();
        currentAdapterIndex = newAdapterIndex;
        const QBluetoothHostInfo info = localAdapters.at(currentAdapterIndex);
        QBluetoothLocalDevice adapter(info.address());
        adapter.setHostMode(QBluetoothLocalDevice::HostDiscoverable);
        server->startServer(info.address());
        localName = info.name();
    }
}

int MainWindow::adapterFromUserSelection() const
{
    int result = 0;
    QBluetoothAddress newAdapter = localAdapters.at(0).address();

    if (ui->secondAdapter->isChecked()) {
        newAdapter = localAdapters.at(1).address();
        result = 1;
    }
    return result;
}

void MainWindow::reactOnSocketError(const QString &error)
{

}

//! [clientDisconnected]
void MainWindow::clientDisconnected()
{
    Client *client = qobject_cast<Client *>(sender());
    if (client) {
        clients.removeOne(client);
        client->deleteLater();
    }
}
//! [clientDisconnected]

//! [Connect to remote service]
void MainWindow::connectClicked()
{
    ui->connectButton->setEnabled(false);

    // scan for services
    const QBluetoothAddress adapter = localAdapters.isEmpty() ?
                                          QBluetoothAddress() :
                                          localAdapters.at(currentAdapterIndex).address();

  RemoteSelector remoteSelector(adapter);
#ifdef Q_OS_ANDROID
    if (QtAndroid::androidSdkVersion() >= 23)
        remoteSelector.startDiscovery(QBluetoothUuid(reverseUuid));
    else
        remoteSelector.startDiscovery(QBluetoothUuid(serviceUuid));
#else
    remoteSelector.startDiscovery(QBluetoothUuid(serviceUuid));
#endif
    if (remoteSelector.exec() == QDialog::Accepted) {
        QBluetoothServiceInfo service = remoteSelector.service();

        qDebug() << "Connecting to service 2" << service.serviceName()
                 << "on" << service.device().name();

        // Create client
        qDebug() << "Going to create client";
        Client *client = new Client(this);
        qDebug() << "Connecting...";

        connect(client, &Client::messageReceived,
                this, &MainWindow::showMessage);
        connect(client, &Client::disconnected,
                this, QOverload<>::of(&MainWindow::clientDisconnected));
        connect(client, QOverload<const QString &>::of(&Client::connected),
                this, &MainWindow::connected);
        connect(client, &Client::socketErrorOccurred,
                this, &MainWindow::reactOnSocketError);
        connect(this, &MainWindow::sendMessage, client, &Client::sendMessage);
        qDebug() << "Start client";
        client->startClient(service);

        clients.append(client);
    }

    ui->connectButton->setEnabled(true);
}
//! [Connect to remote service]

//! [sendClicked]
void MainWindow::sendClicked()
{
    ui->sendButton->setEnabled(false);
    ui->sendText->setEnabled(false);

    showMessage(localName, img->getDataImage());
    emit sendMessage(img->getDataImage());

    ui->sendText->clear();

    ui->sendText->setEnabled(true);
    ui->sendButton->setEnabled(true);
}
//! [sendClicked]

//! [showMessage]
void MainWindow::showMessage(const QString &sender, const QByteArray &message)
{
    qDebug() << message;
}
//! [showMessage]

void MainWindow::captureScreen(){
    QPixmap pixmap;

    // Capture the screen when F12 key is pressed
    QShortcut *shortcut = new QShortcut(QKeySequence(Qt::Key_F12), this);
    QObject::connect(shortcut, &QShortcut::activated, [&]() {
        pixmap = QPixmap::grabWindow(QApplication::desktop()->winId());
        //pixmap.save("screenshot.png");
        img = new BluetoothFormatImage(pixmap);
        // Create a label widget to show the pixmap
        //QLabel *label = new QLabel(this);
        //label->setPixmap(pixmap);
        //label->setScaledContents(true);

        // Set the label as the central widget of the window
        //setCentralWidget(label);

        // Show the window in full screen mode
        //showFullScreen();
        //label->show();

    });
}

void MainWindow::setImg(){
    captureScreen();
}


void MainWindow::on_sendButton_clicked()
{

}

