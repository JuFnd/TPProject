#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "screenshots.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete img;
}

void MainWindow::setImg(){
    captureScreen();
}

void MainWindow::captureScreen(){
    QPixmap pixmap;
    QShortcut *shortcut = new QShortcut(QKeySequence(Qt::Key_F12), this);
    QObject::connect(shortcut, &QShortcut::activated, [&]() {
        pixmap = QPixmap::grabWindow(QApplication::desktop()->winId());
        pixmap.save("screenshot.png");
    });

    img = new BluetoothFormatImage(pixmap);
}

void MainWindow::on_pushButton_clicked()
{
    close();
    Screenshots screen;
    screen.setModal(true);
    screen.exec();
}

