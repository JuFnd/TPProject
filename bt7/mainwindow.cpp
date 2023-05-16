#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "screenshots.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    server = new QBluetoothServer(QBluetoothServiceInfo::RfcommProtocol);
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete img;
    delete server;
}

void MainWindow::setImg(){
    captureScreen();
}

void MainWindow::handleConnection()
{
    // Accept the incoming connection
    QBluetoothSocket* socket = server->nextPendingConnection();

    // Read the incoming message
    QByteArray message = socket->readAll();
    BluetoothFormatImage *content;

    // Do something with the message
    qDebug() << "Received message: " << message;

    // Close the socket
    socket->close();
}


void MainWindow::captureScreen(){
    QPixmap pixmap;

    // Capture the screen when F12 key is pressed
    QShortcut *shortcut = new QShortcut(QKeySequence(Qt::Key_F12), this);
    QObject::connect(shortcut, &QShortcut::activated, [&]() {
        pixmap = QPixmap::grabWindow(QApplication::desktop()->winId());
        //pixmap.save("screenshot.png");
        img = new BluetoothFormatImage(pixmap);
        // Create a label widget to show the pixmap
        QLabel *label = new QLabel(this);
        label->setPixmap(pixmap);
        label->setScaledContents(true);

        // Set the label as the central widget of the window
        setCentralWidget(label);

        // Show the window in full screen mode
        //showFullScreen();
        //label->show();

    });
}


void MainWindow::on_pushButton_clicked()
{
    close();
    Screenshots screen;
    screen.setModal(true);
    screen.exec();
}

